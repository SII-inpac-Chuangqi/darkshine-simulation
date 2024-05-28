_obj=DSimu
_type=Default
_log=${_obj}.${_type}.log
cp ${_DSS_TEST_ROOT}/default.yaml .
cp ${_DSS_TEST_ROOT}/*.root  .
sed -i "/  beam_on:/s/.*/  beam_on: 5000/" default.yaml
sed -i "s:signal_lookup_table\: \":signal_lookup_table\: \"${PWD}/:" default.yaml
sed -i "s:mag_field_input\: \":mag_field_input\: \"${PWD}/:" default.yaml
parDSS.sh Geom |& tee -a ${_log}
parDSS.sh Simu ${_DSS_NPROC} |& tee -a ${_log}
#DSimu -y default.yaml -b 50000 |& tee ${_log} # 1>/dev/null
_DSS_TEST_STATUS=${PIPESTATUS[0]}
if [ ${_DSS_TEST_STATUS} -ne 0 ]; then
    export _DSS_TEST_FATAL="${_type}"
    export _DSS_TEST_LOG="${PWD}/${_log}"
    return
fi