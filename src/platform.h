#pragma once
#include "common.h"
#include "data_storage.h"

extern List *platform_warehouse_list;
extern List *couriers_list;
extern List *couriers_push_list;

void platformShowMenu();

void writeToBeShippedDelivery();

void matchCourier();

void viewSendInfo();

void modifyToBeShippedDelivery();

void deleteToBeShippedDelivery();

void viewPlatformWarehouseInfo();