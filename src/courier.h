#pragma once
#include "data_storage.h"
#include "ui.h"

extern List *admin_warehouse_list;
extern List *platform_warehouse_list;
extern List *users_send_list;
extern List *users_list;
extern List *couriers_push_list;

void courierShowMenu();

void courierPop();

void queryCurrentDelivery();

void confirmCurrentDelivery();

void refuseCurrentDelivery();

void invisibleModel();

void viewCourierDelivery();