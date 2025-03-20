#pragma once
#include "data_storage.h"
#include <time.h>

#define MAX 9000
#define MIN 1000

#define SIZE 20 // 货架每排最大容量

// admin.c文件需要用到的全局变量
extern List *users_list;
extern List *admins_list;
extern List *couriers_list;
extern List *platforms_list;
extern List *platform_warehouse_list;
extern List *admin_warehouse_list;
extern List *users_send_list;
extern List *shelf_a_list;
extern List *shelf_b_list;
extern List *shelf_c_list;
extern List *shelf_d_list;
extern List *shelf_e_list;
extern List *users_push_list;
extern List *couriers_push_list;

void adminShowMenu(); // 管理员菜单

void adminPop(); // 弹窗提醒

void wareHousing(); // 入库

void userManagement(); // 用户管理

void shelfManagement(); // 货架管理

void viewFeedback(); // 查看反馈

void inventoryCheck(); // 库存查看

void viewBusinessStatistics(); // 查看业务统计

void addUser(); // 添加用户

void deleteUser(); // 删除用户

void modifyUser(); // 修改用户信息

void viewUserInfo(); // 查看用户信息

void registerUser(); // 注册用户

void registerCourier(); // 注册快递员

void registerAdmin(); // 注册管理员

void registerPlatform(); // 注册平台

void viewShelfInfo(); // 查看货架信息

void modifyShelfInfo(); // 修改货架信息

void viewShelf(const List *shelf_list); // 查看货架

void modifyShelf(List *shelf_list); // 修改货架

void pushMessageToUser(Package *package); // 推送消息

void addressUserSend(); // 处理用户寄件