#include "BIS.h"
#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(ms) Sleep(ms)          // Windows：毫秒
#else
    #include <unistd.h>
    #define SLEEP(ms) usleep((ms)*1000)  // Linux/macOS：毫秒转微秒
#endif

// 补充user结构体定义（BIS.h中应包含此定义，这里兜底）
typedef struct {
    char name[100];
    long long int IDnumber;
    long long int account;
    double deposit;
    char password[100];
} user;

// 全局变量
user users[2000];
int usercount = 3;
char name[100];
long long int IDnumber;
long long int account;
double deposit;
char password[100];

// 函数声明
void loading_bar(void);
void Add_user();
void Delete_user();
void Check_information();
void Deposit();
void Withdrawal();
void Deposit_transfer();

int main(){
    loading_bar();

    int userchoice = 0;
    // 初始化测试用户
    users[0] = (user){"Mouyu", 13193918, 2313228276, 122500.6, "PASSWORD"};
    users[1] = (user){"Couvor", 132345234, 6868668, 999999, "PASSWORD"};
    users[2] = (user){"Bydzsh", 123131233, 123456, 114514.19, "PASSWORD"};

    printf("=============ydy农村信用社财产管理系统==================\n");
    printf("银行信息管理系统\n");
    printf("输入数字选择对应服务\n");
    printf("1:存款\n2:取款\n3:转账\n4:查看账户信息\n5:创建账户\n6:注销账户\n7:为ydy歌功颂德\n0:退出\n");
    printf("========================================================\n");    
    while(1) {
        if (scanf("%d", &userchoice) != 1) {
            printf("输入格式错误！请重新输入\n");
            int sweeper;
            while ((sweeper = getchar()) != '\n' && sweeper != EOF);
            continue;
        }
        switch(userchoice) {
            case 1: Deposit(); break;
            case 2: Withdrawal(); break;
            case 3: Deposit_transfer(); break;
            case 4: Check_information(); break;
            case 5: Add_user(); break;
            case 6: Delete_user(); break;
            case 7: printf("ydy是最棒的！\n"); break;
            case 0: printf("欢迎再次使用\n"); return 0;
            default: printf("输入错误！请输入0-7之间的数字\n");
        }
    }
    return 0;
}

// 注册账户
void Add_user() {
    FILE *fp;
    user new_user, temp;
    int found = 0;
    while (getchar() != '\n'); // 清空输入缓冲区

    fp = fopen("user.dat", "ab+");
    if (fp == NULL) {
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

    // 检查身份证是否已注册
    rewind(fp);
    while (fread(&temp, sizeof(user), 1, fp)) {
        if (temp.IDnumber == new_user.IDnumber) {
            found = 1;
            break;
        }
    }

    if (found) {
        printf("该身份证已注册，注册失败！\n");
        fclose(fp);
        return;
    }

    // 生成账号（身份证后6位）
    new_user.account = new_user.IDnumber % 1000000;

    // 写入文件
    fwrite(&new_user, sizeof(user), 1, fp);

    // 添加到内存数组
    if (usercount < 2000) {
        users[usercount] = new_user;
        usercount++;
    } else {
        printf("用户数量已达上限！\n");
        fclose(fp);
        return;
    }

    printf("注册成功！\n");
    printf("您的账号是：%lld\n", new_user.account);

    fclose(fp);
}

// 注销账户
void Delete_user() {
    long long del_account;
    char del_pwd[100];
    int del_idx = -1;
    char confirm;

    printf("===== 注销账户 =====\n");
    printf("请输入要注销的账号和密码（格式：账号 密码）：\n");
    // 检测输入格式
    if (scanf("%lld %s", &del_account, del_pwd) != 2) {
        printf("❌ 输入格式错误！请输入“数字账号+空格+密码”的格式\n");
        int sweeper;
        while ((sweeper = getchar()) != '\n' && sweeper != EOF);
        return;
    }

    // 查找用户
    for (int i = 0; i < usercount; i++) {
        if (users[i].account == del_account && strcmp(del_pwd, users[i].password) == 0) {
            del_idx = i;
            break;
        }
    }

    if (del_idx == -1) {
        printf("❌ 账号不存在或密码错误！\n");
        return;
    }

    // 二次确认
    printf("⚠️  警告：注销账户将删除所有信息，且无法恢复！\n");
    printf("确认注销？(Y/N)：");
    scanf(" %c", &confirm); // 空格跳过换行符

    if (confirm != 'Y' && confirm != 'y') {
        printf("✅ 注销操作已取消\n");
        return;
    }

    // 检查余额
    if (users[del_idx].deposit > 0) {
        printf("❌ 账户余额不为0，无法注销！当前余额：%.2lf 元\n", users[del_idx].deposit);
        return;
    }

    // 从内存数组删除（后续元素前移）
    for (int i = del_idx; i < usercount - 1; i++) {
        users[i] = users[i + 1];
    }
    usercount--;

    // 更新文件（重写所有用户）
    FILE *fp = fopen("user.dat", "wb");
    if (fp != NULL) {
        for (int i = 0; i < usercount; i++) {
            fwrite(&users[i], sizeof(user), 1, fp);
        }
        fclose(fp);
    }

    printf("✅ 账户注销成功！\n");
}

// 查看账户信息
void Check_information() {
    int checkflag = 0;
    printf("请输入账号和密码（格式：账号 密码）：\n");
    
    if (scanf("%lld %s", &account, password) != 2) {
        printf("输入格式错误！请重新输入\n");
        int sweeper;
        while ((sweeper = getchar()) != '\n' && sweeper != EOF);
        return;
    }

    for (int i = 0; i < usercount; i++) {
        if (users[i].account == account && strcmp(password, users[i].password) == 0) {
            printf("验证通过！以下是您账户的信息\n");
            printf("姓名:%s\n身份证号:%lld\n账号:%lld\n存款:%.2lf\n", 
                   users[i].name, users[i].IDnumber, users[i].account, users[i].deposit);
            checkflag = 1;
            break; // 找到后退出循环
        }
    }

    if (checkflag == 0) {
        printf("输入错误！账号或密码不正确\n");
    }
}

// 存款
void Deposit() {
    int i, found = -1;
    double money;

    printf("===== 存款功能 =====\n");
    printf("请输入账号和密码(示例: 123456 密码)：\n");
    if (scanf("%lld %s", &account, password) != 2) {
        printf("❌ 输入格式错误！请重新输入\n");
        int sweeper;
        while ((sweeper = getchar()) != '\n' && sweeper != EOF);
        return;
    }

    // 查找用户
    for (i = 0; i < usercount; i++) {
        if (users[i].account == account && strcmp(password, users[i].password) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("❌ 账号或密码错误！\n");
        return;
    }

    printf("请输入存款金额：\n");
    if (scanf("%lf", &money) != 1) {
        printf("❌ 输入格式错误！请重新输入\n");
        int sweeper;
        while ((sweeper = getchar()) != '\n' && sweeper != EOF);
        return;
    }

    if (money <= 0) {
        printf("❌ 存款金额必须大于 0！\n");
        return;
    }

    users[found].deposit += money;

    // 更新文件
    FILE *fp = fopen("user.dat", "wb");
    if (fp != NULL) {
        for (int j = 0; j < usercount; j++) {
            fwrite(&users[j], sizeof(user), 1, fp);
        }
        fclose(fp);
    }

    printf("✅ 存款成功！\n");
    printf("👉 当前账户余额为：%.2lf 元\n", users[found].deposit);
}

// 取款
void Withdrawal() {
    int i, found = -1;
    double money;

    printf("===== 取款功能 =====\n");
    printf("请输入账号和密码(示例: 123456 密码)：\n");
    if (scanf("%lld %s", &account, password) != 2) {
        printf("❌ 输入格式错误！请重新输入\n");
        int sweeper;
        while ((sweeper = getchar()) != '\n' && sweeper != EOF);
        return;
    }

    // 查找用户
    for (i = 0; i < usercount; i++) {
        if (users[i].account == account && strcmp(password, users[i].password) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("❌ 账号或密码错误！\n");
        return;
    }

    printf("请输入取款金额：\n");
    if (scanf("%lf", &money) != 1) {
        printf("❌ 输入格式错误！请重新输入\n");
        int sweeper;
        while ((sweeper = getchar()) != '\n' && sweeper != EOF);
        return;
    }

    if (money <= 0) {
        printf("❌ 取款金额必须大于 0！\n");
        return;
    }

    if (money > users[found].deposit) {
        printf("❌ 余额不足！当前余额为：%.2lf 元\n", users[found].deposit);
        return;
    }

    users[found].deposit -= money;

    // 更新文件
    FILE *fp = fopen("user.dat", "wb");
    if (fp != NULL) {
        for (int j = 0; j < usercount; j++) {
            fwrite(&users[j], sizeof(user), 1, fp);
        }
        fclose(fp);
    }

    printf("✅ 取款成功！\n");
    printf("👉 当前账户余额为：%.2lf 元\n", users[found].deposit);
}

// 转账
void Deposit_transfer() {
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

    // 查找转出用户
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

    // 查找转入用户
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

    // 检查是否转给自己
    if (from_idx == to_idx) {
        printf("❌ 不能向自己的账户转账！\n");
        return;
    }

    // 输入转账金额
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

    // 执行转账
    users[from_idx].deposit -= transfer_money;
    users[to_idx].deposit += transfer_money;

    // 更新文件
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

// 加载进度条
void loading_bar(void) {
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

        fflush(stdout);
        SLEEP(50); // 越大越慢
    }
    printf("\n处理完成！\n");
}

