_obj=DPlot
_type=ECAL
_log=${_obj}.${_type}.log
cp ${_DSS_TEST_ROOT}/ref.dp_ana.root .
cp ${_DSS_TEST_ROOT}/validation.yaml .
cp ${_DSS_TEST_ROOT}/validation.md .
DPlot -c validation.yaml |& tee ${_log}
_DSS_TEST_STATUS=${PIPESTATUS[0]}
if [ ${_DSS_TEST_STATUS} -ne 0 ]; then
    export _DSS_TEST_FATAL="${_type}"
    export _DSS_TEST_LOG="${PWD}/${_log}"
    return
fi

