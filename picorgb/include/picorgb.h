#define RGB(R, G, B) \
    (RGB) { .r = R, .g = G, .b = B }
#define PIO_FREQ 800000
#define IS_RGBW false

#ifdef PICO_DEFAULT_WS2812_PIN
#define WS2812_PIN PICO_DEFAULT_WS2812_PIN
#else
#warning No WS2812PIN Defined!
#endif

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB;

typedef struct
{
    PIO pio;
    uint sm;
    bool enabled;
    uint8_t brightness;
    RGB color;
} PicoRGBConfig;

void picorgb_init(PicoRGBConfig *config, PIO pio, uint pin);

void picorgb_setcolor(PicoRGBConfig *config, RGB color);

void picorgb_setbrightness(PicoRGBConfig *config, uint8_t percent);

void picorgb_setenabled(PicoRGBConfig *config, bool enabled);