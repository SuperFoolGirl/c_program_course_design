#include "../include/login.h"

// 定义全局变量链表--在main函数处定义。其他源文件若想使用，去对应头文件声明全局变量即可

// 对于拥有链表的数据，遵循以下原则：
// 1. 搜索数据时，之间遍历链表。
// 2. 文件只负责储存信息，不负责搜索信息工作。
// 3. 每次对链表进行增删改操作后。无需立刻更新文件。只有在退出程序时，才会统一更新文件，以减少IO操作。

// 四条用户链表
List *users_list; // User类型链表
List *admins_list; // Admin类型链表
List *couriers_list; // Courier类型链表
List *platforms_list; // Platform类型链表

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
        if (clearInputBuffer() != 0)
        {
            break;
        }

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
    }
    // 每次退出程序前，更新文件数据，然后再销毁链表
    // 写在这里还有一个作用：如果终途关闭程序，文件不会被修改，因为关闭控制台后不会再运行代码，因此文件会保持运行前的状态
    // 强行退出时，堆区空间会被释放，不必担心内存泄漏
    rewriteAllFiles();
    // 释放所有链表内存
    freeLists();
    return 0;
}