#pragma once
#include "commit.h"
#include "data_storage.h"

extern List *admin_warehouse_list;
extern List *platform_warehouse_list;
extern List *users_send_list;
extern List *users_list;

void courierShowMenu();

void queryCurrentDelivery();

void confirmCurrentDelivery();