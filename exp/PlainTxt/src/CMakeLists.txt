add_executable(${NAME}
        main.cpp
        Agent.cpp
        CommsAgent.cpp
        WifiCred.cpp
        )

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(${NAME} 
	pico_stdlib
	
	FreeRTOS-Kernel-Heap4 # FreeRTOS kernel and dynamic heap
	freertos_config #FREERTOS_PORT
	pico_cyw43_arch_lwip_sys_freertos
	LWIP_PORT
	picow_wsclient
	json_maker
	tiny_json
	onboard_nvs
	hardware_watchdog
	)

target_include_directories(${NAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})


# enable usb output, disable uart output
pico_enable_stdio_usb(${NAME} 0)
pico_enable_stdio_uart(${NAME} 1)
target_compile_definitions(${NAME} PRIVATE
    PICO_DEFAULT_UART_RX_PIN=17
    PICO_DEFAULT_UART_TX_PIN=16
	CYW43_HOST_NAME="DrJonEA"
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})