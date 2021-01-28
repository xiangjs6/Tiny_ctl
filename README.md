# Tiny_ctl
因为底层重构，目前还无法编译
## 记录问题：
1. hashtable的__next_prime以后调用lower_bound算法
2. power算法待解决
## 编译使用
```shell
cd src
make
cd ../example
make
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/../lib
```
## 说明
1. 自行调用对象的构造函数时需要在末尾添加VAEND，new、和construct函数会自行添加
2. 创建新的类时，不使用VA()宏，在参数末尾需要添加NULL
