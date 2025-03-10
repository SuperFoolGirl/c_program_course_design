#include "data_storage.h"

// 初始化链表
// 由于创建了链表结构体，返回值不再是头节点指针
List *listInit()
{
    // 这里开辟的是List结构体，而不是ListNode结构体
    List *list = (List *)malloc(sizeof(List));
    list->head = NULL;
    list->size = 0;
    return list;
}

// 在链表尾部添加元素
// 浅拷贝，只复制地址，没有深拷贝出来一块新的内存
// 但这样面临一个问题：只要这块内存的信息被改了，其他链表中相同的节点也会改变
// 不过本程序中无妨，我们反而希望信息能够同步
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

            // 释放当前节点的内存
            free(current);

            list->size--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

// 释放内存都需要写4个函数，必须需要类型
// 节点的数据类型也是用动态内存分配的。不能只释放节点，也要释放数据。否则会内存泄漏
// 释放链表内存
// 1. 包裹
void listFreePackage(List *list)
{
    ListNode *current = list->head;
    ListNode *next;

    while (current != NULL)
    {
        next = current->next;

        // 释放 data 所指向的内存，假设 data 指向的是 Package 结构体
        Package *package = (Package *)current->data;
        if (package != NULL)
        {
            free(package);
        }

        free(current);
        current = next;
    }
    free(list); // 最后释放该List结构体
}

// 2. 用户
void listFreeUser(List *list)
{
    ListNode *current = list->head;
    ListNode *next;

    while (current != NULL)
    {
        next = current->next;

        // 释放 data 所指向的内存，假设 data 指向的是 User 结构体
        User *user = (User *)current->data;
        if (user != NULL)
        {
            free(user);
        }

        free(current);
        current = next;
    }
    free(list); // 最后释放该List结构体
}

// 3. 管理员
void listFreeAdmin(List *list)
{
    ListNode *current = list->head;
    ListNode *next;

    while (current != NULL)
    {
        next = current->next;

        // 释放 data 所指向的内存，假设 data 指向的是 Admin 结构体
        Admin *admin = (Admin *)current->data;
        if (admin != NULL)
        {
            free(admin);
        }

        free(current);
        current = next;
    }
    free(list); // 最后释放该List结构体
}

// 4. 快递员
void listFreeCourier(List *list)
{
    ListNode *current = list->head;
    ListNode *next;

    while (current != NULL)
    {
        next = current->next;

        // 释放 data 所指向的内存，假设 data 指向的是 Courier 结构体
        Courier *courier = (Courier *)current->data;
        if (courier != NULL)
        {
            free(courier);
        }

        free(current);
        current = next;
    }
    free(list); // 最后释放该List结构体
}


// 这函数写的简直一坨，恶心
// 格式化读取文件，将文件中的数据读取到链表中
// fscanf参数三需要取地址
void writeListFromFile(const char *file, List *list)
{
    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        printf("文件打开失败！\n");
        printCommonInfo();
        return;
    }

    // 根据文件名判断是哪个表的数据，然后读取数据
    // 解释下strstr函数的原因。因为文件名是相对路径，所以我们需要取文件名的最后一部分来判断
    // 1. 用户表
    if (strstr(file, "users_info.txt") != NULL)
    {
        User *user = (User *)malloc(sizeof(User));
        memset(user, 0, sizeof(User)); // 初始化内存，因为堆区的这块内存可能是脏数据

        while (fscanf(fp, "%s %s %s %d %d %d\n", user->account, user->password, user->phone_number, &user->user_type, &user->receive_status, &user->send_status) != EOF)
        {
            listAdd(list, user);

            user = (User *)malloc(sizeof(User)); // 为下一次循环分配内存
            memset(user, 0, sizeof(User));       // 初始化内存
        }
        free(user); // 释放最后一次分配的内存，这部分内存并没有被使用就出循环了
    }

    // 2. 快递员表
    else if (strstr(file, "couriers_info.txt") != NULL)
    {
        Courier *courier = (Courier *)malloc(sizeof(Courier));
        memset(courier, 0, sizeof(Courier));

        while (fscanf(fp, "%s %s %d\n", courier->account, courier->password, &courier->status) != EOF)
        {
            listAdd(list, courier);

            courier = (Courier *)malloc(sizeof(Courier));
            memset(courier, 0, sizeof(Courier));
        }
        free(courier);
    }
    // 3. 管理员表
    else if (strstr(file, "admins_info.txt") != NULL)
    {
        Admin *admin = (Admin *)malloc(sizeof(Admin));
        memset(admin, 0, sizeof(Admin));

        while (fscanf(fp, "%s %s %d\n", admin->account, admin->password) != EOF)
        {
            listAdd(list, admin);

            admin = (Admin *)malloc(sizeof(Admin));
            memset(admin, 0, sizeof(Admin));
        }
        free(admin);
    }
    // 4. 其他Package类的表可以共用，因为结构体一样
    else if (strstr(file, "platform_warehouse.txt") != NULL || strstr(file, "users_send.txt") != NULL || strstr(file, "shelf_") != NULL || strstr(file, "admin_warehouse.txt") != NULL || strstr(file, "users_push.txt") != NULL || strstr(file, "couriers_push.txt") != NULL) 
    {
        Package *package = (Package *)malloc(sizeof(Package));
        memset(package, 0, sizeof(Package));

        while (fscanf(fp, "%s %s %s %d %d %d %d %d %d\n", package->package_id, package->receiver_account, package->courier_account, &package->pick_up_code, &package->isExpress, &package->volume, &package->weight, &package->special_type, &package->value) != EOF)
        {
            listAdd(list, package);

            package = (Package *)malloc(sizeof(Package));
            memset(package, 0, sizeof(Package));
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
        printCommonInfo();
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
    else if (strstr(file, "platform_warehouse.txt") != NULL || strstr(file, "users_send.txt") != NULL || strstr(file, "shelf_") != NULL || strstr(file, "admin_warehouse.txt") != NULL || strstr(file, "users_push.txt") != NULL || strstr(file, "couriers_push.txt") != NULL)
    {
        while (current != NULL)
        {
            Package *package = (Package *)current->data;
            fprintf(fp, "%s %s %s %d %d %d %d %d %d\n", package->package_id, package->receiver_account, package->courier_account, package->pick_up_code, package->isExpress, package->volume, package->weight, package->special_type, package->value);
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
    new_node->next = list->head; // 这一步对于空链表来说也没错，右值是NULL，左值被赋为NULL
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