#!/bin/bash
diagnostic(){
    echo "================================"
    export
    ls -al
    echo "================================"
}

export _DSS_TEST_ORIGIN=${PWD}
if [[ ! -f "CMakeLists.txt" ]]; then
    echo "Oscar:Build=Wrong#Path@${_DSS_TEST_ORIGIN}"
    exit 1
fi

export _DSS_TEST_PREFIX=${PWD}
export _DSS_NPROC=10
export LD_LIBRARY_PATH="${_DSS_TEST_PREFIX}/install/lib:${LD_LIBRARY_PATH}"
export PATH="${_DSS_TEST_PREFIX}/install/bin:${PATH}"
export PATH="${_DSS_TEST_PREFIX}/Automaton:${PATH}"

mkdir -p ${_DSS_TEST_PREFIX}/test
cd ${_DSS_TEST_PREFIX}/test
for obj in DP_simu DP_ana Tools
do
    export _DSS_TEST_ROOT=${_DSS_TEST_PREFIX}/${obj}/scripts
    . ${_DSS_TEST_ROOT}/test.sh
    if [ -n "${_DSS_TEST_FATAL}" ]; then 
        diagnostic
        # cat ${_DSS_TEST_LOG}
        echo "Oscar:Test=Fail#${obj}#${_DSS_TEST_FATAL}@${_DSS_TEST_LOG}"
        exit 2
    fi
done

cd ${_DSS_TEST_ORIGIN}
echo "Oscar:Test=DONE#${_DSS_TEST_WARN}@${_DSS_TEST_LOG}"
exit 0
