# Tiny_ctl
## 记录问题：
1. 对象抽象程度不够，决定研究如何设计底层对象解决许多抽象问题
2. hashtable的__next_prime以后调用lower_bound算法
3. power算法待解决
## 编译使用
```shell
cd src
make
cd ../example
make
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/../lib
```
## 说明
BinaryOperation函数：当first、second为NULL时，其值相当于为0，继续运算，主要由于赋值需求。当res为NULL时，应该分配空间给res，此值应该为静态变量或者由自动释放池分配，因为算法调用后不会调用释放函数。