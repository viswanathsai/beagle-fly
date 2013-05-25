#!/bin/sh

# toggle io
#define macro
echo "Merge gpio "

#this demo toggle pin R30_1 (P9_29)and R30_2 (P9_30)with input of R31_0(P9_31),
#this demo can run with pwm 
#setup ehrpwm.1:0 as input which is pin 36 on P8, connect pwm signal as input
# and out 2 pwm signals to  R30_1 and R30_2
# 
# steps to setup pwm ehrpwm.1:0
#
#    cd /sys/class/pwm/ehrpwm.1\:0
#    cat request
#    echo 1 > request
#    echo 1 > run
#    echo 100 > period_freq
#    echo 10 > duty_percent

#define macro
dm(){ devmem2 $1 w $2 | sed -n 3p | awk '{ print $6 }' ;}

#load pru
modprobe uio_pruss && dm 0x44e00c00 0

# setup io pin
#echo 0x05 > /sys/kernel/debug/omap_mux/mcasp0_axr0
#echo 0x05 > /sys/kernel/debug/omap_mux/mcasp0_fsx
echo 0x26 > /sys/kernel/debug/omap_mux/mcasp0_axr0
echo 0x26 > /sys/kernel/debug/omap_mux/mcasp0_fsx
echo 0x26 > /sys/kernel/debug/omap_mux/mcasp0_aclkx

#setup pwm
echo 0x2 > /sys/kernel/debug/omap_mux/lcd_data10
echo 1 > /sys/class/pwm/ehrpwm.1:0/request
echo 1 > /sys/class/pwm/ehrpwm.1:0/run
echo 100 >  /sys/class/pwm/ehrpwm.1:0/period_freq
echo 50 >  /sys/class/pwm/ehrpwm.1:0/duty_percent







