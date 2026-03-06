/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : S32K14X
*   Dependencies         : none
*
*   Autosar Version      : 4.4.0
*   Autosar Revision     : ASR_REL_4_4_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 1.0.0
*   Build Version        : S32K1_RTD_1_0_0_D2108_ASR_REL_4_4_REV_0000_20210810
*
*   (c) Copyright 2020-2021 NXP Semiconductors
*   All Rights Reserved.
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/


/** 
* @page misra_violations MISRA-C:2012 violations
*
* @section Reg_eSys_h_REF_1
* Violates MISRA 2012 Advisory Rule 2.5, A project should not contain unused macro declaration.
* Macro are required to make code easier in maintainability. Some macro are required but not use in specific test.
*
*/
#ifndef REG_ESYS_H
#define REG_ESYS_H

/**
*   @file
*
*   @addtogroup BASE_COMPONENT
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/


/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define REG_ESYS_VENDOR_ID                    43
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define REG_ESYS_MODULE_ID                    0
#define REG_ESYS_AR_RELEASE_MAJOR_VERSION     4
#define REG_ESYS_AR_RELEASE_MINOR_VERSION     4
#define REG_ESYS_AR_RELEASE_REVISION_VERSION  0
#define REG_ESYS_SW_MAJOR_VERSION             1
#define REG_ESYS_SW_MINOR_VERSION             0
#define REG_ESYS_SW_PATCH_VERSION             0

/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/


/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                   REGISTER PROTECTION (REG_PROT)                                  
==================================================================================================*/
/**
* @brief Macros defined for the IPVs that are protected.
*/

/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MCAL_AXBS_REG_PROT_AVAILABLE              (STD_ON)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MCAL_XBIC_REG_PROT_AVAILABLE              (STD_ON)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MCAL_DMA_REG_PROT_AVAILABLE               (STD_ON)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MCAL_DMAMUX_REG_PROT_AVAILABLE            (STD_ON)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MCAL_CRC_REG_PROT_AVAILABLE               (STD_ON)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MCAL_SIUL2_REG_PROT_AVAILABLE             (STD_ON)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MCAL_STM_REG_PROT_AVAILABLE               (STD_ON)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MCAL_PRAMC_REG_PROT_AVAILABLE             (STD_ON)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MCAL_PFLASH_REG_PROT_AVAILABLE            (STD_ON)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MCAL_C40ASF_REG_PROT_AVAILABLE            (STD_ON)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MCAL_DCM_REG_PROT_AVAILABLE               (STD_ON)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MCAL_CMU_REG_PROT_AVAILABLE               (STD_ON)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MCAL_FXOSC_REG_PROT_AVAILABLE             (STD_ON)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MCAL_MC_RGM_REG_PROT_AVAILABLE            (STD_ON)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MCAL_MC_CGM_REG_PROT_AVAILABLE            (STD_ON)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MCAL_MC_ME_REG_PROT_AVAILABLE             (STD_ON)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MCAL_PLLDIG_REG_PROT_AVAILABLE            (STD_ON)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MCAL_PMC_REG_PROT_AVAILABLE               (STD_ON)

/**
* @brief Macros defined for the protection size
*/
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define AXBS_PROT_MEM_U32                         ((uint32)0x00000004UL)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define XBIC_PROT_MEM_U32                         ((uint32)0x00000004UL)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define DMA_PROT_MEM_U32                          ((uint32)0x00000004UL)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define DMAMUX_PROT_MEM_U32                       ((uint32)0x00000004UL)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define CRC_PROT_MEM_U32                          ((uint32)0x00000004UL)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define SIUL2_PROT_MEM_U32                        ((uint32)0x00000008UL)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define STM_PROT_MEM_U32                          ((uint32)0x00000004UL)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define PRAMC_PROT_MEM_U32                        ((uint32)0x00000004UL)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define PFLASH_PROT_MEM_U32                       ((uint32)0x00000004UL)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define C40ASF_PROT_MEM_U32                       ((uint32)0x00000004UL)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define DCM_PROT_MEM_U32                          ((uint32)0x00000004UL)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define CMU_PROT_MEM_U32                          ((uint32)0x00000004UL)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define FXOSC_PROT_MEM_U32                        ((uint32)0x00000004UL)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MC_RGM_PROT_MEM_U32                       ((uint32)0x00000004UL)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MC_CGM_PROT_MEM_U32                       ((uint32)0x00000004UL)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define MC_ME_PROT_MEM_U32                        ((uint32)0x00000004UL)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define PLLDIG_PROT_MEM_U32                       ((uint32)0x00000004UL)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define PMC_PROT_MEM_U32                          ((uint32)0x00000004UL)
/** @violates @ref Reg_eSys_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro */
#define FTM_PROT_MEM_U32                          ((uint32)0x00000004UL)


/*==================================================================================================
*                                             ENUMS
==================================================================================================*/


/*==================================================================================================
*                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/


/*==================================================================================================
*                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/


/*==================================================================================================
*                                     FUNCTION PROTOTYPES
==================================================================================================*/


#ifdef __cplusplus
}
#endif


/** @} */

#endif /* REG_ESYS_H */
