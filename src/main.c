#include "login.h"
#include "user.h"
#include "admin.h"
#include "courier.h"
#include "platform.h"

// 定义全局变量链表--在main函数处定义。其他源文件若想使用，去对应头文件声明全局变量即可
// 平台账号没有使用链表储存，因为逻辑上来说，平台与驿站是深度合作关系，管理员无权限修改平台相关信息
// 因此平台信息只能用文件来储存和搜索

// 对于拥有链表的数据，遵循以下原则：
// 1. 搜索数据时，之间遍历链表。
// 2. 文件只负责储存信息，不负责搜索信息工作。
// 3. 每次对链表进行增删改操作后。无需立刻更新文件。只有在退出程序时，才会统一更新文件，以减少IO操作。
List *users_list;
List *admins_list;
List *couriers_list;
List *platform_warehouse_list;
List *admin_warehouse_list;
List *users_send_list;
List *shelf_a_list;
List *shelf_b_list;
List *shelf_c_list;
List *shelf_d_list;
List *shelf_e_list;

int main()
{
    // 统一逻辑：仅写入链表即可，文件无需同步更新。最后退出时会一并更新。由于每次搜索都以链表为依据，因此不会出现因未及时更新导致数据不一致的情况
    users_list = listInit();
    admins_list = listInit();
    couriers_list = listInit();
    platform_warehouse_list = listInit();
    admin_warehouse_list = listInit();
    users_send_list = listInit();
    shelf_a_list = listInit();
    shelf_b_list = listInit();
    shelf_c_list = listInit();
    shelf_d_list = listInit();
    shelf_e_list = listInit();

    // 读取文件中的数据，恢复链表数据
    recoverListData();

    while (1)
    {
        // 界面展示
        system("cls");
        printf("欢迎使用快递管理系统！\n");
        printf("请登录：\n");
        printf("1. 用户\n");
        printf("2. 管理员\n");
        printf("3. 快递员\n");
        printf("4. 运输平台\n");
        printf("5. 注册\n");
        printf("按其他任意键退出\n");
        printf("请选择：\n");

        char identity = getchar();
        clearInputBuffer(); // 清空输入缓冲区，不止用一个getchar()，因为可能输入多个字符

    // 注册
    again:
        if (identity == '5')
        {
            system("cls"); // 清屏逻辑：输入后进入下一个页面时后，第一句加清屏，防止上一个页面的打印信息没有呈现就被清除
            printf("温馨提示：当前仅支持用户注册，其他请联系管理员。\n");

            printf("请输入用户名：\n");
            char account[20];
            scanf("%s", account);
            clearInputBuffer();

            printf("请输入密码：\n");
            char password[20];
            scanf("%s", password);
            clearInputBuffer();

            printf("请再次确认密码：\n");
            char password_confirm[20];
            scanf("%s", password_confirm);
            clearInputBuffer();

            if (strcmp(password, password_confirm) != 0)
            {
                printf("两次密码输入不一致，请重新输入！\n");
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
                        goto again;
                    }
                    node = node->next;
                }

                printf("请输入电话号码：\n");
                char phone_number[20];
                scanf("%s", phone_number);
                clearInputBuffer();

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
                strcpy(user->package_id, "0");
                listAdd(users_list, user);

                printf("注册成功！\n");
                continue;
            }
        }
        // 登录
        else if (identity == '1' || identity == '2' || identity == '3' || identity == '4')
        {
            system("cls");
            char account[20];
            char password[20];
            
            printf("请输入用户名:\n");
            scanf("%s", account);
            clearInputBuffer();

            printf("请输入密码:\n");
            scanf("%s", password);
            clearInputBuffer();

            int ret = 0;
            switch (identity)
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
                continue;
            }

            // 登录成功后，根据身份进入对应的菜单
            switch (identity)
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
        else // 退出
            break;
        // 重写所有文件
        rewriteAllFiles();
    }

    // 释放所有链表内存
    freeLists();
    return 0;
}