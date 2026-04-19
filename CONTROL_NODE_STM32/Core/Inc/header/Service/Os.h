#ifndef OS_H
#define OS_H

#include "Std_Types.h"

/* khởi động scheduler bằng cách cho timer tick chạy */
void Os_Start(void);

/* chạy bộ lập lịch chính theo chu kỳ */
void Os_Scheduler(void);

/* cập nhật tick hệ thống cho os */
void Os_Tick(void);

#endif /* OS_H */
