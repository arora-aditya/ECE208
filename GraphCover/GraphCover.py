#!/usr/bin/env python
# coding: utf-8

# In[1]:


import sympy as sp
from sympy import *
from sympy.abc import *
from sympy.parsing.sympy_parser import parse_expr
from itertools import combinations


# In[2]:


CNF_FILE_NAME = f'graph.cnf'


# In[3]:


def readIntoAM(filename: str):
    am = []
    with open(filename) as f:
        lines = f.readlines()
        for line in lines:
            line = list(map(int, line[:-1]))
            am.append(list(line))
    for i in range(len(am)):
        for j in range(i, len(am[i])):
            am[i][j] = am[j][i]
    return am
    


# In[4]:


adjaceny_matrix = am = readIntoAM(f'./adjacency_matrices_8/testcase_{1}.cov')


# In[5]:


am


# In[6]:


k = 3


# In[7]:


# adjaceny_matrix = am = [
#             [0, 1, 1, 1, 1],
#             [1, 0, 0, 0, 1],
#             [1, 0, 0, 1, 1],
#             [1, 0, 1, 0, 1],
#             [1, 1, 1, 1, 0]
# ]


# In[8]:


max_num = len(am)


# In[9]:


max_num


# In[10]:


def define_variables(max_no: int):
    return symbols(f'x:{max_no}')


# In[11]:


syms: sp.Symbol = define_variables(max_num)
for i in range(max_num):
    exec(f'x{i} = syms[{i}]')


# In[ ]:





# In[12]:


clauses = []


# In[13]:


# add edges to our clauses
for i in range(max_num):
    for j in range(i+1):
        if am[i][j] == 1:
            clauses.append(f'x{i}|x{j}')


# In[14]:


clauses


# In[15]:


# encode addition from 1 to k
# m = len(clauses)
overall_addition = []
se = set()
for symbol in syms:
    se.add(str(symbol))

for i in range(1, k+1):
    x = list(combinations(se, i))
    clause_string = ''
    for part_clause in x:
        clause_string += '&'.join(part_clause) 
        negated = se - set(part_clause)
        if len(negated) != 0:
            clause_string += '&~'+'&~'.join(negated)
        clause_string += '|'
    clause_string.replace('||', '|')
    clause_string = clause_string[:-1]
    overall_addition.append(clause_string)

exec('overall_addition_course = ' + '|'.join(overall_addition))
sp.to_cnf(overall_addition_course, True)


# In[16]:


# se


# In[17]:


overall_addition_course_cnf = sp.to_cnf(overall_addition_course, True)


# In[18]:


# overall_addition_course_cnf


# In[19]:


clauses.append(str(overall_addition_course_cnf))


# In[20]:


# clauses


# In[21]:


exec('a='+'('+')&('.join(clauses) + ')')


# In[22]:


# sp.to_cnf(a, True)


# In[23]:


simplify(a)


# In[24]:


satisfiable(a)


# In[25]:


# a


# In[26]:


def write_cnf(filename, cnf_ans, set_of_vars):
    assert cnf_ans.func == sp.And
    l = []
    for row in am:
        l.append(f'c {row}')
    l.append(f'c k = {k}')
    l.append(f'p cnf {len(set_of_vars)} {len(cnf_ans.args)}')
    for arg in cnf_ans.args:
        s = ''
        if arg.func == sp.Symbol:
            s += str(set_of_vars[arg]) + ' '
        else:
            assert arg.func == sp.Or
            for sub_arg in arg.args:
                if len(sub_arg.args) == 1:
                    s += str(-1*(set_of_vars[sub_arg.args[0]])) + ' '
                elif len(sub_arg.args) == 0:
                    s += str(set_of_vars[sub_arg]) + ' '
        s += '0'
        l.append(s)
    with open(filename,'w') as f:
        f.write('\n'.join(l))


# In[27]:


def set_wrapper(cnf_ans):
    set_of_vars = {}
    counter = 1
    def recursive_add_to_set(a):
        nonlocal counter
        if a.func == sp.Symbol:
            if a not in set_of_vars:
                set_of_vars[a] = counter
                counter += 1
        elif len(a.args)==1:
            b = a.args[0]
            if b not in set_of_vars:
                set_of_vars[b] = counter
                counter += 1
        else:
            for arg in a.args:
                recursive_add_to_set(arg)
    recursive_add_to_set(cnf_ans)
    return set_of_vars


# In[28]:


if k > len(am):
    a = x1 | ~x1
    l = []
    for row in am:
        l.append(f'c {row}')
    l.append(f'c k = {k}')
    l.append(f'p cnf 1 1')
    l.append(f'1 -1 0')
    with open(CNF_FILE_NAME,'w') as f:
        f.write('\n'.join(l))
else:
    write_cnf(CNF_FILE_NAME, a, set_wrapper(a))


# In[ ]:





# In[29]:


def encode_sum(k: int):
#     print(k, end=':')
    overall_addition = []
    se = set()
    for symbol in syms:
        se.add(str(symbol))
    overall_addition_course = None
    for i in range(1, k+1):
        x = list(combinations(se, i))
        clause_string = ''
        for part_clause in x:
            clause_string += '&'.join(part_clause) 
            negated = se - set(part_clause)
            if len(negated) != 0:
                clause_string += '&~'+'&~'.join(negated)
            clause_string += '|'
        clause_string.replace('||', '|')
        clause_string = clause_string[:-1]
        overall_addition.append(clause_string)
    overall_addition_course = parse_expr('|'.join(overall_addition))
    overall_addition_course = sp.to_cnf(overall_addition_course, True)
    return overall_addition_course


# In[36]:


def find_min_size():
    edges = []
    # add edges to our clauses
    for i in range(max_num):
        for j in range(i+1):
            if am[i][j] == 1:
                edges.append(f'x{i}|x{j}')
    for l in range(1, len(am)+1):
        clauses = edges[:]
        clauses.append(str(encode_sum(l)))
        a = parse_expr('('+')&('.join(clauses) + ')')
        if satisfiable(a) != False:
            # print(a)
            # print(f'{l}, {satisfiable(a)}')
            global k
            k = l
            break
    return l, a


# In[37]:


find_min_size()


# In[32]:


am


# In[33]:


correct_ans_8 = [['testcase_0', 5], ['testcase_1', 5], ['testcase_2', 4], ['testcase_3', 5], ['testcase_4', 3], ['testcase_5', 4], ['testcase_6', 4], ['testcase_7', 3], ['testcase_8', 5], ['testcase_9', 5], ['testcase_10', 5], ['testcase_11', 5], ['testcase_12', 4], ['testcase_13', 4], ['testcase_14', 5], ['testcase_15', 4], ['testcase_16', 5], ['testcase_17', 4], ['testcase_18', 5], ['testcase_19', 5], ['testcase_20', 4], ['testcase_21', 5], ['testcase_22', 5], ['testcase_23', 3], ['testcase_24', 4], ['testcase_25', 5], ['testcase_26', 5], ['testcase_27', 5], ['testcase_28', 5], ['testcase_29', 4], ['testcase_30', 3], ['testcase_31', 5], ['testcase_32', 4], ['testcase_33', 5], ['testcase_34', 6], ['testcase_35', 5], ['testcase_36', 5], ['testcase_37', 4], ['testcase_38', 4], ['testcase_39', 5], ['testcase_40', 5], ['testcase_41', 4], ['testcase_42', 4], ['testcase_43', 4], ['testcase_44', 4], ['testcase_45', 5], ['testcase_46', 4], ['testcase_47', 5], ['testcase_48', 5], ['testcase_49', 5], ['testcase_50', 5], ['testcase_51', 4], ['testcase_52', 5], ['testcase_53', 4], ['testcase_54', 4], ['testcase_55', 5], ['testcase_56', 4], ['testcase_57', 5], ['testcase_58', 4], ['testcase_59', 5], ['testcase_60', 4], ['testcase_61', 4], ['testcase_62', 5], ['testcase_63', 5], ['testcase_64', 4], ['testcase_65', 5], ['testcase_66', 4], ['testcase_67', 4], ['testcase_68', 5], ['testcase_69', 5], ['testcase_70', 5], ['testcase_71', 4], ['testcase_72', 5], ['testcase_73', 5], ['testcase_74', 5], ['testcase_75', 5], ['testcase_76', 4], ['testcase_77', 5], ['testcase_78', 4], ['testcase_79', 5], ['testcase_80', 5], ['testcase_81', 5], ['testcase_82', 4], ['testcase_83', 4], ['testcase_84', 5], ['testcase_85', 5], ['testcase_86', 5], ['testcase_87', 4], ['testcase_88', 5], ['testcase_89', 5], ['testcase_90', 4], ['testcase_91', 5], ['testcase_92', 5], ['testcase_93', 5], ['testcase_94', 4], ['testcase_95', 5], ['testcase_96', 4], ['testcase_97', 4], ['testcase_98', 4], ['testcase_99', 5]]
correct_ans_7 = [['testcase_0', 4], ['testcase_1', 5], ['testcase_2', 3], ['testcase_3', 3], ['testcase_4', 3], ['testcase_5', 4], ['testcase_6', 4], ['testcase_7', 4], ['testcase_8', 4], ['testcase_9', 3], ['testcase_10', 4], ['testcase_11', 4], ['testcase_12', 5], ['testcase_13', 5], ['testcase_14', 4], ['testcase_15', 4], ['testcase_16', 4], ['testcase_17', 4], ['testcase_18', 4], ['testcase_19', 4], ['testcase_20', 3], ['testcase_21', 3], ['testcase_22', 4], ['testcase_23', 5], ['testcase_24', 4], ['testcase_25', 4], ['testcase_26', 3], ['testcase_27', 4], ['testcase_28', 4], ['testcase_29', 4], ['testcase_30', 4], ['testcase_31', 4], ['testcase_32', 3], ['testcase_33', 4], ['testcase_34', 4], ['testcase_35', 3], ['testcase_36', 4], ['testcase_37', 3], ['testcase_38', 3], ['testcase_39', 4], ['testcase_40', 4], ['testcase_41', 4], ['testcase_42', 4], ['testcase_43', 4], ['testcase_44', 4], ['testcase_45', 4], ['testcase_46', 3], ['testcase_47', 4], ['testcase_48', 5], ['testcase_49', 4], ['testcase_50', 4], ['testcase_51', 4], ['testcase_52', 4], ['testcase_53', 3], ['testcase_54', 4], ['testcase_55', 5], ['testcase_56', 4], ['testcase_57', 3], ['testcase_58', 4], ['testcase_59', 4], ['testcase_60', 3], ['testcase_61', 4], ['testcase_62', 4], ['testcase_63', 5], ['testcase_64', 5], ['testcase_65', 3], ['testcase_66', 4], ['testcase_67', 4], ['testcase_68', 3], ['testcase_69', 3], ['testcase_70', 4], ['testcase_71', 3], ['testcase_72', 4], ['testcase_73', 5], ['testcase_74', 3], ['testcase_75', 4], ['testcase_76', 4], ['testcase_77', 5], ['testcase_78', 3], ['testcase_79', 3], ['testcase_80', 4], ['testcase_81', 3], ['testcase_82', 4], ['testcase_83', 4], ['testcase_84', 4], ['testcase_85', 3], ['testcase_86', 3], ['testcase_87', 4], ['testcase_88', 4], ['testcase_89', 4], ['testcase_90', 3], ['testcase_91', 5], ['testcase_92', 4], ['testcase_93', 3], ['testcase_94', 3], ['testcase_95', 3], ['testcase_96', 4], ['testcase_97', 4], ['testcase_98', 4], ['testcase_99', 4]]


# In[38]:


ans = []
for i in range(100):
    global am
    adjaceny_matrix = am = readIntoAM(f'./adjacency_matrices_7/testcase_{i}.cov')
    global max_num
    max_num = len(am)
    min_size, cnf_formula = find_min_size()
    ans.append([f'testcase_{i}', min_size])
    assert min_size == correct_ans_7[i][-1], f'./adjacency_matrices_7/testcase_{i}.cov min-size - {min_size}, correct_ans-{correct_ans_7[i][-1]}' 
    write_cnf(f'./output_cnf/{CNF_FILE_NAME}_testcase_{i}', cnf_formula, set_wrapper(cnf_formula))
    print(f'{i}/100', end='\r')
#     print(ans[-1])


# In[ ]:





# In[ ]:





# In[ ]:




