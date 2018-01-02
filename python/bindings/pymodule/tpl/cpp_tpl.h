#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include "cpp/name_trait.h"
#include "cpp/type_pack.h"
#include "python/bindings/pymodule/tpl/cpp_tpl_types.h"

// TODO: Figure out how to handle literals...

namespace py = pybind11;

template <typename ... Args>
inline py::tuple get_py_types(type_pack<Args...> = {}) {
  return TypeRegistry::GetPyInstance().GetPyTypes<Args...>();
}

template <typename ParamList, typename InstantiationFunc>
void RegisterInstantiations(
    py::object tpl, const InstantiationFunc& instantiation_func,
    ParamList = {}) {
  auto add_instantiation = [&](auto param) {
    // Register instantiation in `pybind`, using lambda
    // `auto`-friendly syntax., indexing by canonical Python types.
    tpl.attr("add_instantiation")(
        get_py_types(param),
        instantiation_func(param));
  };
  ParamList::template visit<no_tag>(add_instantiation);
}

template <typename ParamList, typename InstantiationFunc>
py::object RegisterTemplateFunction(
    py::handle scope, const std::string& name,
    const InstantiationFunc& instantiation_func, ParamList param_list = {}) {
  const bool is_method = py::isinstance(scope, py::eval("type"));
  // Add property / descriptor if it does not already exist.
  py::object tpl = py::getattr(scope, name.c_str(), py::none());
  if (tpl.is(py::none())) {
    py::handle cpp_tpl = py::module::import("pymodule.tpl.cpp_tpl");
    if (is_method) {
      tpl = cpp_tpl.attr("TemplateMethod")(name, scope);
    } else {
      tpl = cpp_tpl.attr("TemplateFunction")(name);
    }
    py::setattr(scope, name.c_str(), tpl);
  }
  // Ensure that pybind is aware that it's a function.
  auto cpp_instantiation_func =
      [instantiation_func, scope, tpl, is_method](auto param) {
    std::string instantiation_name =
        py::cast<std::string>(
            tpl.attr("_get_instantiation_name")(get_py_types(param)));
    auto func = instantiation_func(param);
    auto name = py::name(instantiation_name.c_str());
    if (is_method) {
      return py::cpp_function(func, name, py::is_method(scope));
    } else {
      return py::cpp_function(func, name);
    }
  };
  RegisterInstantiations(tpl, cpp_instantiation_func, param_list);
  return tpl;
}
