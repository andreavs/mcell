#pragma once
#include <stdio.h>
#include "mcell_structs.h"
#include "vector.h"
#include <Python.h>

PyObject *pName, *pModule, *pDict,
            *pClass, *pInstance, *pValue;

void initialize_electric_field();
void update_electric_field(PyObject *new_electric_field);
void set_electric_field(struct vector3 *electric_field, struct volume_molecule *m);
