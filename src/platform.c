#include "platform.h"

void platformShowMenu()
{
    while (1)
    {
        system("cls");
        printf("欢迎登陆！");
        printf("请选择您的操作：\n");
        printf("1.写入待发货快递\n");
        printf("2.匹配快递员进行发货\n");
        printf("3.查看发货信息\n");
        printf("按任意键退出\n");

        char choice = getchar();
        clearInputBuffer();

        switch (choice)
        {
        case '1':
            writeToBeShippedDelivery();
            break;
        case '2':
            matchCourier();
            break;
        case '3':
            viewSendInfo();
            break;
        default:
            printf("谢谢，欢迎下次使用！\n");
            return;
        }
    }
}

void writeToBeShippedDelivery()
{
    system("cls");
    Package *package = (Package *)malloc(sizeof(Package));
    strcpy(package->package_id, "A-0-0");

    printf("请输入收件人用户名：\n");
    scanf("%s", package->receiver_account);
    clearInputBuffer();

    printf("请选择是否加急：\n");
    printf("1. 否\n");
    printf("2. 是\n");
    char choice = getchar();
    clearInputBuffer();

    if (choice != '1' && choice != '2')
    {
        printf("输入错误！\n");
        return;
    }
    package->isExpress = choice - '0' - 1;

    printf("请选择体积：\n");
    printf("1. 小\n");
    printf("2. 大\n");
    choice = getchar();
    clearInputBuffer();

    if (choice != '1' && choice != '2')
    {
        printf("输入错误！\n");
        return;
    }
    package->volume = choice - '0' - 1;

    printf("请选择重量：\n");
    printf("1. 轻\n");
    printf("2. 重\n");
    choice = getchar();
    clearInputBuffer();

    if (choice != '1' && choice != '2')
    {
        printf("输入错误！\n");
        return;
    }
    package->weight = choice - '0' - 1;

    printf("请选择快递类型：\n");
    printf("1. 普通\n");
    printf("2. 易碎品、电子产品\n");
    printf("3. 生鲜\n");
    choice = getchar();
    clearInputBuffer();

    if (choice != '1' && choice != '2' && choice != '3')
    {
        printf("输入错误！\n");
        return;
    }
    package->special_type = choice - '0' - 1;

    printf("请选择价值：\n");
    printf("1. 低价值\n");
    printf("2. 高价值\n");
    choice = getchar();
    clearInputBuffer();

    if (choice != '1' && choice != '2')
    {
        printf("输入错误！\n");
        return;
    }
    package->value = choice - '0' - 1;

    if (package->isExpress == 1)
    {
        listAdd(platform_warehouse_list, package);
    }
    else
    {
        listAdd(platform_warehouse_list, package);
    }
    printf("寄件成功！\n");
}

void matchCourier()
{
    system("cls");
    if (platform_warehouse_list->size == 0) // 逻辑判断
    {
        printf("暂无待发货快递！\n");
        return;
    }
    
    // 由于链表已“排序”，所以直接顺着取
    ListNode *current = platform_warehouse_list->head;
    ListNode *courier_current = couriers_list->head;
    while (current != NULL)
    {
        if (courier_current == NULL)
        {
            printf("暂无快递员接单！\n");
            return;
        }
        Courier *courier = (Courier *)courier_current->data; // 注意是怎么样拿到data的 在这句代码里 data的类型是Courier*
        Package *package = (Package *)current->data;
        strcpy(courier->receiver_account, package->receiver_account);

        printf("已向快递员 %s 推送消息\n", courier->account);
        courier->status = 1;
        courier_current = courier_current->next;

        // 平台仓库节点暂时不能删，后面快递员需要用，要由快递员删除
        // 但需要逻辑删除，即改size，否则要是平台用户全部发货后再误触发货函数，会重复执行
        // 这里体现出List结构体size变量的逻辑删除功能了
        current = current->next;
        platform_warehouse_list->size--;
    }
}

void viewSendInfo()
{
    system("cls");
    ListNode *current = couriers_list->head;
    while (current != NULL)
    {
        Courier *courier = (Courier *)current->data;
        if (courier->status == 1)
        {
            printf("快递员 %s 正在派送快递给 %s \n", courier->account, courier->receiver_account);
        }
        current = current->next;
    }
}