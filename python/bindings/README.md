# Note

Sources adapted from:

* https://github.com/RobotLocomotion/module/tree/e132af9/module/bindings

# Structure

* `README.md` - instructions.
* `pymodule/`
    * `_typebinding.cc` - `pybind11` module file.
    * `_util/py_relax.h` - relaxed casting prototype.
    * `test/testTypeBinding.py` - unittest showing behavior.
* `mlmodule/`
    * `PyProxy.m` - hacky proxy wrapper
    * `NumPyProxy.m` - hacky NumPy proxy wrapper (allows MATLAB-syntax slicing, etc.)
    * `test/`
        * `matlab_with_pythonpath.sh` - will build and source environment to use Bazel's Python modules, and start MATLAB with the path setup.
        * `TypeBindingTest.m` - MATLAB unittest.
        * `run_all.m` - Run example script and unittest.
        * `matlab_with_pythonpath_drake.sh` - see below.
* `install_prereqs.sh` - minimal install for using Bazel, tested on basic Docker image (see `:/shell/docker/docker-x11.sh`).

# pybind11 within MATLAB using Bazel

To see Python bindings used from within MATLAB, in `bash`:

    $ ./mlmodule/test/matlab_with_pythonpath.sh

and in MATLAB:
    
    >> run_all

## With `drake`

If you have a recent version of `drake`, you can test this out with the current bindings (minus type relaxations, which require C++ modifications).

First, `export DRAKE=/path/to/drake-distro` in your `bash` session, then execute:

    $ ./mlmodule/test/matlab_with_pythonpath_drake.sh

and in MATLAB:

    >> example_drake
