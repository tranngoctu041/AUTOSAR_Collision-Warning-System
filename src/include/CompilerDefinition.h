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
*   @file           CompilerDefinition.h
*   @version 1.0.0
*
*   @brief   AUTOSAR Base - SWS Compiler abstraction
*   @details The file Compiler.h provides macros for the encapsulation of definitions and
*            declarations.
*            This file contains sample code only. It is not part of the production code deliverables
*
*   @addtogroup BASE_COMPONENT
*   @{
*/

/**
* @page misra_violations MISRA-C:2012 violations
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.1, External identifiers shall be distinct. The used compilers 
* use more than 31 chars for identifiers.
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.2, Identifiers declared in the same scope and name space shall be distinct.. The used compilers 
* use more than 31 chars for identifiers.
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.4, Macro identifiers shall be distinct. The used compilers 
* use more than 31 chars for identifiers.
*
* @section [global]
* Violates MISRA 2012 Required Rule 5.5, Identifiers shall be distinct from macro names. The used compilers 
* use more than 31 chars for identifiers.
*
* @section CompilerDefinition_h_REF_1
* Violates MISRA 2012 Advisory Rule 2.5, A project should not contain unused macro declaration.
* Macro are required to make code easier in maintainability. Some macro are required by ASR even they are not use in MCAL layer
*
* @section CompilerDefinition_h_REF_2
* Violates MISRA 2012 Required Rule 21.1, #def and #undef shall not be used on a reserved identifier or reserved macro name.
* For BASE, definition of compiler name will start with "_". The name does not identical with reserved macro or reserved identifers.
*/

#ifndef COMPILERDEFINITION_H
#define COMPILERDEFINITION_H

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
/**
* @brief  Parameters that shall be published within the compiler abstraction header file and also in
          the module's description file.
@{
*/
/* @violates @ref CompilerDefinition_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define COMPILERDEFINITION_VENDOR_ID                      43
/* @violates @ref CompilerDefinition_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define COMPILERDEFINITION_AR_RELEASE_MAJOR_VERSION       4
/* @violates @ref CompilerDefinition_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define COMPILERDEFINITION_AR_RELEASE_MINOR_VERSION       4
/* @violates @ref CompilerDefinition_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define COMPILERDEFINITION_AR_RELEASE_REVISION_VERSION    0
/* @violates @ref CompilerDefinition_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define COMPILERDEFINITION_SW_MAJOR_VERSION               1
/* @violates @ref CompilerDefinition_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define COMPILERDEFINITION_SW_MINOR_VERSION               0
/* @violates @ref CompilerDefinition_h_REF_1 MISRA 2012 Advisory Rule 2.5, unused macro.*/
#define COMPILERDEFINITION_SW_PATCH_VERSION               0
/**@}*/
/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

#ifdef __ghs__
    /**
    * @brief Symbol required to be defined when GreenHills compiler is used.
    */
    /* @violates @ref CompilerDefinition_h_REF_2 MISRA 2012 Required Rule 21.1, macro for compiler name.*/
    #define _GREENHILLS_C_S32K1XX_
#endif
#ifdef __DCC__
    /**
    * @brief Symbol required to be defined when Diab compiler is used.
    */
    /* @violates @ref CompilerDefinition_h_REF_2 MISRA 2012 Required Rule 21.1, macro for compiler name.*/
    #define _DIABDATA_C_S32K1XX_
#endif
#ifdef __MWERKS__
    /**
    * @brief Symbol required to be defined when Codewarrior compiler is used.
    */
    /* @violates @ref CompilerDefinition_h_REF_2 MISRA 2012 Required Rule 21.1, macro for compiler name.*/
    #define _CODEWARRIOR_C_S32K1XX_
#endif
#ifdef __CSMC__
    /**
    * @brief Symbol required to be defined when Cosmic compiler is used.
    */
  /* @violates @ref CompilerDefinition_h_REF_2 MISRA 2012 Required Rule 21.1, macro for compiler name.*/
  #define _COSMIC_C_S32K1XX_
#endif
#if (defined(__GNUC__) && !defined(__DCC__))
         /**
        * @brief Symbol required to be defined when Linaro ARM compiler is used.
        */
        /* @violates @ref CompilerDefinition_h_REF_2 MISRA 2012 Required Rule 21.1, macro for compiler name.*/
        #define _LINARO_C_S32K1XX_
 #endif      
#ifdef __CC_ARM 
        /**
        * @brief Symbol required to be defined when DS5 ARM compiler is used.
        */
        /* @violates @ref CompilerDefinition_h_REF_2 MISRA 2012 Required Rule 21.1, macro for compiler name.*/
        #define _ARM_DS5_C_S32K1XX_     
#endif
#ifdef __ICCARM__ 
        /**
        * @brief Symbol required to be defined when IAR compiler is used.
        */
        /* @violates @ref CompilerDefinition_h_REF_2 MISRA 2012 Required Rule 21.1, macro for compiler name.*/
        #define _IAR_C_S32K1XX_     
#endif

#ifdef __cplusplus
}
#endif

#endif /* #ifndef COMPILERDEFINITION_H */

/** @} */
