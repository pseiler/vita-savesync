#!/bin/bash
NAME="savemgr"
PUSH=0

if [ "${1}" = "-p" ] && [ ! -z "${2}" ];
then
    PUSH=1;
    MY_IP="${2}";
elif [ "${1}" = "-p" ] && [ -z "${2}" ];
then
    echo "Please add the IP of your PS Vita as the second parameter";
    exit 1
fi;

for i in sce_sys/kernel.skprx sce_sys/user.suprx;
do
    if [ ! -f ${i} ];
    then
        echo "\"${i}\" not found. Please compile VitaShell first and place the modules in the sce_sys directory"
        exit 1
    fi
done


test -d build && rm -rf build;
mkdir build && cd build;
cmake ..;
make
MAKE_RETURN=$?
if [ ${PUSH} -eq 1 ];
then
    if [ ${MAKE_RETURN} -eq 0 ];
    then
        curl -T "${NAME}.vpk" "ftp://${MY_IP}:1337/ux0:/vpk/";
    else
        echo "\"make\" call failed. No upload.";
    fi;
fi;
cd -

exit 0;
