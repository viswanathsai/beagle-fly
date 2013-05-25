#!/bin/sh
# setup environment for serial ttyO1 output

#define macro
echo "define macro dm"
dm(){ devmem2 $1 w $2 | sed -n 3p | awk '{ print $6 }' ;}

echo "Setup envrionment for Pru ICSS"
modprobe uio_pruss && dm 0x44e00c00 0

# setup serial port
echo "Set up environment for ttyO2 out"
# p9-21		UART2_TXD
echo 1 > /sys/kernel/debug/omap_mux/spi0_d0
# p9-22		UART2_RXD
echo 0x21 > /sys/kernel/debug/omap_mux/spi0_sclk

# P9 -24	
#echo 0x0 > /sys/kernel/debug/omap_mux/uart1_txd
# P9 -26
#echo 0x20 > /sys/kernel/debug/omap_mux/uart1_rxd

echo "setup pin mux for PRU0"
# setup io pin for PRU0
echo "# p9 - 24  uart1_txd"
echo 0x26 > /sys/kernel/debug/omap_mux/uart1_txd		
echo "# p9 - 25  mcasp0_ahclkx"
echo 0x26 > /sys/kernel/debug/omap_mux/mcasp0_ahclkx
echo "# p9 - 27  mcasp0_fsr"
echo 0x26 > /sys/kernel/debug/omap_mux/mcasp0_fsr
echo "# p9 - 28  mcasp0_ahclkr"
echo 0x26 > /sys/kernel/debug/omap_mux/mcasp0_ahclkr
echo "# p9 - 29  mcasp0_fsx"
echo 0x26 > /sys/kernel/debug/omap_mux/mcasp0_fsx
echo "# p9 - 30  mcasp0_axr0"
echo 0x26 > /sys/kernel/debug/omap_mux/mcasp0_axr0
echo "# p9 - 31  mcasp0_aclkx"
echo 0x26 > /sys/kernel/debug/omap_mux/mcasp0_aclkx

echo "setup pin mux for PRU1"
#setup io pin for PRU1
echo "setup pin mux for PRU1"
echo "# p8 - 20  gpmc_csn2"
echo 0x5 > /sys/kernel/debug/omap_mux/gpmc_csn2
echo "# p8 - 21  gpmc_csn1"
echo 0x5 > /sys/kernel/debug/omap_mux/gpmc_csn1
echo "# p8 - 27  lcd_vsync"
echo 0x5 > /sys/kernel/debug/omap_mux/lcd_vsync
echo "# p8 - 28  lcd_pclk"
echo 0x5 > /sys/kernel/debug/omap_mux/lcd_pclk 
echo "# p8 - 29  lcd_hsync"
echo 0x5 > /sys/kernel/debug/omap_mux/lcd_hsync
echo "# p8 - 30  lcd_ac_bias_en"
echo 0x5 > /sys/kernel/debug/omap_mux/lcd_ac_bias_en
echo "# p8 - 39  lcd_data6"
echo 0x5 > /sys/kernel/debug/omap_mux/lcd_data6
echo "# p8 - 40  lcd_data7"
echo 0x5 > /sys/kernel/debug/omap_mux/lcd_data7
echo "# p8 - 41  lcd_data4"
echo 0x5 > /sys/kernel/debug/omap_mux/lcd_data4
echo "# p8 - 42  lcd_data5"
echo 0x5 > /sys/kernel/debug/omap_mux/lcd_data5
echo "# p8 - 43  lcd_data2"
echo 0x5 > /sys/kernel/debug/omap_mux/lcd_data2
echo "# p8 - 44  lcd_data3"
echo 0x5 > /sys/kernel/debug/omap_mux/lcd_data3
echo "# p8 - 45  lcd_data0"
echo 0x5 > /sys/kernel/debug/omap_mux/lcd_data0
echo "# p8 - 46  lcd_data1"
echo 0x5 > /sys/kernel/debug/omap_mux/lcd_data1

echo "setup up gpio pin"
echo "# p8 - 3  gpmc_ad6"
echo 0x27 > /sys/kernel/debug/omap_mux/gpmc_ad6
echo 38 > /sys/class/gpio/export
echo in > /sys/class/gpio/gpio38/direction

echo "# p8 - 4  gpmc_ad7"
echo 0x27 > /sys/kernel/debug/omap_mux/gpmc_ad7
echo 39 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio39/direction
echo 0 > /sys/class/gpio/gpio39/value

echo "setup up gpio pin"
echo "# p8 - 5  gpmc_ad2"
echo 0x27 > /sys/kernel/debug/omap_mux/gpmc_ad2
echo 34 > /sys/class/gpio/export
echo in > /sys/class/gpio/gpio34/direction

echo "# p8 - 6  gpmc_ad3"
echo 0x27 > /sys/kernel/debug/omap_mux/gpmc_ad3
echo 35 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio35/direction
echo 0 > /sys/class/gpio/gpio35/value


# setup gpio
# p9-11 gpmc_wait0		gpio_0[30]
echo 0x27 > /sys/kernel/debug/omap_mux/gpmc_wait0
echo 30 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio30/direction
echo 1 > /sys/class/gpio/gpio30/value

# p9-12 gpmc_ben1 		gpio_1[28]
echo 0x27 > /sys/kernel/debug/omap_mux/gpmc_ben1
echo 60 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio60/direction
echo 1 > /sys/class/gpio/gpio60/value

# p9-13 gpmc_wpn  		gpio_0[31]
echo 0x27 > /sys/kernel/debug/omap_mux/gpmc_wpn
echo 31 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio31/direction
echo 1 > /sys/class/gpio/gpio31/value

# p9-14 gpmc_a2			gpio_1[18]
echo 0x27 > /sys/kernel/debug/omap_mux/gpmc_a2
echo 50 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio50/direction
echo 1 > /sys/class/gpio/gpio50/value

# p9-15 gpmc_a0			gpio_1[1]
echo 0x27 > /sys/kernel/debug/omap_mux/gpmc_a0
echo 33 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio33/direction
echo 1 > /sys/class/gpio/gpio33/value

# P9-16 is used for reset zigbee
# p9-16 gpmc_a3			gpio_1[19]
echo 0x27 > /sys/kernel/debug/omap_mux/gpmc_a3
echo 51 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio51/direction
echo 1 > /sys/class/gpio/gpio51/value