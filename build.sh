#!/usr/bin/bash

SHELL_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_DIR=${SHELL_DIR}/build
if [ ! -d ${BUILD_DIR} ]; then
    mkdir -p ${BUILD_DIR}
fi

cd ${BUILD_DIR} && rm -rf ./*
cmake .. $@ && make
cp ${BUILD_DIR}/compile_commands.json ${SHELL_DIR}