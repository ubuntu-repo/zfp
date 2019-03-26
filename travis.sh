#!/usr/bin/env sh
set -e

# pass additional args in $1 (starting with whitespace character)
run_all () {
#  run_all_cmd="ctest -V -C Debug -DC_STANDARD=${C_STANDARD:-99} -DCXX_STANDARD=${CXX_STANDARD:-98} -S \"$TRAVIS_BUILD_DIR/cmake/travis.cmake\""
  run_all_cmd="cmake .. -DC_STANDARD=${C_STANDARD:-99} -DCXX_STANDARD=${CXX_STANDARD:-98}"
  eval "${run_all_cmd}$1"

  cmake --build .
  ctest -V -C Debug
}

mkdir build
cd build

if [ -n "${COVERAGE}" ]; then
  # build
  run_all " -DBUILD_CFP=ON -DBUILD_PYTHON=ON -DBUILD_OPENMP=ON -DBUILD_CUDA=OFF -DWITH_COVERAGE=ON"
else
  # build/test without OpenMP, with CFP (and custom namespace) and zfPy
  run_all " -DBUILD_CFP=ON -DBUILD_PYTHON=ON -DCFP_NAMESPACE=cfp2 -DBUILD_OPENMP=OFF -DBUILD_CUDA=OFF"

  rm -rf ./* ;

  # if OpenMP available, start a 2nd build with it
  if cmake ../tests/ci-utils/ ; then
    rm -rf ./* ;

    # build/test with OpenMP
    run_all " -DBUILD_OPENMP=ON"
  fi
fi
