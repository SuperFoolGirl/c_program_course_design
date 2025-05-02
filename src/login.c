#include "../include/login.h"
#include "../include/user.h"
#include "../include/admin.h"
#include "../include/courier.h"
#include "../include/platform.h"

// 通过查询链表实现登录
int verifyUser(const char *account, const char *password)
{
    ListNode *current = users_list->head;
    while (current != NULL)
    {
        User *user = (User *)current->data;
        if (strcmp(account, user->account) == 0)
        {
            the_user = user; // 赋值给临时结构体
            // 判断是否锁定
            if (user->try_times >= 5 && difftime(time(NULL), user->time) < 60)
            {
                printf("账号已锁定，请1分钟后再试！\n");
                return 0;
            }
            // 判断密码是否正确
            if (strcmp(password, user->password) == 0)
            {
                // 清空尝试次数
                user->try_times = 0;
                // 清空锁定时刻
                user->time = 0;
                return 1;
            }
            else
            {
                // 如果登录失败
                the_user->try_times++; // 记录尝试次数
                if (the_user->try_times >= 5)
                {
                    the_user->time = time(NULL);
                    printf("您已失败多次，账号将锁定1分钟！\n");
                }
                else
                {
                    printf("用户名或密码错误！\n");
                    printf("您还有%d次尝试机会！\n", 5 - the_user->try_times);
                }
                return 0;
            }
        }
        current = current->next;
    }
    // 如果没有找到用户
    printf("该用户不存在！\n");
    return 0;
}

// 读取管理员链表
int verifyAdmin(const char *account, const char *password)
{
    ListNode *current = admins_list->head;
    while (current != NULL)
    {
        Admin *admin = (Admin *)current->data;
        if (strcmp(account, admin->account) == 0)
        {
            the_admin = admin; // 赋值给临时结构体
            // 判断是否锁定
            if (admin->try_times >= 5 && difftime(time(NULL), admin->time) < 60)
            {
                printf("账号已锁定，请1分钟后再试！\n");
                return 0;
            }

            if (strcmp(password, admin->password) == 0)
            {
                // 清空尝试次数
                admin->try_times = 0;
                // 清空锁定时刻
                admin->time = 0;
                return 1;
            }
            else
            {
                // 如果登录失败
                the_admin->try_times++; // 记录尝试次数
                if (the_admin->try_times >= 5)
                {
                    the_admin->time = time(NULL);
                    printf("您已失败多次，账号将锁定1分钟！\n");
                }
                else
                {
                    printf("用户名或密码错误！\n");
                    printf("您还有%d次尝试机会！\n", 5 - the_admin->try_times);
                }
                return 0;
            }
        }
        current = current->next;
    }
    printf("该管理员不存在！\n");
    return 0;
}

// 读取快递员链表
int verifyCourier(const char *account, const char *password)
{
    ListNode *current = couriers_list->head;
    while (current != NULL)
    {
        Courier *courier = (Courier *)current->data;
        if (strcmp(account, courier->account) == 0)
        {
            the_courier = courier; // 赋值给临时结构体
            // 判断是否锁定
            if (courier->try_times >= 5 && difftime(time(NULL), courier->time) < 60)
            {
                printf("账号已锁定，请1分钟后再试！\n");
                return 0;
            }

            if (strcmp(password, courier->password) == 0)
            {
                // 清空尝试次数
                courier->try_times = 0;
                // 清空锁定时刻
                courier->time = 0;
                return 1;
            }
            else
            {
                // 如果登录失败
                the_courier->try_times++; // 记录尝试次数
                if (the_courier->try_times >= 5)
                {
                    the_courier->time = time(NULL);
                    printf("您已失败多次，账号将锁定1分钟！\n");
                }
                else
                {
                    printf("用户名或密码错误！\n");
                    printf("您还有%d次尝试机会！\n", 5 - the_courier->try_times);
                }
                return 0;
            }
        }
        current = current->next;
    }
    printf("该快递员不存在！\n");
    return 0;
}

// 读取运输平台信息并验证
int verifyPlatform(const char *account, const char *password)
{
    ListNode *current = platforms_list->head;
    while (current != NULL)
    {
        Platform *platform = (Platform *)current->data;
        if (strcmp(account, platform->account) == 0)
        {
            the_platform = platform; // 赋值给临时结构体
            // 判断是否锁定
            if (platform->try_times >= 5 && difftime(time(NULL), platform->time) < 60)
            {
                printf("账号已锁定，请1分钟后再试！\n");
                return 0;
            }

            if (strcmp(password, platform->password) == 0)
            {
                // 清空尝试次数
                platform->try_times = 0;
                // 清空锁定时刻
                platform->time = 0;
                return 1;
            }
            else
            {
                // 如果登录失败
                the_platform->try_times++; // 记录尝试次数
                if (the_platform->try_times >= 5)
                {
                    the_platform->time = time(NULL);
                    printf("您已失败多次，账号将锁定1分钟！\n");
                }
                else
                {
                    printf("用户名或密码错误！\n");
                    printf("您还有%d次尝试机会！\n", 5 - the_platform->try_times);
                }
                return 0;
            }
        }
        current = current->next;
    }
    printf("该快递平台不存在！\n");
    return 0;
}

// 读取文件中的数据，恢复链表数据
// 恢复的链表为：用户链表、快递员链表、驿站链表、平台链表
void recoverListData()
{
    writeListFromFile("../res/users_info.txt", users_list);
    writeListFromFile("../res/admins_info.txt", admins_list);
    writeListFromFile("../res/couriers_info.txt", couriers_list);
    writeListFromFile("../res/platforms_info.txt", platforms_list);

    writeListFromFile("../res/platform_warehouse.txt", platform_warehouse_list);
    writeListFromFile("../res/admin_warehouse.txt", admin_warehouse_list);
    writeListFromFile("../res/users_send.txt", users_send_list);

    writeListFromFile("../res/users_push.txt", users_push_list);
    writeListFromFile("../res/couriers_push.txt", couriers_push_list);

    writeListFromFile("../res/shelf_a.txt", shelf_a_list);
    writeListFromFile("../res/shelf_b.txt", shelf_b_list);
    writeListFromFile("../res/shelf_c.txt", shelf_c_list);
    writeListFromFile("../res/shelf_d.txt", shelf_d_list);
    writeListFromFile("../res/shelf_e.txt", shelf_e_list);

    writeListFromFile("../res/feedback.txt", feedback_list);
    writeFileFromList("../res/refuse.txt", refuse_list);

    getMoney("../res/account.txt", &money);
}

// 把链表全部更新到文件中
void rewriteAllFiles()
{
    writeFileFromList("../res/users_info.txt", users_list);
    writeFileFromList("../res/admins_info.txt", admins_list);
    writeFileFromList("../res/couriers_info.txt", couriers_list);
    writeFileFromList("../res/platforms_info.txt", platforms_list);

    writeFileFromList("../res/platform_warehouse.txt", platform_warehouse_list);
    writeFileFromList("../res/admin_warehouse.txt", admin_warehouse_list);
    writeFileFromList("../res/users_send.txt", users_send_list);

    writeFileFromList("../res/users_push.txt", users_push_list);
    writeFileFromList("../res/couriers_push.txt", couriers_push_list);

    writeFileFromList("../res/shelf_a.txt", shelf_a_list);
    writeFileFromList("../res/shelf_b.txt", shelf_b_list);
    writeFileFromList("../res/shelf_c.txt", shelf_c_list);
    writeFileFromList("../res/shelf_d.txt", shelf_d_list);
    writeFileFromList("../res/shelf_e.txt", shelf_e_list);

    writeFileFromList("../res/feedback.txt", feedback_list);
    writeFileFromList("../res/refuse.txt", refuse_list);

    updateMoney("../res/account.txt", money); // 更新账户余额
}

// 释放链表内存
void freeLists()
{
    listFreeUser(users_list);
    listFreeAdmin(admins_list);
    listFreeCourier(couriers_list);
    listFreePlatform(platforms_list);

    listFreePackage(platform_warehouse_list);
    listFreePackage(admin_warehouse_list);
    listFreePackage(users_send_list);

    listFreePackage(users_push_list);
    listFreePackage(couriers_push_list);

    // 如果取件了，那包裹节点会彻底消失
    // 如果没取件，那包裹节点会同时存在于推送链表、临时链表、货架链表
    // 其中临时链表只释放了链表结构体本身，推送链表已经释放了包裹节点及包裹数据
    // 如果下面的货架链表再释放，就会导致重复释放包裹节点
    listFree(shelf_a_list);
    listFree(shelf_b_list);
    listFree(shelf_c_list);
    listFree(shelf_d_list);
    listFree(shelf_e_list);

    listFreeFeedback(feedback_list);

    // 包裹拒收后，对应的节点会在临时、推送、货架中删除，所以这里不存在重复释放的问题
    listFreePackage(refuse_list);
}

// 初始化所有链表
void listsInit()
{
    users_list = listInit();
    admins_list = listInit();
    couriers_list = listInit();
    platforms_list = listInit();

    platform_warehouse_list = listInit();
    admin_warehouse_list = listInit();
    users_send_list = listInit();
    
    users_push_list = listInit();
    couriers_push_list = listInit();

    shelf_a_list = listInit();
    shelf_b_list = listInit();
    shelf_c_list = listInit();
    shelf_d_list = listInit();
    shelf_e_list = listInit();

    feedback_list = listInit();
    refuse_list = listInit();
}

// 注册是先写信息再申请的堆区内存，因此退出不需要释放
void registering()
{
rewrite_account:
    system("cls"); // 清屏逻辑：输入后进入下一个页面时后，第一句加清屏，防止上一个页面的打印信息没有呈现就被清除
    // 平台和快递员不允许随便注册
    printf("如若需要强制退出，请输入“exit”\n\n");
    printf("温馨提示：当前仅支持普通用户注册，其他请联系管理员。\n\n");

    printf("请输入用户名(3-12字符)：\n");
    char account[200];
    scanf("%s", account);
    clearInputBuffer();
    puts("");

    if (checkExit(account))
    {
        return;
    }

    if (checkInputLimit(account) == 0)
    {
        goto rewrite_account;
    }

    // 添加用户名重复检查
    ListNode *node = users_list->head;
    while (node != NULL)
    {
        User *user = (User *)node->data;
        if (strcmp(user->account, account) == 0)
        {
            printf("用户名已存在，请重新输入！\n");
            printCommonInfo();
            goto rewrite_account;
        }
        node = node->next;
    }

rewrite_password:
    printf("请输入密码(3-12字符)：\n");
    char password[200];
    getPassword(password); // 使用getPassword函数获取密码，隐藏输入
    // 不需要清空输入缓冲区，因为getPassword函数已经处理了。这里如果加上clearInputBuffer()，会导致得额外输入一个回车才能结束输入
    puts("");

    if (checkExit(password))
    {
        return;
    }

    if (checkInputLimit(password) == 0)
    {
        goto rewrite_password;
    }

    printf("请再次确认密码：\n");
    char password_confirm[200];
    getPassword(password_confirm); // 使用getPassword函数获取密码，隐藏输入
    puts("");

    if (checkExit(password_confirm))
    {
        return;
    }

    if (strcmp(password, password_confirm) != 0)
    {
        printf("两次密码输入不一致，请重新输入！\n");
        printCommonInfo();
        goto rewrite_password;
    }
    else
    {
    rewrite_phone_number:
        printf("请输入电话号码：\n");
        char phone_number[200];
        scanf("%s", phone_number);
        clearInputBuffer();
        puts("");

        if (checkExit(phone_number))
        {
            return;
        }

        if (checkInputLimit(phone_number) == 0)
        {
            goto rewrite_phone_number;
        }

        int default_user_type = 0;
        int default_receive_status = 0;
        int default_send_status = 0;

        // 先把结构体写入链表
        User *user = (User *)malloc(sizeof(User));
        strcpy(user->account, account);
        strcpy(user->password, password);
        strcpy(user->phone_number, phone_number);
        strcpy(user->friend[0], "0");
        strcpy(user->friend[1], "0");
        strcpy(user->friend[2], "0");
        user->user_type = default_user_type;
        user->receive_status = default_receive_status;
        user->send_status = default_send_status;
        user->delivery_leave = 0;
        user->time = 0;
        user->try_times = 0;
        user->message_status = 0;
        strcpy(user->message, "0");
        listAdd(users_list, user);

        printf("注册成功！\n");
        printCommonInfo();
    }
}

void login()
{
    while (1)
    {
        system("cls");
        printf("请选择登录身份：\n\n");
        printf("1. 用户\n");
        printf("2. 管理员\n");
        printf("3. 快递员\n");
        printf("4. 运输平台\n\n");
        printf("按其他任意键退出...\n");

        char choice = _getch();

        if (choice != '1' && choice != '2' && choice != '3' && choice != '4')
        {
            return;
        }

        system("cls");
        char account[200];
        char password[200];
        printf("如若需要强制退出，请输入“exit”\n\n");

    rewrite_account:
        printf("请输入用户名(3-12字符):\n");
        scanf("%s", account);
        clearInputBuffer();
        puts("");

        if (checkExit(account))
        {
            return;
        }

        if (checkInputLimit(account) == 0)
        {
            goto rewrite_account;
        }

    rewrite_password:
        printf("请输入密码(3-12字符):\n");
        getPassword(password); // 使用getPassword函数获取密码，隐藏输入
        puts("");

        if (checkExit(password))
        {
            return;
        }

        if (checkInputLimit(password) == 0)
        {
            goto rewrite_password;
        }
        
        int ret = 0;
        switch (choice)
        {
        case '1':
            ret = verifyUser(account, password);
            break;
        case '2':
            ret = verifyAdmin(account, password);
            break;
        case '3':
            ret = verifyCourier(account, password);
            break;
        case '4':
            ret = verifyPlatform(account, password);
            break;
        default:
            break;
        }
        // 登录失败
        if (ret == 0)
        {
            printCommonInfo();
            continue;
        }

        // 登录成功后，根据身份进入对应的菜单
        switch (choice)
        {
        case '1':
            userShowMenu();
            break;
        case '2':
            adminShowMenu();
            break;
        case '3':
            courierShowMenu();
            break;
        case '4':
            platformShowMenu();
            break;
        default:
            break;
        }
    }
}

void getPassword(char *password)
{
    int i = 0;
    char ch;
    while ((ch = _getch()) != '\r')
    {
        if (ch == '\b' && i > 0)
        {
            // 处理退格键
            i--;
            printf("\b \b");
        }
        else if (i < MAX_LENGTH - 1)
        {
            password[i++] = ch;
            putchar('*');
        }
    }
    password[i] = '\0';
    putchar('\n');
}