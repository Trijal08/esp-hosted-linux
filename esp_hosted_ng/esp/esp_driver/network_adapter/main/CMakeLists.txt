set(COMPONENT_SRCS "app_main.c" "slave_bt.c" "cmd.c" "stats.c" "esp_wifi_compat.c" "linux_boot.c" "linux_ipc.c" "linux_flash.c")
set(COMPONENT_ADD_INCLUDEDIRS "./include")

if(CONFIG_IDF_TARGET_ESP32S3)
    list(APPEND COMPONENT_SRCS linux_ll_esp32s3.c)
endif()

if(CONFIG_JFFS2_READER)
    list(APPEND COMPONENT_SRCS tiny_jffs2_reader.c)
endif()

if(CONFIG_ESP_SDIO_HOST_INTERFACE)
    list(APPEND COMPONENT_SRCS sdio_slave_api.c)
elseif(CONFIG_ESP_SPI_HOST_INTERFACE)
    list(APPEND COMPONENT_SRCS spi_slave_api.c)
elseif(CONFIG_ESP_SHMEM_HOST_INTERFACE)
    list(APPEND COMPONENT_SRCS shmem_slave_api.c)
endif()


register_component()

target_compile_definitions(${COMPONENT_LIB} PRIVATE)

# The Wi-Fi libraries pull wpa_supplicant objects in only after libmain.a has
# already been scanned, so the esp_wifi_compat.c shims they need have to be
# force-linked.
target_link_libraries(${COMPONENT_LIB} INTERFACE
                      "-u esp_wifi_sta_get_ie"
                      "-u esp_wifi_use_supp_pmk_cache"
                      "-u esp_wifi_is_wpa3_compatible_mode_enabled")

# Add directory of protocomm_priv.h to include paths
idf_component_get_property(protocomm_dir protocomm COMPONENT_DIR)
target_include_directories(${COMPONENT_LIB} PRIVATE "${protocomm_dir}/src/common")
