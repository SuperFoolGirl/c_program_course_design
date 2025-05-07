/*
本头文件用于存储所有可能用到的数据，其中主干数据用链表存储
头文件除了用来包含其他头文件、声明函数，还能用来定义结构体、声明全局变量
*/

#pragma once
#include "common.h"
#include <time.h>

#define MAX_LENGTH 12   // 用户名、密码最大长度
#define MIN_LENGTH 3    // 用户名、密码最小长度

// 1 通用链表
// 通用链表节点结构体
typedef struct ListNode {
    // void*，泛型指针。使用时我们会直接强转为对应表的结构体类型
    // 链表里存储的信息实际上是结构体的指针
    void *data;
    struct ListNode *next;
} ListNode;

// 通用链表结构体
// 我们需要创建多个链表，统一用List结构体来管理
// 结构体里包含链表长度，有利于遍历
typedef struct {
    ListNode *head;   // 链表头指针
    int size;         // 链表长度。一方面可以立刻知道链表长度，另一方面可以用来逻辑删除
} List;

// 初始化链表
List *listInit();

// 在链表尾部添加元素
void listAdd(List *list, void *data);

// 头插
void listAddHead(List *list, void *data);

// 2 用户表结构体
typedef struct User {
    char account[20];        // 用户名
    char password[20];       // 密码
    char phone_number[20];   // 电话号码
    int user_type;           // 用户类型，5类：0-普通用户，1-会员用户，2-企业用户，3-代理商用户，4-合作商家用户
    int receive_status;      // 收快递状态，0-无需取件，1-需要取件
    int send_status;         // 寄件状态，0-无寄件，1-未发出，2-已发出，3-已送达

    char friend[3][20];      // 好友 用于代取
    int delivery_leave;      // 包裹滞留状态，0-正常，1-滞留

    time_t time;             // 记录封号时长
    int try_times;           // 记录输入次数

    char message[200];       // 消息
    int message_status;      // 消息状态，0-已读，1-未读
} User;

// 3 管理员表结构体
typedef struct Admin {
    char account[20];    // 用户名
    char password[20];   // 密码

    time_t time;         // 记录封号时长
    int try_times;       // 记录输入次数
} Admin;

// 4 包裹表
typedef struct Package {
    char package_id[20];         // 快递单号
    char receiver_account[20];   // 收件人用户名
    char courier_account[20];    // 快递员用户名
    int pick_up_code;            // 取件码，需要是整形

    int isExpress;               // 是否加急，0-普通件，1-加急件
    double volume;               // 体积（m³）
    double weight;               // 重量（kg）
    int special_type;            // 特殊类型，0-普通包裹，1-易碎品、电子产品，2-生鲜
    double value;                // 价值（元）

    char sender_account[20];     // 寄件人用户名，寄件时需要填写
    char shelf_id[20];           // 货架ID，寄件时需要填写

    time_t time;
    int rejected;                // 是否拒收，0-未拒收，1-已拒收
    char remark[200];            // 备注信息，用户可以填写拒收原因
} Package;

// 5 快递员表
// 快递员既要去平台处取包裹，又要去驿站处取用户寄的包裹
typedef struct Courier {
    char account[20];    // 用户名
    char password[20];   // 密码
    int status;          // 快递员状态，0-空闲，1-平台任务中，2-驿站任务中，3-隐身状态

    time_t time;         // 记录封号时刻
    int try_times;       // 记录输入次数
} Courier;

// 6 平台表
typedef struct Platform {
    char account[20];    // 平台用户名
    char password[20];   // 密码

    time_t time;         // 记录封号时刻
    int try_times;       // 记录输入次数
} Platform;

// 7 反馈表
typedef struct Feedback {
    char account[20];    // 用户名
    char content[200];   // 反馈内容
} Feedback;

// 传过来一个文件和一个链表，将文件中的数据读取到链表中
void writeListFromFile(const char *file, List *list);

// 根据链表内容，重写文件
void writeFileFromList(const char *file, List *list);

// 取得链表中指定的元素，便于后续修改
// 只通过用户名来获取对应结构体，因为用户名是唯一的
User *userElementGet(List *list, const char *account);
Courier *courierElementGet(List *list, const char *account);
Package *packageElementGet(List *list, const char *package_id);
Package *packageElementGetByCourier(List *list, const char *receiver_account);
Admin *adminElementGet(List *list, const char *account);
Platform *platformElementGet(List *list, const char *account);

// 清空输入缓冲区
int clearInputBuffer();

// 打印通用信息
void printCommonInfo();

// 删除函数
// 对于只知道用户名的情况，可以先调用getelement函数，其返回值作为data写在参数二的位置
void listRemove(List *list, void *data);

// 释放链表内存
void listFreePackage(List *list);
void listFreeUser(List *list);
void listFreeCourier(List *list);
void listFreeAdmin(List *list);
void listFreePlatform(List *list);
void listFree(List *list);                          // 只释放链表本身
void listFreeFeedback(List *list);

struct tm *getTime();                               // 获取当前时间

int checkInputLimit(const char *account);           // 判断当前输入的账号是否超过上限

int checkExit(const char *input);                   // 判断当前输入是否为退出

void getMoney(const char *file, double *money);     // 读取账户余额

void updateMoney(const char *file, double money);   // 更新账户余额
