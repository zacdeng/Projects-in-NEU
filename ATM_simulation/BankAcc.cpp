//
// Created by 20141 on 2020/5/30.
//

#include "BankAcc.h"
#include <ctime>
#include <fstream>
#include <sstream>
#include <string.h>

//获取当前操作时间
time_t now = time(0);
char *t = ctime(&now);

void SetString(string &a,string n){a = n;}
void SetNum(long &b, long n){b = n;}

//初始化账户信息
BankAcc::BankAcc(Account *acc) {
    if(t[strlen(t)-1] == '\n') t[strlen(t)-1] = '\0';
    Acc.password = acc->password;
    Acc.money = acc->money;
    Acc.address = acc->address;
    Acc.name = acc->name;
    Acc.number = acc->number;

    AccD.number = acc->number;
    AccD.date = t;
    AccD.type = "NULL";
    AccD.amount = 0;
    AccD.teller = "NULL";

}

BankAcc::~BankAcc() {}

//取款函数
void BankAcc::Withdraw() {
    // ctime函数返回的字符串末尾自带换行符'\n'，这会严重扰乱csv文件的正常读取，这里将'\n'换成'\0'，以下相同
    if(t[strlen(t)-1] == '\n') t[strlen(t)-1] = '\0';

    cout<<"--------------------------------------------"<<endl;
    cout<<"| Withdraw Page :)                         |"<<endl;
    cout<<"|                                          |"<<endl;
    cout<<"|           credit:________                |"<<endl;
    cout<<"|   (credit must be in multiples of 100)   |"<<endl;
    cout<<"|                                          |"<<endl;
    cout<<"--------------------------------------------"<<endl;

    //循环——限制存取款金额必须大于100元且必须为100的整数倍
    while(cin>>AccD.amount && (AccD.amount!=int(AccD.amount) || AccD.amount%100!=0 || AccD.amount>Acc.money) ){
        if(AccD.amount!=int(AccD.amount) || AccD.amount%100!=0)
            cout<<"credit must be in multiples of 100! Pls try again."<<endl;
        if(AccD.amount>Acc.money)
            cout<<"The available credit is not enough. Pls try again."<<endl;
    }
    Acc.money -= AccD.amount;

    //以下两个string结构为自动补充空格（为了完整显示操作界面）
    string Withdraw_money(16-to_string(AccD.amount).length(),' ');
    string Acc_money(16-to_string(Acc.money).length(),' ');
    cout<<"---------------------------------------"<<endl;
    cout<<"| Withdraw Page :)                    |"<<endl;
    cout<<"|                                     |"<<endl;
    cout<<"|     withdraw credit:" << AccD.amount << Withdraw_money << "|" <<endl;
    cout<<"|    Available credit:" << Acc.money << Acc_money << "|" <<endl;
    cout<<"|                                     |"<<endl;
    cout<<"---------------------------------------"<<endl;

    //更新取款信息
    UpdateAccD(Acc.number,AccD.amount,"Withdraw");

    //实时更改账户信息中的总金额
    ChangeTotalMoney(Acc.number,-AccD.amount);
}

//存款函数
void BankAcc::Deposit() {
    if(t[strlen(t)-1] == '\n') t[strlen(t)-1] = '\0';

    cout<<"--------------------------------------------"<<endl;
    cout<<"| Deposit Page :)                          |"<<endl;
    cout<<"|                                          |"<<endl;
    cout<<"|           credit:________                |"<<endl;
    cout<<"|   (credit must be in multiples of 100)   |"<<endl;
    cout<<"|                                          |"<<endl;
    cout<<"--------------------------------------------"<<endl;
    cin>>AccD.amount;

    //循环——限制存取款金额必须大于100元且必须为100的整数倍
    while(AccD.amount!=int(AccD.amount) || AccD.amount%100!=0){
        if(AccD.amount!=int(AccD.amount) || AccD.amount%100!=0){
            cout<<"credit must be in multiples of 100! Pls try again."<<endl;
            cin>>AccD.amount;
        }
    }
    Acc.money += AccD.amount;

    //以下两个string结构为自动补充空格（为了完整显示操作界面）
    string Deposit_money(16-to_string(AccD.amount).length(),' ');
    string Acc_money(16-to_string(Acc.money).length(),' ');
    cout<<"---------------------------------------"<<endl;
    cout<<"| Deposit Page :)                     |"<<endl;
    cout<<"|                                     |"<<endl;
    cout<<"|      Deposit credit:" << AccD.amount << Deposit_money << "|" <<endl;
    cout<<"|    Available credit:" << Acc.money << Acc_money << "|" <<endl;
    cout<<"|                                     |"<<endl;
    cout<<"---------------------------------------"<<endl;

    //更新存款信息
    UpdateAccD(Acc.number,AccD.amount,"Deposit");

    //实时更改总金额
    ChangeTotalMoney(Acc.number,AccD.amount);
}

//查询存取款信息函数
bool BankAcc::Detail() {
    string line, temp, year, month, day, type;
    istringstream line_stream;
    string filename = Acc.number;
    char tmp;

    //提供两个选项：（a）展示全部 （b）按日期查询
    cout<<"----------------------------------------"<<endl;
    cout<<"| Transaction Detail Page :)           |"<<endl;
    cout<<"|                                      |"<<endl;
    cout<<"|             (a)show all              |"<<endl;
    cout<<"|          (b)enquiry by time          |"<<endl;
    cout<<"|       (q)Back to superior menu       |"<<endl;
    cout<<"|                                      |"<<endl;
    cout<<"----------------------------------------"<<endl;
    while(cin>>tmp){
        switch(tmp){
            case 'q':
                return false;
            case 'A':
            case 'a':
                {
                    //展示全部存取款记录
                    cout<<"--------------------------------------------------------------------------------------"<<endl;
                    cout<<"| Transaction Detail Page :)                                                         |"<<endl;
                    cout<<"|                                                                                    |"<<endl;

                    ifstream icsv(filename+".csv", ios::in);
                    if(!icsv){
                        cout<<"Can't not load the acc.csv file! Pls check it in your computer!"<<endl;
                        return false;
                    }
                    while(getline(icsv,line)){
                        string blank(80-line.length(),' ');
                        cout << "|    ";
                        cout << line;
                        cout<< blank << "|" <<endl;
                    }
                    icsv.close();

                    cout<<"|                                                                                    |"<<endl;
                    cout<<"--------------------------------------------------------------------------------------"<<endl;
                    break;
                }
            case 'B':
            case 'b':
                {
                    //按日期查询（这里需要用到友元函数将查询结果赋给AccD）
                    cout<<"--------------------------------------------"<<endl;
                    cout<<"| Transaction Detail Page :)               |"<<endl;
                    cout<<"|                                          |"<<endl;
                    cout<<"|             year:_________               |"<<endl;
                    cout<<"|            month:_________               |"<<endl;
                    cout<<"|              day:_________               |"<<endl;
                    cout<<"|             type:_________               |"<<endl;
                    cout<<"|                                          |"<<endl;
                    cout<<"|      ******************************      |"<<endl;
                    cout<<"|      * input like   2020 May 30 W *      |"<<endl;
                    cout<<"|      *      W means Withdraw      *      |"<<endl;
                    cout<<"|      *      D means Deposit       *      |"<<endl;
                    cout<<"|      *      T means Transfer      *      |"<<endl;
                    cout<<"|      ******************************      |"<<endl;
                    cout<<"--------------------------------------------"<<endl;
                    cin>>year;
                    cin>>month;
                    cin>>day;
                    cin>>type;
                    ifstream icsv(filename+".csv", ios::in);

                    //获取各信息的对应值
                    while(getline(icsv,line)){
                        line_stream.str(line);

                        getline(line_stream,temp,',');
                        SetString(AccD.number,temp);

                        getline(line_stream,temp,',');
                        if( temp.find(year)!=string::npos && temp.find(month)!=string::npos
                            && temp.find(day)!=string::npos )
                            SetString(AccD.date,temp);
                        else
                            continue;

                        getline(line_stream,temp,',');
                        if( temp.find(type)!=string::npos )
                            SetString(AccD.type,temp);
                        else
                            continue;

                        getline(line_stream,temp,',');
                        SetNum(AccD.amount,stol(temp));

                        getline(line_stream,temp);
                        SetString(AccD.teller,temp);
                    }

                    icsv.close();

                    //同理设置空白格
                    string AccD_num(35-AccD.number.length(),' ');
                    string AccD_date(35-AccD.date.length(),' ');
                    string AccD_type(35-AccD.type.length(),' ');
                    string AccD_amount(35-to_string(AccD.amount).length(),' ');
                    string AccD_teller(35-AccD.teller.length(),' ');
                    cout<<"-----------------------------------------------------------"<<endl;
                    cout<<"| Transaction Detail Page :)                              |"<<endl;
                    cout<<"|                                                         |"<<endl;
                    cout<<"|              card ID:"<< AccD.number << AccD_num << "|" <<endl;
                    cout<<"|                 Date:"<< AccD.date << AccD_date << "|" <<endl;
                    cout<<"|                 Type:"<< AccD.type << AccD_type << "|" <<endl;
                    cout<<"|   Transaction Credit:"<< AccD.amount << AccD_amount << "|" <<endl;
                    cout<<"|               Teller:"<< AccD.teller << AccD_teller << "|" <<endl;
                    cout<<"|                                                         |"<<endl;
                    cout<<"-----------------------------------------------------------"<<endl;
                }

        }
        cout<<"----------------------------------------"<<endl;
        cout<<"| Transaction Detail Page :)           |"<<endl;
        cout<<"|                                      |"<<endl;
        cout<<"|             (a)show all              |"<<endl;
        cout<<"|          (b)enquiry by time          |"<<endl;
        cout<<"|       (q)Back to superior menu       |"<<endl;
        cout<<"|                                      |"<<endl;
        cout<<"----------------------------------------"<<endl;
    }
    return true;
}

//查询户主信息函数
void BankAcc::Enquiry() {
    string Acc_num(25-Acc.number.length(),' ');
    string Acc_name(25-Acc.name.length(),' ');
    string Acc_add(25-Acc.address.length(),' ');
    string Acc_money(25-to_string(Acc.money).length(),' ');
    cout<<"------------------------------------------------"<<endl;
    cout<<"| Enquiry Page :)                              |"<<endl;
    cout<<"|                                              |"<<endl;
    cout<<"|             card ID:"<< Acc.number << Acc_num << "|" <<endl;
    cout<<"|                Name:"<< Acc.name << Acc_name << "|" <<endl;
    cout<<"|             Address:"<< Acc.address << Acc_add << "|" <<endl;
    cout<<"|              Credit:"<< Acc.money << Acc_money << "|" <<endl;
    cout<<"|                                              |"<<endl;
    cout<<"------------------------------------------------"<<endl;
}

//实时更改户主信息中的总金额
/*思路是创建一个临时csv文件，不断读取acc.csv行并拷贝至临时文件中，直至匹配到该账户，
  此时修改总金额并写入临时文件，剩下部分继续拷贝至临时文件*/
void BankAcc::ChangeTotalMoney(string num, long int money_change) {
    istringstream line_stream;
    string line, temp, number, name, password, address, money;

    //拷贝并修改acc.csv文件给临时文件
    ofstream otmp("MoneyTmpFile.csv", ios::app);
    ifstream icsv("acc.csv", ios::in);
    while(getline(icsv, line)){
        line_stream.str(line);
        getline(line_stream,temp,',');
        if(temp!=num){
            otmp<<line<<endl;
            continue;
        }
        else{
            number = temp;

            getline(line_stream,temp,',');
            name = temp;

            getline(line_stream,temp,',');
            password = temp;

            getline(line_stream,temp,',');
            address = temp;

            getline(line_stream,temp,',');
            money = temp;

            otmp<< number << ',' << name << ',' << password <<','
            << address << ',' << to_string(stol(money)+money_change) << endl;
        }
    }
    icsv.close();
    otmp.close();

    //将临时文件覆盖至acc.csv文件实现实时更新
    ifstream itmp("MoneyTmpFile.csv", ios::in);
    ofstream ocsv("acc.csv", ios::out);
    while(getline(itmp, line)){
        ocsv<<line<<endl;
    }
    ocsv.close();
    itmp.close();

    //销毁临时文件释放内存
    system("del MoneyTmpFile.csv");
}

void BankAcc::ModifyPasswd() {
    string passwd, passwd_a;
    cout<<"----------------------------------------------------"<<endl;
    cout<<"| Modify Password Page :)                          |"<<endl;
    cout<<"|                                                  |"<<endl;
    cout<<"|             New Password:____________            |"<<endl;
    cout<<"|       New Password Again:____________            |"<<endl;
    cout<<"|                                                  |"<<endl;
    cout<<"----------------------------------------------------"<<endl;
    while(cin>>passwd && cin>>passwd_a){
        if(passwd == passwd_a)
            break;
        else
            cout<<"The passwords you typed do not match. Type the same password in both text boxes."<<endl;
    }

    ChangePassword(passwd);
    cout<<"Successfully modify! It will take effect next time you log in."<<endl;
}

//修改密码函数（类似修改户主账户总金额，不展开描述）
void BankAcc::ChangePassword(string passwd) {
    istringstream line_stream;
    string line, temp, number, name, password, address, money;

    //拷贝并修改acc.csv文件给临时文件
    ofstream otmp("PasswdTmpFile.csv", ios::app);
    ifstream icsv("acc.csv", ios::in);
    while(getline(icsv, line)){
        line_stream.str(line);
        getline(line_stream,temp,',');
        if(temp!=Acc.number){
            otmp<<line<<endl;
            continue;
        }
        else{
            number = temp;

            getline(line_stream,temp,',');
            name = temp;

            getline(line_stream,temp,',');
            password = temp;

            getline(line_stream,temp,',');
            address = temp;

            getline(line_stream,temp,',');
            money = temp;

            otmp<< number << ',' << name << ',' << passwd <<','
                << address << ',' << money << endl;
        }
    }
    icsv.close();
    otmp.close();

    //将临时文件覆盖至acc.csv文件实现实时更新
    ifstream itmp("PasswdTmpFile.csv", ios::in);
    ofstream ocsv("acc.csv", ios::out);
    while(getline(itmp, line)){
        ocsv<<line<<endl;
    }
    ocsv.close();
    itmp.close();

    //销毁临时文件释放内存
    system("del PasswdTmpFile.csv");
}

//转账函数
void BankAcc::Transfer() {
    string acc_num, line, temp;
    long int credit;
    string Acc_crd(24-to_string(Acc.money).length(),' ');

    cout<<"----------------------------------------------------"<<endl;
    cout<<"| Transfer Page :)                                 |"<<endl;
    cout<<"|                                                  |"<<endl;
    cout<<"|         Available Credit:"<< Acc.money << Acc_crd << "|" <<endl;
    cout<<"|           Target Account:____________            |"<<endl;
    cout<<"|          Transfer Credit:____________            |"<<endl;
    cout<<"|                                                  |"<<endl;
    cout<<"----------------------------------------------------"<<endl;
    cin>>acc_num;

    //检查账户是否存在
    while(!CheckAccount(acc_num))
        cin>>acc_num;

    //检查金额是否合理
    while(cin>>credit){
        if(credit <= Acc.money)
            break;
        else{
            cout<<"The available credit is not enough! Pls input the suitable money."<<endl;
            continue;
        }
    }

    Acc.money -= credit;
    ChangeTotalMoney(Acc.number,-credit);
    ChangeTotalMoney(acc_num,credit);

    cout<<"----------------------------------------------------"<<endl;
    cout<<"| Transfer Page :)                                 |"<<endl;
    cout<<"|                                                  |"<<endl;
    cout<<"|          Successfully Transfer! :)               |"<<endl;
    cout<<"|                                                  |"<<endl;
    cout<<"|         Available Credit:"<< Acc.money << Acc_crd << "|" <<endl;
    cout<<"|                                                  |"<<endl;
    cout<<"----------------------------------------------------"<<endl;

    //更新转出款信息
    UpdateAccD(Acc.number,credit,"Transfer out");

    //更新转入款信息
    UpdateAccD(acc_num,credit,"Transfer in");
}

//检查账户函数
bool BankAcc::CheckAccount(string acc_num) {
    istringstream line_stream;
    string line, temp;
    int flag = 0;

    //检查转账目标是否存在,如果找到目标账户则flag=1，否则为0
    ifstream icsv("acc.csv", ios::in);
    while(getline(icsv, line)){
        line_stream.str(line);
        getline(line_stream,temp,',');
        if(temp!=acc_num){
            continue;
        }
        else{
            flag = 1;
        }
    }

    icsv.close();

    if(flag == 0){
        cout<<"The Target Accoutn is not exist! Pls try again."<<endl;
        return false;
    }
    else
        return true;
}

//更新存取款、转账信息
void BankAcc::UpdateAccD(string card_num, long int money_num, string type) {

    //第一次存取款、转账时创建对应csv文件存储账户存取款信息
    string filename = card_num;
    ifstream icsv(filename+".csv", ios::in);
    if(!icsv){
        ofstream ocsv(filename+".csv", ios::out);
        //ocsv<< "编号" << ',' << "日期" << ',' << "类型" << ','
        //    << "存取金额" << ',' << "经办人" << endl;
        // 编码器不支持中文，输出csv会乱码，这里用英文代替

        ocsv<< "Number" << ',' << "Date" << ',' << "Type" << ','
            << "Transaction Credit" << ',' << "Teller" << endl;
        ocsv.close();
    }
    icsv.close();

    //将存取款、转账信息存入对应csv文件
    ofstream ocsv(filename+".csv", ios::app);
    ocsv<< card_num << ',' << t << ',' << type
        << ',' << money_num << ',' << "System Automation" <<endl;
    ocsv.close();
}