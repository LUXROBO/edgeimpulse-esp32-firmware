/* Edge Impulse inferencing library
 * Copyright (c) 2021 EdgeImpulse Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "../ei_classifier_porting.h"
#if EI_PORTING_ESPRESSIF == 1

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
// Include FreeRTOS for delay
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// for millis and micros
#include "esp_timer.h"

#include "hal/usb_serial_jtag_ll.h"

#define EI_WEAK_FN __attribute__((weak))

static void wait_for_txfifo()
{
    portTickType start_tick = xTaskGetTickCount();
    while (!usb_serial_jtag_ll_txfifo_writable()) {
        portTickType now_tick = xTaskGetTickCount();
        if (now_tick > (start_tick + pdMS_TO_TICKS(200))) {
            return;
        }
    }
}

EI_WEAK_FN EI_IMPULSE_ERROR ei_run_impulse_check_canceled()
{
    return EI_IMPULSE_OK;
}

EI_WEAK_FN EI_IMPULSE_ERROR ei_sleep(int32_t time_ms)
{
    vTaskDelay(time_ms / portTICK_RATE_MS);
    return EI_IMPULSE_OK;
}

uint64_t ei_read_timer_ms()
{
    return esp_timer_get_time()/1000;
}

uint64_t ei_read_timer_us()
{
    return esp_timer_get_time();
}

void ei_putchar(char c)
{
    /* Send char to serial output */
    wait_for_txfifo();
    usb_serial_jtag_ll_write_txfifo((const uint8_t *)&c, 1);
    usb_serial_jtag_ll_txfifo_flush();
}

/**
 *  Printf function uses vsnprintf and output using USB Serial
 */
EI_WEAK_FN void ei_printf(const char *format, ...)
{
    static char print_buf[2048] = {0, };

    va_list args;
    va_start(args, format);
    int r = vsnprintf(print_buf, sizeof(print_buf), format, args);
    va_end(args);

    if (r > 0) {
        wait_for_txfifo();
        usb_serial_jtag_ll_write_txfifo((const uint8_t *)print_buf, r);
        usb_serial_jtag_ll_txfifo_flush();
    }
}

EI_WEAK_FN void ei_printf_float(float f)
{
    ei_printf("%f", f);
}

EI_WEAK_FN void *ei_malloc(size_t size)
{
    return malloc(size);
}

EI_WEAK_FN void *ei_calloc(size_t nitems, size_t size)
{
    return calloc(nitems, size);
}

EI_WEAK_FN void ei_free(void *ptr)
{
    free(ptr);
}

#if defined(__cplusplus) && EI_C_LINKAGE == 1
extern "C"
#endif
EI_WEAK_FN void DebugLog(const char* s)
{
    ei_printf("%s", s);
}

#endif // EI_PORTING_ESPRESSIF == 1
