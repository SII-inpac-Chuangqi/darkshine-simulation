DAna -x > DAna.test.config 2>DAna.export.log
_DSS_TEST_STATUS=$?
if [ ${_DSS_TEST_STATUS} -ne 0 ]; then
    export _DSS_TEST_FATAL="Export"
    export _DSS_TEST_LOG="${PWD}/DAna.export.log"
    return
fi

DAna -c DAna.test.config >DAna.test.log 2>&1
_DSS_TEST_STATUS=$?
if [ ${_DSS_TEST_STATUS} -ne 0 ]; then
    export _DSS_TEST_FATAL="Test"
    export _DSS_TEST_LOG="${PWD}/DAna.test.log"
    return
fi

rootdrawtree -i dp_ana.root -t dp -hs "hECAL=ECAL_E_total[0]" -o DAna.hist.root >DAna.draw.log 2>&1
rootprint -f png DAna.hist.root:hECAL >>DAna.draw.log 2>&1
_DSS_TEST_STATUS=$?
if [ ${_DSS_TEST_STATUS} -ne 0 ]; then
    export _DSS_TEST_FATAL="Draw"
    export _DSS_TEST_LOG="${PWD}/DAna.draw.log"
    return
fi
