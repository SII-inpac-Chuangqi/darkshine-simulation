DSimu -y ${_DSS_TEST_ROOT}/default.yaml >DSimu.test.log 2>&1
_DSS_TEST_STATUS=$?
if [ ${_DSS_TEST_STATUS} -ne 0 ]; then
    export _DSS_TEST_FATAL="Default"
    export _DSS_TEST_LOG="${PWD}/DSimu.test.log"
    return
fi