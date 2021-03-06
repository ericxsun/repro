#!/bin/bash
set -e -u

# Expose Python environment to MATLAB such that we can call pymodule without any
# install steps.

source_dir=$(cd $(dirname $BASH_SOURCE) && pwd)
cd $source_dir

pymodule_dir=$source_dir/../../../pymodule

# Source Bazel python environment
source ${pymodule_dir}/env/setup_target_env.sh \
    //python/bindings/pymodule/sub:sub

# Ensure we can run the test script directly. Fail fast if this does not work.
python ${pymodule_dir}/sub/test/inherit_check_test.py
python ${pymodule_dir}/sub/test/func_ptr_test.py

# Start MATLAB, running startupProject MATLAB function.
matlab -r "run('../startupProject');"

# # https://www.mathworks.com/matlabcentral/newsreader/view_thread/241754
# matlab -nojvm -nosplash -D"valgrind --error-limit=no --tool=memcheck" -r "run('../startupProject');"
