使用c语言编写一个python模块，目的：提高效率。
Linux 版本具体流程：
+ 引入Python.h头文件，通常位于/usr/include/python2.7。
+ 编写待包装的函数，即功能函数。
+ 函数中处理从Python传入的参数。
+ 实现功能逻辑。
+ 处理C中的返回值，包装成Python对象。
+ 在一个P有MethodDef结构体中注册需要的函数。
+ 在一个初始化方法中注册模块名。
+ 将 C源文件编译成链接库。

demo.c为示例c程序，封装了+、-、×三个整形函数。
```c
// 待包装函数
int add(int x, int y)
{
    return x+y;
}
int minus(int x, int y)
{
    return x-y;
}
int multiple(int x, int y)
{
    return x*y;
}
// 引入头文件
#include <Python.h>
static PyObject* W_add(PyObject* self, PyObject* args)
{
    int x;
    int y;
    // 解析传入参数
    if(!PyArg_ParseTuple(args, "i|i", &x, &y))      
    {
        return NULL;
    }
    else
    {
        // 指向功能函数，Py_BuildValue 函数将返回值返回给Python
        return Py_BuildValue("i",add(x, y));
    }  
}
static PyObject* W_minus(PyObject* self, PyObject* args)
{
    int x;
    int y;
    if(!PyArg_ParseTuple(args, "i|i", &x, &y))
    {
        return NULL;
    }
    else
    {
        return Py_BuildValue("i", minus(x, y));
    }  
}
static PyObject* W_multiple(PyObject* self, PyObject* args)
{
    int x;
    int y;
    if(!PyArg_ParseTuple(args, "i|i", &x, &y))
    {
        return NULL;
    }
    else
    {
        return Py_BuildValue("i", multiple(x, y));
    }  
}
// 注册函数
static PyMethodDef ExtendMethods[] ={
    // 将add函数映射到W_add函数
    {"add", W_add, METH_VARARGS, "a add function from C"},
    {"minus", W_minus, METH_VARARGS, "a minus function from C"},
    {"multiple", W_multiple, METH_VARARGS, "a multiple function from C"},
    {NULL, NULL, 0, NULL},
};
PyMODINIT_FUNC initdemo()
{
    Py_InitModule("demo", ExtendMethods);
}
```
编译命令：
```bash
gcc demo.c -I /usr/include/python2.6 -shared -o demo.so
```
若遇到编译报错：
```bash
/usr/bin/ld: /tmp/ccyk2B0K.o: relocation R_X86_64_32 against `.rodata' can not be used when making a shared object; recompile with -fPIC
/tmp/ccyk2B0K.o: error adding symbols: Bad value
collect2: error: ld returned 1 exit status
```  
则添加-fPIC参数：
```bash
gcc demo.c -I /usr/include/python2.7 -fPIC -shared -o demo.so
```
进入Python环境测试：
```python
>>> import demo
>>> demo.add(2,4)
6
>>> demo.minus(2,4)
-2
>>> demo.multiple(2,4)
8
```
则说明编译成功。  
以下提供一个使用CMakeLists.txt编译模板：
```txt
#指定cmake版本
cmake_minimum_required(VERSION 2.8)
project(demo)
#添加头文件引用路径
include_directories(/usr/include/python2.7)
#编译生成动态库
add_library(demo SHARED demo.c)
#系统默认会增加前缀lib-, 即原本应该生成libdemo.so, 但如此则与程序中模块名称不一致，因此修改系统默认前缀
set_target_properties(demo PROPERTIES PREFIX "")
```