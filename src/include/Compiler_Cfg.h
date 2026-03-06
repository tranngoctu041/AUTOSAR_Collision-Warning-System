/*==================================================================================================
* Project : RTD AUTOSAR 4.4
* Platform : CORTEXM
* Peripheral : S32K14X
* Dependencies : none
*
* Autosar Version : 4.4.0
* Autosar Revision : ASR_REL_4_4_REV_0000
* Autosar Conf.Variant :
* SW Version : 1.0.0
* Build Version : S32K1_RTD_1_0_0_D2108_ASR_REL_4_4_REV_0000_20210810
*
* (c) Copyright 2020-2021 NXP Semiconductors
* All Rights Reserved.
*
* NXP Confidential. This software is owned or controlled by NXP and may only be
* used strictly in accordance with the applicable license terms. By expressly
* accepting such terms or by downloading, installing, activating and/or otherwise
* using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms. If you do not agree to be
* bound by the applicable license terms, then you may not retain, install,
* activate or otherwise use the software.
==================================================================================================*/
/**
*   @file    Compiler_Cfg.h
*   @implements     Compiler_Cfg.h_Artifact
*   @version 1.0.0
*
*   @brief   AUTOSAR Base - Module specific memory and pointer classes
*   @details The file Compiler_Cfg.h contains the module specific parameters (ptrclass and 
*            memclass) that are passed to the macros defined in Compiler.h
*            This file contains sample code only. It is not part of the production code deliverables
*
*   @addtogroup BASE_COMPONENT
*   @{
*/
#ifndef COMPILER_CFG_H
#define COMPILER_CFG_H

#ifdef __cplusplus
extern "C"{
#endif

/**
* @page misra_violations MISRA-C:2012 violations
*
* @section Compiler_Cfg_h_REF_1
* Violates MISRA 2012 Advisory Rule 2.5, A project should not contain unused macro declaration.
* Macro are required to make code easier in maintainability. Some macro are required by ASR even they are not use in MCAL layer
*/

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/** 
@{
* @brief ADC memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ADC_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ADC_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ADC_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ADC_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ADC_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ADC_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ADC_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ADC_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ADC_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ADC_VAR
/**@}*/

/** 
@{
* @brief CAN memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_VAR
/**@}*/

/** 
@{
* @brief CAN_43_LLCE memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_43_LLCE_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_43_LLCE_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_43_LLCE_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_43_LLCE_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_43_LLCE_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_43_LLCE_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_43_LLCE_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_43_LLCE_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_43_LLCE_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CAN_43_LLCE_VAR
/**@}*/

/** 
@{
* @brief CANIF memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CANIF_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CANIF_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CANIF_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CANIF_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CANIF_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CANIF_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CANIF_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CANIF_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CANIF_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CANIF_VAR
/**@}*/

/** 
@{
* @brief CRCU memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CRCU_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CRCU_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CRCU_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CRCU_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CRCU_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CRCU_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CRCU_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CRCU_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CRCU_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CRCU_VAR
/**@}*/

/** 
@{
* @brief CSEC memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CSEC_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CSEC_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CSEC_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CSEC_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CSEC_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CSEC_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CSEC_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CSEC_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CSEC_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define CSEC_VAR
/**@}*/

/** 
@{
* @brief DEM memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DEM_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DEM_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DEM_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DEM_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DEM_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DEM_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DEM_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DEM_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DEM_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DEM_VAR
/**@}*/

/** 
@{
* @brief DET memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DET_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DET_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DET_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DET_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DET_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DET_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DET_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DET_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DET_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DET_VAR
/**@}*/

/** 
@{
* @brief DIO memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DIO_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DIO_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DIO_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DIO_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DIO_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DIO_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DIO_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DIO_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DIO_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define DIO_VAR
/**@}*/

/** 
@{
* @brief EEP memory and pointer classes.
*/

/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define EEP_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define EEP_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define EEP_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define EEP_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define EEP_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define EEP_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define EEP_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define EEP_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define EEP_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define EEP_VAR
/**@}*/

/**
@{
* @brief ETH memory and pointer classes.
*/

/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETH_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETH_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETH_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETH_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETH_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETH_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETH_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETH_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETH_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETH_VAR
/**@}*/

/**
@{
* @brief ETH memory and pointer classes.
*/

/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHIF_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHIF_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHIF_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHIF_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHIF_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHIF_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHIF_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHIF_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHIF_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHIF_VAR
/**@}*/

/**
@{
* @brief ETH memory and pointer classes.
*/

/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHTRCV_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHTRCV_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHTRCV_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHTRCV_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHTRCV_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHTRCV_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHTRCV_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHTRCV_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHTRCV_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ETHTRCV_VAR
/**@}*/

/** 
@{
* @brief FEE memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FEE_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FEE_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FEE_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FEE_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FEE_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FEE_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FEE_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FEE_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FEE_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FEE_VAR
/**@}*/

/** 
@{
* @brief FLS memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FLS_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FLS_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FLS_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FLS_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FLS_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FLS_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FLS_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FLS_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FLS_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FLS_VAR
/**@}*/

/** 
@{
* @brief FlexRay memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FR_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FR_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FR_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FR_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FR_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FR_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FR_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FR_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FR_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define FR_VAR
/**@}*/

/** 
@{
* @brief GPT memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define GPT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define GPT_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define GPT_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define GPT_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define GPT_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define GPT_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define GPT_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define GPT_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define GPT_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define GPT_VAR
/**@}*/

/** 
@{
* @brief ICU memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ICU_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ICU_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ICU_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ICU_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ICU_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ICU_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ICU_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ICU_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ICU_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define ICU_VAR
/**@}*/

/** 
@{
* @brief I2C memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define I2C_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define I2C_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define I2C_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define I2C_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define I2C_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define I2C_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define I2C_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define I2C_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define I2C_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define I2C_VAR
/**@}*/

/** 
@{
* @brief LIN memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_VAR
/**@}*/

/** 
@{
* @brief LIN_43_LLCE memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_43_LLCE_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_43_LLCE_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_43_LLCE_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_43_LLCE_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_43_LLCE_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_43_LLCE_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_43_LLCE_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_43_LLCE_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_43_LLCE_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LIN_43_LLCE_VAR
/**@}*/

/** 
@{
* @brief LIN memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LINIF_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LINIF_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LINIF_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LINIF_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LINIF_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LINIF_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LINIF_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LINIF_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LINIF_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define LINIF_VAR
/**@}*/

/** 
@{
* @brief MCEM memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCEM_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCEM_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCEM_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCEM_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCEM_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCEM_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCEM_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCEM_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCEM_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCEM_VAR
/**@}*/

/** 
@{
* @brief MCL memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCL_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCL_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCL_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCL_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCL_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCL_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCL_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCL_VAR
/**@}*/

/** 
@{
* @brief MCU memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCU_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCU_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCU_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCU_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCU_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCU_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCU_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCU_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCU_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define MCU_VAR
/**@}*/

/** 
@{
* @brief PMIC memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PMIC_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PMIC_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PMIC_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PMIC_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PMIC_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PMIC_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PMIC_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PMIC_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PMIC_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PMIC_VAR
/**@}*/

/** 
@{
* @brief PORT memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PORT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PORT_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PORT_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PORT_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PORT_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PORT_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PORT_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PORT_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PORT_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PORT_VAR
/**@}*/

/** 
@{
* @brief PWM memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PWM_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PWM_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PWM_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PWM_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PWM_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PWM_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PWM_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PWM_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PWM_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define PWM_VAR
/**@}*/


/** 
@{
* @brief RamTST memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define RAMTST_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define RAMTST_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define RAMTST_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define RAMTST_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define RAMTST_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define RAMTST_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define RAMTST_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define RAMTST_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define RAMTST_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define RAMTST_VAR
/**@}*/

/** 
@{
* @brief SENT memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SENT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SENT_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SENT_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SENT_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SENT_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SENT_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SENT_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SENT_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SENT_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SENT_VAR
/**@}*/

/** 
@{
* @brief SchM memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SCHM_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SCHM_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SCHM_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SCHM_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SCHM_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SCHM_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SCHM_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SCHM_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SCHM_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SCHM_VAR
/**@}*/

/** 
@{
* @brief SPI memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SPI_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SPI_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SPI_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SPI_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SPI_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SPI_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SPI_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SPI_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SPI_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define SPI_VAR
/**@}*/

/** 
@{
* @brief TM memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define TM_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define TM_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define TM_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define TM_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define TM_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define TM_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define TM_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define TM_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define TM_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define TM_VAR
/**@}*/

/** 
@{
* @brief WDG memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDG_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDG_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDG_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDG_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDG_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDG_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDG_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDG_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDG_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDG_VAR
/**@}*/

/** 
@{
* @brief WDGIF memory and pointer classes.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDGIF_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDGIF_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDGIF_APPL_DATA
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDGIF_APPL_CONST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDGIF_APPL_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDGIF_CALLOUT_CODE
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDGIF_VAR_NOINIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDGIF_VAR_POWER_ON_INIT
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDGIF_VAR_FAST
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define WDGIF_VAR
/**@}*/

/** 
* @brief Define for ComStack Data.
*/
/** @violates @ref Compiler_Cfg_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define AUTOSAR_COMSTACKDATA

/*==================================================================================================
*                                            ENUMS
==================================================================================================*/

/*==================================================================================================
*                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                     FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* #ifndef COMPILER_CFG_H */

/** @} */
