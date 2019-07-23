#!/usr/bin/env python
# coding: utf-8

# In[1]:


import sympy as sp
from sympy import *
from sympy.abc import *
from sympy.parsing.sympy_parser import parse_expr
import itertools


# In[2]:


NNF_FILE_NAME = 'nnf/1.nnf'
CNF_FILE_NAME = 'cnf/1.cnf'


# In[3]:


class MisMatchedBrackets(Exception):
    pass
class EmptyBrackets(Exception):
    pass
class NANException(Exception):
    pass
class OperatorAfterOperatorException(Exception):
    pass
class OperatorAtIndex0(Exception):
    pass
class OperatorAtEnd(Exception):
    pass
class SubformulaError(Exception):
    pass
class DepthError(Exception):
    pass
class Tseitin_Aux(Exception):
    pass
class CNFError(Exception):
    pass


# In[4]:


def detect_operator_errors(NNF: str):
    if '.+' in NNF:
        raise OperatorAfterOperatorException('.+ found in string')
    elif '+.' in NNF:
        raise OperatorAfterOperatorException('+. found in string')
    elif '--' in NNF:
        raise OperatorAfterOperatorException('Double negation found in string')
    if NNF[0] in ('.', '+'):
        raise OperatorAtIndex0(f'Operator {NNF[0]} found at index 0')
    elif NNF[-1] in ('.', '+'):
        raise OperatorAtEnd(f'Operator {NNF[-1]} found at end of NNF')


# In[5]:


def detect_incorrect_brackets(NNF: str):
    emptyBracket = NNF.find('()')
    if emptyBracket != -1:
        raise EmptyBrackets(f'Empty Brackets at index {emptyBracket+1}')
    st = []
    le = len(NNF)
    for i in range(le):
        char = NNF[i]
        if char == '(':
            st.append(i)
        elif char == ')':
            if st == []:
                raise MisMatchedBrackets(f'Extra closing bracket at index {i+1} in file')
            st.pop()
        
    if st != []:
        raise MisMatchedBrackets(f'Unpaired opening bracket at index {st[0]+1} in file')


# In[6]:


def read_NNF_from_file(filename: str=NNF_FILE_NAME):
    f = open(filename, "r")
    line = f.readline()
    for i in range(len(line)-1,-1,-1):
        char = line[i]
        if char == '0':
            line = line[:i]
            break
    line = line.replace(' ', '')
    return line.strip()


# In[7]:


NNF_STRING = read_NNF_from_file()


# In[8]:


NNF_STRING


# In[9]:


def max_no(clause: str):
    final = []
    clause = clause.replace('(', '')
    clause = clause.replace(')', '')
    dot_split = clause.split('.')
    for ds in dot_split:
        final.extend(ds.split('+'))
    for i in range(len(final)):
        try:
            final[i] = abs(int(final[i]))
        except:
            raise NANException(f'Found an invalid character "{final[i]}"')
    return max(final)


# In[ ]:





# In[10]:


def define_variables(max_no: int):
    return symbols(f'x:{max_no}')


# In[11]:


max_num:int  = max_no(NNF_STRING)
syms: sp.Symbol = define_variables(max_num+1)
for i in range(max_num+1):
    exec(f'x{i} = syms[{i}]')


# In[12]:


def replace_nums_with_symbols(clause: str)->str:
    max_num = max_no(clause)
    clause = clause.replace('-', '~')
    split_by_and = clause.split('.')
    syms = define_variables(max_num+1)
    for i in range(max_num+1):
        exec(f'x{i} = syms[{i}]')
    fin_clause = ''
    for sub_clause in split_by_and:
        fin_sub_clause = ''
        for sub_sub_clause in sub_clause.split('+'):
            for i in range(max_num, 0, -1):
                if sub_sub_clause.replace(str(i), str(syms[i])) != sub_sub_clause:
                    sub_sub_clause = sub_sub_clause.replace(str(i), str(syms[i]))
                    break
            fin_sub_clause = fin_sub_clause + '|' + sub_sub_clause
        
        fin_clause = fin_clause + '&' + fin_sub_clause[1:]
    return fin_clause[1:]


# In[13]:


type(parse_expr('x1 & x2 | x3 | x5'))


# In[14]:


def get_sp_clause(clause: str):
    converted_clause_string = replace_nums_with_symbols(clause)
    try:
        return parse_expr(converted_clause_string)
    except Exception as e:
        print(f'Error in clause {clause} {e}')


# In[15]:


def subformulas(expr):
# subformula of expr without atoms
# considering operator as binary
    
    if expr.func == sp.Symbol:
        return []
    elif len(expr.args)==1:
        return [expr] + subformulas(expr.args[0])
    elif len(expr.args)==2:
        return [expr] + subformulas(expr.args[0]) + subformulas(expr.args[1]) 
    elif len(expr.args)>2:
        return [expr] + subformulas(expr.args[0]) + subformulas(eval(str(expr.func)+str(expr.args[1:])))  
    else:
        raise SubformulaError(f'For input expr {expr}')


# In[16]:


def depth(expr):
# still considering operator as binary
    if expr.func == Not:
        return 1 + depth(expr.args[0])
    elif (len(expr.args)==2):
        return 1 + max([depth(x) for x in expr.args])
    elif ( len(expr.args)>2):
        #return 1 + max(depth(expr.args[0]), depth(eval(str(expr.func)+str(expr.args[1:]))))
        return len(expr.args)-1 + max([depth(x) for x in expr.args])
    elif expr.func == Symbol:
        return 0
    else: 
        raise DepthError(f'For input expr {expr}')


# In[17]:


def symbol_init(x, count=0):
    def new_symbol ():
        nonlocal count
        count += 1
        return var(str(x) + str(count)) #var add the new symbol in the name space
    return new_symbol


# In[18]:


def binSubs(x, y, z):
    #replace y by z in x
    #print("subs", x, y, z)
    if (len(y.args) >0) and (len(x.args)>2) and (y!=x) and (y.func==x.func):
     
        if all([y in x.args for y in y.args]): 
            l = list(x.args)
            for i in y.args:
                l.remove(i)
            if (z.func== Symbol):
                t=tuple(l + [z])
            else:
                t=tuple(l+list(z.args))
            return eval(str(x.func) + str(t))
    
    return x.subs(y,z)


# In[19]:


def tseitin1(string):
    # for binary operation
    try:
        expr = get_sp_clause(string)
        s=symbol_init("p")
        if expr.func == Symbol:
            return true
        else:
            return tseitin_aux(sorted(set(subformulas(expr)), key=depth), s) 
    except Exception as e:
        raise e
        return None

def tseitin_aux(L, s):
    #nonlocal newvar
    newvar = s()
    if (L==[]):
        return true
    if depth(L[0]) > 0:
        if len(L[1:])>0 :
            #newList = [x.subs(L[0], newvar) for x in L[1:]]  
            
            newList = [binSubs(x, L[0], newvar) for x in L[1:]]  
            
            return Equivalent(newvar, L[0]) & tseitin_aux(newList, s)
        else:      
            return Equivalent(newvar, L[0]) & newvar
    else:
        raise Tseitin_Aux(f'Error for arguments L:{L} s:{s}')


# In[20]:


def demorgan(a, neg=False, depth=0):
#     print(a)
#     print(depth*"\t","demorgan", a)
    s = f''
#     print(depth*"\t", a)
    if a.func == sp.Or:
        ls = []
        for i in range(len(a.args)):
            if a.args[i].func == sp.And:
    #             print(depth*"\t",'1')
                ls.append(a.args[i].args)
            elif a.args[i].func == sp.Symbol:
                ls.append([a.args[i]])
            elif len(a.args[i].args) == 1:
                ls.append([a.args[i]])
            else:
                print('a', a.args[i])
#                 ls.append()
        ls2 = []
        for element in itertools.product(*ls):
            ls2.append(parse_expr('Or' + str(tuple(element))))
        ls2_str = 'And(' + str(ls2)[1:-1] + ')'
#         print('ls2', ls2_str)
        return parse_expr(ls2_str)
    else:
        return a


# In[21]:


def cnf(a, neg=False, depth=0):
#     print(depth*'\t', a, a.func, len(a.args), neg)
    if a.func == sp.Symbol:
        if neg:
            return ~a
        else:
            return a
    elif len(a.args)==1:
#         print(a.args)
        return cnf(eval(str(a.args[0])), neg=not neg, depth=depth+1)
    elif len(a.args)==2 and a.func == sp.Equivalent:
        if not neg:
            return demorgan(demorgan(cnf(a.args[0], depth=depth+1), depth=depth+1) | demorgan(cnf(a.args[1], neg=True, depth=depth+1), depth=depth+1), depth=depth+1) & demorgan(demorgan(cnf(a.args[0], neg=True, depth=depth+1), depth=depth+1) | demorgan(cnf(a.args[1], depth=depth+1), depth=depth+1), depth=depth+1)
        else:
            assert False
            # return (cnf(a.args[0], neg=True, depth=depth+1) | cnf(a.args[1], neg=True, depth=depth+1) & (cnf(a.args[1], neg=False, depth=depth+1) | cnf(a.args[0], neg=False, depth=depth+1)))
    elif len(a.args)>=2:
        if a.func == sp.And:
            if not neg:
                return demorgan(cnf(a.args[0], depth=depth+1) & demorgan(cnf(eval(str(a.func)+str(a.args[1:])), depth=depth+1), depth=depth+1), depth=depth+1)
            else:
                return demorgan(cnf(a.args[0], neg=True, depth=depth+1) | demorgan(cnf(eval(str(a.func)+str(a.args[1:])), neg=True, depth=depth+1), depth=depth+1), depth=depth+1)
        elif a.func == sp.Or:
            if not neg:
                return demorgan(cnf(a.args[0], depth=depth+1) | demorgan(cnf(eval(str(a.func)+str(a.args[1:])), depth=depth+1), depth=depth+1), depth=depth+1)
            else:
                return demorgan(cnf(a.args[0], depth=depth+1, neg=neg) & demorgan(cnf(eval(str(a.func)+str(a.args[1:])), neg=neg, depth=depth+1), depth=depth+1), depth=depth+1)
    else:
        raise CNFError(f'For input {a}')


# In[22]:


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


# In[23]:


def write_cnf(filename, cnf_ans, set_of_vars):
    assert cnf_ans.func == sp.And
    l = [f'p cnf {len(set_of_vars)} {len(cnf_ans.args)}']
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


# In[24]:


def make_cnf(s):
    global CNF_FILE_NAME
    clause = tseitin1(s)
    cnf_ans = cnf(clause)
#     print(cnf_ans)
    set_of_vars = set_wrapper(cnf_ans)
    write_cnf(CNF_FILE_NAME, cnf_ans, set_of_vars)
    return cnf_ans


# In[25]:


make_cnf(NNF_STRING)


# In[ ]:





# In[26]:


NNF_STRING


# In[ ]:





# In[27]:


sp.to_cnf(tseitin1(NNF_STRING)) == make_cnf(NNF_STRING)


# In[28]:


for i in range(1, 40):
    NNF_FILE_NAME = f'nnf/test_case{i}.nnf'
    CNF_FILE_NAME = f'cnf/test_case{i}.cnf'
    NNF_STRING = read_NNF_from_file(NNF_FILE_NAME)
    detect_incorrect_brackets(NNF_STRING)
    detect_operator_errors(NNF_STRING)
    max_num:int  = max_no(NNF_STRING)
    syms: sp.Symbol = define_variables(max_num+1)
    for j in range(max_num+1):
        exec(f'x{j} = syms[{j}]')
    print(NNF_FILE_NAME, i, sp.to_cnf(tseitin1(NNF_STRING)) == make_cnf(NNF_STRING))


# In[29]:


try:
    NNF_FILE_NAME = f'nnf/3.nnf'
    CNF_FILE_NAME = f'cnf/3.cnf'
    NNF_STRING = read_NNF_from_file(NNF_FILE_NAME)
    detect_incorrect_brackets(NNF_STRING)
    detect_operator_errors(NNF_STRING)
    print(i, sp.to_cnf(tseitin1(NNF_STRING)) == make_cnf(NNF_STRING))
except NANException as e:
    print(f'recieved a NANException as Expected: {e}')


# In[30]:


try:
    read_NNF_from_file('nnf/-1.nnf')
except FileNotFoundError as e:
    print(f'recieved a FileNotFoundError as Expected: {e}')


# In[31]:


try:
    NNF_STRING = read_NNF_from_file('nnf/0.nnf')
    detect_incorrect_brackets(NNF_STRING)
    detect_operator_errors(NNF_STRING)
except EmptyBrackets as e:
    print(f'recieved a EmptyBrackets as Expected: {e}')


# In[32]:


try:
    NNF_STRING = read_NNF_from_file('nnf/2.nnf')
    detect_incorrect_brackets(NNF_STRING)
    detect_operator_errors(NNF_STRING)
except MisMatchedBrackets as e:
    print(f'recieved a MisMatchedBrackets as Expected: {e}')


# In[33]:


for i in range(10):
    NNF_FILE_NAME = f'nnf/fuzzy_testcase_{i}.nnf'
    CNF_FILE_NAME = f'cnf/fuzzy_testcase_{i}.cnf'
    NNF_STRING = read_NNF_from_file(NNF_FILE_NAME)
    detect_incorrect_brackets(NNF_STRING)
    detect_operator_errors(NNF_STRING)
    max_num:int  = max_no(NNF_STRING)
    syms: sp.Symbol = define_variables(max_num+1)
    for j in range(max_num+1):
        exec(f'x{j} = syms[{j}]')
    print(NNF_FILE_NAME, i, sp.to_cnf(tseitin1(NNF_STRING)) == make_cnf(NNF_STRING))


# In[ ]:





# In[ ]:





# In[ ]:




