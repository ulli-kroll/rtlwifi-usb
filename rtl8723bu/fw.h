/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __RTL8723BU_FW_H__
#define __RTL8723BU_FW_H__

void rtl8723bu_fill_h2c_cmd(struct ieee80211_hw *hw, u8 element_id,
			    u32 cmd_len, u8 *cmdbuffer);
void rtl8723bu_c2h_ra_report_handler(struct ieee80211_hw *hw,
				     u8 *cmd_buf, u8 cmd_len);

#endif
