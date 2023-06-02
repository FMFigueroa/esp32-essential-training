idf_component_register(
        SRCS 
            "connect.c"
            "server.c"
            "toggleLed.c"
            "pushBtn.c"
        INCLUDE_DIRS 
            "."
        REQUIRES esp_wifi 
                 driver
                 esp_http_server
                 json
        )