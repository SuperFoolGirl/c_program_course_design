#include "courier.h"

Courier *the_courier = NULL;

// 创建临时链表，思路类似于用户推送
List *courier_delivery_list = NULL;


void courierShowMenu()
{
    system("cls");
    courier_delivery_list = listInit();

    // 推送
    if (the_courier->status == 1 || the_courier->status == 2)
    {
        // 遍历推送链表，找到自己的推送信息，并添加到临时链表
        // 与此同时，删除推送链表对应节点
        ListNode *current = couriers_push_list->head;
        while (current != NULL)
        {
            Package *package = (Package *)current->data;
            if (strcmp(package->courier_account, the_courier->account) == 0) // 如果包裹上标注的快递员账号是自己
            {
                // 将推送信息加入临时链表
                listAdd(courier_delivery_list, package);

                // 删除推送链表对应节点
                listRemove(couriers_push_list, package);
            }
            current = current->next;
        }

        printf("您有新的任务，请及时查看！\n");
        printCommonInfo();
    }

    while (1)
    {
        system("cls");
        printf("欢迎登陆！");
        printf("请选择您的操作：\n");
        printf("1.查询当前任务\n");
        printf("2.确认当前快递送达\n");
        printf("按任意键退出\n");

        char choice = getchar();
        clearInputBuffer();

        switch (choice)
        {
        case '1':
            queryCurrentDelivery();
            break;
        case '2':
            confirmCurrentDelivery();
            break;
        default:
            // 退出时释放临时链表
            listFree(courier_delivery_list);
            return;
        }
    }
}

void queryCurrentDelivery()
{
    system("cls");
    if (the_courier->status == 0)
    {
        printf("当前无任务！\n");
        printCommonInfo();
        return;
    }
    else if (the_courier->status == 1)
    {
        // 遍历临时链表
        ListNode *current = courier_delivery_list->head;
        printf("您的任务是：\n\n");

        while (current != NULL)
        {
            Package *package = (Package *)current->data;
            if (strcmp(package->courier_account, the_courier->account) == 0)
            {
                printf("将收件人为 %s 的快递从 快递平台 送至 驿站 \n", package->receiver_account);
            }
            current = current->next;
        }
        printCommonInfo();
    }
    else
    {
        // 遍历临时链表
        ListNode *current = courier_delivery_list->head;
        printf("您的任务是：\n\n");

        while (current != NULL)
        {
            Package *package = (Package *)current->data;
            if (strcmp(package->courier_account, the_courier->account) == 0)
            {
                printf("将收件人为 %s 的快递从 驿站 送至 用户 \n", package->receiver_account);
            }
            current = current->next;
        }
        printCommonInfo();
    }
}

// 用了临时链表。所以要修改逻辑
// 每次执行任务，只能是单向，status是1或2
void confirmCurrentDelivery()
{
    system("cls");
    if (the_courier->status == 0)
    {
        printf("当前无任务！\n");
        printCommonInfo();
        return;
    }
    // 平台任务
    else if (the_courier->status == 1)
    {
        the_courier->status = 0; // 状态修改为空闲
        ListNode *current = courier_delivery_list->head;
        while (current != NULL)
        {
            Package *package = (Package *)current->data;
            if (strcmp(package->courier_account, the_courier->account) == 0)
            {
                listRemove(courier_delivery_list, package); // 删除临时链表中的快递
                listAdd(admin_warehouse_list, package);     // 加入驿站仓库
            }
        }
        printf("确认成功！\n");
        printCommonInfo();
    }
    // 驿站任务
    else
    {
        the_courier->status = 0;
        ListNode *current = courier_delivery_list->head;
        while (current != NULL)
        {
            Package *package = (Package *)current->data;
            if (strcmp(package->courier_account, the_courier->account) == 0)
            {
                listRemove(courier_delivery_list, package); // 删除临时链表中的快递
                // 无需再加入平台仓库，因为平台是逻辑上的最后一站。加上就循环了

                // 下面要实现对用户的寄件通知及弹窗操作
                User *user = userElementGet(users_list, package->receiver_account); // 之前有说过，为了方便，这里的receiver_account是寄件人账号。后续可以再调整
                user->send_status = 2;
                printf("已通知用户 %s ，快递已送达！\n", user->account);
            }
        }
        printCommonInfo();
    }
}