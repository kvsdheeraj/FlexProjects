#!/bin/bash

make tree V=android_Debug_aarch64 CDSP_FLAG=1 clean

make tree V=hexagon_Debug_dynamic_toolv83_v60 clean

make tree V=android_Debug_aarch64 CDSP_FLAG=1

make tree V=hexagon_Debug_dynamic_toolv83_v60

adb push android_Debug_aarch64/dsp /vendor/bin

adb push hexagon_Debug_dynamic_toolv83_v60/ship/libdsp_skel.so /vendor/lib/rfsa/adsp/

#adb push android_Debug_aarch64/mimosadsp /vendor/bin/

#adb push hexagon_Debug_dynamic_toolv83_v60/ship/libmimosadsp_skel.so /vendor/lib/rfsa/adsp/

