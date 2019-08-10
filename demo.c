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