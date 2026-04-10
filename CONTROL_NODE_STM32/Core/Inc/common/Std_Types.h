#ifndef STD_TYPES_H
#define STD_TYPES_H

#include "Platform_Types.h"
#include "Compiler.h"
#include <stdint.h>

#ifndef NULL_PTR
#define NULL_PTR ((void *)0)
#endif

typedef uint8_t Std_ReturnType;

#define E_OK       ((Std_ReturnType)0x00U)
#define E_NOT_OK   ((Std_ReturnType)0x01U)

#define STD_HIGH   0x01U
#define STD_LOW    0x00U

#define STD_ACTIVE 0x01U
#define STD_IDLE   0x00U

#endif /* STD_TYPES_H */
