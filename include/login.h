#pragma once
#include "data_storage.h"

extern List *users_list;
extern List *admins_list;
extern List *couriers_list;
extern List *platforms_list;

extern List *platform_warehouse_list;
extern List *admin_warehouse_list;

extern List *users_send_list;

extern List *users_push_list;
extern List *couriers_push_list;

extern List *shelf_a_list;
extern List *shelf_b_list;
extern List *shelf_c_list;
extern List *shelf_d_list;
extern List *shelf_e_list;
extern List *feedback_list;
extern List *refuse_list;

// 登录者结构体在登录成功时进行赋值
extern Admin *the_admin;
extern Courier *the_courier;
extern User *the_user;
extern Platform *the_platform;

extern double money; // 驿站账户

int verifyUser(const char *account, const char *password); // 验证用户

int verifyAdmin(const char *account, const char *password); // 验证管理员

int verifyCourier(const char *account, const char *password); // 验证快递员

int verifyPlatform(const char *account, const char *password); // 验证运输平台

void recoverListData(); // 读取文件中的数据，恢复链表数据

void rewriteAllFiles(); // 把链表全部更新到文件中

void freeLists(); // 释放链表内存

void listsInit(); // 初始化所有链表

void registering(); // 注册

void login(); // 登录

void getPassword(char *password); // 获取密码，隐藏输入