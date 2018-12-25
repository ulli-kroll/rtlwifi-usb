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

obj-$(CONFIG_RTL8822BU)        += rtl8822bu.o
rtl8822bu-objs	:= rtl8822bu/dm.o \
		   rtl8822bu/fw.o  \
		   rtl8822bu/hw.o  \
		   rtl8822bu/led.o  \
		   rtl8822bu/phy.o  \
		   rtl8822bu/pwrseq.o  \
		   rtl8822bu/rf.o  \
		   rtl8822bu/sw.o  \
		   rtl8822bu/table.o  \
		   rtl8822bu/trx.o \
		   rtlwifi-usb.o
