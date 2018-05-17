/* electric_field.i */
// %module electric_field
// %{
// /* Put header files here or function declarations like below */
// #import "electric_field.h"
//
// extern PyObject *pName, *pModule, *pDict,
//             *pClass, *pInstance, *pValue;
// extern void initialize_electric_field();
// extern void update_electric_field(PyObject *new_electric_field);
// extern void get_electric_field(double *output, double x, double y, double z);
//
// %}

// %apply double *OUTPUT {double*};

PyObject *pName, *pModule, *pDict,
            *pClass, *pInstance, *pValue;

void initialize_electric_field();
void update_electric_field(PyObject *new_electric_field);
void set_electric_field(struct vector3 *electric_field, double x, double y, double z);
