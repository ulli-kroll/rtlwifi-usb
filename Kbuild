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

obj-$(CONFIG_RTL8188EU)        += rtl8188eu.o
rtl8188eu-objs	:= rtl8188eu/dm.o \
		   rtl8188eu/fw.o  \
		   rtl8188eu/hw.o  \
		   rtl8188eu/led.o  \
		   rtl8188eu/phy.o  \
		   rtl8188eu/pwrseq.o  \
		   rtl8188eu/rf.o  \
		   rtl8188eu/sw.o  \
		   rtl8188eu/table.o  \
		   rtl8188eu/trx.o \
		   rtlwifi-usb.o
