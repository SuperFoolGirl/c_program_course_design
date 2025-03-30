#include "../include/user.h"

User *the_user = NULL;

static int isDeleted = 0;

// 创建临时链表，来存储快递信息
// 不怕退出程序数据消失，因为是基于推送文件的
List *user_delivery_list = NULL;

void userShowMenu()
{
    system("cls");
    user_delivery_list = listInit(); // 初始化临时链表。必须初始化，否则退出时free函数报错

    userPop(); // 弹出推送

    while (1)
    {
        // 如果账号被删除，直接退出
        if (isDeleted)
        {
            listFreeNode(user_delivery_list);
            isDeleted = 0; // 重置标志位
            return;
        }

        system("cls");
        printf("欢迎登录！\n\n");
        printf("请您选择操作：\n");
        printf("1. 取件\n");
        printf("2. 寄件\n");
        printf("3. 查询取件信息\n");
        printf("4. 查询寄件信息\n");
        printf("5. 修改寄件信息\n");
        printf("6. 取消寄件\n");
        printf("7. 反馈\n");
        printf("8. 代取\n");
        printf("9. 修改个人信息\n");
        printf("0. 注销账号\n");
        printf("a. 好友功能\n");
        printf("b. 查询服务记录\n\n");
        printf("按其他任意键退出...\n");

        char choice = getchar();
        if (clearInputBuffer() != 0)
        {
            listFreeNode(user_delivery_list);
            return;
        }

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
            userModifySend();
            break;
        case '6':
            userCancelSend();
            break;
        case '7':
            userFeedback();
            break;
        case '8':
            userSubstitute();
            break;
        case '9':
            userModifyInfo();
            break;
        case '0':
            deleteUserAccount();
            break;
        case 'a':
            friendFunction();
            break;
        case 'b':
            viewUserBehaviors();
            break;
        default:
            // 退出时释放临时链表，但不释放内存
            listFreeNode(user_delivery_list); // 注意用户临时链表的类型是Package
            return;
        }
    }
}

void userPop()
{
    // 包裹滞留弹窗
    if (the_user->delivery_leave == 1)
    {
        printf("您有包裹滞留，请及时取件！\n\n");
        printCommonInfo();
        system("cls"); // 为下一个弹窗清屏
    }

    // 取件弹窗
    if (the_user->receive_status == 1) // 这个还是要有的，迅速判断是否有快递到达，然后进来慢慢找
    {
        printf("您有快递到达，请及时取件！\n\n");

        // 遍历推送链表，找到自己的推送信息
        // 注意，不能直接删除推送链表。如果下号了，那用户再次上号，数据不就彻底没了吗？
        // 因此，取件后再删除推送消息
        ListNode *current = users_push_list->head;
        while (current != NULL)
        {
            Package *package = (Package *)current->data;
            if (strcmp(package->receiver_account, the_user->account) == 0)
            {
                // 将推送信息加入临时链表
                listAdd(user_delivery_list, package);

                printf("货架号：%s\n", package->shelf_id);
                printf("取件码：%d\n", package->pick_up_code);
                printf("--------------------\n");
            }
            current = current->next;
        }
        printCommonInfo();
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
        printf("快递货架号为：%s\n", package->shelf_id);
        printf("请输入取件码：\n");
        int input;
        scanf("%d", &input);
        if (clearInputBuffer() != 0)
        {
            printf("取件码错误！\n");
            printCommonInfo();
            return;
        }
        puts("");

        // 如果输入正确，则取件成功，并执行出库操作和临时、推送链表删除操作
        // 出库操作，在待取快递对应的货架里删除该快递节点
        if (input == package->pick_up_code)
        {
            int index_of_shelf = package->shelf_id[0] - 'A';
            List *shelf_list = NULL;

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

            // 推移到下一个快递，再删除
            current = current->next;

            // 从货架中删除。这里我们把快递信息完整存到临时链表里了，所以不用再调用getElement函数
            listRemove(shelf_list, package);

            // 取件后，删除临时链表的信息
            listRemove(user_delivery_list, package);

            // 删除推送链表的信息
            listRemove(users_push_list, package);

            // 写入行为文件
            recordPickUpBehaviors(the_user->account, package->package_id, getTime());

            printf("取件成功！\n");

            continue; // 跳过一般更新
        }
        else
        {
            // 如果输入错误，则直接退出，允许用户重新查看取件码
            printf("取件码错误！\n");
            printCommonInfo();
            return;
        }
        current = current->next;
    }
    // 全部取完后，将用户的取件状态置为0
    the_user->receive_status = 0;
    // 不管是否滞留，统一置为0即可
    the_user->delivery_leave = 0;
    printf("\n全部取件成功！\n");
    printCommonInfo();
}

void userSend()
{
    system("cls");
    // 寄件操作，填写信息，然后加入待发快递链表
    // 这里照抄平台函数1
    Package *package = (Package *)malloc(sizeof(Package));

    printf("请输入快递单号：\n");
    char package_id[20];
    scanf("%s", package_id);
    clearInputBuffer();
    puts("");

    // 加入快递单号重名检测
    ListNode *current = users_send_list->head;
    while (current != NULL)
    {
        Package *temp = (Package *)current->data;
        if (strcmp(package_id, temp->package_id) == 0)
        {
            printf("快递单号重名！\n");
            printCommonInfo();
            return;
        }
        current = current->next;
    }
    
    strcpy(package->package_id, package_id);

    // 这里为了逻辑处理，必须改为收件人，因为两边来回发件方向相反
    // 因此不再需要写入，直接取值即可
    strcpy(package->receiver_account, the_user->account);

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

    printf("请输入体积(升)：\n");
    scanf("%lf", &package->volume);
    clearInputBuffer();
    puts("");

    printf("请输入重量(kg)：\n");
    scanf("%lf", &package->weight);
    clearInputBuffer();
    puts("");

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

    printf("请输入快递价值(元)：\n");
    scanf("%lf", &package->value);
    clearInputBuffer();
    puts("");

    printf("按任意键跳转付费界面\n");
    getchar();
    clearInputBuffer();

    // 付费模块
    userPay(package, payment(package));
}

void userPay(Package *package, double payment)
{
    system("cls");
    if (the_user->user_type == 0)
    {
        if (package->isExpress == 1)
        {
            printf("您的用户类型为普通用户，请原价支付加急件\n");
            printf("支付金额为：%.2lf元\n", payment * 2);
            
        }
        else
        {
            printf("您的用户类型为普通用户，请原价支付普通件\n");
            printf("支付金额为：%.2lf元\n", payment);
        }
    }
    else if (the_user->user_type == 1)
    {
        if (package->isExpress == 1)
        {
            printf("您的用户类型为会员用户，可享受8折优惠支付加急件\n");
            printf("支付金额为：%.2lf元\n", payment * 2 * 0.8);
        }
        else
        {
            printf("您的用户类型为会员用户，可享受8折优惠\n");
            printf("支付金额为：%.2lf元\n", payment * 0.8);
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
    if (clearInputBuffer() != 0)
    {
        printf("支付取消！\n");
        printCommonInfo();
        return;
    }
    puts("");

    if (choice == '1')
    {
        printf("寄件成功！\n");
        the_user->send_status = 1; // 支付成功后，发件状态置为未发出
        // 写入行为文件
        recordSendBehaviors(the_user->account, package->package_id, getTime());

        // 加急件优先头插
        if (package->isExpress == 1)
        {
            listAddHead(users_send_list, package);
        }
        else
        {
            listAdd(users_send_list, package);
        }
    }
    else
    {
        printf("支付取消！\n");
    }
    printCommonInfo();
}

double payment(Package *package)
{
    double payment = 0.0;
    
    // 体积
    payment += package->volume * 3;

    // 重量
    payment += package->weight * 5;

    if (package->special_type == 0)
    {
        payment += 5;
    }
    else if (package->special_type == 1)
    {
        payment += 10;
    }
    else
    {
        payment += 15;
    }

    // 价值
    payment += package->value * 0.01;

    return payment;
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
            printf("货架号：%s\n", package->shelf_id);
            printf("取件码：%d\n", package->pick_up_code);
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
    printf("请输入您的反馈：\n\n");
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
    printf("\n反馈成功！\n");
    printCommonInfo();
}

void userModifySend()
{
    // 对寄件链表进行修改
    system("cls");

    // 若干提前判断
    if (the_user->send_status == 0)
    {
        printf("暂无快递发出！\n");
        printCommonInfo();
        return;
    }
    if (the_user->send_status == 2)
    {
        printf("您的快递已发出，无法修改！\n");
        printCommonInfo();
        return;
    }

    printf("请输入您的快递单号：\n");
    char package_id[20];
    scanf("%s", package_id);
    clearInputBuffer();
    puts("");

    // 遍历寄件链表，找到要修改的那个快递
    // 根据快递单号找到对应的快递，是唯一的；不要根据用户名来找
    ListNode *current = users_send_list->head;
    while (current != NULL)
    {
        Package *package = (Package *)current->data;
        if (strcmp(package->package_id, package_id) == 0 && strcmp(package->receiver_account, the_user->account) == 0) // 还要判断是本人
        {
            while (1)
            {
                system("cls");
                printf("请选择要修改的信息：\n"); // 寄件人不允许修改
                printf("1. 快递单号\n");
                printf("2. 加急状态\n");
                printf("3. 体积\n");
                printf("4. 重量\n");
                printf("5. 特殊类型\n");
                printf("6. 价值\n");
                printf("7. 收件人\n\n");
                printf("按其他任意键返回...\n");

                char choice = getchar();
                if (clearInputBuffer() != 0)
                {
                    return;
                }
                puts("");

                switch (choice)
                {
                case '1':
                    printf("请输入新的快递单号：\n");
                    scanf("%s", package->package_id);
                    clearInputBuffer();
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
                    printf("请输入新的体积(升)：\n");
                    scanf("%lf", &package->volume);
                    clearInputBuffer();
                    puts("");
                    break;
                case '4':
                    printf("请输入新的重量(kg)：\n");
                    scanf("%lf", &package->weight);
                    clearInputBuffer();
                    puts("");
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
                case '6':
                    printf("请输入新的价值(元)：\n");
                    scanf("%lf", &package->value);
                    clearInputBuffer();
                    puts("");
                    break;
                case '7':
                    printf("请输入新的收件人：\n");
                    scanf("%s", package->receiver_account);
                    clearInputBuffer();
                    break;
                default:
                    return;
                }
            }
        }
        current = current->next;
    }
    printf("未找到该快递！\n");
    printCommonInfo();
}

void userCancelSend()
{
    // 对寄件链表进行删除
    system("cls");

    // 若干提前判断
    if (the_user->send_status == 0)
    {
        printf("暂无快递发出！\n");
        printCommonInfo();
        return;
    }
    if (the_user->send_status == 2)
    {
        printf("您的快递已发出，无法取消！\n");
        printCommonInfo();
        return;
    }

    printf("请输入您的快递单号：\n");
    char package_id[20];
    scanf("%s", package_id);
    clearInputBuffer();
    puts("");

    // 遍历寄件链表，找到要删除的那个快递
    // 根据快递单号找到对应的快递，是唯一的；不要根据用户名来找
    ListNode *current = users_send_list->head;
    while (current != NULL)
    {
        Package *package = (Package *)current->data;
        if (strcmp(package->package_id, package_id) == 0 && strcmp(package->receiver_account, the_user->account) == 0) // 还要判断是本人
        {
            listRemove(users_send_list, package);
            printf("取消成功！\n");
            printf("您支付的费用将会退还到您的账户！\n");
            printCommonInfo();
            return;
        }
        current = current->next;
    }
    printf("未找到该快递！\n");
    printCommonInfo();
}

void recordPickUpBehaviors(const char *user_name, const char *package, struct tm *local_time)
{
    FILE *fp = fopen("../files/pickup_records.txt", "a");
    if (fp == NULL)
    {
        printf("文件打开失败！\n");
        return;
    }
    // 把用户名、包裹号、时间写入文件
    fprintf(fp, "用户名：%s  快递单号：%s  时间：%d-%d-%d %d:%d:%d\n", user_name, package, local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
    fclose(fp);
}

void recordSendBehaviors(const char *user_name, const char *package, struct tm *local_time)
{
    FILE *fp = fopen("../files/send_records.txt", "a");
    if (fp == NULL)
    {
        printf("文件打开失败！\n");
        return;
    }
    // 把用户名、包裹号、时间写入文件
    fprintf(fp, "用户名：%s  快递单号：%s  时间：%d-%d-%d %d:%d:%d\n", user_name, package, local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
    fclose(fp);
}

void deleteUserAccount()
{
    system("cls");

    // 如果当前未取件，不允许注销
    // 这样逻辑是自洽的，因为入库推送的策略，就是自动给用户注册账号
    // 只要用户的快递存在，那么他的号就必须得在
    if (the_user->receive_status == 1)
    {
        printf("您有快递未取件，请先取件！\n");
        printCommonInfo();
        return;
    }

    printf("您确定要注销账号吗？\n");
    printf("1. 是\n");
    printf("按其他任意键返回...\n");

    char choice = getchar();
    if (clearInputBuffer() != 0)
    {
        printf("取消注销！\n");
        printCommonInfo();
        return;
    }
    puts("");

    if (choice == '1')
    {
        // 从用户链表中删除
        listRemove(users_list, the_user);

        isDeleted = 1;
        printf("注销成功！\n");
        printCommonInfo();
    }
    else
    {
        printf("取消注销！\n");
        printCommonInfo();
    }
}

void userSubstitute()
{
    system("cls");

    // 提供两种代取方式
    // 1. 代取人账号和手机号
    // 2. 好友代取

    printf("请选择代取方式：\n\n");
    printf("1. 验证包裹收件人的账号和手机号\n");
    printf("2. 好友代取\n");
    printf("按其他任意键返回...\n\n");

    char choice = getchar();
    if (clearInputBuffer() != 0)
    {
        return;
    }
    puts("");

    switch (choice)
    {
    case '1':
        confirmAccountAndPhoneNumber();
        break;
    case '2':
        helpFriend();
        break;
    default:
        return;
    }
}

void confirmAccountAndPhoneNumber()
{
    system("cls");
    // 通过手机号和用户名来验证代取人与快递主人身份
    // 正确输入后，依然需要取件码验证
    // 因此，需要主人告知代取者取件码
    printf("请输入快递收件人用户名：\n");
    char substitute_account[20];
    scanf("%s", substitute_account);
    clearInputBuffer();
    puts("");

    printf("请输入快递收件人手机号：\n");
    char substitute_phone_number[20];
    scanf("%s", substitute_phone_number);
    clearInputBuffer();
    puts("");

    // 调用get函数，找快递主人
    // get函数写得比较苛刻，找包裹的话，只能用快递单号，没法用用户名去找；而且只能找到一个，因为快递单号是唯一的。但一个用户可能有多个包裹，所以用户名没法送来给包裹写get函数
    User *user = userElementGet(users_list, substitute_account);
    if (user == NULL)
    {
        printf("用户不存在！\n");
        printCommonInfo();
        return;
    }
    if (strcmp(user->phone_number, substitute_phone_number) != 0)
    {
        printf("手机号错误！\n");
        printCommonInfo();
        return;
    }

    // 代取人身份验证成功

    // 判断是否有快递需要代取
    if (user->receive_status == 0)
    {
        printf("暂无快递可代取！\n");
        printCommonInfo();
        return;
    }

    // 遍历推送链表，找到对应的快递
    ListNode *current = users_push_list->head;
    while (current != NULL)
    {
        Package *package = (Package *)current->data;
        if (strcmp(package->receiver_account, substitute_account) == 0)
        {
            printf("货架号：%s\n", package->shelf_id);
            printf("--------------------\n");
            printf("请输入取件码：\n");
            int input;
            scanf("%d", &input);
            if (clearInputBuffer() != 0)
            {
                printf("取件码错误！\n");
                printCommonInfo();
                return;
            }
            puts("");

            if (input == package->pick_up_code)
            {
                // 代取成功，执行出库操作
                int index_of_shelf = package->shelf_id[0] - 'A';
                List *shelf_list = NULL;

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

                // 推移到下一个快递，再删除
                current = current->next;

                // 从货架中删除。这里我们把快递信息完整存到临时链表里了，所以不用再调用getElement函数
                listRemove(shelf_list, package);

                // 代取成功，删除推送链表的信息
                listRemove(users_push_list, package);

                // 写入行为文件
                recordPickUpBehaviors(the_user->account, package->package_id, getTime()); // 用户名写代取人信息

                printf("代取成功！\n\n");
                // 这里不要再按任意键继续了 不符合逻辑

                continue; // 跳过一般更新
            }
            else
            {
                printf("取件码错误！\n");
                printCommonInfo();
                return;
            }
        }
        current = current->next;
    }
    // 代取完后，将代取人的取件状态置为0
    user->receive_status = 0; 
    // 不管是否滞留，统一置为0即可
    user->delivery_leave = 0;
    printf("\n全部代取成功！\n");
    printCommonInfo();
}

void helpFriend()
{
    system("cls");

    if (strcmp(the_user->friend, "0") == 0) // 弊端在于，用户名不能命名为0了，否则检测不到好友
    {
        printf("您没有好友，无法使用好友代取功能！\n");
        printCommonInfo();
        return;
    }

    // 好友必然在用户链表中，但有可能遇到其注销的情况
    User *friend_user = userElementGet(users_list, the_user->friend);
    if (friend_user == NULL)
    {
        printf("好友不存在！\n");
        printCommonInfo();
        return;
    }

    // 如果好友没有快递，直接返回
    if (friend_user->receive_status == 0)
    {
        printf("好友暂无快递可代取！\n");
        printCommonInfo();
        return;
    }

    // 遍历推送链表，找到对应的快递
    ListNode *current = users_push_list->head;
    while (current != NULL)
    {
        Package *package = (Package *)current->data;
        if (strcmp(package->receiver_account, the_user->friend) == 0) // 代取人是好友
        {
            printf("货架号：%s\n", package->shelf_id);
            printf("--------------------\n");
            printf("请输入取件码：\n");
            int input;
            scanf("%d", &input);
            if (clearInputBuffer() != 0)
            {
                printf("取件码错误！\n");
                printCommonInfo();
                return;
            }
            puts("");

            if (input == package->pick_up_code)
            {
                // 代取成功，执行出库操作
                int index_of_shelf = package->shelf_id[0] - 'A';
                List *shelf_list = NULL;

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

                // 推移到下一个快递，再删除
                current = current->next;

                // 从货架中删除。这里我们把快递信息完整存到临时链表里了，所以不用再调用getElement函数
                listRemove(shelf_list, package);

                // 代取成功，删除推送链表的信息
                listRemove(users_push_list, package);

                // 写入行为文件
                recordPickUpBehaviors(the_user->account, package->package_id, getTime()); // 用户名写代取人信息

                printf("代取成功！\n\n");
                // 这里不要再按任意键继续了 不符合逻辑

                continue; // 跳过一般更新
            }
            else
            {
                printf("取件码错误！\n");
                printCommonInfo();
                return;
            }
        }
        current = current->next;
    }
    // 代取完后，将好友的取件状态置为0
    friend_user->receive_status = 0;
    // 不管是否滞留，统一置为0即可
    friend_user->delivery_leave = 0;
    printf("\n全部代取成功！\n");
    printCommonInfo();
}

void userModifyInfo()
{
    while (1)
    {
        system("cls");
        printf("请选择要修改的信息：\n\n");
        printf("1. 用户名\n");
        printf("2. 密码\n");
        printf("3. 手机号\n\n");
        printf("按任意键退出...\n");

        char choice = getchar();
        if (clearInputBuffer() != 0)
        {
            return;
        }
        puts("");

        system("cls");
        switch (choice)
        {
        case '1':
            printf("请输入新的用户名：\n");
            char new_account[20];
            scanf("%s", new_account);
            clearInputBuffer();
            if (userElementGet(users_list, new_account) != NULL)
            {
                printf("用户名已存在！\n");
                printCommonInfo();
                return;
            }
            strcpy(the_user->account, new_account);
            printf("修改成功！\n");
            printCommonInfo();
            break;
        case '2':
            printf("请输入新的密码：\n");
            char new_password[20];
            scanf("%s", new_password);
            clearInputBuffer();
            printf("请再次输入新的密码：\n");
            char new_password_confirm[20];
            scanf("%s", new_password_confirm);
            clearInputBuffer();
            if (strcmp(new_password, new_password_confirm) != 0)
            {
                printf("两次密码不一致！\n");
                printCommonInfo();
                return;
            }
            strcpy(the_user->password, new_password);
            printf("修改成功！\n");
            printCommonInfo();
            break;
        case '3':
            printf("请输入新的手机号：\n");
            char new_phone_number[20];
            scanf("%s", new_phone_number);
            clearInputBuffer();
            strcpy(the_user->phone_number, new_phone_number);
            printf("修改成功！\n");
            printCommonInfo();
            break;
        default:
            return;
        }
    }
}

void friendFunction()
{
    system("cls");
    printf("请选择您的操作：\n\n");
    printf("1. 添加好友\n");
    printf("2. 删除好友\n");
    printf("3. 查看好友\n\n");
    printf("按其他任意键返回...\n");

    char choice = getchar();
    if (clearInputBuffer() != 0)
    {
        return;
    }
    puts("");

    switch (choice)
    {
    case '1':
        addFriend();
        break;
    case '2':
        deleteFriend();
        break;
    case '3':
        viewFriend();
        break;
    default:
        return;
    }
}

void addFriend()
{
    system("cls");
    // 如果有好友，直接返回
    if (strcmp(the_user->friend, "0") != 0)
    {
        printf("您已经有好友了！\n");
        printCommonInfo();
        return;
    }

    printf("请输入好友的用户名：\n");
    char friend_account[20];
    scanf("%s", friend_account);

    // 必须保证好友存在于用户链表
    User *friend_user = userElementGet(users_list, friend_account);
    if (friend_user == NULL)
    {
        printf("好友不存在！\n");
        printCommonInfo();
        return;
    }

    strcpy(the_user->friend, friend_account);
    
    clearInputBuffer();
    puts("");
    printf("添加成功！\n");
    printCommonInfo();
}

void deleteFriend()
{
    system("cls");
    // 如果没有好友，直接返回
    if (strcmp(the_user->friend, "0") == 0)
    {
        printf("您没有好友！\n");
        printCommonInfo();
        return;
    }

    printf("请验证好友的用户名：\n");
    char friend_account[20];
    scanf("%s", friend_account);
    clearInputBuffer();
    puts("");

    if (strcmp(friend_account, the_user->friend) != 0)
    {
        printf("好友用户名错误！\n");
        printCommonInfo();
        return;
    }

    // 删除好友
    strcpy(the_user->friend, "0");
    printf("删除成功！\n");
    printCommonInfo();
}

void viewFriend()
{
    system("cls");
    if (strcmp(the_user->friend, "0") == 0)
    {
        printf("您没有好友！\n");
        printCommonInfo();
        return;
    }
    printf("您的好友为：%s\n", the_user->friend);
    printCommonInfo();
}

void viewUserBehaviors()
{
    system("cls");
    // 分别遍历pickup_records.txt和send_records.txt文件，输出用户的行为记录
    FILE *fp = fopen("../files/pickup_records.txt", "r");
    if (fp == NULL)
    {
        printf("文件打开失败！\n");
        return;
    }
    char line[100];
    printf("您的取件记录：\n");
    printf("--------------------\n");
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (strstr(line, the_user->account) != NULL) // 查找包含用户名的行
        {
            printf("%s", line);
        }
    }
    fclose(fp);
    puts("");

    fp = fopen("../files/send_records.txt", "r");
    if (fp == NULL)
    {
        printf("文件打开失败！\n");
        return;
    }
    printf("您的寄件记录：\n");
    printf("--------------------\n");
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (strstr(line, the_user->account) != NULL)
        {
            printf("%s", line);
        }
    }
    fclose(fp);
    printCommonInfo();
}