_obj=DSimu
_type=Default
_log=${_obj}.${_type}.log
DSimu -y ${_DSS_TEST_ROOT}/default.yaml |& tee ${_log} # 1>/dev/null
_DSS_TEST_STATUS=$?
if [ ${_DSS_TEST_STATUS} -ne 0 ]; then
    export _DSS_TEST_FATAL="${_type}"
    export _DSS_TEST_LOG="${PWD}/${_log}"
    return
fi