#include "admin.h"

Admin *the_admin = NULL;

void adminShowMenu()
{
    system("cls");

    // 弹窗提醒模块
    if (admin_warehouse_list->size > 0)
    {
        printf("快递已到达，请及时入库！\n");

        // 为了防止弹窗一闪而过 需要加一个确认判断
        printCommonInfo();

        system("cls"); // 为下一个弹窗清屏
    }
    puts("");

    // 架子容量警告
    int flag = 0;
    if (shelf_a_list->size >= 80)
    {
        printf("货架A已接近满载，请及时清理！\n");
        flag = 1;
    }
    if (shelf_b_list->size >= 80)
    {
        printf("货架B已接近满载，请及时清理！\n");
        flag = 1;
    }
    if (shelf_c_list->size >= 80)
    {
        printf("货架C已接近满载，请及时清理！\n");
        flag = 1;
    }
    if (shelf_d_list->size >= 80)
    {
        printf("货架D已接近满载，请及时清理！\n");
        flag = 1;
    }
    if (shelf_e_list->size >= 80)
    {
        printf("货架E已接近满载，请及时清理！\n");
        flag = 1;
    }
    if (flag == 1)
    {
        printCommonInfo();
    }

    while (1)
    {
        system("cls");
        printf("欢迎登陆！");
        printf("请您选择操作：\n");
        printf("1. 入库\n");
        printf("2. 用户管理\n");
        printf("3. 货架管理\n");
        printf("4. 查看反馈\n");
        printf("5. 库存盘点\n");
        printf("6. 查看业务统计\n");
        printf("7. 为用户推送消息\n");
        printf("8. 处理用户寄件\n");
        printf("按其他任意键退出\n");

        char choice = getchar();
        clearInputBuffer();

        switch (choice)
        {
        case '1':
            wareHousing();
            break;
        case '2':
            userManagement();
            break;
        case '3':
            shelfManagement();
            break;
        case '4':
            viewFeedback();
            break;
        case '5':
            inventoryCheck();
            break;
        case '6':
            viewBusinessStatistics();
            break;
        case '7':
            pushMessage();
            break;
        case '8':
            addressUserSend();
            break;
        default:
            return; // 这里必须是return 否则无法退出while循环。在进入这些函数的二级菜单中，最后就是break了，因为外边没有while循环
        }
    }
}

void wareHousing()
{
    system("cls");

    if (admin_warehouse_list->size == 0)
    {
        printf("暂无快递到达！\n");
        printCommonInfo();
        return;
    }
    // 把admin_warehouse_list分配至五个货架
    ListNode *current = admin_warehouse_list->head;
    while (current != NULL)
    {
        Package *package = (Package *)current->data;

        // 生鲜
        if (package->special_type == 2)
        {
            // 将格式化数据写入字符串--sprintf
            // 和fprintf区分一下：f--文件，s--字符串
            if (shelf_e_list->size >= 100)
            {
                printf("货架E已满，请等待清空！\n");
                return;
            }
            sprintf(package->package_id, "E-%d-%d", shelf_e_list->size / 20, shelf_e_list->size % 20); // 借助除和模运算，实现货架编号
            listAdd(shelf_e_list, package);
        }
        // 易碎、电子产品、高价值
        else if (package->special_type == 1 || package->value == 1)
        {
            if (shelf_d_list->size >= 100)
            {
                printf("货架D已满，请等待清空！\n");
                return;
            }
            sprintf(package->package_id, "D-%d-%d", shelf_d_list->size / 20, shelf_d_list->size % 20);
            listAdd(shelf_d_list, package);
        }
        // 大件、重件
        else if (package->volume == 1 || package->weight == 1)
        {
            if (shelf_c_list->size >= 100)
            {
                printf("货架C已满，请等待清空！\n");
                return;
            }
            sprintf(package->package_id, "C-%d-%d", shelf_c_list->size / 20, shelf_c_list->size % 20);
            listAdd(shelf_c_list, package);
        }
        // 普通货架
        else
        {
            if (shelf_a_list->size <= shelf_b_list->size)
            {
                if (shelf_a_list->size >= 100)
                {
                    printf("货架A已满，请等待清空！\n");
                    return;
                }
                sprintf(package->package_id, "A-%d-%d", shelf_a_list->size / 20, shelf_a_list->size % 20);
                listAdd(shelf_a_list, package);
            }
            else
            {
                if (shelf_b_list->size >= 100)
                {
                    printf("货架B已满，请等待清空！\n");
                    return;
                }
                sprintf(package->package_id, "B-%d-%d", shelf_b_list->size / 20, shelf_b_list->size % 20);
                listAdd(shelf_b_list, package);
            }
        }
        current = current->next;
    }
    listFree(admin_warehouse_list);    // 清空库存
    admin_warehouse_list = listInit(); // 重新初始化链表，不能直接把链表给清空了。要保证在程序运行时，所有链表一直存在

    printf("入库成功！\n");
    printCommonInfo();
}

void userManagement()
{
    /*
    为了简化操作，我们暂时设定，只有用户表和快递员允许增删查改，组织链表结构
    因为这二者的状态随时变化，需要不断更新，必须依赖链表
    管理员、平台，这二者暂时只支持注册，不支持删除、修改
    后续有精力再进行扩展
    */
    system("cls");
    printf("请选择您要进行的操作：\n");
    printf("1. 添加用户\n");     // 四种类型皆可
    printf("2. 删除用户\n");     // 不支持快递平台
    printf("3. 修改用户信息\n"); // 不支持快递平台
    printf("4. 查看用户信息\n"); // 四种类型皆可
    printf("按其他任意键返回\n");

    char choice = getchar();
    clearInputBuffer();

    switch (choice)
    {
    case '1':
        addUser();
        break;
    case '2':
        deleteUser();
        break;
    case '3':
        modifyUser();
        break;
    case '4':
        viewUserInfo();
        break;
    default:
        break;
    }
}

void shelfManagement()
{
    system("cls");
    printf("请选择您要进行的操作：\n");
    printf("1. 查看货架信息\n");
    printf("2. 修改货架信息\n");
    printf("按其他任意键返回\n");

    char choice = getchar();
    clearInputBuffer();

    switch (choice)
    {
    case '1':
        viewShelfInfo();
        break;
    case '2':
        modifyShelfInfo();
        break;
    default:
        break;
    }
}

void viewFeedback()
{
    system("cls");
    FILE *fp = fopen("../files/feedback.txt", "r");
    if (fp == NULL)
    {
        printf("文件打开失败！\n");
        return;
    }

    char feedback[200];
    while (fgets(feedback, 100, fp) != NULL) // 读取一行
    {
        printf("%s\n", feedback);
        printf("请选择您的操作：\n");
        printf("1. 赔款\n");
        printf("2. 协商\n");
        printf("3. 无需处理\n");

        char choice = getchar();
        clearInputBuffer();
        // 后续暂时不再处理，假装加了个处理反馈的模块
    }
    fclose(fp);

    // 处理完毕，清空文件。借助只读操作清空。
    fopen("../files/feedback.txt", "w");
    fclose(fp);
    printf("处理完毕！\n");
    printCommonInfo();
}

void inventoryCheck()
{
    system("cls");
    // 输出现在的驿站仓库情况、五个货架情况
    printf("驿站仓库快递数目：%d\n", admin_warehouse_list->size);
    printf("货架A快递数目：%d\n", shelf_a_list->size);
    printf("货架B快递数目：%d\n", shelf_b_list->size);
    printf("货架C快递数目：%d\n", shelf_c_list->size);
    printf("货架D快递数目：%d\n", shelf_d_list->size);
    printf("货架E快递数目：%d\n", shelf_e_list->size);

    printCommonInfo();
}

void viewBusinessStatistics()
{
    system("cls");
    printf("查看业务统计模块\n");
    printCommonInfo();
}

void pushMessage()
{
    system("cls");

    // 遍历不为空的货架，在货架中读取收件人信息，然后在用户名单里寻找用户
    pushMessageToUser(shelf_a_list);
    pushMessageToUser(shelf_b_list);
    pushMessageToUser(shelf_c_list);
    pushMessageToUser(shelf_d_list);
    pushMessageToUser(shelf_e_list);

    printf("消息推送完毕！\n");
    printCommonInfo();
}

void pushMessageToUser(List *list)
{
    system("cls");
    if (list->size > 0)
    {
        ListNode *current = list->head;
        while (current != NULL)
        {
            Package *package = (Package *)current->data;
            User *user = userElementGet(users_list, package->receiver_account);
            if (user != NULL)
            {
                printf("已向用户 %s 推送消息\n", user->account);
                user->receive_status = 1;
                listAdd(users_push_list, package);
            }
            current = current->next;
        }
    }
    printf("消息推送完毕！\n");
    printCommonInfo();
}

void addUser()
{
    system("cls");
    printf("选择您要添加的用户类型\n");
    printf("1. 普通用户\n");
    printf("2. 快递员\n");
    printf("3. 管理员\n");
    printf("4. 快递平台\n");
    printf("按任意其他键返回\n");

    char choice = getchar();
    clearInputBuffer();

    switch (choice)
    {
    case '1':
        registerUser();
        break;
    case '2':
        registerCourier();
        break;
    case '3':
        registerAdmin();
        break;
    case '4':
        registerPlatform();
        break;
    default:
        break;
    }
}

void registerUser()
{
    system("cls");

    // 与main函数注册类似
    // 先写入链表，再写入文件
    printf("请输入用户名：\n");
    char account[20];
    scanf("%s", account);
    clearInputBuffer();

    printf("请输入密码：\n");
    char password[20];

    // 用户名重复检查
    ListNode *node = users_list->head;
    while (node != NULL)
    {
        User *user = (User *)node->data;
        if (strcmp(user->account, account) == 0)
        {
            printf("用户名已存在，请重新输入！\n");
            return;
        }
        node = node->next;
    }

    scanf("%s", password);
    clearInputBuffer();

    printf("请输入电话号码：\n");
    char phone_number[20];
    scanf("%s", phone_number);
    clearInputBuffer();

    int default_user_type = 0;
    int default_receive_status = 0;
    int default_send_status = 0;

    User *user = (User *)malloc(sizeof(User));
    strcpy(user->account, account);
    strcpy(user->password, password);
    strcpy(user->phone_number, phone_number);
    user->user_type = default_user_type;
    user->receive_status = default_receive_status;
    user->send_status = default_send_status;
    listAdd(users_list, user);

    printf("添加用户成功！\n");
    printCommonInfo();
}

void registerCourier()
{
    system("cls");
    printf("请输入用户名：\n");
    char account[20];
    scanf("%s", account);
    clearInputBuffer();

    printf("请输入密码：\n");
    char password[20];
    scanf("%s", password);
    clearInputBuffer();

    // 用户名重复检查
    ListNode *node = couriers_list->head;
    while (node != NULL)
    {
        Courier *courier = (Courier *)node->data;
        if (strcmp(courier->account, account) == 0)
        {
            printf("用户名已存在，请重新输入！\n");
            return;
        }
        node = node->next;
    }

    int default_duty = 0;
    int default_status = 0;

    Courier *courier = (Courier *)malloc(sizeof(Courier));
    strcpy(courier->account, account);
    strcpy(courier->password, password);
    courier->status = default_status;
    listAdd(couriers_list, courier);

    printf("添加快递员成功！\n");
    printCommonInfo();
}

void registerAdmin()
{
    system("cls");
    printf("请输入用户名：\n");
    char account[20];
    scanf("%s", account);
    clearInputBuffer();

    printf("请输入密码：\n");
    char password[20];
    scanf("%s", password);
    clearInputBuffer();

    // 用户名重复检查
    ListNode *node = admins_list->head;
    while (node != NULL)
    {
        Admin *admin = (Admin *)node->data;
        if (strcmp(admin->account, account) == 0)
        {
            printf("用户名已存在，请重新输入！\n");
            return;
        }
        node = node->next;
    }

    Admin *admin = (Admin *)malloc(sizeof(Admin));
    strcpy(admin->account, account);
    strcpy(admin->password, password);
    listAdd(admins_list, admin);

    printf("添加管理员成功！\n");
    printCommonInfo();
}

// 无链表，直接写入文件
void registerPlatform()
{
    system("cls");
    printf("请输入用户名：\n");
    char account[20];
    scanf("%s", account);
    clearInputBuffer();

    printf("请输入密码：\n");
    char password[20];
    scanf("%s", password);
    clearInputBuffer();

    // 用户名重复检查
    FILE *fp = fopen("../files/platforms_info.txt", "r");
    if (fp == NULL)
    {
        printf("文件打开失败！\n");
        return;
    }
    char temp_account[20];
    while (fscanf(fp, "%s\n", temp_account) != EOF) // 只格式化读用户名
    {
        if (strcmp(temp_account, account) == 0)
        {
            printf("用户名已存在，请重新输入！\n");
            return;
        }
    }
    fclose(fp);

    fp = fopen("../files/platforms_info.txt", "a");
    if (fp == NULL)
    {
        printf("文件打开失败！\n");
        return;
    }
    fprintf(fp, "%s %s\n", account, password);
    fclose(fp);

    printf("添加平台成功！\n");
    printCommonInfo();
}

void deleteUser()
{
    system("cls");
    printf("请选择要删除的用户类型：\n");
    printf("1. 用户\n");
    printf("2. 快递员\n");
    printf("3. 管理员\n");
    printf("按其他任意键返回\n");

    char choice = getchar();
    clearInputBuffer();
    char account[20];

    switch (choice)
    {
    case '1':
        printf("请输入要删除的用户账号：\n");
        scanf("%s", account);
        clearInputBuffer();

        User *user = userElementGet(users_list, account);
        if (user == NULL)
        {
            printf("用户不存在！\n");
            printCommonInfo();
            return;
        }

        listRemove(users_list, user);
        printf("删除成功！\n");
        printCommonInfo();
        break;
    case '2':
        printf("请输入要删除的快递员账号：\n");
        scanf("%s", account);
        clearInputBuffer();

        Courier *courier = courierElementGet(couriers_list, account);
        if (courier == NULL)
        {
            printf("快递员不存在！\n");
            printCommonInfo();
            return;
        }

        listRemove(couriers_list, courier);
        printf("删除成功！\n");
        printCommonInfo();
        break;
    case '3':
        printf("请输入要删除的管理员账号：\n");
        scanf("%s", account);
        clearInputBuffer();

        Admin *admin = adminElementGet(admins_list, account);
        if (admin == NULL)
        {
            printf("管理员不存在！\n");
            printCommonInfo();
            return;
        }

        listRemove(admins_list, admin);
        printf("删除成功！\n");
        printCommonInfo();
        break;
    default:
        break;
    }
}

void modifyUser()
{
    system("cls");
    printf("请选择要修改的用户类型：\n");
    printf("1. 用户\n");
    printf("2. 快递员\n");
    printf("3. 管理员\n");
    printf("按其他任意键返回\n");

    char choice = getchar();
    clearInputBuffer();

    char account[20];
    char choice2;
    switch (choice)
    {
    case '1':
        printf("请输入要修改的用户账号：\n");
        scanf("%s", account);
        clearInputBuffer();

        User *user = userElementGet(users_list, account);
        if (user == NULL)
        {
            printf("用户不存在！\n");
            printCommonInfo();
            return;
        }
        printf("请选择要修改的信息：\n");
        printf("1. 用户名\n");
        printf("2. 密码\n");
        printf("3. 电话号码\n");
        printf("4. 用户类型\n");
        printf("按其他任意键返回\n");

        choice2 = getchar();
        clearInputBuffer();

        switch (choice2)
        {
        case '1':
            printf("请输入新的用户名：\n");
            char new_account[20];
            scanf("%s", new_account);
            clearInputBuffer();
            puts("");

            strcpy(user->account, new_account);
            printf("修改成功！\n");
            printCommonInfo();
            break;
        case '2':
            printf("请输入新的密码：\n");
            char new_password[20];
            scanf("%s", new_password);
            clearInputBuffer();
            puts("");

            strcpy(user->password, new_password);
            printf("修改成功！\n");
            printCommonInfo();
            break;
        case '3':
            printf("请输入新的电话号码：\n");
            char new_phone_number[20];
            scanf("%s", new_phone_number);
            clearInputBuffer();
            puts("");

            strcpy(user->phone_number, new_phone_number);
            printf("修改成功！\n");
            printCommonInfo();
            break;
        case '4':
            printf("请输入新的用户类型：\n");
            printf("1. 普通用户\n");
            printf("2. 会员用户\n");
            printf("3. 企业用户\n");
            printf("4. 代理商用户\n");
            printf("5. 合作商家用户\n");

            char new_user_type = getchar();
            clearInputBuffer();
            puts("");
            // 这里说一下，虽然没输入整形数据
            // 但整形数据不是不需要清空缓存区，而是，如果保证输入的都是整形的话，那不会读空白字符，因此不需要清空缓存区；但这个程序里显然是整形与字符混和输入


            // 输入错误逻辑
            if (new_user_type != '1' && new_user_type != '2' && new_user_type != '3' && new_user_type != '4' && new_user_type != '5')
            {
                printf("输入错误！\n");
                printCommonInfo();
                return;
            }

            user->user_type = new_user_type - '0' - 1;
            printf("修改成功！\n");
            printCommonInfo();
            break;
        default:
            break;
        }
        break;
    case '2':
        printf("请输入要修改的快递员账号：\n");
        scanf("%s", account);
        clearInputBuffer();

        Courier *courier = courierElementGet(couriers_list, account);
        if (courier == NULL)
        {
            printf("快递员不存在！\n");
            printCommonInfo();
            return;
        }
        printf("请选择要修改的信息：\n");
        printf("1. 用户名\n");
        printf("2. 密码\n");
        printf("按其他任意键返回\n");

        choice2 = getchar();
        clearInputBuffer();
        puts("");

        switch (choice2)
        {
        case '1':
            printf("请输入新的用户名：\n");
            char new_account[20];
            scanf("%s", new_account);
            clearInputBuffer();
            puts("");

            strcpy(courier->account, new_account);
            break;
        case '2':
            printf("请输入新的密码：\n");
            char new_password[20];
            scanf("%s", new_password);
            clearInputBuffer();
            puts("");

            strcpy(courier->password, new_password);
            break;
        default:
            break;
        }
        printf("修改成功！\n");
        printCommonInfo();
        break;
    case '3':
        printf("请输入要修改的管理员账号：\n");
        scanf("%s", account);
        clearInputBuffer();

        Admin *admin = adminElementGet(admins_list, account);
        if (admin == NULL)
        {
            printf("管理员不存在！\n");
            printCommonInfo();
            return;
        }
        printf("请选择要修改的信息：\n");
        printf("1. 用户名\n");
        printf("2. 密码\n");
        printf("按其他任意键返回\n");

        choice2 = getchar();
        clearInputBuffer();
        puts("");

        switch (choice2)
        {
        case '1':
            printf("请输入新的用户名：\n");
            char new_account[20];
            scanf("%s", new_account);
            clearInputBuffer();
            puts("");

            strcpy(admin->account, new_account);
            break;
        case '2':
            printf("请输入新的密码：\n");
            char new_password[20];
            scanf("%s", new_password);
            clearInputBuffer();
            puts("");

            strcpy(admin->password, new_password);
            break;
        default:
            break;
        }
        printf("修改成功！\n");
        printCommonInfo();
        break;
    default:
        break;
    }
}

void viewUserInfo()
{
    system("cls");
    printf("请选择要查看的用户类型：\n");
    printf("1. 用户\n");
    printf("2. 快递员\n");
    printf("3. 管理员\n");
    printf("按其他任意键返回\n");

    char choice = getchar();
    clearInputBuffer();
    puts("");

    char account[20];
    switch (choice)
    {
    case '1':
        printf("请输入要查看的用户账号：\n");
        scanf("%s", account);
        clearInputBuffer();
        puts("");

        User *user = userElementGet(users_list, account);
        if (user == NULL)
        {
            printf("用户不存在！\n");
            printCommonInfo();
            return;
        }
        printf("用户名：%s\n", user->account);
        printf("密码：%s\n", user->password);
        printf("电话号码：%s\n", user->phone_number);

        char user_type[50];
        switch (user->user_type)
        {
        case 0:
            strcpy(user_type, "普通用户");
            break;
        case 1:
            strcpy(user_type, "会员用户");
            break;
        case 2:
            strcpy(user_type, "企业用户");
            break;
        case 3:
            strcpy(user_type, "代理商用户");
            break;
        case 4:
            strcpy(user_type, "合作商家用户");
            break;
        default:
            break;
        }
        printf("用户类型：%s\n", user_type);

        char receive_status[50];
        switch (user->receive_status)
        {
        case 0:
            strcpy(receive_status, "无需取件");
            break;
        case 1:
            strcpy(receive_status, "待取件");
            break;
        default:
            break;
        }
        printf("接收快递状态：%s\n", receive_status);

        char send_status[30];
        switch (user->send_status)
        {
        case 0:
            strcpy(send_status, "未寄件");
            break;
        case 1:
            strcpy(send_status, "未发出");
            break;
        case2:
            strcpy(send_status, "已发出");
            break;
        default:
            break;
        }
        printf("寄件状态：%s\n", send_status);

        break;
    case '2':
        printf("请输入要查看的快递员账号：\n");
        scanf("%s", account);
        clearInputBuffer();

        Courier *courier = courierElementGet(couriers_list, account);
        if (courier == NULL)
        {
            printf("快递员不存在！\n");
            printCommonInfo();
            return;
        }
        printf("用户名：%s\n", courier->account);
        printf("密码：%s\n", courier->password);

        char status[60];
        switch (courier->status)
        {
        case 0:
            strcpy(status, "空闲");
            break;
        case 1:
            strcpy(status, "忙碌，正从快递平台取件");
            break;
        case 2:
            strcpy(status, "忙碌，正从驿站取件");
            break;
        default:
            break;
        }
        printf("状态：%s\n", status);
        break;
    case '3':
        printf("请输入要查看的管理员账号：\n");
        scanf("%s", account);
        clearInputBuffer();

        Admin *admin = adminElementGet(admins_list, account);
        if (admin == NULL)
        {
            printf("管理员不存在！\n");
            printCommonInfo();
            return;
        }
        printf("用户名：%s\n", admin->account);
        printf("密码：%s\n", admin->password);
        break;
    default:
        break;
    }

    printCommonInfo();
}

void viewShelfInfo()
{
    system("cls");
    printf("请选择要查看的货架：\n");
    printf("1. 货架A\n");
    printf("2. 货架B\n");
    printf("3. 货架C\n");
    printf("4. 货架D\n");
    printf("5. 货架E\n");
    printf("按其他任意键返回\n");

    char choice = getchar();
    clearInputBuffer();
    puts("");

    switch (choice)
    {
    case '1':
        viewShelf(shelf_a_list);
        break;
    case '2':
        viewShelf(shelf_b_list);
        break;
    case '3':
        viewShelf(shelf_c_list);
        break;
    case '4':
        viewShelf(shelf_d_list);
        break;
    case '5':
        viewShelf(shelf_e_list);
        break;
    default:
        break;
    }
}

void viewShelf(List *shelf_list)
{
    system("cls");
    if (shelf_list->size == 0)
    {
        printf("货架为空！\n");
        printCommonInfo();
        return;
    }
    ListNode *current = shelf_list->head;
    while (current != NULL)
    {
        Package *package = (Package *)current->data;
        printf("货架编号：%s\n", package->package_id);
        printf("收件人账号：%s\n", package->receiver_account);

        char express[20];
        switch (package->isExpress)
        {
        case 0:
            strcpy(express, "未加急");
            break;
        case 1:
            strcpy(express, "已加急");
            break;
        }
        printf("加急状态：%s\n", express);

        char volume[20];
        switch (package->volume)
        {
        case 0:
            strcpy(volume, "小");
            break;
        case 1:
            strcpy(volume, "大");
            break;
        default:
            break;
        }
        printf("体积：%s\n", volume);

        char weight[20];
        switch (package->weight)
        {
        case 0:
            strcpy(weight, "轻");
            break;
        case 1:
            strcpy(weight, "重");
            break;
        default:
            break;
        }
        printf("重量：%s\n", weight);

        char special_type[50];
        switch (package->special_type)
        {
        case 0:
            strcpy(special_type, "普通");
            break;
        case 1:
            strcpy(special_type, "易碎品、电子产品");
            break;
        case 2:
            strcpy(special_type, "生鲜");
            break;
        default:
            break;
        }
        printf("特殊类型：%s\n", special_type);

        char value[20];
        switch (package->value)
        {
        case 0:
            strcpy(value, "低价值");
            break;
        case 1:
            strcpy(value, "高价值");
            break;
        default:
            break;
        }
        printf("价值：%s\n", value);

        current = current->next;
    }
    printCommonInfo();
}

void modifyShelfInfo()
{
    system("cls");
    printf("请选择要修改的货架：\n");
    printf("1. 货架A\n");
    printf("2. 货架B\n");
    printf("3. 货架C\n");
    printf("4. 货架D\n");
    printf("5. 货架E\n");
    printf("按其他任意键返回\n");

    char choice = getchar();
    clearInputBuffer();
    puts("");

    switch (choice)
    {
    case '1':
        modifyShelf(shelf_a_list);
        break;
    case '2':
        modifyShelf(shelf_b_list);
        break;
    case '3':
        modifyShelf(shelf_c_list);
        break;
    case '4':
        modifyShelf(shelf_d_list);
        break;
    case '5':
        modifyShelf(shelf_e_list);
        break;
    default:
        break;
    }
    printCommonInfo();
}

void modifyShelf(List *shelf_list)
{
    system("cls");
    printf("请输入要修改的货架号：\n");
    char package_id[20];
    scanf("%s", package_id);
    clearInputBuffer();

    ListNode *current = shelf_list->head;
    while (current != NULL)
    {
        Package *package = (Package *)current->data;
        if (strcmp(package_id, package->package_id) == 0)
        {
            printf("请选择要修改的信息：\n");
            printf("1. 收件人账号\n");
            printf("2. 加急状态\n");
            printf("3. 体积\n");
            printf("4. 重量\n");
            printf("5. 特殊类型\n");
            printf("6. 价值\n");
            printf("按其他任意键返回\n");

            char choice = getchar();
            clearInputBuffer();
            gets("");

            switch (choice)
            {
            case '1':
                printf("请输入新的收件人账号：\n");
                char new_receiver_account[20];
                scanf("%s", new_receiver_account);
                clearInputBuffer();
                gets("");

                // 判断新的收件人账号是否存在
                // 原则上，必须该用户存在于用户列表中
                User *user = userElementGet(users_list, new_receiver_account);
                if (user == NULL)
                {
                    printf("用户不存在！\n");
                    printCommonInfo();
                    return;
                }

                strcpy(package->receiver_account, new_receiver_account);
                break;
            case '2':
                printf("请输入新的加急状态：\n");
                printf("1. 否\n");
                printf("2. 是\n");

                char new_express_type = getchar();
                clearInputBuffer();
                puts("");

                // 输入错误判断
                if (new_express_type != '1' && new_express_type != '2')
                {
                    printf("输入错误！\n");
                    printCommonInfo();
                    return;
                }

                package->isExpress = new_express_type - '0' - 1; // 注意减1操作，为了匹配输入
                break;
            case '3':
                printf("请输入新的体积：\n");
                printf("1. 小\n");
                printf("2. 大\n");

                char new_volume = getchar();
                clearInputBuffer();
                puts("");

                if (new_volume != '1' && new_volume != '2')
                {
                    printf("输入错误！\n");
                    printCommonInfo();
                    return;
                }

                package->volume = new_volume - '0' - 1;
                break;
            case '4':
                printf("请输入新的重量：\n");
                printf("1. 轻\n");
                printf("2. 重\n");

                char new_weight = getchar();
                clearInputBuffer();
                puts("");

                if (new_weight != '1' && new_weight != '2')
                {
                    printf("输入错误！\n");
                    printCommonInfo();
                    return;
                }

                package->weight = new_weight - '0' - 1;
                break;
            case '5':
                printf("请输入新的快递类型：\n");
                printf("1. 普通\n");
                printf("2. 易碎品、电子产品\n");
                printf("3. 生鲜\n");

                char new_special_type = getchar();
                clearInputBuffer();
                puts("");

                if (new_special_type != '1' && new_special_type != '2' && new_special_type != '3')
                {
                    printf("输入错误！\n");
                    printCommonInfo();
                    return;
                }

                package->special_type = new_special_type - '0' - 1;
                break;
            case '6':
                printf("请输入新的价值：\n");
                printf("1. 低价值\n");
                printf("2. 高价值\n");

                char new_value = getchar();
                clearInputBuffer();
                puts("");

                if (new_value != '1' && new_value != '2')
                {
                    printf("输入错误！\n");
                    printCommonInfo();
                    return;
                }

                package->value = new_value - '0' - 1;
                break;
            default:
                break;
            }
            printf("修改成功！\n");
            printCommonInfo();
            return;
        }
        current = current->next;
    }
}

// 仿照平台函数来写
void addressUserSend()
{
    system("cls");
    if (users_send_list->size == 0) // 没有待发货快递
    {
        printf("暂无待发货快递！\n");
        printCommonInfo();
        return;
    }

    // 由于链表已“排序”，所以直接顺着取
    ListNode *current = users_send_list->head;
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
        if (current == NULL)
        {
            printf("已全部发货！\n");
            printCommonInfo();
            return;
        }

        Courier *courier = (Courier *)courier_current->data;
        Package *package = (Package *)current->data;

        // 如果快递员现在正在派送从平台到驿站的快递，就跳过
        if (courier->status == 1)
        {
            courier_current = courier_current->next;
            continue; // 为了防止下一个快递员也是1，这里要continue
        }

        strcpy(package->courier_account, courier->account); // 包裹信息上标注快递员信息

        listAdd(couriers_push_list, package); // 加入快递员的推送链表
        printf("已向快递员 %s 推送消息\n", courier->account);
        courier->status = 2; // 标记快递员状态为正在由 驿站->平台 派送

        current = current->next; // 转向下一件快递
        
        listRemove(users_send_list, package); // 从用户待发货链表中删除

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