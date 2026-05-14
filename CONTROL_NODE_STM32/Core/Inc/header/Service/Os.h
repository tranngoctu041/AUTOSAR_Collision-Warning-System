#ifndef OS_H
#define OS_H

#include "Std_Types.h"

/* khởi động scheduler bằng cách cấu hình và chạy SysTick */
void Os_Start(void);

/* kiểm tra thời gian và gọi các task ứng dụng theo chu kỳ */
void Os_Scheduler(void);

/* API để các ngắt hệ thống cập nhật tick cho OS */
void Os_Tick(void);

#endif /* OS_H */
