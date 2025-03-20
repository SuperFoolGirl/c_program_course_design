#include "../include/platform.h"

void platformShowMenu()
{
    while (1)
    {
        system("cls");
        printf("欢迎登录！\n\n");
        printf("请选择您的操作：\n");
        printf("1. 写入待发货快递\n");
        printf("2. 修改待发货快递\n");
        printf("3. 删除待发货快递\n");
        printf("4. 匹配快递员进行发货\n");
        printf("5. 查看发货信息\n");
        printf("6. 查看平台仓库信息\n");
        printf("7. 注销账号\n\n");
        printf("按任意键退出\n");

        char choice = getchar();
        if (clearInputBuffer() != 0)
        {
            return;
        };

        switch (choice)
        {
        case '1':
            writeToBeShippedDelivery();
            break;
        case '2':
            modifyToBeShippedDelivery();
            break;
        case '3':
            deleteToBeShippedDelivery();
            break;
        case '4':
            matchCourier();
            break;
        case '5':
            viewSendInfo();
            break;
        case '6':
            viewPlatformWarehouseInfo();
            break;
        case '7':
            deletePlatformAccount();
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

    // 随便填写一个包裹id，为了查找和删除。不能再随便给个默认值了
    printf("请输入包裹ID：\n");
    char package_id[20];
    scanf("%s", package_id);
    clearInputBuffer();
    puts("");

    // 加入包裹ID重名检测
    ListNode *current = platform_warehouse_list->head;
    while (current != NULL)
    {
        Package *temp = (Package *)current->data;
        if (strcmp(package_id, temp->package_id) == 0)
        {
            printf("包裹ID重名！\n");
            printCommonInfo();
            return;
        }
        current = current->next;
    }

    strcpy(package->package_id, package_id);

    printf("请输入收件人用户名：\n");
    scanf("%s", package->receiver_account);
    clearInputBuffer();
    puts("");

    // 填一个暂时的快递员用户名
    strcpy(package->courier_account, "0");

    // 填一个暂时的取件码
    package->pick_up_code = 0;

    printf("请选择是否加急：\n");
    printf("1. 否\n");
    printf("2. 是\n");
    char choice = getchar();
    if (clearInputBuffer() != 0)
    {
        printf("输入错误！\n");
        printCommonInfo();
        return;
    }
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
    if (clearInputBuffer() != 0)
    {
        printf("输入错误！\n");
        printCommonInfo();
        return;
    }
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
    if (clearInputBuffer() != 0)
    {
        printf("输入错误！\n");
        printCommonInfo();
        return;
    }
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
    if (clearInputBuffer() != 0)
    {
        printf("输入错误！\n");
        printCommonInfo();
        return;
    }
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
    if (clearInputBuffer() != 0)
    {
        printf("输入错误！\n");
        printCommonInfo();
        return;
    }
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
        listAddHead(platform_warehouse_list, package);
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
    // 如果平台仓库没有快递
    if (platform_warehouse_list->size == 0)
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
            printf("\n已全部发货！\n");
            printCommonInfo();
            return;
        }

        Courier *courier = (Courier *)courier_current->data;
        Package *package = (Package *)current->data;

        // 如果快递员现在正在另一侧忙或处于隐身模式，就跳过
        if (courier->status == 2 || courier->status == 3)
        {
            courier_current = courier_current->next;
            continue; // 为了防止下一个快递员也是2，这里要continue
        }

        strcpy(package->courier_account, courier->account); // 包裹信息上标注快递员信息

        listAdd(couriers_push_list, package); // 加入快递员的推送链表
        printf("已向快递员 %s 推送消息\n", courier->account);
        courier->status = 1; // 标记快递员状态为正在由 平台->驿站 派送

        current = current->next; // 转向下一件快递

        // 平台仓库节点暂时不能删，后面快递员需要用，要由快递员删除
        // 但需要逻辑删除，即改size，否则要是平台用户全部发货后再误触发货函数，会重复执行
        // 这里体现出List结构体size变量的逻辑删除功能了
        // current = current->next;
        // platform_warehouse_list->size--;

        // 加了临时链表，所以可以删掉仓库链表的节点了
        listRemove(platform_warehouse_list, package);

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
                printf("\n暂无快递员接单！\n");
                printCommonInfo();
                return;
            }
        }
    }
}

void viewSendInfo()
{
    system("cls");
    if (couriers_push_list->size == 0)
    {
        printf("暂无发货信息！\n");
        printCommonInfo();
        return;
    }

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

void modifyToBeShippedDelivery()
{
    // 根据平台链表，找到对应的快递，然后修改
    system("cls");
    if (platform_warehouse_list->size == 0)
    {
        printf("平台仓库为空！\n");
        printCommonInfo();
        return;
    }
    printf("请输入要修改的快递的包裹ID：\n");
    char package_id[20];
    scanf("%s", package_id);
    clearInputBuffer();
    puts("");

    // 遍历平台仓库链表，找到要修改的那个快递
    ListNode *current = platform_warehouse_list->head;
    while (current != NULL)
    {
        Package *package = (Package *)current->data;
        if (strcmp(package_id, package->package_id) == 0)
        {
            while (1)
            {
                printf("请选择要修改的信息：\n");
                printf("1. 收件人账号\n");
                printf("2. 加急状态\n");
                printf("3. 体积\n");
                printf("4. 重量\n");
                printf("5. 特殊类型\n");
                printf("6. 价值\n\n");
                printf("按其他任意键返回\n");

                char choice = getchar();
                if (clearInputBuffer() != 0)
                {
                    return;
                }
                puts("");

                switch (choice)
                {
                case '1':
                    printf("请输入新的收件人账号：\n");
                    scanf("%s", package->receiver_account);
                    clearInputBuffer();
                    puts("");
                    break;
                case '2':
                    printf("请输入新的加急状态：\n");
                    printf("1. 否\n");
                    printf("2. 是\n");
                    choice = getchar();
                    if (clearInputBuffer() != 0)
                    {
                        printf("输入错误！\n");
                        printCommonInfo();
                        return;
                    }
                    puts("");

                    if (choice != '1' && choice != '2')
                    {
                        printf("输入错误！\n");
                        printCommonInfo();
                        return;
                    }
                    package->isExpress = choice - '0' - 1;
                    break;
                case '3':
                    printf("请输入新的体积：\n");
                    printf("1. 小\n");
                    printf("2. 大\n");
                    choice = getchar();
                    if (clearInputBuffer() != 0)
                    {
                        printf("输入错误！\n");
                        printCommonInfo();
                        return;
                    }
                    puts("");

                    if (choice != '1' && choice != '2')
                    {
                        printf("输入错误！\n");
                        printCommonInfo();
                        return;
                    }
                    package->volume = choice - '0' - 1;
                    break;
                case '4':
                    printf("请输入新的重量：\n");
                    printf("1. 轻\n");
                    printf("2. 重\n");
                    choice = getchar();
                    if (clearInputBuffer() != 0)
                    {
                        printf("输入错误！\n");
                        printCommonInfo();
                        return;
                    }
                    puts("");

                    if (choice != '1' && choice != '2')
                    {
                        printf("输入错误！\n");
                        printCommonInfo();
                        return;
                    }
                    package->weight = choice - '0' - 1;
                    break;
                case '5':
                    printf("请输入新的特殊类型：\n");
                    printf("1. 普通\n");
                    printf("2. 易碎品、电子产品\n");
                    printf("3. 生鲜\n");
                    choice = getchar();
                    if (clearInputBuffer() != 0)
                    {
                        printf("输入错误！\n");
                        printCommonInfo();
                        return;
                    }
                    puts("");

                    if (choice != '1' && choice != '2' && choice != '3')
                    {
                        printf("输入错误！\n");
                        printCommonInfo();
                        return;
                    }
                    package->special_type = choice - '0' - 1;
                    break;
                default:
                    return; // 修改完想修改的信息后，直接退出这个函数
                }
            }
        }
        current = current->next;
    }

    // 能走到这来的，说明没找到；找到了的话，早就return了
    printf("未找到该快递！\n");
    printCommonInfo();
}

void deleteToBeShippedDelivery()
{
    system("cls");
    if (platform_warehouse_list->size == 0)
    {
        printf("平台仓库为空！\n");
        printCommonInfo();
        return;
    }

    printf("请输入要删除的快递的包裹ID：\n");
    char package_id[20];
    scanf("%s", package_id);
    clearInputBuffer();
    puts("");

    // 遍历平台仓库链表，找到要删除的那个快递
    ListNode *current = platform_warehouse_list->head;
    while (current != NULL)
    {
        Package *package = (Package *)current->data;
        if (strcmp(package_id, package->package_id) == 0)
        {
            listRemove(platform_warehouse_list, package);
            printf("删除成功！\n");
            printCommonInfo();
            return;
        }
        current = current->next;
    }

    // 能走到这来的，说明没找到；找到了的话，早就return了
    printf("未找到该快递！\n");
    printCommonInfo();
}

void viewPlatformWarehouseInfo()
{
    // 查看链表信息
    system("cls");
    if (platform_warehouse_list->size == 0)
    {
        printf("平台仓库为空！\n");
        printCommonInfo();
        return;
    }
    printf("平台仓库内共有快递%d件\n\n", platform_warehouse_list->size);

    ListNode *current = platform_warehouse_list->head;
    while (current != NULL)
    {
        Package *package = (Package *)current->data;
        printf("包裹ID：%s\n", package->package_id);
        printf("收件人账号：%s\n", package->receiver_account);
        printf("加急状态：%s\n", package->isExpress == 1 ? "是" : "否");
        printf("体积：%s\n", package->volume == 1 ? "大" : "小");
        printf("重量：%s\n", package->weight == 1 ? "重" : "轻");
        printf("特殊类型：%s\n", package->special_type == 1 ? "易碎品、电子产品" : package->special_type == 2 ? "生鲜"
                                                                                                              : "普通"); // 多重三目运算符
        printf("价值：%s\n", package->value == 1 ? "高价值" : "低价值");
        puts("");
        current = current->next;
    }
}

void deletePlatformAccount()
{
    system("cls");
    printf("请联系管理员进行账号删除！\n");
    printCommonInfo();
}