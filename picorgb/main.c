

// void main()
// {
//     PicoRGBConfig config;
//     picorgb_init(&config, pio0, WS2812_PIN);

//     int state = 0;
//     int r = 255;
//     int g = 0;
//     int b = 0;
//     picorgb_setbrightness(&config, 10);
//     while (true)
//     {
//         if (state == 0)
//         {
//             g++;
//             if (g == 255)
//                 state = 1;
//         }
//         if (state == 1)
//         {
//             r--;
//             if (r == 0)
//                 state = 2;
//         }
//         if (state == 2)
//         {
//             b++;
//             if (b == 255)
//                 state = 3;
//         }
//         if (state == 3)
//         {
//             g--;
//             if (g == 0)
//                 state = 4;
//         }
//         if (state == 4)
//         {
//             r++;
//             if (r == 255)
//                 state = 5;
//         }
//         if (state == 5)
//         {
//             b--;
//             if (b == 0)
//                 state = 0;
//         }
//         picorgb_setcolor(&config, RGB(r, g, b));
//         sleep_ms(10);
//     }
// }