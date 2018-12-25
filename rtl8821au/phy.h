/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __RTL8821AU_PHY_H__
#define __RTL8821AU_PHY_H__

u32 rtl8821au_phy_query_bb_reg(struct ieee80211_hw *hw, u32 regaddr, u32 bitmask);
void rtl8821au_phy_set_bb_reg(struct ieee80211_hw *hw, u32 regaddr, u32 bitmask,
			      u32 data);
u32 rtl8821au_phy_query_rf_reg(struct ieee80211_hw *hw, enum radio_path rfpath,
			       u32 regaddr, u32 bitmask);
void rtl8821au_phy_set_rf_reg(struct ieee80211_hw *hw, enum radio_path rfpath,
			       u32 regaddr, u32 bitmask, u32 data);
void rtl8821au_phy_set_bw_mode(struct ieee80211_hw *hw,
			       enum nl80211_channel_type ch_type);
u8 rtl8821au_phy_sw_chnl(struct ieee80211_hw *hw);
void rtl8821au_phy_scan_operation_backup(struct ieee80211_hw *hw,
					 u8 operation);
bool rtl8821au_phy_set_rf_power_state(struct ieee80211_hw *hw,
				    enum rf_pwrstate rfpwr_state);

#endif
