_obj=DSimu
_type=Default
_log=${_obj}.${_type}.log
cp ${_DSS_TEST_ROOT}/default.yaml .
DSimu -y default.yaml -b 6000 |& tee ${_log} # 1>/dev/null
_DSS_TEST_STATUS=${PIPESTATUS[0]}
if [ ${_DSS_TEST_STATUS} -ne 0 ]; then
    export _DSS_TEST_FATAL="${_type}"
    export _DSS_TEST_LOG="${PWD}/${_log}"
    return
fi