# SPDX-License-Identifier: GPL-2.0
obj-$(CONFIG_RTLWIFI_USB)	+= rtlwifi-usb.o
rtlwifi-usb-objs	:=		\
		base.o		\
		cam.o		\
		core.o		\
		debug.o		\
		efuse.o		\
		ps.o		\
		rc.o		\
		regd.o		\
		stats.o		\
		usb.o

obj-$(CONFIG_RTLBTCOEXIST) += btcoexist.o
btcoexist-objs :=	btcoexist/halbtc8192e2ant.o	\
			btcoexist/halbtc8723b1ant.o	\
			btcoexist/halbtc8723b2ant.o	\
			btcoexist/halbtc8821a1ant.o	\
			btcoexist/halbtc8821a2ant.o	\
			btcoexist/halbtcoutsrc.o	\
			btcoexist/rtl_btc.o

obj-$(CONFIG_RTL8188FU)       += rtl8188fu.o
rtl8188fu-objs	:= rtl8188fu/dm.o \
		   rtl8188fu/fw.o  \
		   rtl8188fu/hw.o  \
		   rtl8188fu/led.o  \
		   rtl8188fu/phy.o  \
		   rtl8188fu/pwrseq.o  \
		   rtl8188fu/rf.o  \
		   rtl8188fu/sw.o  \
		   rtl8188fu/table.o  \
		   rtl8188fu/trx.o \
		   rtlwifi-usb.o
