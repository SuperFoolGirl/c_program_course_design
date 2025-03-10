#pragma once
#include "common.h"
#include "data_storage.h"

extern List *shelf_a_list;
extern List *shelf_b_list;
extern List *shelf_c_list;
extern List *shelf_d_list;
extern List *shelf_e_list;
extern List *users_send_list;
extern List *users_push_list;

void userShowMenu();

void userPickup();

void userSend();

void userQueryPickup();

void userQuerySend();

void userFeedback();

void userPay(int isExpress);

void userModifySend();

void userCancelSend();