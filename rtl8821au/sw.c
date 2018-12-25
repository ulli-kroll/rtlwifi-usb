/* SPDX-License-Identifier: GPL-2.0 */
#include "../wifi.h"
#include "../core.h"
#include "../usb.h"
#include "../efuse.h"
#include "../base.h"
#include "sw.h"
#include "dm.h"
#include "phy.h"
#include "reg.h"
#include "rf.h"
#include "trx.h"
#include "hw.h"
#include "led.h"
#include "fw.h"



static int rtl8821au_init_sw_vars(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	int err;
	char *fw_name;

	rtlpriv->dm.dm_initialgain_enable = true;
	rtlpriv->dm.dm_flag = 0;
	rtlpriv->dm.disable_framebursting = false;
	rtlpriv->dm.thermalvalue = 0;
	rtlpriv->cfg->mod_params->sw_crypto =
		rtlpriv->cfg->mod_params->sw_crypto;

	/* for firmware buf */
	rtlpriv->rtlhal.pfirmware = vzalloc(0x4000);
	if (!rtlpriv->rtlhal.pfirmware) {
		pr_err("Can't alloc buffer for fw\n");
		return 1;
	}

	if (IS_HARDWARE_TYPE_8812AU(rtlpriv))
		fw_name = "rtlwifi/rtl8812aufw.bin";
	else
		fw_name = "rtlwifi/rtl8821aufw.bin";

	/* provide name of alternative file */
	pr_info("Loading firmware %s\n", fw_name);
	rtlpriv->max_fw_size = 0x4000;
	err = request_firmware_nowait(THIS_MODULE, 1,
				      fw_name, rtlpriv->io.dev,
				      GFP_KERNEL, hw, rtl_fw_cb);
	if (err) {
		vfree(rtlpriv->rtlhal.pfirmware);
		rtlpriv->rtlhal.pfirmware = NULL;
	}
	return err;
}

MODULE_FIRMWARE("rtlwifi/rtl8812aufw.bin");
MODULE_FIRMWARE("rtlwifi/rtl8821aufw.bin");

static void rtl8821au_deinit_sw_vars(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);

	if (rtlpriv->rtlhal.pfirmware) {
		vfree(rtlpriv->rtlhal.pfirmware);
		rtlpriv->rtlhal.pfirmware = NULL;
	}
}


/* get bt coexist status */
static bool rtl8821au_get_btc_status(void)
{
	return false;
}

static struct rtl_hal_ops rtl8821au_hal_ops = {
	.init_sw_vars = rtl8821au_init_sw_vars,
	.deinit_sw_vars = rtl8821au_deinit_sw_vars,
	.read_eeprom_info = rtl8821au_read_eeprom_info,
	.hw_init = rtl8821au_hw_init,
	.hw_disable = rtl8821au_card_disable,
	.enable_interrupt = rtl8821au_enable_interrupt,
	.disable_interrupt = rtl8821au_disable_interrupt,
	.set_network_type = rtl8821au_set_network_type,
	.set_chk_bssid = rtl8821au_set_check_bssid,
	.set_qos = rtl8821au_set_qos,
	.set_bcn_reg = rtl8821au_set_beacon_related_registers,
	.set_bcn_intv = rtl8821au_set_beacon_interval,
	.update_interrupt_mask = rtl8821au_update_interrupt_mask,
	.get_hw_reg = rtl8821au_get_hw_reg,
	.set_hw_reg = rtl8821au_set_hw_reg,
	.update_rate_tbl = rtl8821au_update_hal_rate_tbl,
	.fill_tx_desc = rtl8821au_tx_fill_desc,
	.fill_tx_cmddesc = rtl8821au_tx_fill_cmddesc,
	.query_rx_desc = rtl8821au_rx_query_desc,
	.set_channel_access = rtl8821au_update_channel_access_setting,
	.radio_onoff_checking = rtl8821au_gpio_radio_on_off_checking,
	.set_bw_mode = rtl8821au_phy_set_bw_mode,
	.switch_channel = rtl8821au_phy_sw_chnl,
	.dm_watchdog = rtl8821au_dm_watchdog,
	.scan_operation_backup = rtl8821au_phy_scan_operation_backup,
	.set_rf_power_state = rtl8821au_phy_set_rf_power_state,
	.led_control = rtl8821au_led_control,
	.set_desc = rtl8821au_set_desc,
	.get_desc = rtl8821au_get_desc,
	.is_tx_desc_closed = rtl8821au_is_tx_desc_closed,
	.tx_polling = rtl8821au_tx_polling,
	.enable_hw_sec = rtl8821au_enable_hw_security_config,
	.set_key = rtl8821au_set_key,
	.init_sw_leds = rtl8821au_init_sw_leds,
	.get_bbreg = rtl8821au_phy_query_bb_reg,
	.set_bbreg = rtl8821au_phy_set_bb_reg,
	.get_rfreg = rtl8821au_phy_query_rf_reg,
	.set_rfreg = rtl8821au_phy_set_rf_reg,
	.fill_h2c_cmd = rtl8821au_fill_h2c_cmd,
	.get_btc_status = rtl8821au_get_btc_status,
	.c2h_ra_report_handler = rtl8821au_c2h_ra_report_handler,
};

static struct rtl_mod_params rtl8821au_mod_params = {
	.sw_crypto = false,
#if 0
	.inactiveps = true,
	.swctrl_lps = false,
	.fwctrl_lps = true,
#endif
	.debug_level = 0,
	.debug_mask = 0,
};

module_param_named(swenc, rtl8821au_mod_params.sw_crypto, bool, 0444);
module_param_named(ips, rtl8821au_mod_params.inactiveps, bool, 0444);
module_param_named(debug_level, rtl8821au_mod_params.debug_level, int, 0644);
module_param_named(debug_mask, rtl8821au_mod_params.debug_mask, ullong, 0644);
MODULE_PARM_DESC(swenc, "Set to 1 for software crypto (default 0)\n");
MODULE_PARM_DESC(ips, "Set to 0 to not use link power save (default 1)\n");
MODULE_PARM_DESC(debug_level, "Set debug level (0-5) (default 0)");
MODULE_PARM_DESC(debug_mask, "Set debug mask (default 0)");

static struct rtl_hal_usbint_cfg rtl8821au_interface_cfg = {
	/* rx */
	.in_ep_num = RTL8821AU_USB_BULK_IN_NUM,
	.rx_urb_num = RTL8821AU_NUM_RX_URBS,
	.rx_max_size = RTL8821AU_SIZE_MAX_RX_BUFFER,
	.usb_rx_hdl = rtl8821au_rx_hdl,
	.usb_rx_segregate_hdl = NULL,
	/* tx */
	.usb_tx_cleanup = rtl8821au_tx_cleanup,
	.usb_tx_post_hdl = rtl8821au_tx_post_hdl,
	.usb_tx_aggregate_hdl = rtl8821au_tx_aggregate_hdl,
	/* endpoint mapping */
	.usb_endpoint_mapping = rtl8821au_endpoint_mapping,
	.usb_mq_to_hwq = rtl8821au_mq_to_hwq,
};

static struct rtl_hal_cfg rtl8821au_hal_cfg = {
	.name = "rtl8821au",
	.ops = &rtl8821au_hal_ops,
	.mod_params = &rtl8821au_mod_params,
	.usb_interface_cfg = &rtl8821au_interface_cfg,

	.maps[SYS_ISO_CTRL] = REG_SYS_ISO_CTRL,
	.maps[SYS_FUNC_EN] = REG_SYS_FUNC_EN,
	.maps[SYS_CLK] = REG_SYS_CLKR,
	.maps[MAC_RCR_AM] = AM,
	.maps[MAC_RCR_AB] = AB,
	.maps[MAC_RCR_ACRC32] = ACRC32,
	.maps[MAC_RCR_ACF] = ACF,
	.maps[MAC_RCR_AAP] = AAP,
	.maps[EFUSE_TEST] = REG_EFUSE_TEST,
	.maps[EFUSE_CTRL] = REG_EFUSE_CTRL,
	.maps[EFUSE_CLK] = 0,
	.maps[EFUSE_CLK_CTRL] = REG_EFUSE_CTRL,
	.maps[EFUSE_PWC_EV12V] = PWC_EV12V,
	.maps[EFUSE_FEN_ELDR] = FEN_ELDR,
	.maps[EFUSE_LOADER_CLK_EN] = LOADER_CLK_EN,
	.maps[EFUSE_ANA8M] = ANA8M,
	.maps[EFUSE_HWSET_MAX_SIZE] = HWSET_MAX_SIZE,
	.maps[EFUSE_MAX_SECTION_MAP] = EFUSE_MAX_SECTION,
	.maps[EFUSE_REAL_CONTENT_SIZE] =  EFUSE_REAL_CONTENT_LEN,
	.maps[EFUSE_OOB_PROTECT_BYTES_LEN] = EFUSE_OOB_PROTECT_BYTES,
	.maps[EFUSE_ACCESS] = REG_EFUSE_ACCESS,

	.maps[RWCAM] = REG_CAMCMD,
	.maps[WCAMI] = REG_CAMWRITE,
	.maps[RCAMO] = REG_CAMREAD,
	.maps[CAMDBG] = REG_CAMDBG,
	.maps[SECR] = REG_SECCFG,
	.maps[SEC_CAM_NONE] = CAM_NONE,
	.maps[SEC_CAM_WEP40] = CAM_WEP40,
	.maps[SEC_CAM_TKIP] = CAM_TKIP,
	.maps[SEC_CAM_AES] = CAM_AES,
	.maps[SEC_CAM_WEP104] = CAM_WEP104,
#if 0
	.maps[RTL_IMR_BCNDMAINT6] = IMR_BCNDMAINT6,
	.maps[RTL_IMR_BCNDMAINT5] = IMR_BCNDMAINT5,
	.maps[RTL_IMR_BCNDMAINT4] = IMR_BCNDMAINT4,
	.maps[RTL_IMR_BCNDMAINT3] = IMR_BCNDMAINT3,
	.maps[RTL_IMR_BCNDMAINT2] = IMR_BCNDMAINT2,
	.maps[RTL_IMR_BCNDMAINT1] = IMR_BCNDMAINT1,
/*	.maps[RTL_IMR_BCNDOK8] = IMR_BCNDOK8,     */   /*need check*/
	.maps[RTL_IMR_BCNDOK7] = IMR_BCNDOK7,
	.maps[RTL_IMR_BCNDOK6] = IMR_BCNDOK6,
	.maps[RTL_IMR_BCNDOK5] = IMR_BCNDOK5,
	.maps[RTL_IMR_BCNDOK4] = IMR_BCNDOK4,
	.maps[RTL_IMR_BCNDOK3] = IMR_BCNDOK3,
	.maps[RTL_IMR_BCNDOK2] = IMR_BCNDOK2,
	.maps[RTL_IMR_BCNDOK1] = IMR_BCNDOK1,
/*	.maps[RTL_IMR_TIMEOUT2] = IMR_TIMEOUT2,*/
/*	.maps[RTL_IMR_TIMEOUT1] = IMR_TIMEOUT1,*/

	.maps[RTL_IMR_TXFOVW] = IMR_TXFOVW,
	.maps[RTL_IMR_PSTIMEOUT] = IMR_PSTIMEOUT,
	.maps[RTL_IMR_BCNINT] = IMR_BCNDMAINT0,
	.maps[RTL_IMR_RXFOVW] = IMR_RXFOVW,
	.maps[RTL_IMR_RDU] = IMR_RDU,
	.maps[RTL_IMR_ATIMEND] = IMR_ATIMEND,
	.maps[RTL_IMR_BDOK] = IMR_BCNDOK0,
	.maps[RTL_IMR_MGNTDOK] = IMR_MGNTDOK,
	.maps[RTL_IMR_TBDER] = IMR_TBDER,
	.maps[RTL_IMR_HIGHDOK] = IMR_HIGHDOK,
	.maps[RTL_IMR_TBDOK] = IMR_TBDOK,
	.maps[RTL_IMR_BKDOK] = IMR_BKDOK,
	.maps[RTL_IMR_BEDOK] = IMR_BEDOK,
	.maps[RTL_IMR_VIDOK] = IMR_VIDOK,
	.maps[RTL_IMR_VODOK] = IMR_VODOK,
	.maps[RTL_IMR_ROK] = IMR_ROK,
	.maps[RTL_IBSS_INT_MASKS] = (IMR_BCNDMAINT0 | IMR_TBDOK | IMR_TBDER),
#endif
	/* Ths is for the core of rtlwifi */
	.maps[RTL_RC_CCK_RATE1M] = DESC_RATE1M,
	.maps[RTL_RC_CCK_RATE2M] =  DESC_RATE2M,
	.maps[RTL_RC_CCK_RATE5_5M] =  DESC_RATE5_5M,
	.maps[RTL_RC_CCK_RATE11M] =  DESC_RATE11M,
	.maps[RTL_RC_OFDM_RATE6M] =  DESC_RATE6M,
	.maps[RTL_RC_OFDM_RATE9M] =  DESC_RATE9M,
	.maps[RTL_RC_OFDM_RATE12M] =  DESC_RATE12M,
	.maps[RTL_RC_OFDM_RATE18M] =  DESC_RATE18M,
	.maps[RTL_RC_OFDM_RATE24M] =  DESC_RATE24M,
	.maps[RTL_RC_OFDM_RATE36M] =  DESC_RATE36M,
	.maps[RTL_RC_OFDM_RATE48M] =  DESC_RATE48M,
	.maps[RTL_RC_OFDM_RATE54M] =  DESC_RATE54M,

	.maps[RTL_RC_HT_RATEMCS7] =  DESC_RATEMCS7,
	.maps[RTL_RC_HT_RATEMCS15] =  DESC_RATEMCS15,

	/*VHT hightest rate*/
	.maps[RTL_RC_VHT_RATE_1SS_MCS7] = DESC_RATEVHT1SS_MCS7,
	.maps[RTL_RC_VHT_RATE_1SS_MCS8] = DESC_RATEVHT1SS_MCS8,
	.maps[RTL_RC_VHT_RATE_1SS_MCS9] = DESC_RATEVHT1SS_MCS9,
	.maps[RTL_RC_VHT_RATE_2SS_MCS7] = DESC_RATEVHT2SS_MCS7,
	.maps[RTL_RC_VHT_RATE_2SS_MCS8] = DESC_RATEVHT2SS_MCS8,
	.maps[RTL_RC_VHT_RATE_2SS_MCS9] = DESC_RATEVHT2SS_MCS9,
};

#define USB_VENDER_ID_REALTEK		0x0BDA

/* DID_USB_v916_20130116 */
static struct usb_device_id rtl8821au_usb_ids[] ={
	/* RTL8812AU */
	/*=== Realtek demoboard ===*/
	{RTL_USB_DEVICE(USB_VENDER_ID_REALTEK, 0x8812, HARDWARE_TYPE_RTL8812AU)},/* Default ID */
	{RTL_USB_DEVICE(USB_VENDER_ID_REALTEK, 0x881A, HARDWARE_TYPE_RTL8812AU)},/* Default ID */
	{RTL_USB_DEVICE(USB_VENDER_ID_REALTEK, 0x881B, HARDWARE_TYPE_RTL8812AU)},/* Default ID */
	{RTL_USB_DEVICE(USB_VENDER_ID_REALTEK, 0x881C, HARDWARE_TYPE_RTL8812AU)},/* Default ID */
	/*=== Customer ID ===*/
	{RTL_USB_DEVICE(0x050D, 0x1109, HARDWARE_TYPE_RTL8812AU)}, /* Belkin F9L1109 - SerComm */
	{RTL_USB_DEVICE(0x2001, 0x330E, HARDWARE_TYPE_RTL8812AU)}, /* D-Link - ALPHA */
	{RTL_USB_DEVICE(0x7392, 0xA822, HARDWARE_TYPE_RTL8812AU)}, /* Edimax - Edimax */
	{RTL_USB_DEVICE(0x0DF6, 0x0074, HARDWARE_TYPE_RTL8812AU)}, /* Sitecom - Edimax */
	{RTL_USB_DEVICE(0x04BB, 0x0952, HARDWARE_TYPE_RTL8812AU)}, /* I-O DATA - Edimax */
	{RTL_USB_DEVICE(0x0789, 0x016E, HARDWARE_TYPE_RTL8812AU)}, /* Logitec - Edimax */
	{RTL_USB_DEVICE(0x0409, 0x0408, HARDWARE_TYPE_RTL8812AU)}, /* NEC - */
	{RTL_USB_DEVICE(0x0B05, 0x17D2, HARDWARE_TYPE_RTL8812AU)}, /* ASUS - Edimax */
	{RTL_USB_DEVICE(0x0E66, 0x0022, HARDWARE_TYPE_RTL8812AU)}, /* HAWKING - Edimax */
	{RTL_USB_DEVICE(0x0586, 0x3426, HARDWARE_TYPE_RTL8812AU)}, /* ZyXEL - */
	{RTL_USB_DEVICE(0x2001, 0x3313, HARDWARE_TYPE_RTL8812AU)}, /* D-Link - ALPHA */
	{RTL_USB_DEVICE(0x1058, 0x0632, HARDWARE_TYPE_RTL8812AU)}, /* WD - Cybertan*/
	{RTL_USB_DEVICE(0x1740, 0x0100, HARDWARE_TYPE_RTL8812AU)}, /* EnGenius - EnGenius */
	{RTL_USB_DEVICE(0x2019, 0xAB30, HARDWARE_TYPE_RTL8812AU)}, /* Planex - Abocom */
	{RTL_USB_DEVICE(0x07B8, 0x8812, HARDWARE_TYPE_RTL8812AU)}, /* Abocom - Abocom */
	{RTL_USB_DEVICE(0x2001, 0x3315, HARDWARE_TYPE_RTL8812AU)}, /* D-Link - Cameo */
	{RTL_USB_DEVICE(0x2001, 0x3316, HARDWARE_TYPE_RTL8812AU)}, /* D-Link - Cameo */
	{RTL_USB_DEVICE(0x20F4, 0x805B, HARDWARE_TYPE_RTL8812AU)}, /* TRENDnet - Cameo */
	{RTL_USB_DEVICE(0x13B1, 0x003F, HARDWARE_TYPE_RTL8812AU)}, /* Linksys - SerComm */
	{RTL_USB_DEVICE(0x2357, 0x0101, HARDWARE_TYPE_RTL8812AU)}, /* TP-Link - T4U */
	{RTL_USB_DEVICE(0x0BDA, 0x8812, HARDWARE_TYPE_RTL8812AU)}, /* Alfa AWUS036AC */

	/* RTL8821AU */
        /*=== Realtek demoboard ===*/
	{RTL_USB_DEVICE(USB_VENDER_ID_REALTEK, 0x0811, HARDWARE_TYPE_RTL8821AU)},/* Default ID */
	{RTL_USB_DEVICE(USB_VENDER_ID_REALTEK, 0x0821, HARDWARE_TYPE_RTL8821AU)},/* Default ID */
	{RTL_USB_DEVICE(USB_VENDER_ID_REALTEK, 0x8822, HARDWARE_TYPE_RTL8821AU)},/* Default ID */
	/*=== Customer ID ===*/
	{RTL_USB_DEVICE(0x7392, 0xA811, HARDWARE_TYPE_RTL8821AU)}, /* Edimax - Edimax */
	{RTL_USB_DEVICE(0x0BDA, 0xA811, HARDWARE_TYPE_RTL8821AU)}, /* OUTLINK - Edimax */
	{RTL_USB_DEVICE(0x7392, 0xA812, HARDWARE_TYPE_RTL8821AU)}, /* Edimax - Edimax */
	{RTL_USB_DEVICE(0x2001, 0x3314, HARDWARE_TYPE_RTL8821AU)}, /* D-Link - Cameo */
	{RTL_USB_DEVICE(0x0846, 0x9052, HARDWARE_TYPE_RTL8821AU)}, /* Netgear - A6100 */
	{RTL_USB_DEVICE(0x0411, 0x0242, HARDWARE_TYPE_RTL8821AU)}, /* BUFFALO - Edimax */
	{}	/* Terminating entry */
};

MODULE_DEVICE_TABLE(usb, rtl8821au_usb_ids);

static int rtl8821au_probe(struct usb_interface *pusb_intf,
			   const struct usb_device_id *id)
{
	struct usb_device *udev;
	char *speed;
	char *driver_info;

	udev = interface_to_usbdev(pusb_intf);
	switch (udev->speed) {
		case USB_SPEED_LOW :	speed = "LOW";
					break;
		case USB_SPEED_FULL :	speed = "FULL";
					break;
		case USB_SPEED_HIGH :	speed = "HIGH";
					break;
		case USB_SPEED_SUPER :	speed = "HIGH";
					break;
		default :		speed = "UNKNOWN";
					break;
	}

	switch (id->driver_info) {
		case HARDWARE_TYPE_RTL8812AU :	driver_info ="RTL8812";
				break;

		case HARDWARE_TYPE_RTL8821AU :	driver_info ="RTL8821";
				break;
	}

#if 0
	dev_info(&pusb_intf->dev, "Register : %s USB-ID %04x:%04x as %s Speed\n",
		driver_info, pdid->idVendor, pdid->idProduct, speed);
#endif
	return rtl_usb_probe(pusb_intf, id, &rtl8821au_hal_cfg);
}


static struct usb_driver rtl8821au_driver = {
	.name = "rtl8821au",
	.probe = rtl8821au_probe,
	.disconnect = rtl_usb_disconnect,
	.id_table = rtl8821au_usb_ids,
#if 0
	.suspend =  rtl8821au_suspend,
	.resume = rtl8821au_resume,
  	.reset_resume   = rtl8821au_resume,
#endif
#ifdef CONFIG_AUTOSUSPEND
	.supports_autosuspend = 1,
#endif

};

module_usb_driver(rtl8821au_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Realtek Wireless Lan Driver");
MODULE_AUTHOR("Hans Ulli Kroll <ulli.kroll@googlemail.com>");
MODULE_VERSION("git. based on v4.2.2_7502.20130517");
