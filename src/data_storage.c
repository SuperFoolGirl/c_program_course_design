#include "data_storage.h"

// 初始化链表
// 由于创建了链表结构体，返回值不再是头节点指针
List *listInit()
{
    List *list = (List *)malloc(sizeof(List));
    list->head = NULL;
    list->size = 0;
    return list;
}

// 在链表尾部添加元素
void listAdd(List *list, void *data)
{
    ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));
    new_node->data = data;
    new_node->next = NULL;

    // 传过来的list是List类型，里面包含了head头节点和size两个成员
    // 这里的if语句 表明listAdd函数支持从零开始创建链表（仅含init后的一个空的头节点）
    if (list->head == NULL)
    {
        list->head = new_node;
    }
    else
    {
        ListNode *current = list->head; // 记录头节点
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_node;
    }
    list->size++;
}

// 从链表中移除指定元素
void listRemove(List *list, void *data)
{
    ListNode *current = list->head;
    ListNode *prev = NULL;

    while (current != NULL)
    {
        if (current->data == data) // data是void*类型，只要两个结构体地址相同，就认为是同一个元素
        {
            if (prev == NULL) // 处理头节点的删除
            {
                list->head = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            free(current);
            list->size--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

// 释放链表内存
void listFree(List *list)
{
    ListNode *current = list->head;
    ListNode *next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    free(list); // 最后释放该List结构体
}

// 格式化读取文件，将文件中的数据读取到链表中
// fscanf参数三需要取地址
void writeListFromFile(const char *file, List *list)
{
    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        printf("文件打开失败！\n");
        return;
    }

    // 根据文件名判断是哪个表的数据，然后读取数据
    // 解释下strstr函数的原因。因为文件名是相对路径，所以我们需要取文件名的最后一部分来判断
    // 1. 用户表
    if (strstr(file, "users_info.txt") != NULL)
    {
        User *user = (User *)malloc(sizeof(User));
        while (fscanf(fp, "%s %s %s %d %d %d\n", user->account, user->password, user->phone_number, &user->user_type, &user->receive_status, &user->send_status) != EOF)
        {
            listAdd(list, user);
            user = (User *)malloc(sizeof(User));
        }
        free(user); // 释放最后一次分配的内存，这部分内存并没有被使用就出循环了
    }

    // 2. 快递员表
    else if (strstr(file, "couriers_info.txt") != NULL)
    {
        Courier *courier = (Courier *)malloc(sizeof(Courier));
        while (fscanf(fp, "%s %s %d\n", courier->account, courier->password, &courier->status) != EOF)
        {
            listAdd(list, courier);
            courier = (Courier *)malloc(sizeof(Courier));
        }
        free(courier);
    }
    // 3. 管理员表
    else if (strstr(file, "admins_info.txt") != NULL)
    {
        Admin *admin = (Admin *)malloc(sizeof(Admin));
        while (fscanf(fp, "%s %s %d\n", admin->account, admin->password) != EOF)
        {
            listAdd(list, admin);
            admin = (Admin *)malloc(sizeof(Admin));
        }
        free(admin);
    }
    // 4. 平台仓库表和驿站寄件表、五个货架，可以共用
    else if (strstr(file, "platform_warehouse.txt") != NULL || strstr(file, "users_send.txt") != NULL || strstr(file, "shelf_") != NULL) // 这里就不罗列五个架子了
    {
        Package *package = (Package *)malloc(sizeof(Package));
        while (fscanf(fp, "%s %s %s %d %d %d %d %d\n", package->package_id, package->receiver_account, package->courier_account, &package->isExpress, &package->volume, &package->weight, &package->special_type, &package->value) != EOF)
        {
            listAdd(list, package);
            package = (Package *)malloc(sizeof(Package));
        }
        free(package);
    }
    else
        ;
    fclose(fp);
}

// 写法与上个函数类似。这里是格式化写
// fprintf参数三不需要取地址
void writeFileFromList(const char *file, List *list)
{
    FILE *fp = fopen(file, "w");
    if (fp == NULL)
    {
        printf("文件打开失败！\n");
        return;
    }

    ListNode *current = list->head;
    if (strstr(file, "users_info.txt") != NULL)
    {
        while (current != NULL)
        {
            User *user = (User *)current->data;
            fprintf(fp, "%s %s %s %d %d %d\n", user->account, user->password, user->phone_number, user->user_type, user->receive_status, user->send_status);
            current = current->next;
        }
    }
    else if (strstr(file, "admins_info.txt") != NULL)
    {
        while (current != NULL)
        {
            Admin *admin = (Admin *)current->data;
            fprintf(fp, "%s %s\n", admin->account, admin->password);
            current = current->next;
        }
    }
    else if (strstr(file, "couriers_info.txt") != NULL)
    {
        while (current != NULL)
        {
            Courier *courier = (Courier *)current->data;
            fprintf(fp, "%s %s %d\n", courier->account, courier->password, courier->status);
            current = current->next;
        }
    }
    else if (strstr(file, "platform_warehouse.txt") != NULL || strstr(file, "users_send.txt") != NULL || strstr(file, "shelf_") != NULL)
    {
        while (current != NULL)
        {
            Package *package = (Package *)current->data;
            fprintf(fp, "%s %s %s %d %d %d %d %d\n", package->package_id, package->receiver_account, package->courier_account, package->isExpress, package->volume, package->weight, package->special_type, package->value);
            current = current->next;
        }
    }
    else
        ;
    fclose(fp);
}

User *userElementGet(List *list, const char *account)
{
    ListNode *current = list->head;
    while (current != NULL)
    {
        User *user = (User *)current->data; // 这里的current->data是void*类型，需要强制转换
        if (strcmp(account, user->account) == 0)
        {
            return user;
        }
    }
    return NULL;
}

Courier *courierElementGet(List *list, const char *account)
{
    ListNode *current = list->head;
    while (current != NULL)
    {
        Courier *courier = (Courier *)current->data;
        if (strcmp(account, courier->account) == 0)
        {
            return courier;
        }
    }
    return NULL;
}

Admin *adminElementGet(List *list, const char *account)
{
    ListNode *current = list->head;
    while (current != NULL)
    {
        Admin *admin = (Admin *)current->data;
        if (strcmp(account, admin->account) == 0)
        {
            return admin;
        }
    }
    return NULL;
}

Package *packageElementGet(List *list, const char *package_id)
{
    ListNode *current = list->head;
    while (current != NULL)
    {
        Package *package = (Package *)current->data;
        if (strcmp(package_id, package->package_id) == 0)
        {
            return package;
        }
    }
    return NULL;
}

void listAddHead(List *list, void *data)
{
    ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));
    new_node->data = data;
    new_node->next = list->head;
    list->head = new_node;
    list->size++;
}

// 为快递员单独写一个packageElementGet函数，因为需要用用户名配对，而不是包裹ID
Package *packageElementGetByCourier(List *list, const char *receiver_account)
{
    ListNode *current = list->head;
    while (current != NULL)
    {
        Package *package = (Package *)current->data;
        if (strcmp(receiver_account, package->receiver_account) == 0)
        {
            return package;
        }
    }
    return NULL;
}

// 清空输入缓冲区的函数
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void printCommonInfo()
{
    puts("");
    printf("按任意键继续\n");
    getchar();
    clearInputBuffer();
}