#pragma once
#include "commit.h"
#include "data_storage.h"

extern List *platform_warehouse_list;
extern List *couriers_list;

void platformShowMenu();

void writeToBeShippedDelivery();

void matchCourier();

void viewSendInfo();