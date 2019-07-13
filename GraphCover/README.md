# Graph Cover

[first useful link](http://vnsgu.ac.in/dept/publication/vnsgujst41july2015/25.pdf)
[slide 30](https://people.cs.umass.edu/~sheldon/teaching/mhc/cs312/2013sp/Slides/Slides22%20-%20Polynomial%20Reducibility.pdf)


```
Hi Aditya,

I found the following reduction from vertex cover to 3-CNF (cnf with atmost 3 literals per clause):

https://cs.stackexchange.com/questions/22426/reduce-vertex-cover-to-sat

Note that the above reduction also requires to reduce bit-vector adders to SAT.

Translating bit-vector addition to SAT is easy. Say that you encode addiction as 16-bit adders. You can encode a full adder as a ripple-carry adder, and then use your Tseitin translation to get an equivalent CNF.

Hope this helps!

Warm Regards,
Vijay Ganesh.
https://ece.uwaterloo.ca/~vganesh
```
