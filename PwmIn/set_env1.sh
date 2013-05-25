#!/bin/sh
# setup environment for ReadPWM

echo "Set up environment for read PWM "


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

# echo "setup pin mux for PRU0"
# # setup io pin for PRU0
# echo "# p9 - 24  uart1_txd"
# echo 0x26 > /sys/kernel/debug/omap_mux/uart1_txd		
# echo "# p9 - 25  mcasp0_ahclkx"
# echo 0x26 > /sys/kernel/debug/omap_mux/mcasp0_ahclkx
# echo "# p9 - 27  mcasp0_fsr"
# echo 0x26 > /sys/kernel/debug/omap_mux/mcasp0_fsr
# echo "# p9 - 28  mcasp0_ahclkr"
# echo 0x26 > /sys/kernel/debug/omap_mux/mcasp0_ahclkr
# echo "# p9 - 29  mcasp0_fsx"
# echo 0x26 > /sys/kernel/debug/omap_mux/mcasp0_fsx
# echo "# p9 - 30  mcasp0_axr0"
# echo 0x26 > /sys/kernel/debug/omap_mux/mcasp0_axr0
# echo "# p9 - 31  mcasp0_aclkx"
# echo 0x26 > /sys/kernel/debug/omap_mux/mcasp0_aclkx


#setup io pin for PRU1
echo "setup pin mux for PRU1"
echo "# p8 - 20  gpmc_csn2"
echo 0x26 > /sys/kernel/debug/omap_mux/gpmc_csn2
echo "# p8 - 21  gpmc_csn1"
echo 0x26 > /sys/kernel/debug/omap_mux/gpmc_csn1
echo "# p8 - 26  gpmc_csn0"
echo 0x26 > /sys/kernel/debug/omap_mux/gpmc_csn0
echo "# p8 - 27  lcd_vsync"
echo 0x26 > /sys/kernel/debug/omap_mux/lcd_vsync
echo "# p8 - 28  lcd_pclk"
echo 0x26 > /sys/kernel/debug/omap_mux/lcd_pclk 
echo "# p8 - 29  lcd_hsync"
echo 0x26 > /sys/kernel/debug/omap_mux/lcd_hsync
echo "# p8 - 30  lcd_ac_bias_en"
echo 0x26 > /sys/kernel/debug/omap_mux/lcd_ac_bias_en
echo "# p8 - 39  lcd_data6"
echo 0x26 > /sys/kernel/debug/omap_mux/lcd_data6
echo "# p8 - 40  lcd_data7"
echo 0x26 > /sys/kernel/debug/omap_mux/lcd_data7
echo "# p8 - 41  lcd_data4"
echo 0x26 > /sys/kernel/debug/omap_mux/lcd_data4
echo "# p8 - 42  lcd_data5"
echo 0x26 > /sys/kernel/debug/omap_mux/lcd_data5
echo "# p8 - 43  lcd_data2"
echo 0x26 > /sys/kernel/debug/omap_mux/lcd_data2
echo "# p8 - 44  lcd_data3"
echo 0x26 > /sys/kernel/debug/omap_mux/lcd_data3
echo "# p8 - 45  lcd_data0"
echo 0x26 > /sys/kernel/debug/omap_mux/lcd_data0
echo "# p8 - 46  lcd_data1"
echo 0x26 > /sys/kernel/debug/omap_mux/lcd_data1

echo "setup pwm on p8-36 "
#setup pwm
echo 0x2 > /sys/kernel/debug/omap_mux/lcd_data10
echo 1 > /sys/class/pwm/ehrpwm.1:0/request
echo 1 > /sys/class/pwm/ehrpwm.1:0/run
echo 100 >  /sys/class/pwm/ehrpwm.1:0/period_freq
echo 50 >  /sys/class/pwm/ehrpwm.1:0/duty_percent




