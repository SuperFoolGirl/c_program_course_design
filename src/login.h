#pragma once
#include "data_storage.h"

extern List *users_list;
extern List *admins_list;
extern List *couriers_list;

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

// 登录者结构体在登录成功时进行赋值
extern Admin *the_admin;
extern Courier *the_courier;
extern User *the_user;

int verifyUser(const char *account, const char *password);

int verifyAdmin(const char *account, const char *password);

int verifyCourier(const char *account, const char *password);

int verifyPlatform(const char *account, const char *password);

void recoverListData();

void rewriteAllFiles();

void freeLists();

void listsInit();

int loginCallback();

int registerCallback();