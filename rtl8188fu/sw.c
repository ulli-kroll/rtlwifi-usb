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

static int rtl8188fu_init_sw_vars(struct ieee80211_hw *hw)
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

	fw_name = "rtlwifi/rtl8188fufw.bin";

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

MODULE_FIRMWARE("rtlwifi/rtl8188fufw.bin");

static void rtl8188fu_deinit_sw_vars(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);

	if (rtlpriv->rtlhal.pfirmware) {
		vfree(rtlpriv->rtlhal.pfirmware);
		rtlpriv->rtlhal.pfirmware = NULL;
	}
}


/* get bt coexist status */
static bool rtl8188fu_get_btc_status(void)
{
	return false;
}

static struct rtl_hal_ops rtl8188fu_hal_ops = {
	.init_sw_vars = rtl8188fu_init_sw_vars,
	.deinit_sw_vars = rtl8188fu_deinit_sw_vars,
	.read_eeprom_info = rtl8188fu_read_eeprom_info,
	.hw_init = rtl8188fu_hw_init,
	.hw_disable = rtl8188fu_card_disable,
	.enable_interrupt = rtl8188fu_enable_interrupt,
	.disable_interrupt = rtl8188fu_disable_interrupt,
	.set_network_type = rtl8188fu_set_network_type,
	.set_chk_bssid = rtl8188fu_set_check_bssid,
	.set_qos = rtl8188fu_set_qos,
	.set_bcn_reg = rtl8188fu_set_beacon_related_registers,
	.set_bcn_intv = rtl8188fu_set_beacon_interval,
	.update_interrupt_mask = rtl8188fu_update_interrupt_mask,
	.get_hw_reg = rtl8188fu_get_hw_reg,
	.set_hw_reg = rtl8188fu_set_hw_reg,
	.update_rate_tbl = rtl8188fu_update_hal_rate_tbl,
	.fill_tx_desc = rtl8188fu_tx_fill_desc,
	.fill_tx_cmddesc = rtl8188fu_tx_fill_cmddesc,
	.query_rx_desc = rtl8188fu_rx_query_desc,
	.set_channel_access = rtl8188fu_update_channel_access_setting,
	.radio_onoff_checking = rtl8188fu_gpio_radio_on_off_checking,
	.set_bw_mode = rtl8188fu_phy_set_bw_mode,
	.switch_channel = rtl8188fu_phy_sw_chnl,
	.dm_watchdog = rtl8188fu_dm_watchdog,
	.scan_operation_backup = rtl8188fu_phy_scan_operation_backup,
	.set_rf_power_state = rtl8188fu_phy_set_rf_power_state,
	.led_control = rtl8188fu_led_control,
	.set_desc = rtl8188fu_set_desc,
	.get_desc = rtl8188fu_get_desc,
	.is_tx_desc_closed = rtl8188fu_is_tx_desc_closed,
	.tx_polling = rtl8188fu_tx_polling,
	.enable_hw_sec = rtl8188fu_enable_hw_security_config,
	.set_key = rtl8188fu_set_key,
	.init_sw_leds = rtl8188fu_init_sw_leds,
	.get_bbreg = rtl8188fu_phy_query_bb_reg,
	.set_bbreg = rtl8188fu_phy_set_bb_reg,
	.get_rfreg = rtl8188fu_phy_query_rf_reg,
	.set_rfreg = rtl8188fu_phy_set_rf_reg,
	.fill_h2c_cmd = rtl8188fu_fill_h2c_cmd,
	.get_btc_status = rtl8188fu_get_btc_status,
	.c2h_ra_report_handler = rtl8188fu_c2h_ra_report_handler,
};

static struct rtl_mod_params rtl8188fu_mod_params = {
	.sw_crypto = false,
#if 0
	.inactiveps = true,
	.swctrl_lps = false,
	.fwctrl_lps = true,
#endif
	.debug_level = 0,
	.debug_mask = 0,
};

module_param_named(swenc, rtl8188fu_mod_params.sw_crypto, bool, 0444);
module_param_named(ips, rtl8188fu_mod_params.inactiveps, bool, 0444);
module_param_named(debug_level, rtl8188fu_mod_params.debug_level, int, 0644);
module_param_named(debug_mask, rtl8188fu_mod_params.debug_mask, ullong, 0644);
MODULE_PARM_DESC(swenc, "Set to 1 for software crypto (default 0)\n");
MODULE_PARM_DESC(ips, "Set to 0 to not use link power save (default 1)\n");
MODULE_PARM_DESC(debug_level, "Set debug level (0-5) (default 0)");
MODULE_PARM_DESC(debug_mask, "Set debug mask (default 0)");

static struct rtl_hal_usbint_cfg rtl8188fu_interface_cfg = {
	/* rx */
	.in_ep_num = RTL8188FU_USB_BULK_IN_NUM,
	.rx_urb_num = RTL8188FU_NUM_RX_URBS,
	.rx_max_size = RTL8188FU_SIZE_MAX_RX_BUFFER,
	.usb_rx_hdl = rtl8188fu_rx_hdl,
	.usb_rx_segregate_hdl = NULL,
	/* tx */
	.usb_tx_cleanup = rtl8188fu_tx_cleanup,
	.usb_tx_post_hdl = rtl8188fu_tx_post_hdl,
	.usb_tx_aggregate_hdl = rtl8188fu_tx_aggregate_hdl,
	/* endpoint mapping */
	.usb_endpoint_mapping = rtl8188fu_endpoint_mapping,
	.usb_mq_to_hwq = rtl8188fu_mq_to_hwq,
};

static struct rtl_hal_cfg rtl8188fu_hal_cfg = {
	.name = "rtl8188fu",
	.ops = &rtl8188fu_hal_ops,
	.mod_params = &rtl8188fu_mod_params,
	.usb_interface_cfg = &rtl8188fu_interface_cfg,

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
};

#define USB_VENDER_ID_REALTEK		0x0BDA

static struct usb_device_id rtl8188fu_usb_ids[] ={
	/* CONFIG_RTL8188F */
	/*=== Realtek demoboard ===*/
	{ USB_DEVICE_AND_INTERFACE_INFO(USB_VENDER_ID_REALTEK, 0xF179, 0xff, 0xff, 0xff), .driver_info = HARDWARE_TYPE_RTL8188FU}, /* 8188FU 1*1 */

	{ },
};

MODULE_DEVICE_TABLE(usb, rtl8188fu_usb_ids);

static int rtl8188fu_probe(struct usb_interface *pusb_intf,
			   const struct usb_device_id *id)
{
	return rtl_usb_probe(pusb_intf, id, &rtl8188fu_hal_cfg);
}


static struct usb_driver rtl8188fu_driver = {
	.name = "rtl8188fu",
	.probe = rtl8188fu_probe,
	.disconnect = rtl_usb_disconnect,
	.id_table = rtl8188fu_usb_ids,
#if 0
	.suspend =  rtl8188fu_suspend,
	.resume = rtl8188fu_resume,
  	.reset_resume   = rtl8188fu_resume,
#endif
#ifdef CONFIG_AUTOSUSPEND
	.supports_autosuspend = 1,
#endif

};

module_usb_driver(rtl8188fu_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Realtek Wireless Lan Driver");
MODULE_AUTHOR("Hans Ulli Kroll <ulli.kroll@googlemail.com>");
