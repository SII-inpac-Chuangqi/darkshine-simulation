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
mkdir -p build
mkdir -p install
mkdir -p test

cd ${_DSS_TEST_PREFIX}/build
cmake -DWITH_GEANT4_UIVIS=OFF -DCMAKE_INSTALL_PREFIX=${_DSS_TEST_PREFIX}/install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../ |& tee ${_DSS_TEST_PREFIX}/test/DSS.build.log 1>/dev/null \
                && make install -j16 |& tee -a ${_DSS_TEST_PREFIX}/test/DSS.build.log 1>/dev/null
_DSS_TEST_STATUS=$?
if [ ${_DSS_TEST_STATUS} -ne 0 ]; then
    diagnostic
    cat ${_DSS_TEST_PREFIX}/test/DSS.build.log
    echo "Oscar:Build=Fail#Build@${_DSS_TEST_PREFIX}/test/DSS.build.log"
    exit 2
fi

cd ${_DSS_TEST_ORIGIN}
echo "Oscar:Build=Done#@"
exit 0
