/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_fir_init_q31.c
 * Description:  Q31 FIR filter initialization function.
 *
 * $Date:        18. March 2019
 * $Revision:    V1.6.0
 *
 * Target Processor: Cortex-M cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2019 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "edge-impulse-sdk/CMSIS/DSP/Include/dsp/filtering_functions.h"

/**
  @ingroup groupFilters
 */

/**
  @addtogroup FIR
  @{
 */

/**
  @brief         Initialization function for the Q31 FIR filter.
  @param[in,out] S          points to an instance of the Q31 FIR filter structure
  @param[in]     numTaps    number of filter coefficients in the filter
  @param[in]     pCoeffs    points to the filter coefficients buffer
  @param[in]     pState     points to the state buffer
  @param[in]     blockSize  number of samples processed
  @return        none

  @par           Details
                   <code>pCoeffs</code> points to the array of filter coefficients stored in time reversed order:
  <pre>
      {b[numTaps-1], b[numTaps-2], b[N-2], ..., b[1], b[0]}
  </pre>
                   <code>pState</code> points to the array of state variables.
                   <code>pState</code> is of length <code>numTaps+blockSize-1</code> samples (except for Helium - see below), where <code>blockSize</code> is the number of input samples processed by each call to <code>arm_fir_q31()</code>.

   @par          Initialization of Helium version
                   For Helium version the array of coefficients must be a multiple of 16 even if less
                   then 16 coefficients are used. The additional coefficients must be set to 0.
                   It does not mean that all the coefficients will be used in the filter (numTaps
                   is still set to its right value in the init function.) It just means that
                   the implementation may require to read more coefficients due to the vectorization and
                   to avoid having to manage too many different cases in the code.

    @par          Helium state buffer
                   The state buffer must contain some additional temporary data
                   used during the computation but which is not the state of the FIR.
                   The first 2*4*ceil(blockSize/4) samples are temporary data.
                   The remaining samples are the state of the FIR filter.
                   So the state buffer has size <code> numTaps + 8*ceil(blockSize/4) + blockSize - 1 </code>

 */

void arm_fir_init_q31(
        arm_fir_instance_q31 * S,
        uint16_t numTaps,
  const q31_t * pCoeffs,
        q31_t * pState,
        uint32_t blockSize)
{
  /* Assign filter taps */
  S->numTaps = numTaps;

  /* Assign coefficient pointer */
  S->pCoeffs = pCoeffs;

  /* Clear state buffer. The size is always (blockSize + numTaps - 1) */
  #if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)
  memset(pState, 0, (numTaps + (blockSize - 1U) + 2*ROUND_UP(blockSize, 4)) * sizeof(q31_t));
  #else
  memset(pState, 0, (numTaps + (blockSize - 1U)) * sizeof(q31_t));
  #endif

  /* Assign state pointer */
  S->pState = pState;
}

/**
  @} end of FIR group
 */
