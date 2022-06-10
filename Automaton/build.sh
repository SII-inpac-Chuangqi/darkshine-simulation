#!/bin/bash
export _DSS_TEST_ORIGIN=${PWD}
cd ..
if [[ ! -f "CMakeLists.txt" ]]; then
    ls -al
    echo "Oscar:Build=Wrong#Path@${_DSS_TEST_ORIGIN}"
    exit 1
fi

export _DSS_TEST_PREFIX=${PWD}
mkdir -p build
mkdir -p install

cd ${_DSS_TEST_PREFIX}/build
cmake -DWITH_GEANT4_UIVIS=OFF -DCMAKE_INSTALL__DSS_TEST_PREFIX=${_DSS_TEST_PREFIX}/install -DCMAKE_BUILD_TYPE=RelWithDebInfo ../ >${_DSS_TEST_ORIGIN}/DSS.build.log 2>&1 \
                && make install -j16 >>${_DSS_TEST_ORIGIN}/DSS.build.log 2>&1
_DSS_TEST_STATUS=$?
if [ ${_DSS_TEST_STATUS} -ne 0 ]; then
    echo "Oscar:Build=Fail#Build@${_DSS_TEST_ORIGIN}/DSS.build.log"
    exit 2
fi

cd ${_DSS_TEST_ORIGIN}
echo "Oscar:Build=Done#@"
exit 0
