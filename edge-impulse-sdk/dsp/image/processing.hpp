/* Edge Impulse inferencing library
 * Copyright (c) 2020 EdgeImpulse Inc.
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

#ifndef __EIDSP_IMAGE_PROCESSING__H__
#define __EIDSP_IMAGE_PROCESSING__H__

#include "edge-impulse-sdk/dsp/ei_utils.h"
#include "edge-impulse-sdk/porting/ei_classifier_porting.h"
#include "edge-impulse-sdk/dsp/returntypes.hpp"

namespace ei { namespace image { namespace processing {

enum YUV_OPTIONS
{
    BIG_ENDIAN_ORDER = 1, //RGB reading from low to high memory.  Otherwise, uses native encoding
    PAD_4B = 2, // pad 0x00 on the high B. ie 0x00RRGGBB
};

/**
 * @brief Convert YUV to RGB
 *
 * @param rgb_out Output buffer (can be the same as yuv_in if big enough)
 * @param yuv_in Input buffer
 * @param in_size_B Size of input image in B
 * @param opts Note, only BIG_ENDIAN_ORDER supported presently
 */
int yuv422_to_rgb888(
    unsigned char *rgb_out,
    unsigned const char *yuv_in,
    unsigned int in_size_B,
    YUV_OPTIONS opts);

/**
 * @brief Crops an image. Can be in-place. 4B alignment for best performance
 * (Alignment is tested, will fall back to B by B movement)
 *
 * @param srcWidth X dimension in pixels
 * @param srcHeight Y dimension in pixels
 * @param srcImage Input buffer
 * @param startX X coord of first pixel to keep
 * @param startY Y coord of the first pixel to keep
 * @param dstWidth Desired X dimension in pixels (should be smaller than srcWidth)
 * @param dstHeight Desired Y dimension in pixels (should be smaller than srcHeight)
 * @param dstImage Output buffer, can be the same as srcImage
 * @param iBpp 8 or 16 for bits per pixel
 */
int cropImage(
    const uint8_t *srcImage,
    int srcWidth,
    int srcHeight,
    int startX,
    int startY,
    uint8_t *dstImage,
    int dstWidth,
    int dstHeight,
    int iBpp);

/**
 * @copydoc cropImage(
    int srcWidth,
    int srcHeight,
    const uint8_t *srcImage,
    int startX,
    int startY,
    int dstWidth,
    int dstHeight,
    uint8_t *dstImage,
    int iBpp)
 */
void crop_image_rgb888_packed(
    const uint8_t *srcImage,
    int srcWidth,
    int srcHeight,
    int startX,
    int startY,
    uint8_t *dstImage,
    int dstWidth,
    int dstHeight);

constexpr int RGB888_B_SIZE = 3;
constexpr int MONO_B_SIZE = 1;

/**
 * @brief Resize an image using interpolation
 * Can be used to resize the image smaller or larger
 * If resizing much smaller than 1/3 size, then a more rubust algorithm should average all of the pixels
 * This algorithm uses bilinear interpolation - averages a 2x2 region to generate each new pixel
 *
 * @param srcWidth Input image width in pixels
 * @param srcHeight Input image height in pixels
 * @param srcImage Input buffer
 * @param dstWidth Output image width in pixels
 * @param dstHeight Output image height in pixels
 * @param dstImage Output buffer, can be same as input buffer
 * @param pixel_size_B Size of pixels in Bytes.  3 for RGB, 1 for mono
 */
void resize_image(
    const uint8_t *srcImage,
    int srcWidth,
    int srcHeight,
    uint8_t *dstImage,
    int dstWidth,
    int dstHeight,
    int pixel_size_B);

/**
 * @brief Calculate new dims that match the aspect ratio of destination
 * This prevents a squashed look
 * The smallest axis is held constant
 *
 * @param srcWidth Input width in pixels
 * @param srcHeight Input height in pixels
 * @param dstWidth Ultimate width in pixels
 * @param dstHeight Ultimate height in pixels
 * @param[out] cropWidth Width in pixels that matches the aspect ratio
 * @param[out] cropHeight Height in pixels that matches the aspect ratio
 */
void calculate_crop_dims(
    int srcWidth,
    int srcHeight,
    int dstWidth,
    int dstHeight,
    int &cropWidth,
    int &cropHeight);

/**
 * @brief Crops, then interpolates to a desired new image size
 * Can be done in place (set srcImage == dstImage)
 *
 * @param srcImage Input image buffer
 * @param srcWidth Input width in pixels
 * @param srcHeight Input height in pixels
 * @param dstImage Output image buffer, can be same as input buffer
 * @param dstWidth Desired new width in pixels
 * @param dstHeight Desired new height in pixels
 */
void crop_and_interpolate_rgb888(
    const uint8_t *srcImage,
    int srcWidth,
    int srcHeight,
    uint8_t *dstImage,
    int dstWidth,
    int dstHeight);

}}} //namespaces
#endif //!__EI_IMAGE_PROCESSING__H__