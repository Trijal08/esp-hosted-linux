/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Compatibility shims for building against an ESP-IDF newer than the v5.5.1
 * the esp-hosted-ng Wi-Fi libraries in esp_driver/lib/ were built for.
 *
 * IDF v6.x wpa_supplicant calls a couple of Wi-Fi driver APIs that were
 * renamed/generalised after v5.5; the blobs still export the v5.5 spelling.
 * Each shim below maps the new API onto the old one.
 */

#include <stdint.h>
#include <stdbool.h>

#define WLAN_EID_RSNX   244

/* provided by the v5.5-era libnet80211.a/libcore.a */
bool esp_wifi_skip_supp_pmkcaching(void);
uint8_t *esp_wifi_sta_get_rsnxe(uint8_t *bssid);

bool esp_wifi_is_wpa3_compatible_mode_enabled(uint8_t if_index)
{
    /* v5.5 has no wpa3_compatible mode, and CONFIG_ESP_WIFI_WPA3_COMPATIBLE_SUPPORT
     * is off for that reason; esp_hostap.c calls this unconditionally though. */
    (void)if_index;
    return false;
}

bool esp_wifi_use_supp_pmk_cache(void)
{
    return !esp_wifi_skip_supp_pmkcaching();
}

uint8_t *esp_wifi_sta_get_ie(uint8_t *bssid, uint8_t elem_id)
{
    /*
     * v5.5 only exposed the AP's RSNXE. The AP RSN IE was never available to
     * the supplicant there either (wpa_sm::ap_rsn_ie stayed NULL, which
     * wpa_supplicant handles), so NULL for every other element keeps the
     * v5.5 behaviour.
     */
    if (elem_id == WLAN_EID_RSNX) {
        return esp_wifi_sta_get_rsnxe(bssid);
    }

    return NULL;
}
