/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_mat_solve_lower_triangular_f16.c
 * Description:  Solve linear system LT X = A with LT lower triangular matrix
 *
 *
 * Target Processor: Cortex-M cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2020 ARM Limited or its affiliates. All rights reserved.
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

#include "edge-impulse-sdk/CMSIS/DSP/Include/dsp/matrix_functions_f16.h"

#if defined(ARM_FLOAT16_SUPPORTED)
/**
  @ingroup groupMatrix
 */


/**
  @addtogroup MatrixInv
  @{
 */


   /**
   * @brief Solve LT . X = A where LT is a lower triangular matrix
   * @param[in]  lt  The lower triangular matrix
   * @param[in]  a  The matrix a
   * @param[out] dst The solution X of LT . X = A
   * @return The function returns ARM_MATH_SINGULAR, if the system can't be solved.
   */

#if defined(ARM_MATH_MVE_FLOAT16) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "edge-impulse-sdk/CMSIS/DSP/Include/arm_helium_utils.h"

  arm_status arm_mat_solve_lower_triangular_f16(
  const arm_matrix_instance_f16 * lt,
  const arm_matrix_instance_f16 * a,
  arm_matrix_instance_f16 * dst)
  {
  arm_status status;                             /* status of matrix inverse */


#ifdef ARM_MATH_MATRIX_CHECK

  /* Check for matrix mismatch condition */
  if ((ut->numRows != lt->numCols) ||
      (a->numRows != a->numCols) ||
      (ut->numRows != a->numRows)   )
  {
    /* Set status as ARM_MATH_SIZE_MISMATCH */
    status = ARM_MATH_SIZE_MISMATCH;
  }
  else

#endif /* #ifdef ARM_MATH_MATRIX_CHECK */

  {
    /* a1 b1 c1   x1 = a1
          b2 c2   x2   a2
             c3   x3   a3

    x3 = a3 / c3
    x2 = (a2 - c2 x3) / b2

    */
    int i,j,k,n;

    n = dst->numRows;

    float16_t *pX = dst->pData;
    float16_t *pLT = lt->pData;
    float16_t *pA = a->pData;

    float16_t *lt_row;
    float16_t *a_col;

    _Float16 invLT;

    f16x8_t vecA;
    f16x8_t vecX;

    for(i=0; i < n ; i++)
    {

      for(j=0; j+7 < n; j += 8)
      {
            vecA = vld1q_f16(&pA[i * n + j]);

            for(k=0; k < i; k++)
            {
                vecX = vld1q_f16(&pX[n*k+j]);
                vecA = vfmsq(vecA,vdupq_n_f16(pLT[n*i + k]),vecX);
            }

            if (pLT[n*i + i]==0.0f16)
            {
              return(ARM_MATH_SINGULAR);
            }

            invLT = 1.0f16 / (_Float16)pLT[n*i + i];
            vecA = vmulq(vecA,vdupq_n_f16(invLT));
            vst1q(&pX[i*n+j],vecA);

       }

       for(; j < n; j ++)
       {
            a_col = &pA[j];
            lt_row = &pLT[n*i];

            _Float16 tmp=a_col[i * n];

            for(k=0; k < i; k++)
            {
                tmp -= (_Float16)lt_row[k] * (_Float16)pX[n*k+j];
            }

            if (lt_row[i]==0.0f16)
            {
              return(ARM_MATH_SINGULAR);
            }
            tmp = tmp / (_Float16)lt_row[i];
            pX[i*n+j] = tmp;
        }

    }
    status = ARM_MATH_SUCCESS;

  }

  /* Return to application */
  return (status);
}

#else
  arm_status arm_mat_solve_lower_triangular_f16(
  const arm_matrix_instance_f16 * lt,
  const arm_matrix_instance_f16 * a,
  arm_matrix_instance_f16 * dst)
  {
  arm_status status;                             /* status of matrix inverse */


#ifdef ARM_MATH_MATRIX_CHECK

  /* Check for matrix mismatch condition */
  if ((ut->numRows != lt->numCols) ||
      (a->numRows != a->numCols) ||
      (ut->numRows != a->numRows)   )
  {
    /* Set status as ARM_MATH_SIZE_MISMATCH */
    status = ARM_MATH_SIZE_MISMATCH;
  }
  else

#endif /* #ifdef ARM_MATH_MATRIX_CHECK */

  {
    /* a1 b1 c1   x1 = a1
          b2 c2   x2   a2
             c3   x3   a3

    x3 = a3 / c3
    x2 = (a2 - c2 x3) / b2

    */
    int i,j,k,n;

    n = dst->numRows;

    float16_t *pX = dst->pData;
    float16_t *pLT = lt->pData;
    float16_t *pA = a->pData;

    float16_t *lt_row;
    float16_t *a_col;

    for(j=0; j < n; j ++)
    {
       a_col = &pA[j];

       for(i=0; i < n ; i++)
       {
            lt_row = &pLT[n*i];

            float16_t tmp=a_col[i * n];

            for(k=0; k < i; k++)
            {
                tmp -= lt_row[k] * pX[n*k+j];
            }

            if (lt_row[i]==0.0f)
            {
              return(ARM_MATH_SINGULAR);
            }
            tmp = tmp / lt_row[i];
            pX[i*n+j] = tmp;
       }

    }
    status = ARM_MATH_SUCCESS;

  }

  /* Return to application */
  return (status);
}
#endif /* defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE) */

/**
  @} end of MatrixInv group
 */
#endif /* #if defined(ARM_FLOAT16_SUPPORTED) */
