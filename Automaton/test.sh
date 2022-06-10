#!/bin/bash
export _DSS_TEST_ORIGIN=${PWD}
cd ..
if [[ ! -f "CMakeLists.txt" ]]; then
    echo "Oscar:Build=Wrong#Path@${_DSS_TEST_ORIGIN}"
    exit 1
fi

export _DSS_TEST_PREFIX=${PWD}
export LD_LIBRARY_PATH="${_DSS_TEST_PREFIX}/install/lib:${LD_LIBRARY_PATH}"
export PATH="${_DSS_TEST_PREFIX}/install/bin:${PATH}"

mkdir -p ${_DSS_TEST_PREFIX}/test
cd ${_DSS_TEST_PREFIX}/test
for obj in DP_simu DP_ana
do
    export _DSS_TEST_ROOT=${_DSS_TEST_PREFIX}/${obj}/scripts
    . ${_DSS_TEST_ROOT}/test.cmd
    if [ -n "${_DSS_TEST_FATAL}" ]; then 
        echo "Oscar:Test=Fail#${obj}#${_DSS_TEST_FATAL}@${_DSS_TEST_LOG}"
        exit 2
    fi
done

cd ${_DSS_TEST_ORIGIN}
echo "Oscar:Test=DONE#${_DSS_TEST_WARN}@${_DSS_TEST_LOG}"
exit 0