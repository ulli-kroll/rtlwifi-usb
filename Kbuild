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

obj-$(CONFIG_RTL8821AU)	+= rtl8821au.o
rtl8821au-objs	:= rtl8821au/dm.o \
		   rtl8821au/fw.o  \
		   rtl8821au/hw.o  \
		   rtl8821au/led.o  \
		   rtl8821au/phy.o  \
		   rtl8821au/pwrseq.o  \
		   rtl8821au/rf.o \
		   rtl8821au/sw.o  \
		   rtl8821au/table.o  \
		   rtl8821au/trx.o \
		   rtlwifi-usb.o
