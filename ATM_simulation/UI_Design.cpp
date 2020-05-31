//
// Created by 20141 on 2020/5/30.
//

#include "UI_Design.h"
#include <fstream>
#include <sstream>
char Welcome(){
    char ch;
    cout<<"--------------------------------------------"<<endl;
    cout<<"|                                          |"<<endl;
    cout<<"|         Welcome to ATM system! :)        |"<<endl;
    cout<<"|                                          |"<<endl;
    cout<<"|                (a)log in                 |"<<endl;
    cout<<"|          (b)create a new account         |"<<endl;
    cout<<"|                (q)quit                   |"<<endl;
    cout<<"|                                          |"<<endl;
    cout<<"|                                          |"<<endl;
    cout<<"|                       DEV:Zacdeng (demo) |"<<endl;
    cout<<"--------------------------------------------"<<endl;
    while(cin>>ch && ch!='a' && ch!='A' && ch!='B' && ch!='b' && ch!='q' && ch!='Q'){
        cout<<"The function isn't defined! Pls choose the right function."<<endl;
    }
    return ch;
}

bool load_acc(Account &acc){
    string line, temp, acc_num, password; //创建string类型变量，line用作读取csv文件行存储，temp用作csv文件列内容存储
    istringstream line_stream;  //创建字符串读取流line_stream

    cout<<"---------------------------------------"<<endl;
    cout<<"| Login Page :)                       |"<<endl;
    cout<<"|                                     |"<<endl;
    cout<<"|      account number:________        |"<<endl;
    cout<<"|            password:________        |"<<endl;
    cout<<"|                                     |"<<endl;
    cout<<"---------------------------------------"<<endl;
    cin>>acc_num;
    cin>>password;

    //如果没有账户文件，则报错
    ifstream icsv("acc.csv", ios::in);
    if(!icsv){
        cout<<"Can't not load the acc.csv file! Pls check it in your computer!"<<endl;
        return false;
    }

    //读取对应账户名的各项信息，做本次操作的账户使用
    while(getline(icsv,line)){
        line_stream.str(line);  //将line流化

        //如果账号不存在，则一直读取直到读取至相应账号为止
        getline(line_stream,temp,',');
        if(temp!=acc_num) continue;
        acc.number = temp;

        getline(line_stream,temp,',');
        acc.name = temp;

        //检查密码是否正确，不正确则无法登录获取信息
        getline(line_stream,temp,',');
        if(password!=temp) {
            cout<<"Find your account but the password is wrong. Try again!."<<endl;
            return false;
        }
        acc.password = temp;

        getline(line_stream,temp,',');
        acc.address = temp;

        getline(line_stream,temp);
        acc.money = stoi(temp);
    }
    icsv.close();
    return true;
}

void create_acc(){
    int i = 0;
    string line, name, password, address;

    //第一次操作系统则创建对应csv文件存储账户信息
    ifstream icsv("acc.csv", ios::in);
    if(!icsv){
        ofstream ocsv("acc.csv", ios::app);
        //ocsv<< "编号" << ',' << "姓名" << ',' << "支取密码" << ','
        //    << "客户地址" << ',' << "客户账户总金额" << endl;
        // 编码器不支持中文，输出csv会乱码，这里用英文代替

        ocsv<< "Number" << ',' << "Name" << ',' << "Password" << ','
            << "Address" << ',' << "Total Credit" << endl;
        i++;
        ocsv.close();
    }
    while(getline(icsv,line)) i++; //统计行数，用作账户初始化的登录名 2020+i
    icsv.close();

    ofstream ocsv("acc.csv", ios::app);
    cout<<"-------------------------------------"<<endl;
    cout<<"| CreateAcc Page :)                 |"<<endl;
    cout<<"|                                   |"<<endl;
    cout<<"|          Name:___________         |"<<endl;
    cout<<"|      Password:___________         |"<<endl;
    cout<<"|       Address:___________         |"<<endl;
    cout<<"|                                   |"<<endl;
    cout<<"-------------------------------------"<<endl;
    cin>>name;
    cin>>password;
    cin>>address;
    ocsv<< "2020"+to_string(i) << ',' << name << ',' << password << ','
        << address << ',' << to_string(0) <<endl;
    cout<<"You have create an account!"<<endl;
    cout<<"Your account number is 2020" << to_string(i) << endl << "And your password is "
        << password << endl << "Now you can log in."<<endl;
    ocsv.close();
}

char LoginWelcome(){
    //登录后功能选项
    char tmp;
    cout<<"-------------------------------------------------"<<endl;
    cout<<"| ATM Service :)                                |"<<endl;
    cout<<"|                                               |"<<endl;
    cout<<"|            Welcome to ATM system!             |"<<endl;
    cout<<"|                                               |"<<endl;
    cout<<"|               (a)Deposit                      |"<<endl;
    cout<<"|               (b)Withdraw                     |"<<endl;
    cout<<"|               (c)Account Enquiry              |"<<endl;
    cout<<"|               (d)Transaction Detail           |"<<endl;
    cout<<"|               (e)Modify Password              |"<<endl;
    cout<<"|               (f)Transfer                     |"<<endl;
    cout<<"|               (q)Quit                         |"<<endl;
    cout<<"|                                               |"<<endl;
    cout<<"-------------------------------------------------"<<endl;
    cin>>tmp;
    return tmp;
}

void QuitUI(){
    cout<<"-------------------------------------------------"<<endl;
    cout<<"| Quit ATM Service                              |"<<endl;
    cout<<"|                                               |"<<endl;
    cout<<"|            Thank you for using!               |"<<endl;
    cout<<"|              Have a good day!                 |"<<endl;
    cout<<"|                                               |"<<endl;
    cout<<"|                     :)                        |"<<endl;
    cout<<"|                                               |"<<endl;
    cout<<"-------------------------------------------------"<<endl;
}