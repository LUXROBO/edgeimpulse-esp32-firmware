/* Edge Impulse ingestion SDK
 * Copyright (c) 2022 EdgeImpulse Inc.
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

/* Include ----------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>

#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "ei_device_espressif_esp32.h"
#include "ei_analogsensor.h"
#include "sensor_aq.h"

static esp_adc_cal_characteristics_t *adc_chars;
static const adc1_channel_t channel = ADC1_CHANNEL_0;
static const adc_bits_width_t width = (adc_bits_width_t)ADC_WIDTH_BIT_DEFAULT;
static const adc_atten_t atten = ADC_ATTEN_DB_11;
static const adc_unit_t unit = ADC_UNIT_1;

/* Constant defines -------------------------------------------------------- */
static float analog_data[ANALOG_AXIS_SAMPLED];

bool ei_analog_sensor_init(void)
{
    adc1_config_width(width);
    adc1_config_channel_atten(channel, atten);

    ei_add_sensor_to_fusion_list(analog_sensor);

    return true;
}

float *ei_fusion_analog_sensor_read_data(int n_samples)
{
    analog_data[0] = adc1_get_raw((adc1_channel_t)channel);

    return analog_data;
}