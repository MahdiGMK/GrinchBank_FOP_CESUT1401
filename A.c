#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define endl      \
    printf("\n"); \
    fflush(stdout);
#define flush fflush(stdout);

const int Log = 20;

typedef long long ll;
typedef unsigned long long ull;

const int Mod = 998244353;

int max(int a, int b)
{
    return a > b ? a : b;
}
int min(int a, int b)
{
    return a < b ? a : b;
}
void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
struct Action;
typedef struct Action Action;
struct Account;
typedef struct Account Account;

struct Action
{
    enum
    {
        REGISTER = 1,
        DEPOSIT = 2,
        WITHDRAW = 3,
        TRANSFER = 4
    } type;

    Account *acc;
    ll val1, val2;
    Action *prv;
    Action *nxt;
};
struct Account
{
    char *fname, *lname;
    ll balance;
    Action *lastaction;
};

void Print(Action *action)
{
    switch (action->type)
    {
    case REGISTER:
        printf("account created");
        endl;
        return;
    case DEPOSIT:
        printf("deposit         +$%lld", action->val1);
        endl;
        return;
    case WITHDRAW:
        printf("withdraw        -$%lld", action->val1);
        endl;
        return;
    case TRANSFER:
        if (action->val2 > 0)
            printf("from %lld +$%lld", action->val1, action->val2);
        else
            printf("to %lld -$%lld", action->val1, -action->val2);
        endl;
        return;
    }
}

int acc_cnt = 0;
Account accs[1000000];
Account *MakeACC(char *fname, char *lname)
{
    Account *acc = accs + acc_cnt;
    acc->fname = fname;
    acc->lname = lname;
    acc->balance = 0;
    acc->lastaction = malloc(sizeof(Action));

    Action *reg = acc->lastaction;
    reg->acc = acc;
    reg->nxt = reg->prv = NULL;
    reg->type = REGISTER;

    acc_cnt++;

    return acc;
}
Account *GetACC(ll id)
{
    id -= 1000000000;
    if (id < 0 || id >= acc_cnt)
        return NULL;
    return accs + id;
}
void BindToACC(Action *act)
{
    Account *acc = act->acc;
    Action *prv = acc->lastaction;

    prv->nxt = act;
    act->prv = prv;
    act->nxt = NULL;
    acc->lastaction = act;
}
Action *MakeACT()
{
    return (Action *)malloc(sizeof(Action));
}

int streq(char *s1, char *s2)
{
    size_t l1 = strlen(s1);
    size_t l2 = strlen(s2);
    if (l1 != l2)
        return 0;

    for (int i = 0; i < l1; i++)
    {
        if (s1[i] != s2[i])
            return 0;
    }
    return 1;
}

void Register()
{
    char *fname = (char *)malloc(40);
    char *lname = (char *)malloc(40);
    scanf("%s %s", fname, lname);

    Account *acc = MakeACC(fname, lname);

    printf("Account number: %lld", 1000000000LL + acc_cnt - 1);
    endl;
}

void Deposit()
{
    ll id, val;
    scanf("%lld%lld", &id, &val);
    Account *acc = GetACC(id);
    if (val <= 0)
    {
        printf("Invalid amount");
        endl;
        return;
    }
    if (acc == NULL)
    {
        printf("Invalid account number");
        endl;
        return;
    }

    Action *act = MakeACT();
    act->acc = acc;
    act->type = DEPOSIT;
    act->val1 = val;
    acc->balance += val;
    BindToACC(act);

    printf("Success");
    endl;
}
void Withdraw()
{
    ll id, val;
    scanf("%lld%lld", &id, &val);
    Account *acc = GetACC(id);
    if (val <= 0)
    {
        printf("Invalid amount");
        endl;
        return;
    }
    if (acc == NULL)
    {
        printf("Invalid account number");
        endl;
        return;
    }
    if (val > acc->balance)
    {
        printf("Not enough balance");
        endl;
        return;
    }

    Action *act = MakeACT();
    act->acc = acc;
    act->type = WITHDRAW;
    act->val1 = val;
    acc->balance -= val;
    BindToACC(act);

    printf("Success");
    endl;
}

void Balance()
{
    ll id;
    scanf("%lld", &id);
    Account *acc = GetACC(id);
    if (acc == NULL)
    {
        printf("Invalid account number");
        endl;
        return;
    }
    printf("$%lld", acc->balance);
    endl;
}
ll TransferCost(ll val)
{
    if (val < 500000)
        return 600;
    if (val < 4000000)
        return 800;
    return 1200;
}
void Transfer()
{
    ll fid, tid, val;
    scanf("%lld%lld%lld", &fid, &tid, &val);
    Account *f = GetACC(fid), *t = GetACC(tid);
    if (val <= 0)
    {
        printf("Invalid amount");
        endl;
        return;
    }
    if (f == NULL)
    {
        printf("Invalid sender account number");
        endl;
        return;
    }
    if (t == NULL)
    {
        printf("Invalid receiver account number");
        endl;
        return;
    }
    if (f->balance < val + TransferCost(val))
    {
        printf("Not enough balance");
        endl;
        return;
    }

    Action *send_act = MakeACT();
    send_act->acc = f;
    send_act->type = TRANSFER;
    send_act->val1 = tid;
    send_act->val2 = -val - TransferCost(val);
    BindToACC(send_act);

    Action *receive_act = MakeACT();
    receive_act->acc = t;
    receive_act->type = TRANSFER;
    receive_act->val1 = fid;
    receive_act->val2 = val;
    BindToACC(receive_act);

    f->balance += send_act->val2;
    t->balance += receive_act->val2;

    printf("Success");
    endl;
}

void Info()
{
    ll id;
    scanf("%lld", &id);
    Account *acc = GetACC(id);
    if (acc == NULL)
    {
        printf("Invalid account number");
        endl;
        return;
    }
    printf("%s %s\n$%lld", acc->fname, acc->lname, acc->balance);
    endl;
}

void Batchreg()
{
    char *fname = (char *)malloc(40);
    char *lname = (char *)malloc(40);
    int cnt = 0;
    scanf("%s %s %d", fname, lname, &cnt);

    int f = acc_cnt;
    while (cnt--)
    {
        Account *acc = MakeACC(fname, lname);
    }
    int t = acc_cnt;

    printf("Account numbers: %lld-%lld", 1000000000LL + f, 1000000000LL + t - 1);
    endl;
}

void Recent()
{
    ll id;
    scanf("%lld", &id);
    Account *acc = GetACC(id);
    if (acc == NULL)
    {
        printf("Invalid account number");
        endl;
        return;
    }
    Action *act = acc->lastaction;
    int d = 9;
    while (act->prv != NULL && d--)
    {
        act = act->prv;
    }
    while (act != NULL)
    {
        Print(act);
        act = act->nxt;
    }
}

int main()
{
    char *s = (char *)malloc(10);
    while (1)
    {
        scanf("%s", s);

        if (strcmp(s, "register") == 0)
        {
            Register();
        }
        else if (strcmp(s, "deposit") == 0)
        {
            Deposit();
        }
        else if (strcmp(s, "withdraw") == 0)
        {
            Withdraw();
        }
        else if (strcmp(s, "balance") == 0)
        {
            Balance();
        }
        else if (strcmp(s, "transfer") == 0)
        {
            Transfer();
        }
        else if (strcmp(s, "info") == 0)
        {
            Info();
        }
        else if (strcmp(s, "batchreg") == 0)
        {
            Batchreg();
        }
        else if (strcmp(s, "recent") == 0)
        {
            Recent();
        }
        else if (strcmp(s, "exit") == 0)
        {
            return 0;
        }
        else
        {
            printf("Invalid input\n");
            while (getchar() != '\n')
                ;
        }
    }
}