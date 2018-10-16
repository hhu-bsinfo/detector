#!/bin/bash

##
# Check if the installed version of the ibmad-library supports all 64-bit error counters.
#
# ret stdout: 1 if the counters are supported;
#             0 if the counters are not supported
##

MAD_PATH="/usr/include/infiniband/mad.h"

counters=(IB_PC_EXT_ERR_RCV_F IB_PC_EXT_ERR_PHYSRCV_F IB_PC_EXT_ERR_SWITCH_REL_F IB_PC_EXT_XMT_DISCARDS_F \
          IB_PC_EXT_ERR_XMTCONSTR_F IB_PC_EXT_ERR_RCVCONSTR_F IB_PC_EXT_ERR_LOCALINTEG_F \
          IB_PC_EXT_ERR_EXCESS_OVR_F IB_PC_EXT_XMT_WAIT_F)

if [ ! -f ${MAD_PATH} ]; then
    printf "0"
    exit
fi

for counter in ${counters[*]}; do
    string=$(cat ${MAD_PATH} 2>/dev/null | grep ${counter})

    if [ -z ${string} ]; then
        printf "0"
        exit
    fi
done

printf "1"
exit