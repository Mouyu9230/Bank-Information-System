#include "BIS.h"
#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(ms) Sleep(ms)          // Windows：毫秒
#else
    #include <unistd.h>
    #define SLEEP(ms) usleep((ms)*1000)  // Linux/macOS：毫秒转微秒
#endif
    user users[2000];
    int usercount=3;
    char name[100];
    long long int IDnumber;
    long long int account;
    double deposit;
    char password[100];
int main(){
loading_bar();

    int userchoice=0;
    users[0]=(user){"Mouyu",13193918,2313228276,122500.6,"PASSWORD"};
    users[1]=(user){"Couvor",132345234,6868668,999999,"PASSWORD"};
    users[2]=(user){"zsh",123131233,123456,1000,"PASSWORD"};


    printf("=============银行信息管理系统==================\n");
    printf("银行信息管理系统\n");
    printf("输入数字选择对应服务\n");
    printf("1:存款\n2:取款\n3:转账\n4:查看账户信息\n5:创建账户\n6:注销账户\n0:退出\n");
    printf("========================================================\n");    
        while(1){
     if(scanf("%d",&userchoice)!=1) {
        printf("输入格式错误！请重新输入\n");
        int sweeper;
        while((sweeper=getchar())!='\n'&&sweeper!=EOF);
        continue;
    }
        switch(userchoice){
            case 1: Deposit();break;
            case 2: Withdrawal();break;
            case 3: Deposit_transfer();break;
            case 4: Check_information();break;
            case 5: Add_user();break;
            case 6: Delete_user();break;
            case 0: printf("欢迎再次使用");return 0;
            default:printf("wrong input!");
        };
    }
    return 0;
}




//写以下部分的内容
//----------------------------------------------------------------------------------------

void Add_user() //注册账户 陈华宇
{
    FILE *fp;
    user new_user, temp;
    int found = 0;
    while(getchar()!='\n');

    fp = fopen("user.dat", "ab+");
    if (fp == NULL)
    {
        printf("文件打开失败！\n");
        return;
    }

    printf("请输入姓名：");
    scanf("%s", new_user.name);

    printf("请输入身份证号：");
    scanf("%lld", &new_user.IDnumber);

    printf("请输入密码：");
    scanf("%s", new_user.password);

    printf("请输入初始存款：");
    scanf("%lf", &new_user.deposit);

    rewind(fp);
    while (fread(&temp, sizeof(user), 1, fp))
    {
        if (temp.IDnumber == new_user.IDnumber)
        {
            found = 1;
            break;
        }
    }

    if (found)
    {
        printf("该身份证已注册，注册失败！\n");
        fclose(fp);
        return;
    }

    new_user.account = new_user.IDnumber % 1000000;

    fwrite(&new_user, sizeof(user), 1, fp);

    // 关键补充：将新用户加入users数组
    if (usercount < 2000) {
        users[usercount] = new_user;
        usercount++; // 更新用户计数
    } else {
        printf("用户数量已达上限！\n");
    }

    printf("注册成功！\n");
    printf("您的账号是：%lld\n", new_user.account);

    fclose(fp);
}


void Delete_user(){//注销账户  郁博翔
    int found = -1;
    long long del_account;
    char del_pwd[100];

    printf("请输入要注销的账号和密码（格式：账号 密码）：\n");
    if (scanf("%lld %s", &del_account, del_pwd) != 2) {
        printf("输入格式错误！请重新输入\n");
        int sweeper;
        while ((sweeper = getchar()) != '\n' && sweeper != EOF);
        return;
    }

    // 查找用户
    for (int i = 0; i < usercount; i++) {
        if (users[i].account == del_account &&
            strcmp(users[i].password, del_pwd) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("账号或密码错误，注销失败！\n");
        return;
    }

    // 覆盖删除：后面的用户前移
    for (int i = found; i < usercount - 1; i++) {
        users[i] = users[i + 1];
    }
    usercount--;

    // 同步写回文件
    FILE *fp = fopen("user.dat", "wb");
    if (fp == NULL) {
        printf("文件写入失败！但内存中已删除该账户\n");
        return;
    }
    for (int i = 0; i < usercount; i++) {
        fwrite(&users[i], sizeof(user), 1, fp);
    }
    fclose(fp);

    printf("账户注销成功！\n");
}

void Check_information(){//查看账户信息 陈镜宇
    int checkflag=0;
    printf("请输入账号和密码\n");
    if(scanf("%lld %s",&account,password)!=2){//推荐大家处理输入问题时增加这样的一个检测格式是否正确和清除输入缓冲区的模块，如果输入的格式不符合要求，不仅会scanf失败，而且错误的信息会留在缓冲区里，我就犯了这么个错整出来了个死循环
         printf("输入格式错误！请重新输入\n");
         int sweeper;
         while((sweeper=getchar())!='\n'&&sweeper!=EOF);
         return;
}
    for(int i=0;i<usercount;i++){
        if(users[i].account==account&&strcmp(password,users[i].password)==0){
            printf("验证通过！以下是您账户的信息\n");
            printf("姓名:%s\n身份证号:%lld\n账号:%lld\n存款:%lf\n",users[i].name,users[i].IDnumber,users[i].account,users[i].deposit);
            checkflag=1;
        }
    }
    if(checkflag==0){
        printf("输入错误！\n");
    }
}

void Deposit(){//存款
        int i, found = -1;
    double money;

    printf("请输入账号和密码(示例: 123456 密码)：\n");
    if (scanf("%lld %s", &account, password) != 2) {
        printf("输入格式错误！请重新输入\n");
        int sweeper;
        while ((sweeper = getchar()) != '\n' && sweeper != EOF);
        return;
    }

    // 查找用户
    for (i = 0; i < usercount; i++) {
        if (users[i].account == account &&
            strcmp(password, users[i].password) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("账号或密码错误！\n");
        return;
    }

    printf("请输入存款金额：\n");
    if (scanf("%lf", &money) != 1) {
        printf("输入格式错误！请重新输入\n");
        int sweeper;
        while ((sweeper = getchar()) != '\n' && sweeper != EOF);
        return;
    }

    if (money <= 0) {
        printf("存款金额必须大于 0！\n");
        return;
    }

    users[found].deposit += money;
    printf("存款成功！\n");
    printf("当前账户余额为：%.2lf 元\n", users[found].deposit);

}

void Withdrawal(){int i, found = -1;
    double money;

    printf("请输入账号和密码(示例: 123456 密码)：\n");
    if (scanf("%lld %s", &account, password) != 2) {
        printf("输入格式错误！请重新输入\n");
        int sweeper;
        while ((sweeper = getchar()) != '\n' && sweeper != EOF);
        return;
    }

    // 查找用户
    for (i = 0; i < usercount; i++) {
        if (users[i].account == account &&
            strcmp(password, users[i].password) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("账号或密码错误！\n");
        return;
    }

    printf("请输入取款金额：\n");
    if (scanf("%lf", &money) != 1) {
        printf("输入格式错误！请重新输入\n");
        int sweeper;
        while ((sweeper = getchar()) != '\n' && sweeper != EOF);
        return;
    }

    if (money <= 0) {
        printf("取款金额必须大于 0！\n");
        return;
    }

    if (money > users[found].deposit) {
        printf("余额不足！当前余额为：%.2lf 元\n", users[found].deposit);
        return;
    }

    users[found].deposit -= money;
    printf("取款成功！\n");
    printf("当前账户余额为：%.2lf 元\n", users[found].deposit);
}

void Deposit_transfer(){//转账  代宸旭
    int from_idx = -1, to_idx = -1;
    long long from_account, to_account;
    char from_pwd[100];
    double transfer_money;

    printf("===== 转账功能 =====\n");
    printf("请输入转出账号和密码（格式：账号 密码）：\n");
    if (scanf("%lld %s", &from_account, from_pwd) != 2) {
        printf("❌ 输入格式错误！请输入“数字账号+空格+密码”的格式\n");
        int sweeper;
        while ((sweeper = getchar()) != '\n' && sweeper != EOF);
        return;
    }

    for (int i = 0; i < usercount; i++) {
        if (users[i].account == from_account && strcmp(from_pwd, users[i].password) == 0) {
            from_idx = i;
            break;
        }
    }
    if (from_idx == -1) {
        printf("❌ 转出账号不存在或密码错误！\n");
        return;
    }

    printf("请输入转入账号：\n");
    if (scanf("%lld", &to_account) != 1) {
        printf("❌ 转入账号格式错误！请输入纯数字账号\n");
        int sweeper;
        while ((sweeper = getchar()) != '\n' && sweeper != EOF);
        return;
    }

    for (int i = 0; i < usercount; i++) {
        if (users[i].account == to_account) {
            to_idx = i;
            break;
        }
    }
    if (to_idx == -1) {
        printf("❌ 转入账号不存在！\n");
        return;
    }

    printf("请输入转账金额：\n");
    if (scanf("%lf", &transfer_money) != 1) {
        printf("❌ 金额格式错误！请输入数字（如 100.50）\n");
        int sweeper;
        while ((sweeper = getchar()) != '\n' && sweeper != EOF);
        return;
    }
    if (transfer_money <= 0) {
        printf("❌ 转账金额必须大于0！\n");
        return;
    }
    if (transfer_money > users[from_idx].deposit) {
        printf("❌ 余额不足！当前转出账户余额：%.2lf 元\n", users[from_idx].deposit);
        return;
    }

    users[from_idx].deposit -= transfer_money;
    users[to_idx].deposit += transfer_money;

    FILE *fp = fopen("user.dat", "wb");
    if (fp != NULL) {
        for (int i = 0; i < usercount; i++) {
            fwrite(&users[i], sizeof(user), 1, fp);
        }
        fclose(fp);
    }

    printf("✅ 转账成功！\n");
    printf("👉 转出账户剩余余额：%.2lf 元\n", users[from_idx].deposit);
    printf("👉 转入账户当前余额：%.2lf 元\n", users[to_idx].deposit);
}

void loading_bar(void) {//加载进度条函数
    const int width = 30;  // 进度条长度

    printf("正在处理，请稍候...\n");

    for (int i = 0; i <= width; i++) {
        int percent = i * 100 / width;

        printf("\r[");
        for (int j = 0; j < i; j++) {
            printf("#");
        }
        for (int j = i; j < width; j++) {
            printf(" ");
        }
        printf("] %3d%%", percent);

        fflush(stdout);        SLEEP(50); // 越大越慢
    }
    printf("\n处理完成！\n");
}
//----------------------------------------------------------------------------------------
