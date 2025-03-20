#pragma once
#include "data_storage.h"
#include <time.h>

extern List *shelf_a_list;
extern List *shelf_b_list;
extern List *shelf_c_list;
extern List *shelf_d_list;
extern List *shelf_e_list;
extern List *users_send_list;
extern List *users_push_list;
extern List *users_list;

void userShowMenu(); // 用户菜单

void userPop(); // 弹窗提醒

void userPickup(); // 取件

void userSend(); // 寄件

void userQueryPickup(); // 查询取件信息

void userQuerySend(); // 查询寄件信息

void userFeedback(); // 反馈

void userPay(const int isExpress, int payment); // 付费

void userModifySend(); // 修改寄件信息

void userCancelSend(); // 取消寄件

int payment(Package *package); // 计算价格

void recordSendBehaviors(const char *account, const char *package_id, struct tm *time); // 记录寄件行为

void recordPickUpBehaviors(const char *account, const char *package_id, struct tm *time); // 记录取件行为

void deleteUserAccount(); // 注销账号

void userSubstitute(); // 代取