/******************************************************************************
 * @file     bayes_functions.h
 * @brief    Public header file for CMSIS DSP Library
 * @version  V1.9.0
 * @date     20. July 2020
 ******************************************************************************/
/*
 * Copyright (c) 2010-2020 Arm Limited or its affiliates. All rights reserved.
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


#ifndef _BAYES_FUNCTIONS_H_
#define _BAYES_FUNCTIONS_H_

#include "edge-impulse-sdk/CMSIS/DSP/Include/arm_math_types.h"
#include "edge-impulse-sdk/CMSIS/DSP/Include/arm_math_memory.h"

#include "edge-impulse-sdk/CMSIS/DSP/Include/dsp/none.h"
#include "edge-impulse-sdk/CMSIS/DSP/Include/dsp/utils.h"

#include "edge-impulse-sdk/CMSIS/DSP/Include/dsp/statistics_functions.h"

/**
 * @defgroup groupBayes Bayesian estimators
 *
 * Implement the naive gaussian Bayes estimator.
 * The training must be done from scikit-learn.
 *
 * The parameters can be easily
 * generated from the scikit-learn object. Some examples are given in
 * DSP/Testing/PatternGeneration/Bayes.py
 */

#ifdef   __cplusplus
extern "C"
{
#endif

/**
 * @brief Instance structure for Naive Gaussian Bayesian estimator.
 */
typedef struct
{
  uint32_t vectorDimension;  /**< Dimension of vector space */
  uint32_t numberOfClasses;  /**< Number of different classes  */
  const float32_t *theta;          /**< Mean values for the Gaussians */
  const float32_t *sigma;          /**< Variances for the Gaussians */
  const float32_t *classPriors;    /**< Class prior probabilities */
  float32_t epsilon;         /**< Additive value to variances */
} arm_gaussian_naive_bayes_instance_f32;

/**
 * @brief Naive Gaussian Bayesian Estimator
 *
 * @param[in]  S         points to a naive bayes instance structure
 * @param[in]  in        points to the elements of the input vector.
 * @param[in]  pBuffer   points to a buffer of length numberOfClasses
 * @return The predicted class
 *
 */


uint32_t arm_gaussian_naive_bayes_predict_f32(const arm_gaussian_naive_bayes_instance_f32 *S,
   const float32_t * in,
   float32_t *pBuffer);


#ifdef   __cplusplus
}
#endif

#endif /* ifndef _BAYES_FUNCTIONS_H_ */
