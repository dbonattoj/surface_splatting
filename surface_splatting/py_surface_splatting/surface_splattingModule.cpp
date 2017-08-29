#include <Python.h>


#include "splat_renderer.hpp"

SurfaceRenderer sr = nullptr;

static PyObject * SurfaceSplatting_SurfaceSplatting(PyObject * self, PyObject *args) {
	const int * camera; // TODO not an int
	if (!PyArg_ParseTuple(args, "i", &camera))
		return NULL;

	sr = new SurfaceRenderer(camera);

	return Py_BuildValue("i", 0);
}

static PyObject * SurfaceSplatting_renderFrame(PyObject * self, PyObject *args) {
	const int * surfels; // TODO not an int
	if (!PyArg_ParseTuple(args, "i", &surfels))
		return NULL;

	sr->render_frame(surfels);

	int texture = sr->getTextureID();

	return Py_BuildValue("i", texture);
}

static PyObject * SurfaceSplatting_reshape(PyObject * self, PyObject *args) {
	const int * width;
	const int * height;
	if (!PyArg_ParseTuple(args, "i", &width) && !PyArg_ParseTuple(args, "i", &height))
		return NULL;

	sr->reshape(width, height);

	return Py_BuildValue("i", 0);
}

static PyObject * SurfaceSplatting_delete(PyObject * self, PyObject *args) {

	sr = nullptr;

	return Py_BuildValue("i", 0);
}


// Structure to define how the functions are presented in python
static PyMethodDef myLibrary_methods[] = {
	// The first property is the name exposed to python, the second is the C++ function name        
	{ "create", (PyCFunction)SurfaceSplatting_SurfaceSplatting, METH_VARARGS, nullptr },
	{ "renderFrame", (PyCFunction)SurfaceSplatting_renderFrame, METH_VARARGS, nullptr },
	{ "reshape", (PyCFunction)SurfaceSplatting_reshape, METH_VARARGS, nullptr },
	{ "delete", (PyCFunction)SurfaceSplatting_delete, METH_VARARGS, nullptr },

	// Terminate the array with an object containing nulls.
	{ nullptr, nullptr, 0, nullptr }
};

// Structure to define the module as Python
static PyModuleDef myLibrary_module = {
	PyModuleDef_HEAD_INIT,
	"SurfaceSplatting",                        // Module name
	"Provide access to Surface Splatting from C++",  // Module description
	0,
	myLibrary_methods                   // Structure that defines the methods
};


// Add a method that Python calls when it loads the module, which must be must be named PyInit_<module-name>
PyMODINIT_FUNC PyInit_myLibrary() {
	return PyModule_Create(&myLibrary_module);
}
