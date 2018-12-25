/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __RTL8188EU_HW_H__
#define __RTL8188EU_HW_H__

int rtl8188eu_hw_init(struct ieee80211_hw *hw);
void rtl8188eu_card_disable(struct ieee80211_hw *hw);
void rtl8188eu_read_eeprom_info(struct ieee80211_hw *hw);
void rtl8188eu_disable_interrupt(struct ieee80211_hw *hw);
void rtl8188eu_enable_interrupt(struct ieee80211_hw *hw);
void rtl8188eu_update_interrupt_mask(struct ieee80211_hw *hw,
				     u32 add_msr, u32 rm_msr);
void rtl8188eu_update_channel_access_setting(struct ieee80211_hw *hw);
void rtl8188eu_get_hw_reg(struct ieee80211_hw *hw, u8 variable, u8 *val);
void rtl8188eu_set_hw_reg(struct ieee80211_hw *hw, u8 variable, u8 *val);
int rtl8188eu_set_network_type(struct ieee80211_hw *hw, enum nl80211_iftype type);
void rtl8188eu_set_check_bssid(struct ieee80211_hw *hw, bool check_bssid);
void rtl8188eu_set_qos(struct ieee80211_hw *hw, int aci);
void rtl8188eu_set_beacon_related_registers(struct ieee80211_hw *hw);
void rtl8188eu_set_beacon_interval(struct ieee80211_hw *hw);
void rtl8188eu_update_hal_rate_tbl(struct ieee80211_hw *hw,
				   struct ieee80211_sta *sta,
				   u8 rssi_level, bool update_bw);
bool rtl8188eu_gpio_radio_on_off_checking(struct ieee80211_hw *hw, u8 * valid);
void rtl8188eu_set_key(struct ieee80211_hw *hw, u32 key_index,
		       u8 *p_macaddr, bool is_group, u8 enc_algo,
		       bool is_wepkey, bool clear_all);
void rtl8188eu_enable_hw_security_config(struct ieee80211_hw *hw);
#endif
