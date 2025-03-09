#include "user.h"

User *the_user = NULL;

// 创建临时链表，来存储快递信息
// 不怕退出程序数据消失，因为是基于推送文件的
List *user_delivery_list = NULL;

void userShowMenu()
{
    system("cls");
    
    // 取件弹窗
    if (the_user->receive_status == 1) // 这个还是要有的，迅速判断是否有快递到达，然后进来慢慢找
    {
        user_delivery_list = listInit(); // 初始化临时链表
        printf("您有快递到达，请及时取件！\n\n");

        // 遍历推送链表，找到自己的推送信息
        // 与此同时，删除推送链表对应节点。因为有用信息已经转移到临时链表了
        ListNode *current = users_push_list->head;
        while (current != NULL)
        {
            Package *package = (Package *)current->data;
            if (strcmp(package->receiver_account, the_user->account) == 0)
            {
                listAdd(user_delivery_list, package);
                listRemove(users_push_list, package);

                printf("取件码：%s\n", package->package_id);
                printf("--------------------\n");
            }
            current = current->next;
        }
        printCommonInfo();
    }

    while (1)
    {
        system("cls");
        printf("欢迎登陆！");
        printf("请您选择操作：\n");
        printf("1. 取件\n");
        printf("2. 寄件\n");
        printf("3. 查询取件信息\n");
        printf("4. 查询寄件信息\n");
        printf("5. 反馈\n");
        printf("按其他任意键退出\n");

        char choice = getchar();
        clearInputBuffer();

        switch (choice)
        {
        case '1':
            userPickup();
            break;
        case '2':
            userSend();
            break;
        case '3':
            userQueryPickup();
            break;
        case '4':
            userQuerySend();
            break;
        case '5':
            userFeedback();
            break;
        default:
            // 退出时释放临时链表
            listFree(user_delivery_list);
            return;
        }
    }
}

void userPickup()
{
    system("cls");

    // 如果没有快递可取，直接返回
    if (the_user->receive_status == 0)
    {
        printf("暂无快递可取！\n");
        printCommonInfo();
        return;
    }

    // 通过自己的临时链表拿到取件码
    ListNode *current = user_delivery_list->head;
    while (current != NULL)
    {
        Package *package = (Package *)current->data;
        printf("请输入取件码：\n");
        char input[20];
        scanf("%s", input);
        clearInputBuffer();
        puts("");

        // 如果输入正确，则取件成功，并执行出库操作和推送链表删除操作
        // 出库操作，在待取快递对应的货架里删除该快递节点
        if (strcmp(input, package->package_id) == 0)
        {
            int index_of_shelf = package->package_id[0] - 'A';
            List *shelf_list;

            // 根据货架号的第一个字母，选择对应的货架链表
            switch (index_of_shelf)
            {
            case 0:
                shelf_list = shelf_a_list;
                break;
            case 1:
                shelf_list = shelf_b_list;
                break;
            case 2:
                shelf_list = shelf_c_list;
                break;
            case 3:
                shelf_list = shelf_d_list;
                break;
            case 4:
                shelf_list = shelf_e_list;
                break;
            default:
                break;
            }
            // 从货架中删除。这里我们把快递信息完整存到临时链表里了，所以不用再调用getElement函数
            listRemove(shelf_list, package);

            // 取件后，删除临时链表的信息
            listRemove(user_delivery_list, package);

            // 推移到下一个快递
            printf("取件成功！\n");
            current = current->next;
        }
        else
        {
            // 如果输入错误，则继续循环，直到输入正确（不进行current的更新）
            printf("取件码错误！\n");
            printCommonInfo();
        }
    }
    // 全部取完后，将用户的取件状态置为0
    the_user->receive_status = 0;
    puts("");
    printf("全部取件成功！\n");
    printCommonInfo();
}

void userSend()
{
    system("cls");
    // 寄件操作，填写信息，然后加入待发快递链表
    // 由于不需要考虑取件码，包裹id可以随便写
    // 这里照抄平台函数1
    Package *package = (Package *)malloc(sizeof(Package));
    strcpy(package->package_id, "A-0-0");

    // 这里为了逻辑处理，必须改为收件人，因为两边来回发件方向相反
    // 因此不再需要写入，直接取值即可
    strcpy(package->receiver_account, the_user->account);

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

    // 加急件优先头插
    if (package->isExpress == 1)
    {
        listAddHead(users_send_list, package);
    }
    else
    {
        listAdd(users_send_list, package);
    }

    the_user->send_status = 1; // 发件状态置为未发出
    puts("");
    printf("寄件成功！\n");
    printf("按任意键跳转付费界面\n");
    getchar();
    clearInputBuffer();

    // 付费模块
    userPay(package->isExpress);
}

void userPay(int isExpress)
{
    system("cls");
    if (the_user->user_type == 0)
    {
        if (isExpress == 1)
        {
            printf("您的用户类型为普通用户，请原价支付加急件\n");
        }
        else
        {
            printf("您的用户类型为普通用户，请原价支付普通件\n");
        }
    }
    else if (the_user->user_type == 1)
    {
        if (isExpress == 1)
        {
            printf("您的用户类型为会员用户，可享受8折优惠支付加急件\n");
        }
        else
        {
            printf("您的用户类型为会员用户，可享受8折优惠\n");
        }
    }
    else if (the_user->user_type == 2)
    {
        printf("您的用户类型为企业用户，请月付\n");
    }
    else if (the_user->user_type == 3)
    {
        printf("您的用户类型为代理商用户，恭喜您成功发展下级用户，获得佣金\n");
    }
    else if (the_user->user_type == 4)
    {
        printf("您的用户类型为合作商家用户，请年付\n");
    }
    else
    {
        printf("用户类型错误！\n");
        printCommonInfo();
        return;
    }

    puts("");
    printf("确认支付？\n");
    printf("1. 确认\n");
    printf("按其他任意键取消支付\n");

    char choice = getchar();
    clearInputBuffer();

    if (choice == '1')
    {
        printf("支付成功！\n");
        the_user->send_status = 0; // 重新置为未发出状态
    }
    else
    {
        printf("支付取消！\n");
    }
    printCommonInfo();
}

void userQueryPickup()
{
    system("cls");
    if (the_user->receive_status == 1)
    {
        printf("您有快递到达！\n\n");

        // 这里不用再遍历推送链表，遍历自己的临时链表即可
        ListNode *current = user_delivery_list->head;
        while (current != NULL)
        {
            Package *package = (Package *)current->data;
            printf("取件码为：%s\n", package->package_id);
            printf("--------------------\n");
            current = current->next;
        }
    }
    else
    {
        printf("暂无快递到达！\n");
    }
    // 注意显示信息的代码逻辑，要加一个按任意键继续的代码来防止信息一闪而过
    printCommonInfo();
}

void userQuerySend()
{
    system("cls");
    if (the_user->send_status == 0)
    {
        printf("暂无快递发出！\n");
    }
    else if (the_user->send_status == 1)
    {
        printf("您的快递暂未发出！\n");
    }
    else
    {
        printf("您的快递已发出！\n");
        the_user->send_status = 0; // 重新置为无发件状态
    }

    printCommonInfo();
}

void userFeedback()
{
    system("cls");
    printf("请输入您的反馈：\n");
    FILE *fp = fopen("../files/feedback.txt", "a");
    if (fp == NULL)
    {
        printf("文件打开失败！\n");
        return;
    }
    char feedback[100];
    scanf("%s", feedback);
    fprintf(fp, "%s\n", feedback);
    fclose(fp);
    printf("反馈成功！\n");
    printCommonInfo();
}