#include "../include/login.h"
#include "../include/user.h"
#include "../include/admin.h"
#include "../include/courier.h"
#include "../include/platform.h"

// 登录者结构体在登录成功时进行赋值
extern Admin *the_admin;
extern Courier *the_courier;
extern User *the_user;

// 通过查询链表实现登录
int verifyUser(const char *account, const char *password)
{
    ListNode *current = users_list->head;
    while (current != NULL)
    {
        User *user = (User *)current->data;
        if (strcmp(account, user->account) == 0 && strcmp(password, user->password) == 0)
        {
            the_user = user; // 赋值给临时结构体
            return 1;
        }
        current = current->next;
    }
    return 0;
}

// 读取管理员链表
int verifyAdmin(const char *account, const char *password)
{
    ListNode *current = admins_list->head;
    while (current != NULL)
    {
        Admin *admin = (Admin *)current->data;
        if (strcmp(account, admin->account) == 0 && strcmp(password, admin->password) == 0)
        {
            the_admin = admin; // 赋值给临时结构体
            return 1;
        }
        current = current->next;
    }
    return 0;
}

// 读取快递员链表
int verifyCourier(const char *account, const char *password)
{
    ListNode *current = couriers_list->head;
    while (current != NULL)
    {
        Courier *courier = (Courier *)current->data;
        if (strcmp(account, courier->account) == 0 && strcmp(password, courier->password) == 0)
        {
            the_courier = courier; // 赋值给临时结构体
            return 1;
        }
        current = current->next;
    }
    return 0;
}

// 读取运输平台信息并验证
int verifyPlatform(const char *account, const char *password)
{
    ListNode *current = platforms_list->head;
    while (current != NULL)
    {
        Platform *platform = (Platform *)current->data;
        if (strcmp(account, platform->account) == 0 && strcmp(password, platform->password) == 0)
        {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

// 读取文件中的数据，恢复链表数据
// 恢复的链表为：用户链表、快递员链表、驿站链表、平台链表
void recoverListData()
{
    writeListFromFile("../files/users_info.txt", users_list);
    writeListFromFile("../files/admins_info.txt", admins_list);
    writeListFromFile("../files/couriers_info.txt", couriers_list);
    writeListFromFile("../files/platforms_info.txt", platforms_list);
    writeListFromFile("../files/platform_warehouse.txt", platform_warehouse_list);
    writeListFromFile("../files/admin_warehouse.txt", admin_warehouse_list);
    writeListFromFile("../files/users_send.txt", users_send_list);
    writeListFromFile("../files/users_push.txt", users_push_list);
    writeListFromFile("../files/couriers_push.txt", couriers_push_list);
    writeListFromFile("../files/shelf_a.txt", shelf_a_list);
    writeListFromFile("../files/shelf_b.txt", shelf_b_list);
    writeListFromFile("../files/shelf_c.txt", shelf_c_list);
    writeListFromFile("../files/shelf_d.txt", shelf_d_list);
    writeListFromFile("../files/shelf_e.txt", shelf_e_list);
}

// 把链表全部更新到文件中
void rewriteAllFiles()
{
    writeFileFromList("../files/users_info.txt", users_list);
    writeFileFromList("../files/admins_info.txt", admins_list);
    writeFileFromList("../files/couriers_info.txt", couriers_list);
    writeFileFromList("../files/platforms_info.txt", platforms_list);
    writeFileFromList("../files/platform_warehouse.txt", platform_warehouse_list);
    writeFileFromList("../files/admin_warehouse.txt", admin_warehouse_list);
    writeFileFromList("../files/users_send.txt", users_send_list);
    writeFileFromList("../files/users_push.txt", users_push_list);
    writeFileFromList("../files/couriers_push.txt", couriers_push_list);
    writeFileFromList("../files/shelf_a.txt", shelf_a_list);
    writeFileFromList("../files/shelf_b.txt", shelf_b_list);
    writeFileFromList("../files/shelf_c.txt", shelf_c_list);
    writeFileFromList("../files/shelf_d.txt", shelf_d_list);
    writeFileFromList("../files/shelf_e.txt", shelf_e_list);
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
    listFreePackage(shelf_a_list);
    listFreePackage(shelf_b_list);
    listFreePackage(shelf_c_list);
    listFreePackage(shelf_d_list);
    listFreePackage(shelf_e_list);
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
}

void registering()
{
again:
    system("cls"); // 清屏逻辑：输入后进入下一个页面时后，第一句加清屏，防止上一个页面的打印信息没有呈现就被清除
    // 平台和快递员不允许随便注册
    printf("温馨提示：当前仅支持普通用户注册，其他请联系管理员。\n\n");

    printf("请输入用户名：\n");
    char account[20];
    scanf("%s", account);
    clearInputBuffer();
    puts("");

    printf("请输入密码：\n");
    char password[20];
    scanf("%s", password);
    clearInputBuffer();
    puts("");

    printf("请再次确认密码：\n");
    char password_confirm[20];
    scanf("%s", password_confirm);
    clearInputBuffer();
    puts("");

    if (strcmp(password, password_confirm) != 0)
    {
        printf("两次密码输入不一致，请重新输入！\n");
        printCommonInfo();
        goto again;
    }
    else
    {
        // 添加用户名重复检查
        ListNode *node = users_list->head;
        while (node != NULL)
        {
            User *user = (User *)node->data;
            if (strcmp(user->account, account) == 0)
            {
                printf("用户名已存在，请重新输入！\n");
                printCommonInfo();
                goto again;
            }
            node = node->next;
        }

        printf("请输入电话号码：\n");
        char phone_number[20];
        scanf("%s", phone_number);
        clearInputBuffer();
        puts("");

        int default_user_type = 0;
        int default_receive_status = 0;
        int default_send_status = 0;

        // 先把结构体写入链表
        User *user = (User *)malloc(sizeof(User));
        strcpy(user->account, account);
        strcpy(user->password, password);
        strcpy(user->phone_number, phone_number);
        user->user_type = default_user_type;
        user->receive_status = default_receive_status;
        user->send_status = default_send_status;
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
        printf("请选择登录身份：\n");
        printf("1. 用户\n");
        printf("2. 管理员\n");
        printf("3. 快递员\n");
        printf("4. 运输平台\n\n");
        printf("按其他任意键退出\n");

        char choice = getchar();
        clearInputBuffer();

        if (choice != '1' && choice != '2' && choice != '3' && choice != '4')
        {
            return;
        }

        system("cls");
        char account[20];
        char password[20];

        printf("请输入用户名:\n");
        scanf("%s", account);
        clearInputBuffer();
        puts("");

        printf("请输入密码:\n");
        scanf("%s", password);
        clearInputBuffer();
        puts("");

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
            printf("登陆失败，请检查用户名和密码是否正确\n");
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