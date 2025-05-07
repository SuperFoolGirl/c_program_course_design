#include "../include/admin.h"

Admin *the_admin = NULL;

void adminShowMenu() {
    system("cls");

    // 遍历所有货架，寻找滞留包裹
    findLeavePackage();

    // 弹窗
    adminPop();

    while (1) {
        system("cls");
        printf("欢迎登录！\n\n");
        printf("请您选择操作：\n");
        printf("1. 入库\n");
        printf("2. 用户管理\n");
        printf("3. 货架管理\n");
        printf("4. 查看反馈\n");
        printf("5. 库存盘点\n");
        printf("6. 查看业务统计\n");
        printf("7. 处理用户寄件\n");
        printf("8. 查看快递单修改日志\n");
        printf("9. 修改用户寄件信息\n");
        printf("0. 查看驿站账户\n");
        printf("a. 处理拒收包裹\n");
        printf("b. 查看用户寄件信息\n\n");
        printf("按其他任意键退出...\n");

        char choice = _getch();

        switch (choice) {
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
            addressUserSend();
            break;
        case '8':
            viewPackageLog();
            break;
        case '9':
            modifyUserSend();
            break;
        case '0':
            viewAccount();
            break;
        case 'a':
            handleAbnormalPackage();
            break;
        case 'b':
            viewUserSendInfo();
            break;
        default:
            return;   // 这里必须是return
                      // 否则无法退出while循环。在进入这些函数的二级菜单中，最后就是break了，因为外边没有while循环
        }
    }
}

void adminPop() {
    // 弹窗提醒模块
    if (admin_warehouse_list->size > 0) {
        printf("快递已到达，请及时入库！\n");

        // 为了防止弹窗一闪而过 需要加一个确认判断
        printCommonInfo();

        system("cls");   // 为下一个弹窗清屏
    }

    // 寄件处理提醒
    if (users_send_list->size > 0) {
        printf("有新的寄件请求，请及时处理！\n");
        printCommonInfo();
        system("cls");   // 为下一个弹窗清屏
    }

    // 架子容量警告，达到80%时提醒
    int flag = 0;
    if (shelf_a_list->size >= SIZE * 4) {
        printf("货架A已接近满载，请及时清理！\n");
        flag = 1;
    }
    if (shelf_b_list->size >= SIZE * 4) {
        printf("货架B已接近满载，请及时清理！\n");
        flag = 1;
    }
    if (shelf_c_list->size >= SIZE * 4) {
        printf("货架C已接近满载，请及时清理！\n");
        flag = 1;
    }
    if (shelf_d_list->size >= SIZE * 4) {
        printf("货架D已接近满载，请及时清理！\n");
        flag = 1;
    }
    if (shelf_e_list->size >= SIZE * 4) {
        printf("货架E已接近满载，请及时清理！\n");
        flag = 1;
    }
    if (flag == 1) {
        printCommonInfo();
    }
    system("cls");   // 为下一个弹窗清屏

    // 反馈提醒
    if (feedback_list->size > 0) {
        printf("您有新的反馈，请及时查看！\n");
        printCommonInfo();
        system("cls");   // 为下一个弹窗清屏
    }

    // 拒收提醒
    if (refuse_list->size > 0) {
        printf("您有新的拒收包裹，请及时处理！\n");
        printCommonInfo();
        system("cls");   // 为下一个弹窗清屏
    }
}

void wareHousing() {
    system("cls");

    // 随机数生成取件码
    // 用当前时间作为随机数种子，确保每次运行程序生成的随机数序列不同
    srand((unsigned int)time(NULL));

    // 记录当前时间，写入包裹的到达时间
    time_t current_time = time(NULL);

    if (admin_warehouse_list->size == 0) {
        printf("暂无快递到达！\n");
        printCommonInfo();
        return;
    }
    // 把admin_warehouse_list分配至五个货架
    // 分配后，自动完成对用户的消息推送
    ListNode *current = admin_warehouse_list->head;
    while (current != NULL) {
        Package *package = (Package *)current->data;
        package->time = current_time;   // 记录包裹到达时间

        // 生鲜
        if (package->special_type == 2) {
            // 将格式化数据写入字符串--sprintf
            // 和fprintf区分一下：f--文件，s--字符串
            if (shelf_e_list->size >= 100) {
                printf("货架E已满，请等待清空！\n");
                return;
            }

            sprintf(package->shelf_id, "E-%d-%d", shelf_e_list->size / SIZE,
                    shelf_e_list->size % SIZE);           // 借助除和模运算，实现货架编号
            package->pick_up_code = rand() % MAX + MIN;   // 生成随机取件码

            listAdd(shelf_e_list, package);
        }
        // 易碎、电子产品、高价值
        else if (package->special_type == 1 || package->value >= 200) {
            if (shelf_d_list->size >= 100) {
                printf("货架D已满，请等待清空！\n");
                return;
            }

            sprintf(package->shelf_id, "D-%d-%d", shelf_d_list->size / SIZE, shelf_d_list->size % SIZE);
            package->pick_up_code = rand() % MAX + MIN;   // 生成随机取件码

            listAdd(shelf_d_list, package);
        }
        // 大件、重件
        else if (package->volume >= 100 || package->weight >= 10) {
            if (shelf_c_list->size >= 100) {
                printf("货架C已满，请等待清空！\n");
                return;
            }

            sprintf(package->shelf_id, "C-%d-%d", shelf_c_list->size / SIZE, shelf_c_list->size % SIZE);
            package->pick_up_code = rand() % MAX + MIN;   // 生成随机取件码

            listAdd(shelf_c_list, package);
        }
        // 普通货架
        else {
            if (shelf_a_list->size <= shelf_b_list->size) {
                if (shelf_a_list->size >= 100) {
                    printf("货架A已满，请等待清空！\n");
                    return;
                }
                sprintf(package->shelf_id, "A-%d-%d", shelf_a_list->size / SIZE, shelf_a_list->size % SIZE);
                package->pick_up_code = rand() % MAX + MIN;   // 生成随机取件码

                listAdd(shelf_a_list, package);
            } else {
                if (shelf_b_list->size >= 100) {
                    printf("货架B已满，请等待清空！\n");
                    return;
                }
                sprintf(package->shelf_id, "B-%d-%d", shelf_b_list->size / SIZE, shelf_b_list->size % SIZE);
                package->pick_up_code = rand() % MAX + MIN;   // 生成随机取件码

                listAdd(shelf_b_list, package);
            }
        }

        // 消息推送
        // 根据收件人账号，找到用户，然后推送消息
        // 这里要负责处理未注册用户的情况
        pushMessageToUser(package);

        current = current->next;
        listRemove(admin_warehouse_list, package);
    }

    printf("入库成功并全部向用户推送取件消息！\n");
    printCommonInfo();
}

void userManagement() {
    while (1) {
        system("cls");
        printf("请选择您要进行的操作：\n\n");
        printf("1. 添加用户\n");
        printf("2. 删除用户\n");
        printf("3. 修改用户信息\n");
        printf("4. 查询用户信息\n");
        printf("5. 列出全部用户信息\n\n");
        printf("按其他任意键返回...\n");

        char choice = _getch();

        system("cls");
        switch (choice) {
        case '1':
            addUser();
            break;
        case '2':
            deleteUser();
            break;
        case '3':
            modifyFunc();
            break;
        case '4':
            viewUserInfo();
            break;
        case '5':
            listAllInfo();
            break;
        default:
            return;
        }
    }
}

void shelfManagement() {
    // 可能需要多次查看，给一个死循环
    while (1) {
        system("cls");
        printf("请选择您要进行的操作：\n\n");
        printf("1. 查看货架信息\n");
        printf("2. 修改货架信息\n\n");
        printf("按其他任意键返回...\n");

        char choice = _getch();

        switch (choice) {
        case '1':
            viewShelfInfo();
            break;
        case '2':
            modifyShelfInfo();
            break;
        default:
            return;
        }
    }
}

void viewFeedback() {
    system("cls");
    // 遍历反馈链表，输出所有反馈信息
    if (feedback_list->size == 0) {
        printf("暂无反馈信息！\n");
        printCommonInfo();
        return;
    }
    ListNode *current = feedback_list->head;
    while (current != NULL) {
        system("cls");
        printf("如若需要强制退出，请输入“exit”\n\n");

        Feedback *feedback = (Feedback *)current->data;
        printf("反馈人：%s\n", feedback->account);
        printf("%s\n", feedback->content);
        printf("--------------------------------------------------------\n\n");
        printf("请对该反馈进行回复：\n");

        User *user = userElementGet(users_list, feedback->account);
        char message[200];
        scanf("%s", message);
        clearInputBuffer();
        puts("");

        if (checkExit(message)) {
            return;
        }

        strcpy(user->message, message);
        user->message_status = 1;

        printf("请选择附加操作：\n\n");
        printf("1. 赔偿\n");
        printf("2. 协商\n");
        printf("3. 取消\n\n");
        printf("按其他任意键退出反馈处理功能...\n");

        char choice = _getch();
        puts("");

        switch (choice) {
        case '1':
            printf("请输入赔偿金额：\n");
            int compensation;
            scanf("%d", &compensation);
            clearInputBuffer();
            puts("");
            money -= compensation;
            printf("赔偿成功！\n");
            printCommonInfo();
            break;
        case '2':
            printf("请自行与用户协商！\n");
            printCommonInfo();
            break;
        case '3':
            break;
        default:
            return;
        }

        // 删除反馈信息
        ListNode *del = current;
        current = current->next;
        listRemove(feedback_list, del->data);
    }
    printf("已处理全部反馈！\n");
    printCommonInfo();
}

void inventoryCheck() {
    system("cls");
    // 输出现在的驿站仓库情况、五个货架情况
    printf("驿站仓库快递数目：%d\n\n",
           shelf_a_list->size + shelf_b_list->size + shelf_c_list->size + shelf_d_list->size + shelf_e_list->size);
    printf("货架A快递数目：%d\n", shelf_a_list->size);
    printf("货架B快递数目：%d\n", shelf_b_list->size);
    printf("货架C快递数目：%d\n", shelf_c_list->size);
    printf("货架D快递数目：%d\n", shelf_d_list->size);
    printf("货架E快递数目：%d\n\n", shelf_e_list->size);

    if (shelf_a_list->size >= SIZE * 4) {
        printf("货架A已接近满载，请及时清理！\n");
    }
    if (shelf_b_list->size >= SIZE * 4) {
        printf("货架B已接近满载，请及时清理！\n");
    }
    if (shelf_c_list->size >= SIZE * 4) {
        printf("货架C已接近满载，请及时清理！\n");
    }
    if (shelf_d_list->size >= SIZE * 4) {
        printf("货架D已接近满载，请及时清理！\n");
    }
    if (shelf_e_list->size >= SIZE * 4) {
        printf("货架E已接近满载，请及时清理！\n");
    }

    printCommonInfo();
}

void viewBusinessStatistics() {
    system("cls");
    // 显示所有用户的取件、收件情况
    // 分别打开pickup_records.txt和send_records.txt，全部打印
    FILE *fp = fopen("../res/pickup_records.txt", "r");
    if (fp == NULL) {
        printf("文件打开失败！\n");
        return;
    }
    // 读取文件内容并打印，并进行计数
    char record[200];
    int cnt = 0;
    printf("取件记录：\n\n");
    while (fgets(record, 100, fp) != NULL) {
        cnt++;
        printf("%s", record);
    }
    printf("\n取件总数：%d", cnt);
    fclose(fp);

    printf("\n\n=============================================================\n\n");

    fp = fopen("../res/send_records.txt", "r");
    if (fp == NULL) {
        printf("文件打开失败！\n");
        return;
    }
    cnt = 0;
    printf("寄件记录：\n\n");
    while (fgets(record, 100, fp) != NULL) {
        cnt++;
        printf("%s", record);
    }
    printf("\n寄件总数：%d\n", cnt);
    fclose(fp);

    printCommonInfo();
}

void pushMessageToUser(Package *package)   // 不能用const修饰，listAdd函数会报警
{
    User *user = userElementGet(users_list, package->receiver_account);
    if (user == NULL) {
        // 处理该用户未注册或注销的情况
        printf("包裹 %s 的收件人 %s 不存在！\n", package->package_id, package->receiver_account);
        printf("系统将自动为用户注册账号：\n");
        printf("默认密码为123456\n");
        printf("请及时联系用户完善账号信息!\n");

        user = (User *)malloc(sizeof(User));
        strcpy(user->account, package->receiver_account);
        strcpy(user->password, "123456");
        strcpy(user->friend[0], "0");
        strcpy(user->friend[1], "0");
        strcpy(user->friend[2], "0");
        user->time = 0;
        user->try_times = 0;
        user->message_status = 0;
        strcpy(user->message, "0");
        strcpy(user->phone_number, "0");
        user->user_type = 0;
        user->receive_status = 0;
        user->send_status = 0;
        user->delivery_leave = 0;
        listAdd(users_list, user);

        printCommonInfo();
    }
    user->receive_status = 1;            // 修改用户状态
    listAdd(users_push_list, package);   // 推送信息加入推送链表
}

void addUser() {
    while (1) {
        system("cls");
        printf("选择您要添加的用户类型\n\n");
        printf("1. 普通用户\n");
        printf("2. 快递员\n");
        printf("3. 管理员\n");
        printf("4. 快递平台\n\n");
        printf("按任意其他键返回...\n");

        char choice = _getch();

        switch (choice) {
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
            return;
        }
    }
}

void registerUser() {
    system("cls");
    printf("如若需要强制退出，请输入“exit”\n\n");
    // 与main函数注册类似
    // 先写入链表，再写入文件
rewrite_account:
    printf("请输入用户名：\n");
    char account[200];
    scanf("%s", account);
    clearInputBuffer();
    puts("");

    if (checkExit(account)) {
        return;
    }

    if (checkInputLimit(account) == 0) {
        goto rewrite_account;
    }

    // 用户名重复检查
    ListNode *node = users_list->head;
    while (node != NULL) {
        User *user = (User *)node->data;
        if (strcmp(user->account, account) == 0) {
            printf("用户名已存在，请重新输入！\n");
            printCommonInfo();
            goto rewrite_account;
        }
        node = node->next;
    }

rewrite_password:
    printf("请输入密码：\n");
    char password[200];
    getPassword(password);   // 使用getPassword函数获取密码，隐藏输入
    puts("");

    if (checkExit(password)) {
        return;
    }

    if (checkInputLimit(password) == 0) {
        goto rewrite_password;
    }

    printf("请再次确认密码：\n");
    char password_confirm[200];
    getPassword(password_confirm);   // 使用getPassword函数获取密码，隐藏输入
    puts("");

    if (checkExit(password_confirm)) {
        return;
    }

    if (strcmp(password, password_confirm) != 0) {
        printf("两次密码输入不一致，请重新输入！\n");
        printCommonInfo();
        goto rewrite_password;
    }

rewrite_phone:
    printf("请输入电话号码：\n");
    char phone_number[200];
    scanf("%s", phone_number);
    clearInputBuffer();
    puts("");

    if (checkExit(phone_number)) {
        return;
    }

    if (checkInputLimit(phone_number) == 0) {
        goto rewrite_phone;
    }

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
    strcpy(user->friend[0], "0");
    strcpy(user->friend[1], "0");
    strcpy(user->friend[2], "0");
    user->delivery_leave = 0;
    user->time = 0;
    user->try_times = 0;
    user->message_status = 0;
    strcpy(user->message, "0");
    listAdd(users_list, user);

    printf("添加用户成功！\n");
    printCommonInfo();
}

void registerCourier() {
    system("cls");
    printf("如若需要强制退出，请输入“exit”\n\n");
rewrite_account:
    printf("请输入用户名：\n");
    char account[200];
    scanf("%s", account);
    clearInputBuffer();
    puts("");

    if (checkExit(account)) {
        return;
    }

    if (checkInputLimit(account) == 0) {
        goto rewrite_account;
    }

    // 用户名重复检查
    ListNode *node = couriers_list->head;
    while (node != NULL) {
        Courier *courier = (Courier *)node->data;
        if (strcmp(courier->account, account) == 0) {
            printf("用户名已存在，请重新输入！\n");
            printCommonInfo();
            goto rewrite_account;
        }
        node = node->next;
    }

rewrite_password:
    printf("请输入密码：\n");
    char password[200];
    getPassword(password);   // 使用getPassword函数获取密码，隐藏输入
    puts("");

    if (checkExit(password)) {
        return;
    }

    if (checkInputLimit(password) == 0) {
        goto rewrite_password;
    }

    printf("请再次确认密码：\n");
    char password_confirm[200];
    getPassword(password_confirm);   // 使用getPassword函数获取密码，隐藏输入
    puts("");

    if (checkExit(password_confirm)) {
        return;
    }

    if (strcmp(password, password_confirm) != 0) {
        printf("两次密码输入不一致，请重新输入！\n");
        printCommonInfo();
        goto rewrite_password;
    }

    Courier *courier = (Courier *)malloc(sizeof(Courier));
    strcpy(courier->account, account);
    strcpy(courier->password, password);
    courier->status = 0;
    courier->time = 0;
    courier->try_times = 0;
    listAdd(couriers_list, courier);

    printf("添加快递员成功！\n");
    printCommonInfo();
}

void registerAdmin() {
    system("cls");
    printf("如若需要强制退出，请输入“exit”\n\n");
rewrite_account:
    printf("请输入用户名：\n");
    char account[200];
    scanf("%s", account);
    clearInputBuffer();
    puts("");

    if (checkExit(account)) {
        return;
    }

    if (checkInputLimit(account) == 0) {
        goto rewrite_account;
    }

    // 用户名重复检查
    ListNode *node = admins_list->head;
    while (node != NULL) {
        Admin *admin = (Admin *)node->data;
        if (strcmp(admin->account, account) == 0) {
            printf("用户名已存在，请重新输入！\n");
            printCommonInfo();
            goto rewrite_account;
        }
        node = node->next;
    }

rewrite_password:
    printf("请输入密码：\n");
    char password[200];
    getPassword(password);   // 使用getPassword函数获取密码，隐藏输入
    puts("");

    if (checkExit(password)) {
        return;
    }

    if (checkInputLimit(password) == 0) {
        goto rewrite_password;
    }

    printf("请再次确认密码：\n");
    char password_confirm[200];
    getPassword(password_confirm);   // 使用getPassword函数获取密码，隐藏输入
    puts("");

    if (checkExit(password_confirm)) {
        return;
    }

    if (strcmp(password, password_confirm) != 0) {
        printf("两次密码输入不一致，请重新输入！\n");
        printCommonInfo();
        goto rewrite_password;
    }

    Admin *admin = (Admin *)malloc(sizeof(Admin));
    strcpy(admin->account, account);
    strcpy(admin->password, password);
    admin->time = 0;
    admin->try_times = 0;
    listAdd(admins_list, admin);

    printf("添加管理员成功！\n");
    printCommonInfo();
}

void registerPlatform() {
    system("cls");
    printf("如若需要强制退出，请输入“exit”\n\n");
rewrite_account:
    printf("请输入用户名：\n");
    char account[200];
    scanf("%s", account);
    clearInputBuffer();
    puts("");

    if (checkExit(account)) {
        return;
    }

    if (checkInputLimit(account) == 0) {
        goto rewrite_account;
    }

    // 用户名重复检查
    ListNode *node = platforms_list->head;
    while (node != NULL) {
        Platform *platform = (Platform *)node->data;
        if (strcmp(platform->account, account) == 0) {
            printf("用户名已存在，请重新输入！\n");
            printCommonInfo();
            goto rewrite_account;
        }
        node = node->next;
    }

rewrite_password:
    printf("请输入密码：\n");
    char password[200];
    getPassword(password);   // 使用getPassword函数获取密码，隐藏输入
    puts("");

    if (checkExit(password)) {
        return;
    }

    if (checkInputLimit(password) == 0) {
        goto rewrite_password;
    }

    printf("请再次确认密码：\n");
    char password_confirm[200];
    getPassword(password_confirm);   // 使用getPassword函数获取密码，隐藏输入
    puts("");

    if (checkExit(password_confirm)) {
        return;
    }

    if (strcmp(password, password_confirm) != 0) {
        printf("两次密码输入不一致，请重新输入！\n");
        printCommonInfo();
        goto rewrite_password;
    }

    Platform *platform = (Platform *)malloc(sizeof(Platform));
    strcpy(platform->account, account);
    strcpy(platform->password, password);
    platform->time = 0;
    platform->try_times = 0;
    listAdd(platforms_list, platform);

    printf("添加平台成功！\n");
    printCommonInfo();
}

void deleteUser() {
    while (1) {
        system("cls");
        printf("请选择要删除的用户类型：\n\n");
        printf("1. 用户\n");
        printf("2. 快递员\n");
        printf("3. 管理员\n");
        printf("4. 快递平台\n\n");
        printf("按其他任意键返回...\n");

        char choice = _getch();
        puts("");
        char account[200];

        system("cls");
        printf("如若需要强制退出，请输入“exit”\n\n");
        switch (choice) {
        case '1':
rewrite_user:
            printf("请输入要删除的用户账号：\n");
            scanf("%s", account);
            clearInputBuffer();
            puts("");

            if (checkExit(account)) {
                return;
            }

            User *user = userElementGet(users_list, account);
            if (user == NULL) {
                printf("用户不存在！\n");
                printCommonInfo();
                goto rewrite_user;
            }

            // 如果该用户有未取件的快递，则不允许删除
            if (user->receive_status == 1) {
                printf("该用户有未取件的快递，请先处理！\n");
                printCommonInfo();
                return;
            }

            listRemove(users_list, user);
            printf("删除成功！\n");
            printCommonInfo();
            break;
        case '2':
rewrite_courier:
            printf("请输入要删除的快递员账号：\n");
            scanf("%s", account);
            clearInputBuffer();
            puts("");

            if (checkExit(account)) {
                return;
            }

            Courier *courier = courierElementGet(couriers_list, account);
            if (courier == NULL) {
                printf("快递员不存在！\n");
                printCommonInfo();
                goto rewrite_courier;
            }

            listRemove(couriers_list, courier);
            printf("删除成功！\n");
            printCommonInfo();
            break;
        case '3':
rewrite_admin:
            printf("请输入要删除的管理员账号：\n");
            scanf("%s", account);
            clearInputBuffer();
            puts("");

            if (checkExit(account)) {
                return;
            }

            Admin *admin = adminElementGet(admins_list, account);
            if (admin == NULL) {
                printf("管理员不存在！\n");
                printCommonInfo();
                goto rewrite_admin;
            }

            // 不允许删除自己，从而保证管理员数目至少为1
            if (strcmp(admin->account, the_admin->account) == 0) {
                printf("您无法删除自己！\n");
                printCommonInfo();
                return;
            }

            listRemove(admins_list, admin);
            printf("删除成功！\n");
            printCommonInfo();
            break;
        case '4':
rewrite_platform:
            printf("请输入要删除的平台账号：\n");
            scanf("%s", account);
            clearInputBuffer();
            puts("");

            if (checkExit(account)) {
                return;
            }

            Platform *platform = platformElementGet(platforms_list, account);
            if (platform == NULL) {
                printf("平台不存在！\n");
                printCommonInfo();
                goto rewrite_platform;
            }

            listRemove(platforms_list, platform);
            printf("删除成功！\n");
            printCommonInfo();
            break;
        default:
            return;
        }
    }
}

void modifyFunc() {
    while (1) {
        system("cls");
        printf("请选择要修改的用户类型：\n\n");
        printf("1. 用户\n");
        printf("2. 快递员\n");
        printf("3. 管理员\n");
        printf("4. 快递平台\n\n");
        printf("按其他任意键返回...\n");

        char choice = _getch();

        switch (choice) {
        case '1':
            modifyUser();
            break;
        case '2':
            modifyCourier();
            break;
        case '3':
            modifyAdmin();
            break;
        case '4':
            modifyPlatform();
            break;
        default:
            return;
        }
    }
}

void viewUserInfo() {
    while (1) {
        system("cls");
        printf("请选择要查看的用户类型：\n\n");
        printf("1. 用户\n");
        printf("2. 快递员\n");
        printf("3. 管理员\n");
        printf("4. 快递平台\n\n");
        printf("按其他任意键返回...\n");

        char choice = _getch();

        char account[200];
        system("cls");
        printf("如若需要强制退出，请输入“exit”\n\n");
        switch (choice) {
        case '1':
            printf("请输入要查看的用户账号：\n");
            scanf("%s", account);
            clearInputBuffer();
            puts("");

            if (checkExit(account)) {
                return;
            }

            User *user = userElementGet(users_list, account);
            if (user == NULL) {
                printf("用户不存在！\n");
                printCommonInfo();
                return;
            }
            system("cls");
            printf("用户名：%s\n", user->account);
            printf("密码：%s\n", user->password);
            printf("电话号码：%s\n", user->phone_number);

            char user_type[50];
            switch (user->user_type) {
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
            switch (user->receive_status) {
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
            switch (user->send_status) {
            case 0:
                strcpy(send_status, "未寄件");
                break;
            case 1:
                strcpy(send_status, "未发出");
                break;
case2:
                strcpy(send_status, "已发出");
                break;
            case 3:
                strcpy(send_status, "已送达");
                break;
            default:
                break;
            }
            printf("寄件状态：%s\n", send_status);
            printCommonInfo();
            break;
        case '2':
            printf("请输入要查看的快递员账号：\n");
            scanf("%s", account);
            clearInputBuffer();
            puts("");

            if (checkExit(account)) {
                return;
            }

            Courier *courier = courierElementGet(couriers_list, account);
            if (courier == NULL) {
                printf("快递员不存在！\n");
                printCommonInfo();
                return;
            }
            system("cls");
            printf("用户名：%s\n", courier->account);
            printf("密码：%s\n", courier->password);

            char status[60];
            switch (courier->status) {
            case 0:
                strcpy(status, "空闲");
                break;
            case 1:
                strcpy(status, "忙碌，正从快递平台取件");
                break;
            case 2:
                strcpy(status, "忙碌，正从驿站取件");
            case 3:
                strcpy(status, "隐身中");
                break;
            default:
                break;
            }
            printf("状态：%s\n", status);
            printCommonInfo();
            break;
        case '3':
            printf("请输入要查看的管理员账号：\n");
            scanf("%s", account);
            clearInputBuffer();
            puts("");

            if (checkExit(account)) {
                return;
            }

            Admin *admin = adminElementGet(admins_list, account);
            if (admin == NULL) {
                printf("管理员不存在！\n");
                printCommonInfo();
                return;
            }
            system("cls");
            printf("用户名：%s\n", admin->account);
            printf("密码：%s\n", admin->password);
            printCommonInfo();
            break;
        case '4':
            printf("请输入要查看的平台账号：\n");
            scanf("%s", account);
            clearInputBuffer();
            puts("");

            if (checkExit(account)) {
                return;
            }

            Platform *platform = platformElementGet(platforms_list, account);
            if (platform == NULL) {
                printf("平台不存在！\n");
                printCommonInfo();
                return;
            }
            system("cls");
            printf("用户名：%s\n", platform->account);
            printf("密码：%s\n", platform->password);
            printCommonInfo();
            break;
        default:
            return;
        }
    }
}

void viewShelfInfo() {
    while (1) {
        system("cls");
        printf("请选择要查看的货架：\n\n");
        printf("1. 货架A\n");
        printf("2. 货架B\n");
        printf("3. 货架C\n");
        printf("4. 货架D\n");
        printf("5. 货架E\n\n");
        printf("按其他任意键返回...\n");

        char choice = _getch();

        switch (choice) {
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
            return;
        }
    }
}

void viewShelf(const List *shelf_list) {
    system("cls");
    if (shelf_list->size == 0) {
        printf("货架为空！\n");
        printCommonInfo();
        return;
    }
    printf("货架内共有快递%d件：\n\n", shelf_list->size);

    // 打印表头
    printf("+--------------+----------+------------+------------+----------+----------+----------+----------+----------"
           "--------+------------+\n");
    printf("|   快递单号   | 货架编号 | 收件人账号 | 寄件人账号 |  取件码  | 加急状态 | 体积(升) | 重量(kg) |     "
           "特殊类型     |  价值(元)  |\n");
    printf("+--------------+----------+------------+------------+----------+----------+----------+----------+----------"
           "--------+------------+\n");

    ListNode *current = shelf_list->head;
    while (current != NULL) {
        Package *package = (Package *)current->data;

        char express[200];
        switch (package->isExpress) {
        case 0:
            strcpy(express, "未加急");
            break;
        case 1:
            strcpy(express, "已加急");
            break;
        }

        char special_type[50];
        switch (package->special_type) {
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

        // 调整各列宽度，确保不跨行
        printf("| %-12s | %-8s | %-12s | %-12s | %-8d | %-11s | %-8.2lf | %-8.2lf | %-18s | %-10.2lf |\n",
               package->package_id, package->shelf_id, package->receiver_account, package->sender_account,
               package->pick_up_code, express, package->volume, package->weight, special_type, package->value);

        current = current->next;
    }
    // 打印表尾
    printf("+--------------+----------+------------+------------+----------+----------+----------+----------+----------"
           "--------+------------+\n");
    printCommonInfo();
}

void modifyShelfInfo() {
    while (1) {
        system("cls");
        printf("请选择要修改的货架：\n\n");
        printf("1. 货架A\n");
        printf("2. 货架B\n");
        printf("3. 货架C\n");
        printf("4. 货架D\n");
        printf("5. 货架E\n\n");
        printf("按其他任意键返回...\n");

        char choice = _getch();

        system("cls");
        switch (choice) {
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
            return;
        }
    }
}

void modifyShelf(List *shelf_list) {
    system("cls");
    if (shelf_list->size == 0) {
        printf("货架为空！\n");
        printCommonInfo();
        return;
    }
    printf("如若需要强制退出，请输入“exit”\n\n");
    printf("请输入要修改的快递单号：\n");
    char package_id[200];
    scanf("%s", package_id);
    clearInputBuffer();
    puts("");

    if (checkExit(package_id)) {
        return;
    }

    ListNode *current = shelf_list->head;
    while (current != NULL) {
        Package *package = (Package *)current->data;

        // 遍历货架链表，直到找到要修改的那个快递
        if (strcmp(package_id, package->package_id) == 0) {
            while (1) {
                system("cls");
                printf("请选择要修改的信息：\n");
                printf("1. 收件人账号\n");
                // 快递员、寄件人、快递单号没有改的必要
                // 取件码为随机数生成，不支持更改
                printf("2. 加急状态\n");
                printf("3. 体积\n");
                printf("4. 重量\n");
                printf("5. 特殊类型\n");
                printf("6. 价值\n");
                printf("7. 货架位置\n\n");
                printf("按其他任意键返回...\n");

                char choice = _getch();

                system("cls");
                printf("如若需要强制退出，请输入“exit”\n\n");
                switch (choice) {
                case '1':
rewrite_receiver_account:
                    printf("请输入新的收件人账号：\n");
                    char new_receiver_account[200];
                    scanf("%s", new_receiver_account);
                    clearInputBuffer();
                    puts("");

                    if (checkExit(new_receiver_account)) {
                        return;
                    }

                    if (checkInputLimit(new_receiver_account) == 0) {
                        goto rewrite_receiver_account;
                    }

                    // 判断新的收件人账号是否存在
                    // 原则上，必须该用户存在于用户列表中
                    User *user = userElementGet(users_list, new_receiver_account);
                    if (user == NULL) {
                        printf("用户不存在！\n");
                        printCommonInfo();
                        goto rewrite_receiver_account;
                    }

                    // 还要修改这两个人的接受快递状态
                    User *old_user = userElementGet(users_list, package->receiver_account);
                    old_user->receive_status = 0;
                    user->receive_status = 1;

                    strcpy(package->receiver_account, new_receiver_account);
                    // 推送链表中的信息不需要更改，浅拷贝，所有数据都一样
                    break;
                case '2':
rewrite_express_type:
                    printf("请输入新的加急状态：\n");
                    printf("1. 不加急\n");
                    printf("2. 加急\n");

                    char new_express_type = getchar();
                    if (clearInputBuffer() != 0) {
                        if (new_express_type == 'e') {
                            return;
                        }
                        printf("输入错误！\n");
                        printCommonInfo();
                        goto rewrite_express_type;
                    }
                    puts("");

                    // 输入错误判断
                    if (new_express_type != '1' && new_express_type != '2') {
                        printf("输入错误！\n");
                        printCommonInfo();
                        goto rewrite_express_type;
                    }

                    package->isExpress = new_express_type - '0' - 1;   // 注意减1操作，为了匹配输入
                    break;
                case '3':
rewrite_volume:
                    printf("请输入新的体积(升)：\n");
                    double volume = 0;
                    int ret = scanf("%lf", &volume);
                    clearInputBuffer();
                    puts("");

                    if (ret == 0) {
                        return;
                    }

                    if (volume <= 0 || volume > 5000) {
                        printf("输入错误！\n");
                        printCommonInfo();
                        goto rewrite_volume;
                    }
                    package->volume = volume;

                    break;
                case '4':
rewrite_weight:
                    printf("请输入新的重量(kg)：\n");
                    double weight = 0;
                    ret = scanf("%lf", &weight);
                    clearInputBuffer();
                    puts("");

                    if (ret == 0) {
                        return;
                    }

                    if (weight <= 0 || weight > 200) {
                        printf("输入错误！\n");
                        printCommonInfo();
                        goto rewrite_weight;
                    }
                    package->weight = weight;
                    break;
                case '5':
rewrite_special_type:
                    printf("请输入新的快递类型：\n");
                    printf("1. 普通\n");
                    printf("2. 易碎品、电子产品\n");
                    printf("3. 生鲜\n");

                    char new_special_type = getchar();
                    if (clearInputBuffer() != 0) {
                        if (new_special_type == 'e') {
                            return;
                        }
                        printf("输入错误！\n");
                        printCommonInfo();
                        goto rewrite_special_type;
                    }
                    puts("");

                    if (new_special_type != '1' && new_special_type != '2' && new_special_type != '3') {
                        printf("输入错误！\n");
                        printCommonInfo();
                        goto rewrite_special_type;
                    }

                    package->special_type = new_special_type - '0' - 1;
                    break;
                case '6':
rewrite_value:
                    printf("请输入新的价值(元)：\n");
                    double value = 0;
                    ret = scanf("%lf", &value);
                    clearInputBuffer();
                    puts("");

                    if (ret == 0) {
                        return;
                    }

                    if (value <= 0 || value > 1000000) {
                        printf("输入错误！\n");
                        printCommonInfo();
                        goto rewrite_value;
                    }
                    package->value = value;

                    break;
                case '7':
                    modifyShelfPosition(package, shelf_list);
                    break;
                default:
                    return;
                }

                // 写入快递单修改文件modify_records.txt
                // 写入内容：快递号，快递收件人账号，修改内容，修改时间
                char modify_info[50];
                switch (choice) {
                case '1':
                    strcpy(modify_info, "收件人账号");
                    break;
                case '2':
                    strcpy(modify_info, "加急状态");
                    break;
                case '3':
                    strcpy(modify_info, "体积");
                    break;
                case '4':
                    strcpy(modify_info, "重量");
                    break;
                case '5':
                    strcpy(modify_info, "特殊类型");
                    break;
                case '6':
                    strcpy(modify_info, "价值");
                    break;
                case '7':
                    strcpy(modify_info, "货架位置");
                    break;
                default:
                    break;
                }
                recordModifyInfo(package->shelf_id, package->receiver_account, modify_info, getTime());

                printf("修改成功！\n");
                printCommonInfo();
            }
        }
        current = current->next;
    }
    printf("快递单号不存在！\n");
    printCommonInfo();
}

// 仿照平台函数来写
void addressUserSend() {
    system("cls");
    if (users_send_list->size == 0)   // 没有待发货快递
    {
        printf("暂无待发货快递！\n");
        printCommonInfo();
        return;
    }

    // 由于链表已“排序”，所以直接顺着取
    ListNode *current = users_send_list->head;
    ListNode *courier_current = couriers_list->head;

    if (courier_current == NULL) {
        printf("暂无快递员接单！\n");
        printCommonInfo();
        return;
    }

    int base = 0;
    while (courier_current != NULL) {
        if (current == NULL) {
            printf("\n已全部发货！\n");
            printCommonInfo();
            return;
        }

        Courier *courier = (Courier *)courier_current->data;
        Package *package = (Package *)current->data;

        // 如果快递员在另一侧忙或处于隐身状态，直接跳过
        if (courier->status == 1 || courier->status == 3) {
            courier_current = courier_current->next;
            continue;                                         // 为了防止下一个快递员也是1，这里要continue
        }

        strcpy(package->courier_account, courier->account);   // 包裹信息上标注快递员信息

        listAdd(couriers_push_list, package);                 // 加入快递员的推送链表
        printf("快递 %s 已向快递员 %s 发出派送请求\n", package->package_id, courier->account);
        courier->status = 2;                                  // 标记快递员状态为正在由 驿站->平台 派送
        User *user = userElementGet(users_list, package->sender_account);
        user->send_status = 2;                                // 标记用户状态为已发出

        current = current->next;                              // 转向下一件快递

        listRemove(users_send_list, package);                 // 从用户待发货链表中删除

        // 判断本次base点的情况
        base++;
        if (package->volume >= 0.5) {
            base++;
        }
        if (package->weight >= 3) {
            base++;
        }

        // 如果base点已经到了10，说明这个快递员已经满负荷了
        if (base == 10) {
            // 补丁性质的代码：如果下一个包裹存在且也是同一个人的，那就继续让该快递员工作。虽然不能解决根本问题，但先这样吧
            if (current != NULL && user->account != ((Package *)current->data)->sender_account) {
                courier_current = courier_current->next;
                base = 0;
                // 判断下一个快递员是否为空
                if (courier_current == NULL) {
                    printf("暂无快递员接单！\n");
                    printCommonInfo();
                    return;
                }
            }
        }
    }
}

void modifyShelfPosition(Package *package, List *shelf_list)   // shelf_list为当前货架
{
    system("cls");
    // 注意 由于货架是有序插入的，不能自定义货架号，只能改货架，将其尾插

    // 修改货架号的逻辑
    // 1. 从原货架链表中删除
    // 2. 加入新的货架链表
    // 3. 修改快递包裹的货架号
    // 4. 调整用户推送表中的货架号

    // 获取当前货架
    char now_shelf = package->shelf_id[0];
rewrite_shelf:
    printf("请选择新的货架：\n\n");
    printf("1. 货架A\n");
    printf("2. 货架B\n");
    printf("3. 货架C\n");
    printf("4. 货架D\n");
    printf("5. 货架E\n\n");
    printf("按其他任意键返回...\n");

    char choice = getchar();
    if (clearInputBuffer() != 0) {
        if (choice == 'e') {
            return;
        }
        return;
    }
    puts("");

    switch (choice) {
    case '1':
        if (now_shelf == 'A') {
            printf("货架未改变！\n");
            printCommonInfo();
            goto rewrite_shelf;
        }
        // 新货架满了
        if (shelf_a_list->size == 10) {
            printf("货架已满！\n");
            printCommonInfo();
            goto rewrite_shelf;
        }
        // 从原货架链表中删除
        listRemove(shelf_list, package);
        // 加入新的货架链表
        listAdd(shelf_a_list, package);
        // 修改快递包裹的货架号
        // 回忆sprintf，格式化写入字符串
        // C没有类似的直接写入字符串，非格式化写入还是用strcpy就好
        sprintf(package->shelf_id, "A-%d-%d", shelf_a_list->size / SIZE, shelf_a_list->size % SIZE);
        // 由于浅拷贝，用户推送表中的货架号也会自动修改
        break;
    case '2':
        if (now_shelf == 'B') {
            printf("货架未改变！\n");
            printCommonInfo();
            goto rewrite_shelf;
        }
        if (shelf_b_list->size == 10) {
            printf("货架已满！\n");
            printCommonInfo();
            goto rewrite_shelf;
        }
        listRemove(shelf_list, package);
        listAdd(shelf_b_list, package);
        sprintf(package->shelf_id, "B-%d-%d", shelf_b_list->size / SIZE, shelf_b_list->size % SIZE);
        break;
    case '3':
        if (now_shelf == 'C') {
            printf("货架未改变！\n");
            printCommonInfo();
            goto rewrite_shelf;
        }
        if (shelf_c_list->size == 10) {
            printf("货架已满！\n");
            printCommonInfo();
            goto rewrite_shelf;
        }
        listRemove(shelf_list, package);
        listAdd(shelf_c_list, package);
        sprintf(package->shelf_id, "C-%d-%d", shelf_c_list->size / SIZE, shelf_c_list->size % SIZE);
        break;
    case '4':
        if (now_shelf == 'D') {
            printf("货架未改变！\n");
            printCommonInfo();
            goto rewrite_shelf;
        }
        if (shelf_d_list->size == 10) {
            printf("货架已满！\n");
            printCommonInfo();
            goto rewrite_shelf;
        }
        listRemove(shelf_list, package);
        listAdd(shelf_d_list, package);
        sprintf(package->shelf_id, "D-%d-%d", shelf_d_list->size / SIZE, shelf_d_list->size % SIZE);
        break;
    case '5':
        if (now_shelf == 'E') {
            printf("货架未改变！\n");
            printCommonInfo();
            goto rewrite_shelf;
        }
        if (shelf_e_list->size == 10) {
            printf("货架已满！\n");
            printCommonInfo();
            goto rewrite_shelf;
        }
        listRemove(shelf_list, package);
        listAdd(shelf_e_list, package);
        sprintf(package->shelf_id, "E-%d-%d", shelf_e_list->size / SIZE, shelf_e_list->size % SIZE);
        break;
    default:
        return;
    }
}

void recordModifyInfo(const char *shelf_id, const char *account, const char *modify_info, struct tm *local_time) {
    FILE *fp = fopen("../res/modify_records.txt", "a");
    if (fp == NULL) {
        printf("文件打开失败！\n");
        printCommonInfo();
    }

    fprintf(fp, "(取件) 货架号：%s  用户名：%s  修改信息：%s  时间：%d-%d-%d %d:%d:%d\n", shelf_id, account,
            modify_info, local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour,
            local_time->tm_min, local_time->tm_sec);
    fclose(fp);
}

void recordModifyInfoOfSend(const char *package_id,
                            const char *account,
                            const char *modify_info,
                            struct tm *local_time) {
    FILE *fp = fopen("../res/modify_records.txt", "a");
    if (fp == NULL) {
        printf("文件打开失败！\n");
        printCommonInfo();
    }

    fprintf(fp, "(寄件) 快递单号：%s  用户名：%s  修改信息：%s  时间：%d-%d-%d %d:%d:%d\n", package_id, account,
            modify_info, local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour,
            local_time->tm_min, local_time->tm_sec);
    fclose(fp);
}

void viewPackageLog() {
    system("cls");
    FILE *fp = fopen("../res/modify_records.txt", "r");
    if (fp == NULL) {
        printf("文件打开失败！\n");
        printCommonInfo();
    }

    char line[100];
    while (fgets(line, 100, fp) != NULL) {
        printf("%s", line);
    }
    fclose(fp);
    printCommonInfo();
}

void modifyUserSend() {
    // 修改完后要进行写入modify_records.txt
    system("cls");
    if (users_send_list->size == 0) {
        printf("暂无待发货快递！\n");
        printCommonInfo();
        return;
    }

    printf("请选择要修改快递的快递单号：\n");
    char package_id[200];
    scanf("%s", package_id);
    clearInputBuffer();
    puts("");

    ListNode *current = users_send_list->head;
    while (current != NULL) {
        Package *package = (Package *)current->data;
        if (strcmp(package_id, package->package_id) == 0) {
            system("cls");
            printf("如若需要强制退出，请输入“exit”\n\n");
            printf("请选择要修改的信息：\n\n");
            printf("1. 快递单号\n");
            printf("2. 加急状态\n");
            printf("3. 体积\n");
            printf("4. 重量\n");
            printf("5. 特殊类型\n");
            printf("6. 价值\n");
            printf("7. 收件人\n\n");
            printf("按其他任意键返回...\n");

            char choice = _getch();

            switch (choice) {
            case '1':
rewrite_package_id:
                printf("请输入新的快递单号：\n");
                char new_package_id[200];
                scanf("%s", new_package_id);
                clearInputBuffer();
                puts("");

                if (checkExit(new_package_id)) {
                    return;
                }

                if (checkInputLimit(new_package_id) == 0) {
                    goto rewrite_package_id;
                }

                // 快递单号重复检查
                ListNode *current = users_send_list->head;
                while (current != NULL) {
                    Package *package = (Package *)current->data;
                    if (strcmp(package->package_id, new_package_id) == 0) {
                        printf("快递单号已存在，请重新输入！\n");
                        printCommonInfo();
                        goto rewrite_package_id;
                    }
                    current = current->next;
                }

                strcpy(package->package_id, new_package_id);
                break;
            case '2':
rewrite_express_type:
                printf("请输入新的加急状态：\n");
                printf("1. 否\n");
                printf("2. 是\n");

                char new_express_type = getchar();
                if (clearInputBuffer() != 0) {
                    if (new_express_type == 'e') {
                        return;
                    }
                    printf("输入错误！\n");
                    printCommonInfo();
                    goto rewrite_express_type;
                }
                puts("");

                if (new_express_type != '1' && new_express_type != '2') {
                    printf("输入错误！\n");
                    printCommonInfo();
                    goto rewrite_express_type;
                }

                package->isExpress = new_express_type - '0' - 1;
                break;
            case 3:
rewrite_volume:
                printf("请输入新的体积：\n");
                double volume = 0;
                int ret = scanf("%lf", &volume);
                clearInputBuffer();
                puts("");

                if (ret == 0) {
                    return;
                }

                if (volume <= 0 || volume > 5000) {
                    printf("输入错误！\n");
                    printCommonInfo();
                    goto rewrite_volume;
                }
                package->volume = volume;
                break;
            case 4:
rewrite_weight:
                printf("请输入新的重量：\n");
                double weight = 0;
                ret = scanf("%lf", &weight);
                clearInputBuffer();
                puts("");

                if (ret == 0) {
                    return;
                }

                if (weight <= 0 || weight > 200) {
                    printf("输入错误！\n");
                    printCommonInfo();
                    goto rewrite_weight;
                }
                package->weight = weight;
                break;
            case 5:
rewrite_special_type:
                printf("请输入新的快递类型：\n");
                printf("1. 普通\n");
                printf("2. 易碎品、电子产品\n");
                printf("3. 生鲜\n");

                char new_special_type = getchar();
                if (clearInputBuffer() != 0) {
                    if (new_special_type == 'e') {
                        return;
                    }
                    printf("输入错误！\n");
                    printCommonInfo();
                    goto rewrite_special_type;
                }
                puts("");

                if (new_special_type != '1' && new_special_type != '2' && new_special_type != '3') {
                    printf("输入错误！\n");
                    printCommonInfo();
                    goto rewrite_special_type;
                }

                package->special_type = new_special_type - '0' - 1;
                break;
            case 6:
rewrite_value:
                printf("请输入新的价值：\n");
                double value = 0;
                ret = scanf("%lf", &value);
                clearInputBuffer();
                puts("");

                if (ret == 0) {
                    return;
                }

                if (value <= 0 || value > 1000000) {
                    printf("输入错误！\n");
                    printCommonInfo();
                    goto rewrite_value;
                }
                package->value = value;
                break;
            case 7:
rewrite_receiver_account:
                printf("请输入新的收件人账号：\n");
                char new_receiver_account[200];
                scanf("%s", new_receiver_account);
                clearInputBuffer();
                puts("");

                if (checkExit(new_receiver_account)) {
                    return;
                }

                if (checkInputLimit(new_receiver_account) == 0) {
                    goto rewrite_receiver_account;
                }
                strcpy(package->receiver_account, new_receiver_account);
                break;
            default:
                return;
            }

            // 写入快递单修改文件modify_records.txt
            char modify_info[50];
            switch (choice) {
            case '1':
                strcpy(modify_info, "快递单号");
                break;
            case '2':
                strcpy(modify_info, "加急状态");
                break;
            case '3':
                strcpy(modify_info, "体积");
                break;
            case '4':
                strcpy(modify_info, "重量");
                break;
            case '5':
                strcpy(modify_info, "特殊类型");
                break;
            case '6':
                strcpy(modify_info, "价值");
                break;
            case '7':
                strcpy(modify_info, "收件人");
                break;
            default:
                break;
            }
            recordModifyInfoOfSend(package->package_id, package->receiver_account, modify_info, getTime());

            printf("修改成功！\n");
            printCommonInfo();
            return;
        }
        current = current->next;
    }
}

void listAllInfo() {
    while (1) {
        system("cls");
        printf("请选择要查看的用户类型：\n\n");
        printf("1. 用户\n");
        printf("2. 快递员\n");
        printf("3. 管理员\n");
        printf("4. 快递平台\n\n");
        printf("按其他任意键返回...\n");

        char choice = _getch();

        switch (choice) {
        case '1':
            listAllUsers();
            break;
        case '2':
            listAllCouriers();
            break;
        case '3':
            listAllAdmins();
            break;
        case '4':
            listAllPlatforms();
            break;
        default:
            return;
        }
    }
}

void listAllUsers() {
    system("cls");
    if (users_list->size == 0) {
        printf("用户列表为空！\n");
        printCommonInfo();
        return;
    }
    printf("用户列表如下，共有用户%d位：\n\n", users_list->size);

    // 打印表头
    printf("+----------------+----------------+----------------+\n");
    printf("|    用户名      |      密码      |    电话号码    |\n");
    printf("+----------------+----------------+----------------+\n");

    ListNode *current = users_list->head;
    while (current != NULL) {
        User *user = (User *)current->data;
        // 精细调整各列宽度和制表符相关设置
        printf("| %-16s | %-14s | %-14s |\n", user->account, user->password, user->phone_number);
        current = current->next;
    }
    // 打印表尾
    printf("+----------------+----------------+----------------+\n");
    printCommonInfo();
}

void listAllCouriers() {
    system("cls");
    if (couriers_list->size == 0) {
        printf("快递员列表为空！\n");
        printCommonInfo();
        return;
    }
    printf("快递员列表如下，共有快递员%d位：\n\n", couriers_list->size);

    // 打印表头
    printf("+----------------+----------------+\n");
    printf("|     用户名     |      密码      |\n");
    printf("+----------------+----------------+\n");

    ListNode *current = couriers_list->head;
    while (current != NULL) {
        Courier *courier = (Courier *)current->data;
        // 精细调整各列宽度和制表符相关设置
        printf("| %-17s | %-14s |\n", courier->account, courier->password);
        current = current->next;
    }
    // 打印表尾
    printf("+----------------+----------------+\n");
    printCommonInfo();
}

void listAllAdmins() {
    system("cls");
    if (admins_list->size == 0) {
        printf("管理员列表为空！\n");
        printCommonInfo();
        return;
    }
    printf("管理员列表如下，共有管理员%d位：\n\n", admins_list->size);

    // 打印表头
    printf("+----------------+----------------+\n");
    printf("|     用户名     |      密码      |\n");
    printf("+----------------+----------------+\n");

    ListNode *current = admins_list->head;
    while (current != NULL) {
        Admin *admin = (Admin *)current->data;
        // 精细调整各列宽度和制表符相关设置
        printf("| %-17s | %-14s |\n", admin->account, admin->password);
        current = current->next;
    }
    // 打印表尾
    printf("+----------------+----------------+\n");
    printCommonInfo();
}

void listAllPlatforms() {
    system("cls");
    if (platforms_list->size == 0) {
        printf("快递平台列表为空！\n");
        printCommonInfo();
        return;
    }
    printf("快递平台列表如下，共有平台%d个：\n\n", platforms_list->size);

    // 打印表头
    printf("+----------------+----------------+\n");
    printf("|     用户名     |      密码      |\n");
    printf("+----------------+----------------+\n");

    ListNode *current = platforms_list->head;
    while (current != NULL) {
        Platform *platform = (Platform *)current->data;
        // 精细调整各列宽度和制表符相关设置
        printf("| %-16s | %-14s |\n", platform->account, platform->password);
        current = current->next;
    }
    // 打印表尾
    printf("+----------------+----------------+\n");
    printCommonInfo();
}

void findLeavePackage() {
    findLeavePackageFromShelf(shelf_a_list);
    findLeavePackageFromShelf(shelf_b_list);
    findLeavePackageFromShelf(shelf_c_list);
    findLeavePackageFromShelf(shelf_d_list);
    findLeavePackageFromShelf(shelf_e_list);
}

void findLeavePackageFromShelf(List *shelf_list) {
    if (shelf_list->size == 0) {
        return;
    }

    time_t current_time = time(NULL);
    // 如果超过MAX_TIME，说明快递已经滞留了
    ListNode *current = shelf_list->head;
    while (current != NULL) {
        Package *package = (Package *)current->data;
        if (difftime(current_time, package->time) > MAX_TIME) {
            User *user = userElementGet(users_list, package->receiver_account);
            if (user == NULL) {
                printf("用户不存在！\n");
                printCommonInfo();
                return;
            }
            user->delivery_leave = 1;   // 设置为滞留状态
        }
        current = current->next;
    }
}

void viewAccount() {
    system("cls");
    printf("当前驿站余额为：%.2lf元\n", money);
    printCommonInfo();
}

void modifyUser() {
    system("cls");
    char account[200];
    char choice2;
    printf("如若需要强制退出，请输入“exit”\n\n");
    printf("请输入要修改的用户账号：\n");
    scanf("%s", account);
    clearInputBuffer();
    puts("");

    if (checkExit(account)) {
        return;
    }

    User *user = userElementGet(users_list, account);
    if (user == NULL) {
        printf("用户不存在！\n");
        printCommonInfo();
        return;
    }
    system("cls");
    printf("请选择要修改的信息：\n");
    printf("1. 用户名\n");
    printf("2. 密码\n");
    printf("3. 电话号码\n");
    printf("4. 用户类型\n\n");
    printf("按其他任意键返回...\n");

    choice2 = _getch();

    system("cls");
    printf("如若需要强制退出，请输入“exit”\n\n");
    switch (choice2) {
    case '1':
rewrite_account2:
        printf("请输入新的用户名：\n");
        char new_account[200];
        scanf("%s", new_account);
        clearInputBuffer();
        puts("");

        if (checkExit(new_account)) {
            return;
        }

        if (checkInputLimit(new_account) == 0) {
            goto rewrite_account2;
        }

        // 用户名重复检查
        ListNode *current = users_list->head;
        while (current != NULL) {
            User *user = (User *)current->data;
            if (strcmp(user->account, new_account) == 0) {
                printf("用户名已存在，请重新输入！\n");
                printCommonInfo();
                goto rewrite_account2;
            }
            current = current->next;
        }

        strcpy(user->account, new_account);
        printf("修改成功！\n");
        printCommonInfo();
        break;
    case '2':
rewrite_password:
        printf("请输入新的密码：\n");
        char new_password[200];
        getPassword(new_password);
        puts("");

        if (checkExit(new_password)) {
            return;
        }

        if (checkInputLimit(new_password) == 0) {
            goto rewrite_password;
        }

        // 确认密码
        printf("请再次输入新的密码：\n");
        char new_password_confirm[200];
        getPassword(new_password_confirm);
        puts("");

        if (checkExit(new_password_confirm)) {
            return;
        }

        if (strcmp(new_password, new_password_confirm) != 0) {
            printf("两次密码不一致！\n");
            printCommonInfo();
            goto rewrite_password;
        }

        strcpy(user->password, new_password);
        printf("修改成功！\n");
        printCommonInfo();
        break;
    case '3':
rewrite_phone_number:
        printf("请输入新的电话号码：\n");
        char new_phone_number[200];
        scanf("%s", new_phone_number);
        clearInputBuffer();
        puts("");

        if (checkExit(new_phone_number)) {
            return;
        }

        if (checkInputLimit(new_phone_number) == 0) {
            goto rewrite_phone_number;
        }

        strcpy(user->phone_number, new_phone_number);
        printf("修改成功！\n");
        printCommonInfo();
        break;
    case '4':
rewrite_user_type:
        printf("请输入新的用户类型：\n");
        printf("1. 普通用户\n");
        printf("2. 会员用户\n");
        printf("3. 企业用户\n");
        printf("4. 代理商用户\n");
        printf("5. 合作商家用户\n");

        char new_user_type = getchar();
        if (clearInputBuffer() != 0) {
            if (new_user_type == 'e') {
                return;
            }
            printf("输入错误！\n");
            printCommonInfo();
            goto rewrite_user_type;
        }
        puts("");
        // 这里说一下，虽然没输入整形数据
        // 但整形数据不是不需要清空缓存区，而是，如果保证输入的都是整形的话，那不会读空白字符，因此不需要清空缓存区；但这个程序里显然是整形与字符混和输入

        // 输入错误逻辑
        if (new_user_type != '1' && new_user_type != '2' && new_user_type != '3' && new_user_type != '4' &&
            new_user_type != '5') {
            printf("输入错误！\n");
            printCommonInfo();
            goto rewrite_user_type;
        }

        user->user_type = new_user_type - '0' - 1;
        printf("修改成功！\n");
        printCommonInfo();
        break;
    default:
        break;
    }
}

void modifyCourier() {
    system("cls");
    char account[200];
    char choice2;
    printf("如若需要强制退出，请输入“exit”\n\n");
    printf("请输入要修改的快递员账号：\n");
    scanf("%s", account);
    clearInputBuffer();
    puts("");

    if (checkExit(account)) {
        return;
    }

    Courier *courier = courierElementGet(couriers_list, account);
    if (courier == NULL) {
        printf("快递员不存在！\n");
        printCommonInfo();
        return;
    }
    system("cls");
    printf("请选择要修改的信息：\n");
    printf("1. 用户名\n");
    printf("2. 密码\n\n");
    printf("按其他任意键返回...\n");

    choice2 = _getch();

    system("cls");
    printf("如若需要强制退出，请输入“exit”\n\n");
    switch (choice2) {
    case '1':
rewrite_account2:
        printf("请输入新的用户名：\n");
        char new_account[200];
        scanf("%s", new_account);
        clearInputBuffer();
        puts("");

        if (checkExit(new_account)) {
            return;
        }

        if (checkInputLimit(new_account) == 0) {
            goto rewrite_account2;
        }

        // 用户名重复检查
        ListNode *current = couriers_list->head;
        while (current != NULL) {
            Courier *courier = (Courier *)current->data;
            if (strcmp(courier->account, new_account) == 0) {
                printf("用户名已存在，请重新输入！\n");
                printCommonInfo();
                goto rewrite_account2;
            }
            current = current->next;
        }

        strcpy(courier->account, new_account);
        break;
    case '2':
rewrite_password:
        printf("请输入新的密码：\n");
        char new_password[200];
        getPassword(new_password);
        puts("");

        if (checkExit(new_password)) {
            return;
        }

        if (checkInputLimit(new_password) == 0) {
            goto rewrite_password;
        }

        // 确认密码
        printf("请再次输入新的密码：\n");
        char new_password_confirm[200];
        getPassword(new_password_confirm);
        puts("");

        if (checkExit(new_password_confirm)) {
            return;
        }

        if (strcmp(new_password, new_password_confirm) != 0) {
            printf("两次密码不一致！\n");
            printCommonInfo();
            goto rewrite_password;
        }

        strcpy(courier->password, new_password);
        break;
    default:
        return;
    }
    printf("修改成功！\n");
    printCommonInfo();
}

void modifyAdmin() {
    system("cls");
    char account[200];
    char choice2;
    printf("如若需要强制退出，请输入“exit”\n\n");
    printf("请输入要修改的管理员账号：\n");
    scanf("%s", account);
    clearInputBuffer();
    puts("");

    if (checkExit(account)) {
        return;
    }

    Admin *admin = adminElementGet(admins_list, account);
    if (admin == NULL) {
        printf("管理员不存在！\n");
        printCommonInfo();
        return;
    }
    system("cls");
    printf("请选择要修改的信息：\n");
    printf("1. 用户名\n");
    printf("2. 密码\n\n");
    printf("按其他任意键返回...\n");

    choice2 = _getch();

    system("cls");
    printf("如若需要强制退出，请输入“exit”\n\n");
    switch (choice2) {
    case '1':
rewrite_account2:
        printf("请输入新的用户名：\n");
        char new_account[200];
        scanf("%s", new_account);
        clearInputBuffer();
        puts("");

        if (checkExit(new_account)) {
            return;
        }

        if (checkInputLimit(new_account) == 0) {
            goto rewrite_account2;
        }

        // 用户名重复检查
        ListNode *current = admins_list->head;
        while (current != NULL) {
            Admin *admin = (Admin *)current->data;
            if (strcmp(admin->account, new_account) == 0) {
                printf("用户名已存在，请重新输入！\n");
                printCommonInfo();
                goto rewrite_account2;
            }
            current = current->next;
        }

        strcpy(admin->account, new_account);
        break;
    case '2':
rewrite_password:
        printf("请输入新的密码：\n");
        char new_password[200];
        getPassword(new_password);
        puts("");

        if (checkExit(new_password)) {
            return;
        }

        if (checkInputLimit(new_password) == 0) {
            goto rewrite_password;
        }

        // 确认密码
        printf("请再次输入新的密码：\n");
        char new_password_confirm[200];
        getPassword(new_password_confirm);
        puts("");

        if (checkExit(new_password_confirm)) {
            return;
        }

        if (strcmp(new_password, new_password_confirm) != 0) {
            printf("两次密码不一致！\n");
            printCommonInfo();
            goto rewrite_password;
        }

        strcpy(admin->password, new_password);
        break;
    default:
        return;
    }
    printf("修改成功！\n");
    printCommonInfo();
}

void modifyPlatform() {
    system("cls");
    char account[200];
    char choice2;
    printf("如若需要强制退出，请输入“exit”\n\n");
    printf("请输入要修改的平台账号：\n");
    scanf("%s", account);
    clearInputBuffer();
    puts("");

    if (checkExit(account)) {
        return;
    }

    Platform *platform = platformElementGet(platforms_list, account);
    if (platform == NULL) {
        printf("平台不存在！\n");
        printCommonInfo();
        return;
    }
    system("cls");
    printf("请选择要修改的信息：\n");
    printf("1. 用户名\n");
    printf("2. 密码\n\n");
    printf("按其他任意键返回...\n");

    choice2 = _getch();

    system("cls");
    printf("如若需要强制退出，请输入“exit”\n\n");
    switch (choice2) {
    case '1':
rewrite_account2:
        printf("请输入新的用户名：\n");
        char new_account[200];
        scanf("%s", new_account);
        clearInputBuffer();
        puts("");

        if (checkExit(new_account)) {
            return;
        }

        if (checkInputLimit(new_account) == 0) {
            goto rewrite_account2;
        }

        // 用户名重复检查
        ListNode *current = platforms_list->head;
        while (current != NULL) {
            Platform *platform = (Platform *)current->data;
            if (strcmp(platform->account, new_account) == 0) {
                printf("用户名已存在，请重新输入！\n");
                printCommonInfo();
                goto rewrite_account2;
            }
            current = current->next;
        }

        strcpy(platform->account, new_account);
        break;
    case '2':
rewrite_password:
        printf("请输入新的密码：\n");
        char new_password[200];
        getPassword(new_password);
        puts("");

        if (checkExit(new_password)) {
            return;
        }

        if (checkInputLimit(new_password) == 0) {
            goto rewrite_password;
        }

        // 确认密码
        printf("请再次输入新的密码：\n");
        char new_password_confirm[200];
        getPassword(new_password_confirm);
        puts("");

        if (checkExit(new_password_confirm)) {
            return;
        }

        if (strcmp(new_password, new_password_confirm) != 0) {
            printf("两次密码不一致！\n");
            printCommonInfo();
            goto rewrite_password;
        }

        strcpy(platform->password, new_password);
        break;
    default:
        return;
    }
    printf("修改成功！\n");
    printCommonInfo();
}

void handleAbnormalPackage() {
    system("cls");
    // 处理拒收链表
    if (refuse_list->size == 0) {
        printf("暂无拒收快递！\n");
        printCommonInfo();
        return;
    }
    ListNode *current = refuse_list->head;
    while (current != NULL) {
        system("cls");
        printf("如若需要强制退出，请输入“exit”\n\n");
        Package *package = (Package *)current->data;
        printf("包裹%s拒收原因如下：\n", package->package_id);
        printf("%s\n", package->remark);
        printf("\n请选择处理方案：\n");
        printf("1. 赔偿\n");
        printf("2. 退回\n");
        printf("3. 协商\n\n");
        printf("按任意键退出异常包裹处理功能...\n");
        char choice = _getch();
        puts("");

        switch (choice) {
        case '1':
            printf("请输入赔偿金额(元)：\n");
            int compensation;
            scanf("%d", &compensation);
            clearInputBuffer();
            puts("");
            money -= compensation;
            printf("赔偿成功！\n");
            break;
        case '2':
            printf("包裹已退回！\n");
            break;
        case '3':
            printf("请自行与用户协商！\n");
            break;
        default:
            return;
        }

        printf("\n请给用户留言：\n\n");
        char message[100];
        scanf("%s", message);
        clearInputBuffer();
        puts("");

        if (checkExit(message)) {
            return;
        }

        User *user = userElementGet(users_list, package->receiver_account);
        strcpy(user->message, message);   // 留言给用户
        user->message_status = 1;         // 设置为已读状态

        // 处理完后删除节点
        ListNode *del = current;
        current = current->next;
        listRemove(refuse_list, del->data);   // 删除节点
    }
    printf("已处理完全部拒收包裹！\n");
    printCommonInfo();
}

void viewUserSendInfo() {
    system("cls");
    if (users_send_list->size == 0) {
        printf("用户寄件仓库为空！\n");
        printCommonInfo();
        return;
    }
    printf("用户寄件仓库中共有快递%d件：\n\n", users_send_list->size);

    // 打印表头
    printf("+----------------+----------------+----------------+--------------+----------+----------+------------------"
           "+------------+\n");
    printf("|   快递单号     |   收件人账号   |   寄件人账号   |   加急状态   | 体积(升) | 重量(kg) |     特殊类型     "
           "|  价值(元)  |\n");
    printf("+----------------+----------------+----------------+--------------+----------+----------+------------------"
           "+------------+\n");

    ListNode *current = users_send_list->head;
    while (current != NULL) {
        Package *package = (Package *)current->data;
        // 精细调整各列宽度和制表符相关设置
        printf("| %-14s | %-16s | %-16s | %-13s | %-8.2lf | %-8.2lf | %-18s | %-10.2lf |\n", package->package_id,
               package->receiver_account, package->sender_account, package->isExpress == 1 ? "是" : "否",
               package->volume, package->weight,
               package->special_type == 1   ? "易碎品、电子产品"
               : package->special_type == 2 ? "生鲜"
                                            : "普通",
               package->value);
        current = current->next;
    }
    // 打印表尾
    printf("+----------------+----------------+----------------+--------------+----------+----------+------------------"
           "+------------+\n");
    printCommonInfo();
}
