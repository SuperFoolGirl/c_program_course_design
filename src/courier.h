#pragma once
#include "data_storage.h"

extern List *admin_warehouse_list;
extern List *platform_warehouse_list;
extern List *users_send_list;
extern List *users_list;
extern List *couriers_push_list;
extern List *couriers_list;

void courierShowMenu(); // 快递员菜单

void courierPop(); // 弹窗提醒

void queryCurrentDelivery(); // 查询当前任务

void confirmCurrentDelivery(); // 确认当前快递送达

void refuseCurrentDelivery(); // 拒绝当前任务

void invisibleModel(); // 隐身状态

void viewCourierDelivery(); // 查看包裹信息

void deleteCourierAccount(); // 注销账号