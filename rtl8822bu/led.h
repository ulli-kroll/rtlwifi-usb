/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __RTL8822BU_LED_H__
#define __RTL8822BU_LED_H__

void rtl8822bu_init_sw_leds(struct ieee80211_hw *hw);
void rtl8822bu_led_control(struct ieee80211_hw *hw, enum led_ctl_mode ledaction);

#endif