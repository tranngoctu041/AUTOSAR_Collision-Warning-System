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

#ifndef DEVASSERT_H
#define DEVASSERT_H

#include "PlatformTypes.h"

/*
    Drivers can use a mechanism to validate data coming from upper software layers (application code) by performing
    a number of checks on input parameters' range or other invariants that can be statically checked (not dependent on
    runtime conditions). A failed validation is indicative of a software bug in application code, therefore it is important
    to use this mechanism during development.
*/

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define DEVASSERT_VENDOR_ID                    43
#define DEVASSERT_AR_RELEASE_MAJOR_VERSION     4
#define DEVASSERT_AR_RELEASE_MINOR_VERSION     4
#define DEVASSERT_AR_RELEASE_REVISION_VERSION  0
#define DEVASSERT_SW_MAJOR_VERSION             1
#define DEVASSERT_SW_MINOR_VERSION             0
#define DEVASSERT_SW_PATCH_VERSION             0

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if the files Devassert.h and PlatformTypes.h are of the same version */
    #if ((DEVASSERT_AR_RELEASE_MAJOR_VERSION != PLATFORM_TYPES_AR_RELEASE_MAJOR_VERSION) || \
        (DEVASSERT_AR_RELEASE_MINOR_VERSION != PLATFORM_TYPES_AR_RELEASE_MINOR_VERSION)     \
        )
        #error "AUTOSAR Version Numbers of Devassert.h and PlatformTypes.h are different"
    #endif
#endif

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/

/* Implement default assert macro */
static inline void DevAssert(volatile boolean x)
{
    if(x) { } else { for(;;) {} }
}

#endif /* DEVASSERT_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
