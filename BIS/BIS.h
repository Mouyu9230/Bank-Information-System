#ifndef BIS_H
#define BIS_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef struct user//账户结构体
{
    char name[100];//姓名
    long long int IDnumber;//身份证号
    long long int account;//账号
    double deposit;//存款
    char password[100];//密码

}user;


void Add_user();

void Delete_user();

void Check_information();

void Deposit();

void Withdrawal();

void Deposit_transfer();

void loading_bar(void);

void Load_users_from_file();

void Save_users_to_file();

#endif
