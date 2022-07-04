#!/usr/bin/env bash

# Copyright (C) 2019-2020 Lee C. Bussy (@LBussy)

# This file is part of Lee Bussy's LCBUrl.

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

declare CWD GITNAME GITROOT ENVIRONMENTS RESULTS PIO IS_MAC

get_mac() {
    if [[ $OSTYPE == 'darwin'* ]]; then
        IS_MAC=true
    else
        IS_MAC=false
    fi
}

get_pio() {
    echo -e "\nChecking PlatformIO environment."
    if grep -q "WSL" -s /proc/version; then
        # Running WSL
        PIO="/mnt/c/Users/$LOGNAME/.platformio/penv/Scripts/platformio.exe"
    elif grep -q "@WIN" -s /proc/version; then
        # Running Git Bash
        PIO="$HOME/.platformio/penv/Scripts/platformio.exe"
    else
        # Running some form of Linux
        PIO="$HOME/.platformio/penv/bin/platformio"
    fi
    if [[ ! -f "$PIO" ]]; then
        # PIO does not exist
        echo -e "\nERROR: Unable to find PlatformIO."
        exit
    fi
}

get_git() {
    if [[ ! -f $(which git 2>/dev/null) ]]; then
        echo -e "\nERROR: Git not found."
        exit
    fi
    echo -e "\nDetermining git root."
    if [[ ! $(git status 2>/dev/null) ]]; then
        echo -e "\nERROR: Git repository not found."
        exit
    fi
    GITROOT=$(git rev-parse --show-toplevel)
    GITNAME=$(git rev-parse --show-toplevel)
    GITNAME=${GITROOT##*/}
}

check_root() {
    local CWD
    CWD=$(pwd)
    if [[ ! -d "$GITROOT" ]]; then
        echo -e "\nERROR: Repository not found."
        exit
    fi
    cd "$GITROOT" || exit
}

get_envs() {
    echo -e "\nGathering build environments for $GITNAME."
    cd "$GITROOT" || exit
    if [ $IS_MAC = true ]; then
        # ENVIRONMENTS=( $("$PIO" project data | grep "env_name" | cut -d'"' -f2) )
        while IFS='' read -r line; do ENVIRONMENTS+=("$line"); done < <("$PIO" project data | grep "env_name" | cut -d'"' -f2)
    else
        readarray -t ENVIRONMENTS < <("$PIO" project data | grep "env_name" | cut -d'"' -f2)
    fi
}

list_envs() {
    local env
    echo -e "\nProcessing the following environments for $GITNAME:"
    for env in "${ENVIRONMENTS[@]}"
    do
        [[ -n "$env" ]] && echo -e "\t$env"
    done
    sleep 3
}

copy_example() {
    echo -e "\nCopying the example script for $GITNAME:"
    cp "$GITROOT/examples/Basic/Basic.ino" "$GITROOT/src/main.cpp" || exit
}

build_binaries() {
    local env
    cd "$GITROOT" || (echo -e "Environment not found." && exit)
    for env in "${ENVIRONMENTS[@]}"
    do
        if [ -n "$env" ]; then
            echo -e "\nBuilding binaries for $env."
            sleep 3
            eval "$PIO" run -e "$env"
            RESULTS+=("$?")
        fi
    done
}

clean_example() {
    echo -e "\nCleaning the example script from $GITNAME:"
    rm "$GITROOT/src/main.cpp" || exit
}

list_results() {
    local env num res
    num=0
    echo -e "\nBuild results for $GITNAME:"
    for env in "${ENVIRONMENTS[@]}"
    do
        if [ "${RESULTS[$num]}" ]; then res="Ok"; else res="Failed";fi
        [[ -n "$env" ]] && echo -e "\t$env:\t$res"
        ((num+=1))
    done
    sleep 3
}

main() {
    get_mac "$@"
    get_pio "$@"
    get_git "$@"
    check_root "$@"
    get_envs "$@"
    list_envs "$@"
    copy_example "$@"
    build_binaries "$@"
    clean_example "$@"
    list_results "$@"
    cd "$CWD" || exit
    echo -e "\nTest build for $GITNAME complete:\n"
}

main "$@" && exit 0
