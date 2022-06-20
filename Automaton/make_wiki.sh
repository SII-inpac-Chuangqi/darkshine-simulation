#!/bin/bash
apt-get update && apt-get install -y gettext jq
# this script will build wiki and put it into the correct folder in test
# then it will be uploaded automatically
# note: this could only add something and upload the plots, never delete nor edit
# scheme:
#  - User scripts add plots and md block
#  - Oscar generate the test/wiki/xxx.md and test/wiki/plots/xxxx
#  - Oscar call the uploader and specify the uploader folder and teh way update the sidebar
#   - first upload all plots one by one, collect targeted url 
#   - update the wiki content
#   - upload the wiki page

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

mkdir -p ${_DSS_TEST_PREFIX}/test
cd ${_DSS_TEST_PREFIX}/test
#update perf stats
export DSS_PERF_STATS="`grep "Performance Statistics" DSimu.Default.log`"
#
for wiki in validation.md
do
    envsubst < ${wiki} >> wiki.md |& tee ${_DSS_TEST_PREFIX}/test/DSS.wiki.log
    _DSS_TEST_STATUS=${PIPESTATUS[0]}
    if [ ${_DSS_TEST_STATUS} -ne 0 ]; then
        diagnostic
        echo "Oscar:Wiki=Fail#build#{wiki}@${_DSS_TEST_STATUS}"
        exit 2
    fi
done

${_DSS_TEST_PREFIX}/Automaton/update_wiki wiki.md ${CI_COMMIT_SHORT_SHA} "${CI_COMMIT_TIMESTAMP} ${CI_COMMIT_SHORT_SHA} : ${CI_COMMIT_TITLE}" Validation-History Validation-History  |& tee -a ${_DSS_TEST_PREFIX}/test/DSS.wiki.log
_DSS_TEST_STATUS=${PIPESTATUS[0]}
if [ ${_DSS_TEST_STATUS} -ne 0 ]; then
    diagnostic
    echo "Oscar:Wiki=Fail#upload#${wiki}#${_DSS_TEST_STATUS}"
    exit 3
fi

cd ${_DSS_TEST_ORIGIN}
echo "Oscar:Wiki=Done#@"
exit 0