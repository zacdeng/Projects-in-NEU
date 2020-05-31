//
// Created by 20141 on 2020/5/30.
//

#ifndef BANK_SIMULATE_UI_DESIGN_H
#define BANK_SIMULATE_UI_DESIGN_H
#include <iostream>
using namespace std;

//创建客户账户基本信息
struct Account{
    string number, name, password, address;
    long int money;
};

char Welcome();               //初始界面
bool load_acc(Account &acc);  //登录账户
void create_acc();            //创建账户
char LoginWelcome();          //登录后选项操作界面
void QuitUI();                //退出系统界面
#endif //BANK_SIMULATE_UI_DESIGN_H
