#include "pico/stdlib.h"
#include "hardware/pio.h"

#include "ws2812.pio.h"
#include "picorgb.h"

uint32_t rgb_to_32(RGB color) {
    return ((uint32_t) color.r << 24) | ((uint32_t) color.g << 16) | ((uint32_t) color.b << 8);
}

void picorgb_init(PicoRGBConfig * config, PIO pio, uint pin, uint16_t length) {
    uint offset = pio_add_program(pio, &ws2812_program);
    uint sm = pio_claim_unused_sm(pio, true);
    ws2812_program_init(pio, sm, offset, pin, PIO_FREQ, IS_RGBW);
    config->pio = pio;
    config->sm = sm;
    config->enabled = true;
    config->brightness = 100;
    config->color = RGB(0,0,0);
    config->chain_length = length;
    config->skip_start = 0;
    config->skip_end = 0;
    return;
}

void picorgb_setcolor(PicoRGBConfig *config, RGB color) {
    
    // Set config color to current color
    config->color.r = color.r;
    config->color.g = color.g;
    config->color.b = color.b;

    // Skip setting color if brightness is 0 or if not enabled
    if (config->brightness == 0 || !config->enabled) return;
    
    // If brightness is scaled, scale colors to simulate brightness
    RGB output_color = color;
    if (config->brightness != 100) {
        float scalar = config->brightness / 100.0f;
        output_color.r *= scalar;
        output_color.g *= scalar;
        output_color.b *= scalar;
    }
    uint32_t pixel = rgb_to_32(output_color);
    uint32_t off_pixel = 0; // Black/off pixel
    
    // Send data for the entire chain
    for (uint16_t i = 0; i < config->chain_length; i++) {
        if (i < config->skip_start || i >= (config->chain_length - config->skip_end)) {
            // Send off pixel for skipped LEDs
            pio_sm_put_blocking(config->pio, config->sm, off_pixel);
        } else {
            // Send the color for active LEDs
            pio_sm_put_blocking(config->pio, config->sm, pixel);
        }
    }
}

void picorgb_setbrightness(PicoRGBConfig * config, uint8_t percent) {
    if (percent > 100) {
        percent = 100;
    }
    config->brightness = percent;
}

void picorgb_setenabled(PicoRGBConfig *config, bool enabled) {
    if (!enabled && config->enabled) {
        // Turn off all LEDs in the chain
        for (uint16_t i = 0; i < config->chain_length; i++) {
            pio_sm_put_blocking(config->pio, config->sm, (uint32_t) 0);
        }
    }
    else if (enabled && !config->enabled) {
        picorgb_setcolor(config, RGB(config->color.r, config->color.g, config->color.b));
    }
    config->enabled = enabled;
}

void picorgb_setchain(PicoRGBConfig *config, uint16_t chain_length, uint16_t skip_start, uint16_t skip_end) {
    // Validate parameters
    if (chain_length == 0) {
        chain_length = 1; // Minimum of 1 LED
    }
    if (skip_start + skip_end >= chain_length) {
        // Can't skip more LEDs than exist in the chain
        skip_start = 0;
        skip_end = 0;
    }
    
    config->chain_length = chain_length;
    config->skip_start = skip_start;
    config->skip_end = skip_end;
    
    // If currently enabled, update the LED chain with current color
    if (config->enabled) {
        picorgb_setcolor(config, config->color);
    }
}
