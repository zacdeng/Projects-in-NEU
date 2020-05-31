//
//Created by RSE-1702-Zacdeng-20173878
//ATM 模拟
//
#include <iostream>
#include "BankAcc.h"

using namespace std;

int main(){
    Account acc;
    char option, option2;
    int flag = 0;

    //登录、创建银行账户界面
    while((option = Welcome())!='q'){
        switch (option){
            case 'B':
            case 'b':
                create_acc();
                break;
            case 'A':
            case 'a':
                flag = 1;
                while(!load_acc(acc)); //无法正确登录则循环直至账户密码都正确
        }
        if(flag==1) break;
    }

    BankAcc bank_acc(&acc);

    //账户操作界面
    if(option!='q'){
        while((option2 = LoginWelcome())!='q'){
            switch (option2){
                case 'A':
                case 'a':
                    bank_acc.Deposit();
                    break;
                case 'B':
                case 'b':
                    bank_acc.Withdraw();
                    break;
                case 'C':
                case 'c':
                    bank_acc.Enquiry();
                    break;
                case 'D':
                case 'd':
                    while(bank_acc.Detail());
                    break;
                case 'E':
                case 'e':
                    bank_acc.ModifyPasswd();
                    break;
                case 'F':
                case 'f':
                    bank_acc.Transfer();
            }
        }
        if(option2 = 'q') QuitUI();
    }
    else{
        QuitUI();
    }

    return 0;
}

