add_library(picorgb)

pico_generate_pio_header(picorgb ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

target_sources(picorgb PUBLIC ${CMAKE_CURRENT_LIST_DIR}/picorgb.c)
target_include_directories(picorgb PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(picorgb PUBLIC pico_stdlib hardware_pio hardware_clocks)