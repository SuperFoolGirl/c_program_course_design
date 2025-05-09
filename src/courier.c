#include "../include/courier.h"

Courier *the_courier = NULL;

// 创建临时链表，思路类似于用户推送
List *courier_delivery_list = NULL;

void courierShowMenu() {
    system("cls");
    courier_delivery_list = listInit();

    courierPop();

    while (1) {
        system("cls");
        printf("欢迎登录！\n\n");
        printf("请选择您的操作：\n");
        printf("1. 查询当前任务\n");
        printf("2. 确认当前快递送达\n");
        printf("3. 拒绝当前任务\n");
        printf("4. 隐身状态\n");
        printf("5. 查看包裹信息\n");
        printf("6. 注销账号\n\n");
        printf("按任意键退出...\n");

        char choice = _getch();

        switch (choice) {
        case '1':
            queryCurrentDelivery();
            break;
        case '2':
            confirmCurrentDelivery();
            break;
        case '3':
            refuseCurrentDelivery();
            break;
        case '4':
            invisibleModel();
            break;
        case '5':
            viewCourierDelivery();
            break;
        case '6':
            deleteCourierAccount();
            break;
        default:
            // 退出时释放临时链表
            // 注意 不能调用listFreePackage函数，因为这个函数会释放掉包裹的内存
            listFree(courier_delivery_list);
            return;
        }
    }
}

void courierPop() {
    // 注意里面删除节点的机制
    // current的更新依赖于本身存在，不可提前remove掉。或者提前记录以下
    if (the_courier->status == 1 || the_courier->status == 2) {
        // 遍历推送链表，找到自己的推送信息，并添加到临时链表
        ListNode *current = couriers_push_list->head;
        while (current != NULL) {
            Package *package = (Package *)current->data;
            if (strcmp(package->courier_account, the_courier->account) == 0)   // 如果包裹上标注的快递员账号是自己
            {
                // 将推送信息加入临时链表
                listAdd(courier_delivery_list, package);
            }
            current = current->next;
        }

        printf("您有新的任务，请及时查看！\n");
        printCommonInfo();
    }
}

void queryCurrentDelivery() {
    system("cls");
    if (the_courier->status == 0) {
        printf("当前无任务！\n");
        printCommonInfo();
        return;
    } else if (the_courier->status == 3) {
        printf("您当前处于隐身状态！\n");
        printCommonInfo();
        return;
    } else if (the_courier->status == 1) {
        // 遍历临时链表
        ListNode *current = courier_delivery_list->head;
        printf("您的任务如下，共%d件：\n\n", courier_delivery_list->size);

        while (current != NULL) {
            Package *package = (Package *)current->data;
            printf("将快递单号为 %s 的快递从 快递平台 送至 驿站\n", package->package_id);
            current = current->next;
        }
        printCommonInfo();
    } else {
        // 遍历临时链表
        ListNode *current = courier_delivery_list->head;
        printf("您的任务是：\n\n");

        while (current != NULL) {
            Package *package = (Package *)current->data;
            printf("将收件人为 %s 的快递从 驿站 送至 平台\n", package->receiver_account);
            current = current->next;
        }
        printCommonInfo();
    }
}

// 用了临时链表。所以要修改逻辑
// 每次执行任务，只能是单向，status是1或2
void confirmCurrentDelivery() {
    system("cls");
    if (the_courier->status == 0) {
        printf("当前无任务！\n");
        printCommonInfo();
        return;
    } else if (the_courier->status == 3) {
        printf("您当前处于隐身状态！\n");
        printCommonInfo();
        return;
    }
    // 平台任务
    else if (the_courier->status == 1) {
        the_courier->status = 0;   // 状态修改为空闲
        ListNode *current = courier_delivery_list->head;
        while (current != NULL) {
            Package *package = (Package *)current->data;
            // 这里的代码逻辑和弹窗处相同，注意边读边删时，必须先往后走再删
            current = current->next;                      // 先往后走，再删除节点
            listRemove(courier_delivery_list, package);   // 删除临时链表中的快递
            listRemove(couriers_push_list, package);      // 删除推送链表
            listAdd(admin_warehouse_list, package);       // 加入驿站仓库
        }
        printf("确认成功！\n");
        printCommonInfo();
    }
    // 驿站任务
    else {
        the_courier->status = 0;
        ListNode *current = courier_delivery_list->head;
        while (current != NULL) {
            Package *package = (Package *)current->data;
            current = current->next;
            listRemove(courier_delivery_list, package);   // 删除临时链表中的快递
            listRemove(couriers_push_list, package);      // 删除推送链表
            // 无需再加入平台仓库，因为平台是逻辑上的最后一站。加上就循环了

            // 下面要实现对用户的寄件通知及弹窗操作
            User *user = userElementGet(
                users_list,
                package->sender_account);   // 之前有说过，为了方便，这里的receiver_account是寄件人账号。后续可以再调整
            user->send_status = 3;
            printf("已通知用户 %s ，快递已送达！\n", user->account);
        }
        printCommonInfo();
    }
}

void refuseCurrentDelivery() {
    // 如果当前被指派任务，可进行拒绝操作
    // 删掉临时链表里的任务，并还给平台仓库和驿站仓库
    // 理应能通知平台和驿站 但暂时先不做
    system("cls");

    if (the_courier->status == 0) {
        printf("当前无任务！\n");
        printCommonInfo();
        return;
    } else if (the_courier->status == 3) {
        printf("您当前处于隐身状态！\n");
        printCommonInfo();
        return;
    }
    // 平台给安排活了
    else if (the_courier->status == 1) {
        the_courier->status = 0;
        ListNode *current = courier_delivery_list->head;
        while (current != NULL) {
            Package *package = (Package *)current->data;

            current = current->next;
            listRemove(courier_delivery_list, package);   // 删除临时链表中的快递
            listRemove(couriers_push_list, package);      // 删除推送链表
            listAdd(platform_warehouse_list, package);    // 加入平台仓库
        }
        printf("已拒绝任务！\n");
        printCommonInfo();
    }
    // 驿站安排活了
    else {
        the_courier->status = 0;
        ListNode *current = courier_delivery_list->head;
        while (current != NULL) {
            Package *package = (Package *)current->data;

            current = current->next;
            listRemove(courier_delivery_list, package);   // 删除临时链表中的快递，可自动完成size--
            listAdd(users_send_list, package);            // 加入驿站寄件链表
        }
        printf("已拒绝任务！\n");
        printCommonInfo();
    }
}

void invisibleModel() {
    // 隐身模式，不接收任务
    system("cls");

    if (courier_delivery_list->size != 0) {
        printf("您有未完成的任务，无法进入隐身模式！\n");
        printCommonInfo();
        return;
    }

    // 设定status取3时，为隐身模式
    if (the_courier->status == 3) {
        printf("您正处于隐身模式，是否关闭？\n");
        printf("1. 是\n");
        printf("2. 否\n");

        char choice = _getch();

        if (choice == '1') {
            the_courier->status = 0;
            printf("已关闭隐身模式！\n");
            printCommonInfo();
            return;
        } else {
            printf("已取消！\n");
            printCommonInfo();
            return;
        }
    }

    // 未处于隐身模式
    if (the_courier->status == 0) {
        printf("您是否进入隐身模式？\n");
        printf("1. 是\n");
        printf("2. 否\n");

        char choice = _getch();

        if (choice == '1') {
            the_courier->status = 3;
            printf("已进入隐身模式！\n");
            printCommonInfo();
        } else {
            printf("已取消！\n");
            printCommonInfo();
        }
    }
}

void viewCourierDelivery() {
    system("cls");
    if (courier_delivery_list->size == 0) {
        printf("暂无任务！\n");
        printCommonInfo();
        return;
    }

    printf("您运送的快递信息如下，共%d件：\n\n", courier_delivery_list->size);

    // 打印表头
    printf("+--------------+------------+------------+------------+--------------+----------+----------+---------------"
           "---+------------+\n");
    printf("|   快递单号   |   收件人   |   寄件人   |   快递员   |   是否加急   | 体积(升) | 重量(kg) |     快递类型  "
           "   |  价值(元)  |\n");
    printf("+--------------+------------+------------+------------+--------------+----------+----------+---------------"
           "---+------------+\n");

    ListNode *current = courier_delivery_list->head;
    while (current != NULL) {
        Package *package = (Package *)current->data;
        // 精细调整各列宽度和制表符相关设置
        printf("| %-12s | %-12s | %-12s | %-13s | %-13s | %-8.2lf | %-8.2lf | %-18s | %-10.2lf |\n",
               package->package_id, package->receiver_account, package->sender_account, package->courier_account,
               package->isExpress == 1 ? "是" : "否", package->volume, package->weight,
               package->special_type == 1   ? "易碎品、电子产品"
               : package->special_type == 2 ? "生鲜"
                                            : "普通",
               package->value);
        current = current->next;
    }
    // 打印表尾
    printf("+--------------+------------+------------+------------+--------------+----------+----------+---------------"
           "---+------------+\n");
    printCommonInfo();
}

void deleteCourierAccount() {
    // 只有用户可以自由注销账号
    system("cls");
    printf("请联系管理员注销账号！\n");
    printCommonInfo();
}
