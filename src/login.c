#include "login.h"

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

// 读取运输平台信息并验证（无链表）
int verifyPlatform(const char *account, const char *password)
{
    FILE *fp = fopen("../files/platforms_info.txt", "r");
    if (fp == NULL)
    {
        printf("文件打开失败！\n");
        return 0;
    }

    char file_account[20];
    char file_password[20];

    while (fscanf(fp, "%s %s", file_account, file_password) != EOF)
    {
        if (strcmp(account, file_account) == 0 && strcmp(password, file_password) == 0)
        {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// 读取文件中的数据，恢复链表数据
// 恢复的链表为：用户链表、快递员链表、驿站链表、平台链表
void recoverListData()
{
    writeListFromFile("../files/users_info.txt", users_list);
    writeListFromFile("../files/admins_info.txt", admins_list);
    writeListFromFile("../files/couriers_info.txt", couriers_list);
    writeListFromFile("../files/platform_warehouse.txt", platform_warehouse_list);
    writeListFromFile("../files/admin_warehouse.txt", admin_warehouse_list);
    writeListFromFile("../files/users_send.txt", users_send_list);
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
    writeFileFromList("../files/platform_warehouse.txt", platform_warehouse_list);
    writeFileFromList("../files/admin_warehouse.txt", admin_warehouse_list);
    writeFileFromList("../files/users_send.txt", users_send_list);
    writeFileFromList("../files/shelf_a.txt", shelf_a_list);
    writeFileFromList("../files/shelf_b.txt", shelf_b_list);
    writeFileFromList("../files/shelf_c.txt", shelf_c_list);
    writeFileFromList("../files/shelf_d.txt", shelf_d_list);
    writeFileFromList("../files/shelf_e.txt", shelf_e_list);
}

// 释放链表内存
void freeLists()
{
    listFree(users_list);
    listFree(admins_list);
    listFree(couriers_list);
    listFree(platform_warehouse_list);
    listFree(admin_warehouse_list);
    listFree(users_send_list);
    listFree(shelf_a_list);
    listFree(shelf_b_list);
    listFree(shelf_c_list);
    listFree(shelf_d_list);
    listFree(shelf_e_list);
}