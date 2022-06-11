_obj=DAna
_type=Export
_log=${_obj}.${_type}.log
DAna -x |$ tee ${_log} > DAna.test.config
_DSS_TEST_STATUS=${PIPESTATUS[0]}
if [ ${_DSS_TEST_STATUS} -ne 0 ]; then
    export _DSS_TEST_FATAL="${_type}"
    export _DSS_TEST_LOG="${PWD}/${_log}"
    return
fi

_obj=DAna
_type=Test
_log=${_obj}.${_type}.log
DAna -c DAna.test.config |& tee ${_log} # 1>/dev/null
_DSS_TEST_STATUS=${PIPESTATUS[0]}
if [ ${_DSS_TEST_STATUS} -ne 0 ]; then
    export _DSS_TEST_FATAL="${_type}"
    export _DSS_TEST_LOG="${PWD}/${_log}"
    return
fi

_obj=ROOT
_type=Draw
_log=${_obj}.${_type}.log
rootdrawtree -i dp_ana.root -t dp -hs "hECAL=ECAL_E_total[0]" -o DAna.hist.root |& tee ${_log} # 1>/dev/null
rootprint -f png DAna.hist.root:hECAL |& tee -a ${_log} # 1>/dev/null
_DSS_TEST_STATUS=${PIPESTATUS[0]}
if [ ${_DSS_TEST_STATUS} -ne 0 ]; then
    export _DSS_TEST_FATAL="${_type}"
    export _DSS_TEST_LOG="${PWD}/${_log}"
    return
fi
