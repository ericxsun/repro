#include <cstddef>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

#include <pybind11/embed.h>

namespace py = pybind11;
using namespace py::literals;
using namespace std;

void custom_init_move(py::module& m);

int main(int, char**) {
  py::scoped_interpreter guard{};

  cout << "Start" << endl;

//  py::module m;
//  custom_init_move(m);

  cout << "Registered" << endl;

  py::dict globals = py::globals();
//  globals["move"] = m;

  cout << "Eval" << endl;

  py::eval(
R"(def create_obj():
    return move.Test(10)

obj = move.Test(10)
print(obj.value())
)", py::globals());

  py::eval(
R"(
obj = move.check_creation(create_obj, False)
print(obj.value())
)", py::globals());

  cout << "Done" << endl;

  return 0;
}
