//
// Created by 20141 on 2020/5/30.
//

#ifndef BANK_SIMULATE_BANKACC_H
#define BANK_SIMULATE_BANKACC_H

#include "UI_Design.h"

class BankAcc{
private:
    struct BankAccount{
        string number, name, password, address;
        long int money;
    };

    struct AccDetail{
        string number, date, type, teller;
        long int amount;
    };

    BankAccount Acc;    //创建账户信息结构
    AccDetail AccD;     //创建账户存取款信息结构
    void ChangeTotalMoney(string, long int);    //实时更改存取款后账户信息中总金额的函数
    void ChangePassword(string);        //更改密码
    bool CheckAccount(string);                //检查账号是否存在
    void UpdateAccD(string, long int, string);
public:
    BankAcc(Account *);
    ~BankAcc();

    friend void SetString(string &a,string n);  //修改string 类型的AccD数据（供按日期查询记录使用）
    friend void SetNum(long &b, long n);        //修改long int 类型的AccD数据（供按日期查询记录使用）
    void Deposit();     //存款
    void Withdraw();    //取款
    void Enquiry();     //查询户主信息
    bool Detail();      //查询存取款记录信息
    void ModifyPasswd();//更改密码
    void Transfer();    //转账
};

#endif //BANK_SIMULATE_BANKACC_H