#!/bin/bash
if ! command -v jq &> /dev/null
then
    echo "Error: jq could not be found"
    exit -1
fi
if ! command -v envsubst &> /dev/null
then
    echo "Error: envsubst could not be found"
    exit -1
fi

gitlab_url=https://code.ihep.ac.cn
gitlab_project_id=2358
key=$(cat /root/dss/GITLAB_PROJECT_KEY)

# input
if [ "$#" -ne 5 ]
then
    echo "Error: Illegal number of parameters: input_md output_name output_title output_folder update_md" "$#"
    exit 1
fi
input_md=${1}
output_name=${2}
output_title=${3}
output_folder=${4}
update_md=${5}
echo "${input_md}-->[${output_title}](${output_folder}/${output_name})@${update_md}"
# check CI env
if [ "$CI" != "true" ]
then
    echo "Error: Only support run in CI jobs"
    exit 2
fi
# check whether exist
ret_msg=$(curl -s --header "PRIVATE-TOKEN: ${key}" "${gitlab_url}/api/v4/projects/${gitlab_project_id}/wikis/${output_folder}%2F${output_name}")
ret=$(echo "$ret_msg" | jq -r '.slug')
if [ "${ret}" != "null" ]
then
    echo "Error: Wiki exist ${ret}"
    echo "Return message:"
    echo "$ret_msg"
    exit 3
fi 
# first upload all plots one by one, collect targeted url 
global_rematch() { 
    local s=$1 regex=$2 
    while [[ $s =~ $regex ]]; do 
        echo "${BASH_REMATCH[1]}"
        s=${s#*"${BASH_REMATCH[1]}"}
    done
}
mapfile -t images < <( global_rematch "`cat ${input_md}`" "\!\[[a-zA-Z0-9_/.]*\]\(([a-zA-Z0-9_/.]+)\)" )
cp ${input_md} ${input_md}.upload
for i in "${images[@]}"
do
    if [ ! -f ${i} ]
    then
        echo "Warning: File missing ${i} !"
    else
        echo "Info: Uploading ${i}..."
        ret_msg=$(curl -s --request POST --header "PRIVATE-TOKEN: ${key}" \
             --form "file=@${i}" "${gitlab_url}/api/v4/projects/${gitlab_project_id}/wikis/attachments" )
        ret=$(echo "$ret_msg" | jq -r '.file_path')
        if [ "${ret}" != "null" ]
        then
            echo "Ok: ${i}-->${ret}"
            sed -i "s|${i}|${ret}|g" ${input_md}.upload
        else
            echo "Warning: File uploaded failed ${i} !"
            echo "Return message:"
            echo "${ret}"
        fi
    fi
done
#   - update the wiki content
md="`cat ${input_md}.upload`"
ret_msg=$(curl -s --data "format=markdown&title=${output_folder}/${output_name}&content=${md}" \
     --header "PRIVATE-TOKEN: ${key}" "${gitlab_url}/api/v4/projects/${gitlab_project_id}/wikis" )
ret=$(echo "$ret_msg"  | jq -r '.slug')
if [ "${ret}" != "null" ]
then
    echo "Ok: Page uploaded ${ret}"
else
    echo "Error: Page uplaod failed ${input_md}"
    echo "Return message:"
    echo "$ret_msg"
    exit 4
fi    
#   - upload the wiki page -- only add one line, update sidebar is complex...
new_line="[${output_title}](${output_folder}/${output_name})"
ret_msg=$(curl -s --header "PRIVATE-TOKEN: ${key}" "${gitlab_url}/api/v4/projects/${gitlab_project_id}/wikis/${update_md}")
ret=$( echo "$ret_msg" | jq -r '.content')
if [ "${ret}" != "null" ]
then
    echo "Ok: Summary got ${update_md}"
    to_update=${ret}$'\n'$'\n'${new_line}
else
    echo "Error: Summary get failed ${update_md}"
    echo "Return message:"
    echo "$ret_msg"
    exit 5
fi 

ret_msg=$(curl -s --request PUT --data "format=markdown&content=${to_update}" \
     --header "PRIVATE-TOKEN: ${key}" "${gitlab_url}/api/v4/projects/${gitlab_project_id}/wikis/${update_md}")
ret=$(echo "$ret_msg" | jq -r '.slug')
if [ "${ret}" != "null" ]
then
    echo "Ok: Summary updated ${ret}"
else
    echo "Error: Summary update failed ${update_md}"
    echo "Return message:"
    echo "$ret_msg"
    exit 6
fi 

exit 0
