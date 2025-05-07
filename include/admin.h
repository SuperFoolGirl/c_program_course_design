#pragma once
#include "data_storage.h"
#include "login.h"

#define MAX 9000
#define MIN 1000
#define SIZE 20          // 货架每排最大容量
#define MAX_TIME 86400   // 一天后滞留

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
extern List *feedback_list;
extern List *refuse_list;
extern double money;

void adminShowMenu();                       // 管理员菜单

void adminPop();                            // 弹窗提醒

void wareHousing();                         // 入库

void userManagement();                      // 用户管理

void shelfManagement();                     // 货架管理

void viewFeedback();                        // 查看反馈

void inventoryCheck();                      // 库存查看

void viewBusinessStatistics();              // 查看业务统计

void addUser();                             // 添加用户

void deleteUser();                          // 删除用户

void modifyFunc();                          // 修改用户信息

void viewUserInfo();                        // 查看用户信息

void registerUser();                        // 注册用户

void registerCourier();                     // 注册快递员

void registerAdmin();                       // 注册管理员

void registerPlatform();                    // 注册平台

void viewShelfInfo();                       // 查看货架信息

void modifyShelfInfo();                     // 修改货架信息

void viewShelf(const List *shelf_list);     // 查看货架

void modifyShelf(List *shelf_list);         // 修改货架

void pushMessageToUser(Package *package);   // 推送消息

void addressUserSend();                     // 处理用户寄件

void recordModifyInfo(const char *package_id,
                      const char *account,
                      const char *modity_info,
                      struct tm *time);                         // 记录修改信息

void modifyShelfPosition(Package *package, List *shelf_list);   // 修改货架位置

void viewPackageLog();                                          // 查看快递单修改日志

void modifyUserSend();                                          // 修改用户寄件信息

void recordModifyInfoOfSend(const char *package_id,
                            const char *account,
                            const char *modity_info,
                            struct tm *time);       // 记录修改寄件信息

void listAllInfo();                                 // 查看所有信息

void listAllUsers();                                // 查看所有用户信息

void listAllCouriers();                             // 查看所有快递员信息

void listAllAdmins();                               // 查看所有管理员信息

void listAllPlatforms();                            // 查看所有平台信息

void handleAbnormalPackage();                       // 处理异常包裹

void findLeavePackage();                            // 查找滞留包裹

void findLeavePackageFromShelf(List *shelf_list);   // 查找滞留包裹

void viewAccount();

void modifyUser();         // 修改用户信息

void modifyCourier();      // 修改快递员信息

void modifyPlatform();     // 修改平台信息

void modifyAdmin();        // 修改管理员信息

void viewUserSendInfo();   // 查看用户寄件信息
