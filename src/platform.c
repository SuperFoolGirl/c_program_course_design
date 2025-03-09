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
    puts("");

    printf("请选择是否加急：\n");
    printf("1. 否\n");
    printf("2. 是\n");
    char choice = getchar();
    clearInputBuffer();
    puts("");

    if (choice != '1' && choice != '2')
    {
        printf("输入错误！\n");
        printCommonInfo();
        return;
    }
    package->isExpress = choice - '0' - 1;

    printf("请选择体积：\n");
    printf("1. 小\n");
    printf("2. 大\n");
    choice = getchar();
    clearInputBuffer();
    puts("");

    if (choice != '1' && choice != '2')
    {
        printf("输入错误！\n");
        printCommonInfo();
        return;
    }
    package->volume = choice - '0' - 1;

    printf("请选择重量：\n");
    printf("1. 轻\n");
    printf("2. 重\n");
    choice = getchar();
    clearInputBuffer();
    puts("");

    if (choice != '1' && choice != '2')
    {
        printf("输入错误！\n");
        printCommonInfo();
        return;
    }
    package->weight = choice - '0' - 1;

    printf("请选择快递类型：\n");
    printf("1. 普通\n");
    printf("2. 易碎品、电子产品\n");
    printf("3. 生鲜\n");
    choice = getchar();
    clearInputBuffer();
    puts("");

    if (choice != '1' && choice != '2' && choice != '3')
    {
        printf("输入错误！\n");
        printCommonInfo();
        return;
    }
    package->special_type = choice - '0' - 1;

    printf("请选择价值：\n");
    printf("1. 低价值\n");
    printf("2. 高价值\n");
    choice = getchar();
    clearInputBuffer();
    puts("");

    if (choice != '1' && choice != '2')
    {
        printf("输入错误！\n");
        printCommonInfo();
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
    printCommonInfo();
}

void matchCourier()
{
    // 逻辑：遍历平台仓库，把平台仓库的快递分配给快递员
    // 设定上，每个快递员有十个指标，普通快递占一个指标，大体积和大重量快递，每个点各占一个指标
    // 当本次快递的指标满了，会强行算上这一件，下一件再换快递员
    // 指标可以直接存为临时变量，不需要放到Courier结构体中。但这样一来需要让外循环是快递员，内循环是平台仓库
    // 当快递员无法再继续接单时，才换下一个快递员

    system("cls");
    if (platform_warehouse_list->size == 0) // 逻辑判断
    {
        printf("暂无待发货快递！\n");
        printCommonInfo();
        return;
    }

    // 由于链表已“排序”，所以直接顺着取
    ListNode *current = platform_warehouse_list->head;
    ListNode *courier_current = couriers_list->head;

    if (courier_current == NULL)
    {
        printf("暂无快递员接单！\n");
        printCommonInfo();
        return;
    }

    int base = 0;
    while (courier_current != NULL)
    {
        // 如果快递没了
        if (current == NULL)
        {
            printf("已全部发货！\n");
            printCommonInfo();
            return;
        }

        Courier *courier = (Courier *)courier_current->data;
        Package *package = (Package *)current->data;

        // 如果快递员现在正在派送从驿站到用户的快递，就跳过
        if (courier->status == 2)
        {
            courier_current = courier_current->next;
            continue; // 为了防止下一个快递员也是2，这里要continue
        }

        strcpy(package->courier_account, courier->account); // 包裹信息上标注快递员信息

        listAdd(couriers_push_list, package);                   // 加入快递员的推送链表
        printf("已向快递员 %s 推送消息\n", courier->account);
        courier->status = 1; // 标记快递员状态为正在由 平台->驿站 派送

        Package *tmp = package; // 保存当前节点
        current = current->next; // 换下一件快递

        // 平台仓库节点暂时不能删，后面快递员需要用，要由快递员删除
        // 但需要逻辑删除，即改size，否则要是平台用户全部发货后再误触发货函数，会重复执行
        // 这里体现出List结构体size变量的逻辑删除功能了
        // current = current->next;
        // platform_warehouse_list->size--;

        // 现在可以直接删除平台仓库节点了
        // 每次删除的都是头节点
        listRemove(platform_warehouse_list, tmp);

        // 判断本次base点的情况
        base++;
        if (package->volume == 1)
        {
            base++;
        }
        if (package->weight == 1)
        {
            base++;
        }

        // 如果base点已经到了10，说明这个快递员已经满负荷了
        if (base == 10)
        {
            courier_current = courier_current->next;
            base = 0;
            // 判断下一个快递员是否为空
            if (courier_current == NULL)
            {
                printf("暂无快递员接单！\n");
                printCommonInfo();
                return;
            }
        }
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
            printf("快递员 %s 正在派送快递到驿站\n", courier->account);
        }
        current = current->next;
    }
    printCommonInfo();
}