#pragma once
#include "data_storage.h"
#include "ui.h"
#include <time.h>

#define MAX 9000
#define MIN 1000

#define SIZE 20 // 货架每排最大容量

// admin.c文件需要用到的全局变量
extern List *users_list;
extern List *admins_list;
extern List *couriers_list;
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

void adminShowMenu();

void adminPop();

void wareHousing();

void userManagement();

void shelfManagement();

void viewFeedback();

void inventoryCheck();

void viewBusinessStatistics();

void addUser();

void deleteUser();

void modifyUser();

void viewUserInfo();

void registerUser();

void registerCourier();

void registerAdmin();

void registerPlatform();

void viewShelfInfo();

void modifyShelfInfo();

void viewShelf(const List *shelf_list);

void modifyShelf(List *shelf_list);

void pushMessageToUser(Package *package);

void addressUserSend();