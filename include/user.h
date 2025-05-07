#pragma once
#include "data_storage.h"
#include "login.h"

extern List *shelf_a_list;
extern List *shelf_b_list;
extern List *shelf_c_list;
extern List *shelf_d_list;
extern List *shelf_e_list;
extern List *users_send_list;
extern List *users_push_list;
extern List *users_list;
extern List *feedback_list;
extern List *refuse_list;
extern double money;

void userShowMenu();                                                                        // 用户菜单

void userPop();                                                                             // 弹窗提醒

void userPickup();                                                                          // 取件

void userSend();                                                                            // 寄件

void userQueryPickup();                                                                     // 查询取件信息

void userQuerySend();                                                                       // 查询寄件信息

void userFeedback();                                                                        // 反馈

void userPay(Package *package, double payment);                                             // 付费

void userModifySend();                                                                      // 修改寄件信息

void userCancelSend();                                                                      // 取消寄件

double payment(Package *package);                                                           // 计算价格

void recordSendBehaviors(const char *account, const char *package_id, struct tm *time);     // 记录寄件行为

void recordPickUpBehaviors(const char *account, const char *package_id, struct tm *time);   // 记录取件行为

void recordRefuseBehaviors(const char *account, const char *package_id, struct tm *time);   // 记录拒收行为

void deleteUserAccount();                                                                   // 注销账号

void userSubstitute();                                                                      // 代取

void userModifyInfo();                                                                      // 修改个人信息

void friendFunction();                                                                      // 好友功能

void viewUserBehaviors();                                                                   // 查看用户行为

void confirmAccountAndPhoneNumber();                                                        // 确认账号和电话号码

void helpFriend();                                                                          // 帮助好友取件

void addFriend();                                                                           // 添加好友

void viewFriend();                                                                          // 查看好友信息

void deleteFriend();                                                                        // 删除好友

void refuseDelivery(Package *package);                                                      // 拒绝当前快递

void viewMessage();                                                                         // 查看消息

void viewRefuseDelivery();                                                                  // 查看拒收快递
