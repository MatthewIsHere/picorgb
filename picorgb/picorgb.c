#include "pico/stdlib.h"
#include "hardware/pio.h"

#include "ws2812.pio.h"
#include "picorgb.h"

uint32_t rgb_to_32(RGB color) {
    return ((uint32_t) color.r << 24) | ((uint32_t) color.g << 16) | ((uint32_t) color.b << 8);
}

void picorgb_init(PicoRGBConfig * config, PIO pio, uint pin) {
    uint offset = pio_add_program(pio, &ws2812_program);
    uint sm = pio_claim_unused_sm(pio, true);
    ws2812_program_init(pio, sm, offset, pin, PIO_FREQ, IS_RGBW);
    RGB zero = RGB(0, 0, 0);
    config->pio = pio;
    config->sm = sm;
    config->enabled = true;
    config->brightness = 100;
    config->color = zero;
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
    if (config->brightness != 100) {
        float scalar = config->brightness / 100.0f;
        color.r *= scalar;
        color.g *= scalar;
        color.b *= scalar;
    }
    uint32_t pixel = rgb_to_32(color);
    pio_sm_put_blocking(config->pio, config->sm, pixel);
}

void picorgb_setbrightness(PicoRGBConfig * config, uint8_t percent) {
    if (percent > 100) {
        percent = 100;
    }
    config->brightness = percent;
    if (percent != 100 && config->enabled) {
        picorgb_setcolor(config, config->color);
    }
}

void picorgb_setenabled(PicoRGBConfig *config, bool enabled) {
    if (!enabled && config->enabled) {
        pio_sm_put_blocking(config->pio, config->sm, (uint32_t) 0);
    }
    else if (enabled && !config->enabled) {
        picorgb_setcolor(config, RGB(config->color.r, config->color.g, config->color.b));
    }
    config->enabled = enabled;
}
