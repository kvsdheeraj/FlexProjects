#!/bin/bash

cd  control
make tree V=android_Debug_aarch64 CDSP_FLAG=1 clean
make tree V=android_Debug_aarch64 CDSP_FLAG=1
cp -f android_Debug_aarch64/ship/control ../build/bin
cp -f android_Debug_aarch64/ship/control.a ../build/lib
adb push android_Debug_aarch64/control /vendor/bin/

cd ../mics
make tree V=android_Debug_aarch64 CDSP_FLAG=1 clean
make tree V=android_Debug_aarch64 CDSP_FLAG=1
cp -f android_Debug_aarch64/ship/mics ../build/bin
cp -f android_Debug_aarch64/ship/mics.a ../build/lib
adb push android_Debug_aarch64/mics /vendor/bin/


cd ../speaker
make tree V=android_Debug_aarch64 CDSP_FLAG=1 clean
make tree V=android_Debug_aarch64 CDSP_FLAG=1
cp -f android_Debug_aarch64/ship/speaker ../build/bin
cp -f android_Debug_aarch64/ship/speaker.a ../build/lib
adb push android_Debug_aarch64/speaker /vendor/bin/

cd ../dsp
make tree V=android_Debug_aarch64 CDSP_FLAG=1 clean
make tree V=hexagon_Debug_dynamic_toolv83_v60 clean
make tree V=hexagon_Debug_dynamic_toolv83_v60
cp -f hexagon_Debug_dynamic_toolv83_v60/ship/libdsp_skel.so ../build/lib/dsp    
make tree V=android_Debug_aarch64 CDSP_FLAG=1
cp -f android_Debug_aarch64/ship/dsp ../build/bin

adb push android_Debug_aarch64/dsp /vendor/bin/
adb push hexagon_Debug_dynamic_toolv83_v60/ship/libdsp_skel.so /vendor/lib/rfsa/adsp/
