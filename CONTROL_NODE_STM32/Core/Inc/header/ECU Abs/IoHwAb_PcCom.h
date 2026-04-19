#ifndef IOHWAB_PCCOM_H
#define IOHWAB_PCCOM_H

#include "Std_Types.h"

/* khởi tạo tầng giao tiếp pc */
void IoHwAb_PcCom_Init(void);

/* gửi 1 frame hmi snapshot lên pc */
Std_ReturnType IoHwAb_PcCom_SendHmiFrame(uint16 front_ultra,
                                         uint16 fd_scaled,
                                         uint16 lidar,
                                         uint16 left_ultra,
                                         uint16 right_ultra,
                                         float32 alpha,
                                         float32 sim_speed,
                                         uint16 d_final,
                                         sint16 v_rel_scaled,
                                         uint8 fcw_state,
                                         uint8 bsd_state,
                                         uint8 status);

#endif /* IOHWAB_PCCOM_H */