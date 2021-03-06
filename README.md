# Tiny_ctl
## 记录问题：
## 编译使用
```shell
cd src
make
cd ../example
make
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/../lib
```
## 说明
1. 自行调用对象的构造函数和construct时需要在末尾添加VAEND，new函数会自行添加
2. 创建新的类时，不使用VA()宏，在参数末尾需要添加NULL
3. 建立对象时，对于父类，需要调用VAEND将每个类的构造参数隔离开
4. 容器储存POD和OBJ类型，迭代器储存ADDR和OBJ类型，类似Compare等操作函数接受OBJ和ADDR类型
