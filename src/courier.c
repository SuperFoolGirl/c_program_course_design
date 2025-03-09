#include "courier.h"

Courier *the_courier = NULL;

void courierShowMenu()
{
    system("cls");
    // 推送
    if (the_courier->status == 1 || the_courier->status == 2)
    {
        printf("您有新的任务，请及时查看！\n");
        printf("按任意键继续\n");
        getchar();
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
            printf("谢谢，欢迎下次使用！\n");
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
        return;
    }
    else if (the_courier->status == 1)
    {
        printf("您的任务是：将收件人为 %s 的快递从 快递平台 送至 驿站 \n", the_courier->receiver_account);
    }
    else
    {
        printf("您的任务是：将收件人为 %s 的快递从 驿站 送至 快递平台\n", the_courier->receiver_account);
    }
}

void confirmCurrentDelivery()
{
    system("cls");
    if (the_courier->status == 0)
    {
        printf("当前无任务！\n");
        return;
    }
    // 平台任务
    else if (the_courier->status == 1)
    {
        the_courier->status = 0;
        // 放入驿站仓库，需要从平台仓库中用receiver_account找到对应的快递
        Package *package = packageElementGetByCourier(platform_warehouse_list, the_courier->receiver_account);
        listRemove(platform_warehouse_list, package); // 删除平台仓库中的快递
        listAdd(admin_warehouse_list, package);       // 加入驿站仓库
        // 修改快递员记录的收件人账号为默认值
        strcpy(the_courier->receiver_account, "0");
        printf("确认成功！\n");
    }
    // 驿站任务
    else
    {
        the_courier->status = 0;
        // 从用户发件中找到对应的快递
        Package *package = packageElementGetByCourier(users_send_list, the_courier->receiver_account);
        listRemove(users_send_list, package); // 删除驿站仓库中的快递
        // 修改快递员记录的收件人账号为默认值
        strcpy(the_courier->receiver_account, "0");
        // 不需要再加入平台仓库，否则无限循环

        // 下面要实现对用户的寄件通知及弹窗操作
        User *user = userElementGet(users_list, package->receiver_account);
        user->send_status = 2;
        printf("已通知用户 %s ，快递已送达！\n", user->account);
    }
}