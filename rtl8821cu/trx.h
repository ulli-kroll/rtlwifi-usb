/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __RTL8821CU_TRX_H__
#define __RTL8821CU_TRX_H__

#define RTL8821CU_USB_BULK_IN_NUM		1
#define RTL8821CU_NUM_RX_URBS			8
#define RTL8821CU_SIZE_MAX_RX_BUFFER		15360	/* 8192 */

u16 rtl8821cu_mq_to_hwq(__le16 fc, u16 mac80211_queue_index);
void rtl8821cu_tx_fill_desc(struct ieee80211_hw *hw,
			    struct ieee80211_hdr *hdr, u8 *pdesc_tx,
			    u8 *pbd_desc_tx, struct ieee80211_tx_info *info,
			    struct ieee80211_sta *sta,
			    struct sk_buff *skb,
			    u8 queue_index,
			    struct rtl_tcb_desc *tcb_desc);
void rtl8821cu_fill_fake_txdesc(struct ieee80211_hw *hw, u8 * pDesc,
			        u32 buffer_len, bool bIsPsPoll);
void rtl8821cu_tx_fill_cmddesc(struct ieee80211_hw *hw,
			       u8 *pdesc, bool b_firstseg,
			       bool b_lastseg, struct sk_buff *skb);
bool rtl8821cu_rx_query_desc(struct ieee80211_hw *hw,
			     struct rtl_stats *stats,
			     struct ieee80211_rx_status *rx_status,
			     u8 *p_desc, struct sk_buff *skb);
void rtl8821cu_set_desc(struct ieee80211_hw *hw, u8 *pdesc,
			bool istx, u8 desc_name, u8 *val);
u64 rtl8821cu_get_desc(struct ieee80211_hw *hw,
		       u8 *pdesc, bool istx, u8 desc_name);
bool rtl8821cu_is_tx_desc_closed(struct ieee80211_hw *hw,
				 u8 hw_queue, u16 index);
void rtl8821cu_tx_polling(struct ieee80211_hw *hw, u8 hw_queue);
int rtl8821cu_tx_post_hdl(struct ieee80211_hw *hw, struct urb *urb,
			  struct sk_buff *skb);
void rtl8821cu_tx_cleanup(struct ieee80211_hw *hw, struct sk_buff  *skb);
int  rtl8821cu_endpoint_mapping(struct ieee80211_hw *hw);
struct sk_buff *rtl8821cu_tx_aggregate_hdl(struct ieee80211_hw *,
					   struct sk_buff_head *);
void  rtl8821cu_rx_hdl(struct ieee80211_hw *hw, struct sk_buff * skb);
#endif
