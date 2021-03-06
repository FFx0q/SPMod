#!/bin/bash

if [[ $LINK_TYPE == "dynamic" ]]
then
    BUILD_DYNAMIC="ON"
else
    BUILD_DYNAMIC="OFF"
fi

mkdir -p ${TRAVIS_BUILD_DIR}/build
cd ${TRAVIS_BUILD_DIR}/build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../upload -DBUILD_DYNAMIC=${BUILD_DYNAMIC} ..
make -j 16 install
