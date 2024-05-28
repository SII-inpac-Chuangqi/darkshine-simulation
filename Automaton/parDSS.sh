#!/bin/bash

# =================================================
# Default values
# -------------------------------------------------
script_path=$(readlink -f "$0")
basedir=$(dirname "$script_path")
seed_file="${basedir}/seed_positive_int_1E4.txt"
yaml="default.yaml"
config="config.txt"
Geom=false
Simu=false
Ana=false
ReAna=false
Merge=false
Remove=false
checked_simu=false
checked_ana=false
simu_ok=false
ana_ok=false
use_bash=false
use_singularity=false

# =================================================
# Read from argument
# -------------------------------------------------
task=$1
njobs=$2
batch_id=$3

# =================================================
# Bash Colors
# -------------------------------------------------
Color_Off='\033[0m'
Black='\033[0;30m'
Red='\033[0;31m'
Green='\033[0;32m'
Yellow='\033[0;33m'
Blue='\033[0;34m'
Purple='\033[0;35m'
Cyan='\033[0;36m'
White='\033[0;37m'

# =================================================
# Functions
# -------------------------------------------------

print_usage() {
    echo "Usage: $(basename "$0") <task> <njobs> <batch_id[0]>"
    echo "- task     : Geom, Simu, Ana, Merge, Remove, ReAna, all"
    echo "   - Geom  : Generate Geometry.root"
    echo "   - Simu  : Run DSimu in batch."
    echo "             Input: default.yaml"
    echo "             Output: batch_<batch_id>/dp_simu.root"
    echo "   - Ana   : Run DAna in batch."
    echo "             Input: config.txt"
    echo "             Output: batch_<batch_id>/dp_ana.root"
    echo "   - Merge : Check and merge dp_simu.root and dp_ana.root"
    echo "             Input: batch_<batch_id>"
    echo "             Output: dp_ana/, dp_simu/"
    echo "   - Remove: Check and remove batch_<batch_id>"
    echo "   - ReAna : Run DAna after batch_<batch_id> is removed."
    echo "             Input: dp_simu/, config.txt"
    echo "             Output: dp_ana/"
    echo "   - all   : Run Geom, Simu, Ana, Merge, Remove"
    echo "             Input: default.yaml, config.txt"
    echo "             Output: dp_simu/, dp_ana/"
    echo "- njobs    : Number of process."
    echo "- batch_id : Decides jid_begin = batch_id * njobs. Default: 0"
}

run_CMD() {
    if [[ ${use_bash} == true ]]; then
        echo "CMD: ${CMD}"
        ${CMD}
    elif [[ ${use_singularity} == true ]]; then
        if [[ -z ${DSS_SETUP} ]]; then
            DSS_SETUP="/lustre/collider/hepmc/darkshine-docker/darkshine-simulation/setup.sh"
        fi
        binds="/lustre:/lustre,/home:/home"
        echo "CMD: ${CMD}"
        singularity run --bind ${binds} --home /root --cleanenv --no-home --pwd ${PWD} ${DSS_CONTAINER} bash -c ". ${DSS_SETUP} ${DSS_VERSION} && ${CMD}"
    fi
}

run_Geom() {
    if [[ -f Geometry.root ]]; then
        echo "Geometry.root already exists. Skip."
        return
    fi
    CMD="DSimu -y ${yaml} -b 1 --save_geometry true -f Geometry.root"
    echo ${CMD}
    run_CMD > ${logname}/geom.out 2> ${logname}/geom.err
    rm currentEvent.rndm
    rm currentRun.rndm
    # Check after run
    if [[ `tail -n1 ${logname}/geom.out` != "RunManagerKernel is deleted. Good bye :)" ]]; then
        rm Geometry.root
        echo -e "${Red}[ERROR] Geometry generation failed. See log ${logname}/geom.out and ${logname}/geom.err"
        exit
    fi
}

run_Simu() {
    # Get random seed from seed file
    n=$((jid + 1))
    RANDOMSEED=$(sed -n ${n}p ${seed_file})
    # Create and goto jdir
    jdir=${jobsdir}/j${jid}
    mkdir -p ${jdir}
    cd $jdir
    # Inside jdir
    # Check before run
    state_file=DSimu_State
    if [[ ! -f ${state_file} ]]; then
        echo "false" > ${state_file}
    fi
    if [[ $(cat ${state_file}) == "true" ]]; then
        echo "Skip finished DSimu job ${jid}"
        return
    fi
    # Run
    CMD="DSimu -y ${yaml} --save_geometry false --seed ${RANDOMSEED} --Run_Number $jid"
    echo ${CMD}
    run_CMD > simu.out 2> simu.err
    # Check after run
    if [[ `tail -n1 simu.out` == "RunManagerKernel is deleted. Good bye :)" ]]
    then
        echo "true" > ${state_file}
        # perf statistics
        echo "======================================================================"
        echo "DSimu Performance Statistics for job ${jid}:"
        cat simu.out | grep "Performance Statistics"
        echo "======================================================================"
    fi
    cd ${curpwd}
}

run_Ana() {
    jdir=${jobsdir}/j${jid}
    if [[ ! -d ${jdir} ]]; then
        echo -e "${Red}[ERROR] ${jdir} not found."
        return
    fi
    # Inside jdir
    cd ${jdir}
    # Check before run
    state_file=DAna_State
    if [[ ! -f ${state_file} ]]; then
        echo "false" > ${state_file}
    fi
    if [[ $(cat ${state_file}) == "true" ]]; then
        echo "Skip finished DAna job ${jid}"
        return
    fi
    # Run
    CMD="DAna -c ${config}"
    echo ${CMD}
    run_CMD > ana.out 2> ana.err
    # Check after run
    if [[ $(tail -n1 ana.out) == " ==> Done ..." ]]; then
        echo "true" > ${state_file}
        echo "======================================================================"
        echo "DAna Performance Statistics for job ${jid}:"
        sed -n '/---> Run Summary <---/,/======/p' ana.out
    fi
    cd ${curpwd}
}

run_ReAna() {
    # read simu from dp_simu/ then run ana
    jdir=${jobsdir}/j${jid}
    mkdir -p ${jdir}
    if [[ ! -f ${jdir}/dp_simu.root ]]; then
        ln -s ${PWD}/dp_simu/merge_${jid}.root ${jdir}/dp_simu.root
    fi
    if [[ ! -f ${jdir}/DSimu_State ]]; then
        echo "true" > ${jdir}/DSimu_State
    fi
    run_Ana
    if [[ $(cat ${jdir}/DAna_State) == true ]]; then
        mkdir -p "dp_ana"
        mv ${jdir}/dp_ana.root "dp_ana"/merge_${jid}.root
        if [[ -f ${jdir}/Tracker_GNN.root ]];then
            mkdir -p "Tracker_GNN"
            mv ${jdir}/Tracker_GNN.root "Tracker_GNN"/merge_${jid}.root
        fi
    fi
}

run_sequence() {
    if [[ ${Simu} == true ]]; then
        run_Simu
    fi
    if [[ ${Ana} == true ]]; then
        run_Ana
    fi
    if [[ ${ReAna} == true ]]; then
        run_ReAna
    fi
}

run_Check_Simu() {
    simu_ok=true
    simu_list=""
    for jid in $(seq ${jid_begin} 1 ${jid_end})
    do
        state_file=${jobname}/j${jid}/DSimu_State
        if [[ -f ${state_file} ]]; then
            if [[ $(cat ${state_file}) == true ]]; then
                simu_list="${simu_list} ${jobname}/j${jid}/dp_simu.root"
            else
                simu_ok=false
                echo "DSimu job ${jid} not finished."
            fi
        else
            simu_ok=false
            echo "${state_file} not found."
        fi
    done
    checked_simu=true
}

run_Check_Ana() {
    ana_ok=true
    ana_list=""
    gnn_list=""
    for jid in $(seq ${jid_begin} 1 ${jid_end})
    do
        state_file=${jobname}/j${jid}/DAna_State
        if [[ -f ${state_file} ]]; then
            if [[ $(cat ${state_file}) == true ]]; then
                ana_list="${ana_list} ${jobname}/j${jid}/dp_ana.root"
                gnn_list="${gnn_list} ${jobname}/j${jid}/Tracker_GNN.root"
            else
                ana_ok=false
                echo "DAna job ${jid} not finished."
            fi
        else
            ana_ok=false
            echo "${state_file} not found."
        fi
    done
    checked_ana=true
}

run_Merge() {
    mkdir -p ${merge_name}
    target=${merge_name}/merge_${batch_id}.root
    CMD="hadd ${target} ${merge_sources}"
    echo ${CMD}
    run_CMD > ${logname}/${merge_name}_merge_${batch_id}.out 2> ${logname}/${merge_name}_merge_${batch_id}.err
}

# =================================================
# Check tasks
# -------------------------------------------------
case ${task} in
    Geom)
        Geom=true
        echo -e "${Green}-- task     = Geom${Color_Off}"
        ;;
    Simu)
        Simu=true
        echo -e "${Green}-- task     = Simu${Color_Off}"
        ;;
    Ana)
        Ana=true
        echo -e "${Green}-- task     = Ana${Color_Off}"
        ;;
    Merge)
        Merge=true
        echo -e "${Green}-- task     = Merge${Color_Off}"
        ;;
    Remove)
        Remove=true
        echo -e "${Green}-- task     = Remove${Color_Off}"
        ;;
    ReAna)
        ReAna=true
        Remove=true
        echo -e "${Green}-- task     = ReAna${Color_Off}"
        ;;
    all)
        Geom=true
        Simu=true
        Ana=true
        Merge=true
        Remove=true
        echo -e "${Green}-- task     = all${Color_Off}"
        ;;
    -h)
        print_usage
        exit
        ;;
    --help)
        print_usage
        exit
        ;;
    *)
        echo -e "${Red}task ${task} unknown${Color_Off}"
        print_usage
        exit
        ;;
esac

# =================================================
# Check input values
# -------------------------------------------------

# Check njobs
if [[ -z $njobs ]]; then
    if [[ ${task} == "Geom" ]]; then
        njobs=1
    else
        echo -e "${Red}[ERROR] njobs unkonwn${Color_Off}"
        print_usage
        exit
    fi
fi

# Check batch_id
if [[ -z $batch_id ]]; then
    batch_id=0
fi

jid_begin=$((batch_id * njobs))
jid_end=$((jid_begin + njobs - 1))

# Check file number in ReAna mode
if [[ ${ReAna} == true ]]; then
    simu_total_n=$(ls dp_simu/merge_*.root | wc -l)
    simu_left_n=$((simu_total_n - jid_begin))
    if [[ ${simu_left_n} -le 0 ]]; then
        echo -e "${Red}[ERROR] No input simu file starting from dp_simu/merge_${jid_begin}.root"
        exit
    fi
    if [[ ${simu_left_n} < ${njobs} ]]; then
        njobs=${simu_left_n}
        jid_end=$((jid_begin + njobs - 1))
    fi
fi

echo -e "${Green}-- njobs    = ${njobs}${Color_Off}"
echo -e "${Green}-- batch id = ${batch_id}${Color_Off}"

# Check DSS Version
echo -e "${Green}-- DSS_CONTAINER = ${DSS_CONTAINER}${Color_Off}"
if [[ -z ${DSS_CONTAINER} ]]; then
    use_bash=true
    echo -e "${Green}-- Using bash runner.${Color_Off}"
else
    use_singularity=true
    echo -e "${Green}-- Using singularity runner.${Color_Off}"
    if [[ -z ${DSS_VERSION} ]]; then
        echo -e"${Yellow}-- ${DSS_VERSION} not set, using defualt.${Color_Off}"
    else
        echo -e "${Green}-- Using DSS version ${DSS_VERSION}${Color_Off}"
    fi
fi

# Check yaml
if [[ ${Geom} == true || ${Simu} == true ]]; then
    if [[ ! -f ${yaml} ]]; then
        echo -e "${Red}[ERROR] ${yaml} not found!${Color_Off}"
        exit
    fi
fi
yaml=${PWD}/${yaml}

# Check config.txt
if [[ ${Ana} == true || ${ReAna} == true ]]; then
    if [[ ! -f ${config} ]]; then
        echo -e "${Red}[ERROR] ${config} not found!${Color_Off}"
        exit
    fi
fi
config=${PWD}/${config}

# Check if merged file already exists

if [[ ${Simu} == true ]]; then
    if [[ -f "dp_simu/merge_${batch_id}.root" ]]; then
        echo "dp_simu/merge_${batch_id}.root" already exists. Skip jobs.
        exit
    fi
fi

if [[ ${Ana} == true ]]; then
    if [[ -f "dp_ana/merge_${batch_id}.root" ]]; then
        echo "dp_ana/merge_${batch_id}.root" already exists. Skip jobs.
        exit
    fi
fi

# =================================================
# Make jobs dir
# -------------------------------------------------

curpwd=$PWD
jobname=batch_${batch_id}
logname=log/${jobname}
echo "Making ./${jobname}"
if [[ ${task} != "Geom" ]]; then
    mkdir -p ${jobname}
fi
mkdir -p log
mkdir -p ${logname}
jobsdir=$PWD/${jobname}

# =================================================
# Main
# -------------------------------------------------

if [[ ${Geom} == true ]]; then
    run_Geom
    if [[ -f ${config} ]]; then
        echo "Automatic setting InputGeoFile in config.txt"
        sed -i "/InputGeoFile   =/c InputGeoFile   = ${curpwd}/Geometry.root" ${config}
    fi
    if [[ ${Simu} != true ]]; then
        echo "Done."
        exit
    fi
fi

for jid in $(seq ${jid_begin} 1 ${jid_end})
do
    run_sequence &
done
wait

# Merge and Finalize

if [[ ${Merge} == true ]]; then
    cd ${curpwd}
    run_Check_Simu
    run_Check_Ana
    if [[ ${simu_ok} == true ]]; then
        merge_name="dp_simu"
        merge_sources=${simu_list}
        run_Merge &
    fi
    if [[ ${ana_ok} == true ]]; then
        merge_name="dp_ana"
        merge_sources=${ana_list}
        run_Merge &
        merge_name=Tracker_GNN
        merge_sources=${gnn_list}
        run_Merge &
    fi
fi
wait

if [[ ${Remove} == true ]]; then
    if [[ ${checked_simu} == false ]]; then
        run_Check_Simu
    fi
    if [[ ${checked_ana} == false ]]; then
        run_Check_Ana
    fi
    cd ${curpwd}
    if [[ ${simu_ok} != true ]]; then
        echo "Simu / Merge not finished. Keeping ${jobname} dir."
    elif [[ ${ana_ok} != true ]]; then
        echo "Ana / Merge not finished. Keeping ${jobname} dir."
    else
        echo "Remvoing ${jobname} dir."
        rm -r ${jobname}
    fi
fi

echo "Done."
