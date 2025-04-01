#pragma once
#include "data_storage.h"

extern List *platform_warehouse_list;
extern List *couriers_list;
extern List *couriers_push_list;

void platformShowMenu(); // 平台菜单

void writeToBeShippedDelivery(); // 写入待发货快递

void matchCourier(); // 匹配快递员进行发货

void viewSendInfo(); // 查看发货信息

void modifyToBeShippedDelivery(); // 修改待发货快递

void deleteToBeShippedDelivery(); // 删除待发货快递

void viewPlatformWarehouseInfo(); // 查看平台仓库信息

void deletePlatformAccount(); // 注销账号

void viewRefuseDelivery(); // 查看拒收记录