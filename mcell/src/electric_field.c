#include <stdio.h>
#include "electric_field.h"

void initialize_electric_field(){
    Py_Initialize();

    PyObject *sys = PyImport_ImportModule("sys");
    PyObject *path = PyObject_GetAttrString(sys, "path");
    PyList_Append(path, PyUnicode_FromString("."));

    // Build the name object
    pName = PyUnicode_DecodeFSDefault("pymcell.electric_field_class");
    if (!pName)
    {
        PyErr_Print();
        printf("ERROR in pName\n");
        exit(1);
    }

//     Load the module object
    pModule = PyImport_Import(pName);
    if (!pModule)
    {
        PyErr_Print();
        printf("ERROR in pModule\n");
        exit(1);
    }

    // pDict is a borrowed reference
    pDict = PyModule_GetDict(pModule);
    if (!pDict)
    {
        PyErr_Print();
        printf("ERROR in pDict\n");
        exit(1);
    }


    // Build the name of a callable class
    pClass = PyDict_GetItemString(pDict, "Electric_field");
    if (!pClass)
    {
        PyErr_Print();
        printf("ERROR in pClass\n");
        exit(1);
    }


    // Create an instance of the class
    if (PyCallable_Check(pClass))
    {
        pInstance = PyObject_CallObject(pClass, NULL);
    }
    if (!pInstance)
    {
        PyErr_Print();
        printf("ERROR in pInstance\n");
        exit(1);
    }
}

void update_electric_field(PyObject *new_electric_field){
        PyObject_SetAttrString(pInstance, "get_electric_field", new_electric_field);
}

void set_electric_field(struct vector3 *electric_field, struct volume_molecule *m){
    pValue = PyObject_CallMethod(pInstance, "get_electric_field", "(ddd)", m->pos.x, m->pos.y, m->pos.z);
    if(!pValue)
    {
        PyErr_Print();
    }

    PyArg_ParseTuple(pValue, "ddd", &electric_field->x, &electric_field->y, &electric_field->z);

    Py_DECREF(pValue);
}

/*
void set_electric_field(struct vector3 *electric_field, double x, double y, double z){
    pValue = PyObject_CallMethod(pInstance, "get_electric_field", "(ddd)", x, y, z);
    if(!pValue)
    {
        PyErr_Print();
        printf("Something went wrong! \n");
    }

    int ok;
    ok = PyArg_ParseTuple(pValue, "ddd", &x, &y, &z);


    electric_field->x = x;
    electric_field->y = y;
    electric_field->z = z;
}
*/
