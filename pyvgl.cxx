#include "pyvgl.h"
#include <vgl/vgl_vector_2d.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_vector_3d.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_ray_3d.h>
#include <vgl/algo/vgl_rotation_3d.h>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

namespace pyvxl { namespace vgl {

// Helper functions

template<class T>
double getitem_3d(T const& a, long i)
{
  // wrap around
  if (i < 0) {
    i += 3;
  }
  if (i==0) {
    return a.x();
  }
  else if (i==1) {
    return a.y();
  }
  else if (i==2) {
    return a.z();
  }
  else {
    throw py::index_error("index out of range");
  }
  return 0; // to avoid compiler warning
}

template<class T>
double getitem_2d(T const& a, long i)
{
  // wrap around
  if (i < 0) {
    i += 2;
  }
  if (i==0) {
    return a.x();
  }
  else if (i==1) {
    return a.y();
  }
  else {
    throw py::index_error("index out of range");
  }
  return 0; // to avoid compiler warning
}

template <class T, class BUFF_T>
T type_from_buffer_2d(py::array_t<BUFF_T> b)
{
  py::buffer_info info = b.request();
  if (info.format != py::format_descriptor<BUFF_T>::format()) {
    throw std::runtime_error("Incompatible scalar type");
  }
  if (info.ndim != 1) {
    throw std::runtime_error("Expecting a 1-dimensional vector");
  }
  const size_t num_elements = info.shape[0];
  if (num_elements != 2) {
    throw std::runtime_error("Expecting 2-d input vector");
  }
  // in-place constructor
  const BUFF_T* data_ptr = static_cast<BUFF_T*>(info.ptr);
  const size_t stride = info.strides[0] / sizeof(BUFF_T);
  BUFF_T x = *data_ptr;
  BUFF_T y = *(data_ptr + stride);

  return T(x,y);
}

template <class T, class BUFF_T>
T type_from_buffer_3d(py::array_t<BUFF_T> b)
{
  py::buffer_info info = b.request();
  if (info.format != py::format_descriptor<BUFF_T>::format()) {
    throw std::runtime_error("Incompatible scalar type");
  }
  if (info.ndim != 1) {
    throw std::runtime_error("Expecting a 1-dimensional vector");
  }
  const size_t num_elements = info.shape[0];
  if (num_elements != 3) {
    throw std::runtime_error("Expecting 3-d input vector");
  }
  // in-place constructor
  const BUFF_T* data_ptr = static_cast<BUFF_T*>(info.ptr);
  const size_t stride = info.strides[0] / sizeof(BUFF_T);
  BUFF_T x = *data_ptr;
  BUFF_T y = *(data_ptr + stride);
  BUFF_T z = *(data_ptr + 2*stride);

  return T(x,y,z);
}

void wrap_vgl(py::module &m)
{
  py::class_<vgl_point_2d<double> > (m, "point_2d")
    .def(py::init<double,double>())
    .def(py::init(&type_from_buffer_2d<vgl_point_2d<double>, double>))
    .def("__len__", [](vgl_point_2d<double>){return (size_t)2;})
    .def("__getitem__", getitem_2d<vgl_point_2d<double> >)
    .def_property_readonly("x", (double (vgl_point_2d<double>::*)() const) &vgl_point_2d<double>::x)
    .def_property_readonly("y", (double (vgl_point_2d<double>::*)() const) &vgl_point_2d<double>::y)
    .def(py::self - py::self);

  py::class_<vgl_vector_2d<double> > (m, "vector_2d")
    .def(py::init<double,double>())
    .def(py::init(&type_from_buffer_2d<vgl_vector_2d<double>, double>))
    .def("__len__", [](vgl_vector_2d<double>){return (size_t)2;})
    .def("__getitem__",getitem_2d<vgl_vector_2d<double> >)
    .def_property_readonly("x", &vgl_vector_2d<double>::x)
    .def_property_readonly("y", &vgl_vector_2d<double>::y)
    .def("length", &vgl_vector_2d<double>::length)
    .def(py::self + py::self)
    .def(py::self - py::self);

  py::class_<vgl_point_3d<double> > (m, "point_3d")
    .def(py::init<double,double,double>())
    .def(py::init(&type_from_buffer_3d<vgl_point_3d<double>, double>))
    .def("__len__", [](vgl_point_3d<double>){return (size_t)3;})
    .def("__getitem__", getitem_3d<vgl_point_3d<double> >)
    .def_property_readonly("x", (double (vgl_point_3d<double>::*)() const) &vgl_point_3d<double>::x)
    .def_property_readonly("y", (double (vgl_point_3d<double>::*)() const) &vgl_point_3d<double>::y)
    .def_property_readonly("z", (double (vgl_point_3d<double>::*)() const) &vgl_point_3d<double>::z)
    .def(py::self - py::self);

  py::class_<vgl_vector_3d<double> > (m, "vector_3d")
    .def(py::init<double,double,double>())
    .def(py::init(&type_from_buffer_3d<vgl_vector_3d<double>, double>))
    .def("__len__", [](vgl_vector_3d<double>){return (size_t)3;})
    .def("__getitem__", getitem_3d<vgl_vector_3d<double> >)
    .def_property_readonly("x", &vgl_vector_3d<double>::x)
    .def_property_readonly("y", &vgl_vector_3d<double>::y)
    .def_property_readonly("z", &vgl_vector_3d<double>::z)
    .def("length", &vgl_vector_3d<double>::length)
    .def(py::self + py::self)
    .def(py::self - py::self);

  py::class_<vgl_ray_3d<double> >(m, "ray_3d")
    .def(py::init<vgl_point_3d<double>, vgl_vector_3d<double> >())
    .def_property_readonly("origin", &vgl_ray_3d<double>::origin)
    .def_property_readonly("direction", &vgl_ray_3d<double>::direction);

  py::class_ <vgl_rotation_3d<double> > (m, "rotation_3d")
    .def(py::init<vnl_vector_fixed<double,4> >())
    .def(py::init<vnl_matrix_fixed<double,3,3> >())
    .def("as_matrix", &vgl_rotation_3d<double>::as_matrix)
    .def("as_quaternion", &vgl_rotation_3d<double>::as_quaternion)
    .def(py::self * py::self);

  
}
}}
