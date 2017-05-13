#!/bin/bash

# BASH v4+ required!

declare -A _Person_=()
declare -A _Team_=()
_uid_=0

source helpers.bashc

_g_SUCCESS_CODE=0;
_g_FAILURE_CODE=0;

_f_init() {
    _g_SUCCESS_CODE=0;
    _g_FAILURE_CODE=1;
}

_f_import() {
    local _l_file="$1"
    source $1
}

_f_print() {
    local _l_message="$1"
    echo "$1"
}

_f_replaceFirst() {
    local _l_x=""
    _l_x=$(sed "0,/{}/{s/{}/$2/}" <<< "$1")
    echo ${_l_x}
}

_f_stringf_1() {
    local _l_message="$1"
    local _l_val1="$2"
    local _l_val2="$3"
    local _l_x=""
    _l_x="${_l_message}"
    _l_x="$(_f_replaceFirst "${_l_x}" "${_l_val1}")"
    _l_x="$(_f_replaceFirst "${_l_x}" "${_l_val2}")"
    echo ${_l_x}
}

_f_stringf_2() {
    local _l_message="$1"
    local _l_val1="$2"
    echo $(_f_stringf_1 "${_l_message}" "${_l_val1}" "")
}

_c_Team_f_toString() {
    local _l_this="$1"
    echo $(_f_stringf_1 "Team {} has {} members" "${_Team_[${_l_this},"_v_name"]}" "2")
    echo $(_f_stringf_1 "First is: {}. Second is: {}" "${_Person_[${_Team_[${_l_this},"_v_p1"]},"_v_name"]}" "${_Person_[${_Team_[${_l_this},"_v_p2"]},"_v_name"]}")
}

_f_main() {
    local _l_name="$1"
    
    if [[ "${_l_name}" == "" ]]
    then
        _f_print "Pleas enter a name as an argument when running this script"
        return ${_g_FAILURE_CODE}
    fi

    _f_init

    _f_print "$(_f_stringf_2 "Hello {}, welcome to BashClass" ${_l_name})"
    
    local _l_i=0
    _l_i=0
    while [ ${_l_i} -lt 10 ]
    do
        # [START CREATING TEAM]
        # Create object of Team
        ((_uid_++))
        local _l_team=${_uid_}
        _Team_[${_uid_},"_uid_"]=${_uid_}

        # Initialize all default values
        _Team_[${_uid_},"_v_name"]=""

        # Create object of Person
        ((_uid_++))
        _Team_[${_l_team},"_v_p1"]=${_uid_}

        # Initialize all default values
        _Person_[${_uid_},"_v_name"]=""
        _Person_[${_uid_},"_v_id"]=0

        # Create object of Person
        ((_uid_++))
        _Team_[${_l_team},"_v_p2"]=${_uid_}

        # Initialize all default values
        _Person_[${_uid_},"_v_name"]=""
        _Person_[${_uid_},"_v_id"]=0
        # [END CREATING TEAM]

        _Team_[${_l_team},"_v_name"]="Bash team #${_l_i}"
        _Person_[${_Team_[${_l_team},"_v_p1"]},"_v_name"]="One"
        _Person_[${_Team_[${_l_team},"_v_p1"]},"_v_id"]=1
        _Person_[${_Team_[${_l_team},"_v_p2"]},"_v_name"]="Two"
        _Person_[${_Team_[${_l_team},"_v_p2"]},"_v_id"]=2

        _c_Team_f_toString ${_l_team}

        _l_i=$(((${_l_i} + 1)))
    done


    echo Thank you for using BashClass
    echo Good bye!

    return $_g_SUCCESS_CODE
}

_f_main $1
