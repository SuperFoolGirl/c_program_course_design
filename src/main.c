#include "login.h"

// 定义全局变量链表--在main函数处定义。其他源文件若想使用，去对应头文件声明全局变量即可
// 平台账号没有使用链表储存，因为逻辑上来说，平台与驿站是深度合作关系，管理员无权限修改平台相关信息
// 因此平台信息只能用文件来储存和搜索

// 对于拥有链表的数据，遵循以下原则：
// 1. 搜索数据时，之间遍历链表。
// 2. 文件只负责储存信息，不负责搜索信息工作。
// 3. 每次对链表进行增删改操作后。无需立刻更新文件。只有在退出程序时，才会统一更新文件，以减少IO操作。

// 三条用户链表
List *users_list; // User类型链表
List *admins_list; // Admin类型链表
List *couriers_list; // Courier类型链表

// 两个仓库链表
List *platform_warehouse_list; // Package类型链表
List *admin_warehouse_list; // Package类型链表

// 用户寄件链表
List *users_send_list; // Package类型链表

// 两个推送链表
List *users_push_list; // Package类型链表
List *couriers_push_list; // Package类型链表

// 五个货架链表
List *shelf_a_list; // Package类型链表
List *shelf_b_list; // Package类型链表
List *shelf_c_list; // Package类型链表
List *shelf_d_list; // Package类型链表
List *shelf_e_list; // Package类型链表



int main()
{
    // 初始化所有链表
    listsInit();

    // 读取文件中的数据，恢复链表数据
    recoverListData();

    while (1) 
    {
        // 界面展示
        system("cls");
        printf("欢迎使用快递管理系统！\n\n");
        printf("请选择：\n");
        printf("1. 登录\n");
        printf("2. 注册\n\n");
        printf("按其他任意键退出\n");

        char choice = getchar();
        clearInputBuffer();

        if (choice == '1')
        {
            login();
        }
        else if (choice == '2')
        {
            registering();
        }
        else
        {
            break;
        }

        // 重写所有文件，注意这句在while循环内部
        rewriteAllFiles();
    }

    // 释放所有链表内存
    freeLists();
    return 0;
}