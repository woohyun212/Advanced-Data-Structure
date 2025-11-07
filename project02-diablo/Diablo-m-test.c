#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>


void Q1_1(); void Q1_2(); void Q1_3(); void Q1_4(); void Q1_5(); void Q1_6();
void Q2_1(); void Q2_2(); void Q2_3(); void Q2_4(); void Q2_5(); void Q2_6();
void Q3_1(); void Q3_2(); void Q3_3(); void Q3_4(); void Q3_5(); void Q3_6();
void Q4_1(); void Q4_2(); void Q4_3();
void h_m(); void set();
void Opening(); void Load(); void Insert_magic(); void Insert_weapon(); void Insert_defence();
void Play_1(); void Item_store(); void Defence_Store(); void Save_option(); void Condition();
void Weapon_Store(); void Battle(); void cheatcenter();
void M_A(); void Mg();
int my_random(int n);


void clrscr()
{
    // 화면 clear
    printf("\033[2J\033[1;1H");
}

void textcolor(int color)
{
    // 색깔바꾸기
}

void gotoxy(int x, int y)
{
    // 커서 이동
    printf("\033[%d;%dH", y, x);
}

void delay(int ms)
{
    // ms 단위 딜레이
    // usleep(ms * 1000);
    usleep(ms * 1);
}


int kbhit(void)
{
    // 키 입력 여부 반환 함수
    struct termios oldt, newt;
    int ch;
    int oldf;
    int hit = 0;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        hit = 1;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    return hit;
}


int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}


int my_random(int n)
{
    if (n <= 0) return 0;
    return rand() % n;
}


void randomize()
{
    srand(time(NULL));
}

struct monster_struct
{
    char name[100];
    int lv;
    int hp;
    int mp;
    int attack;
    int exp;
    int gold;
    int defence;
    int nhp;
    int nmp;
} monster;

struct magic_sturct
{
    char name[100];
    int power;
    int ump;
    int lv;
} magic[8];

struct defence_struct
{
    char name[100];
    int defence;
    int hp;
    int mp;
    int cost;
} defence[100];

struct weapon_struct
{
    char name[100];
    int power;
    int hp_bonus;
    int mp_bonus;
    int cost;
} weapon[100];

struct my_sturct
{
    char name[100];
    int item[8];
    int cs;
    int lv;
    int hp;
    int nhp;
    int mp;
    int nmp;
    int attack;
    int exp;
    int gold;
    int defence;
    int wh;
    int code;
} user;


int cheat = 0, l_m, count1, count2;

int main()
{
    int a;
    randomize();
    cheat = 0;
    while (1)
    {
        clrscr();
        textcolor(15);
        clrscr();
        printf("\n\n\n\n\n\n");
        printf("\n            ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
        printf("\n            ┃                                            ┃");
        printf("\n            ┃              Text Diablo III               ┃");
        printf("\n            ┃            Title: 하수구의 군주            ┃");
        printf("\n            ┃                             Demo V0.99     ┃");
        printf("\n            ┃               1. New Game                  ┃");
        printf("\n            ┃               2. Load Game                 ┃");
        printf("\n            ┃               3. End                       ┃");
        printf("\n            ┃ Press Input number:                        ┃");
        printf("\n            ┃                          Copy Left 2025    ┃");
        printf("\n            ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
        gotoxy(35, 16);
        scanf("%d", &a);
        if (a > 0 && a <= 3) break;
    }
    if (a == 1) Opening();
    if (a == 2) Load();
    if (a == 3) exit(0);
    Insert_magic();
    Insert_weapon();
    Insert_defence();
    Play_1();
    return 0;
}

void Insert_weapon()
{
    int i;
    FILE* fp13 = fopen("weapon.qwe", "r");
    fscanf(fp13, "%d", &count1);
    for (i = 0; i < 100; i++)
        fscanf(fp13, "%s %d %d %d %d", weapon[i].name, &weapon[i].power, &weapon[i].hp_bonus, &weapon[i].mp_bonus,
               &weapon[i].cost);
    fclose(fp13);
    return;
}

void Insert_defence()
{
    int i;
    FILE* fp56 = fopen("defence.qwe", "rt");
    fscanf(fp56, "%d", &count2);
    for (i = 0; i < count2; i++)
        fscanf(fp56, "%s %d %d %d %d", defence[i].name, &defence[i].defence, &defence[i].hp, &defence[i].mp,
               &defence[i].cost);
    fclose(fp56);
    return;
}

void Play_1()
{
    int i, q;
    clrscr();
    user.nhp = user.hp;
    user.nmp = user.mp;
    for (i = 0; i < 8; i++)
        if (user.item[i] > 20)
        {
            user.item[i] = 20;
            printf("%d번 포션의 수량이 20개 이상이 되어서 자동으로 소모됩니다", i + 1);
            getch();
            clrscr();
        }
    if (user.defence > 20)
    {
        user.defence = 20;
        printf("디펜스가 20이상이 되면 자동으로 디펜스가 하락됩니다.");
        getch();
        clrscr();
    }
    printf("     ━━━━━━━━━ Camp ━━━━━━━━\n\n");
    printf("                    1. Condition                  \n");
    printf("                    2. Item Store                 \n");
    printf("                    3. Weapon Store               \n");
    printf("                    4. Armor Store                \n");
    printf("                    5. Battle Quest               \n");
    printf("                    6. Save                       \n");
    printf("                    7. Exit                       \n");
    printf("   Please Insert Number:                          \n");
    gotoxy(26, 10);
    scanf("%d", &q);
    if (q == 2) Item_store();
    if (q == 4) Defence_Store();
    if (q == 6) Save_option();
    if (q == 7) exit(0);
    if (q == 1) Condition();
    if (q == 3) Weapon_Store();
    if (q == 5) Battle();
    if (q == 1008) cheatcenter();
    Play_1();
    return;
}

void cheatcenter()
{
    int ca;
    clrscr();
    while (1)
    {
        printf("\n1.Money + 10000");
        printf("\n2.Att + 1");
        printf("\n3.Hp + 10");
        printf("\n4.Mp + 10");
        printf("\n5.Waypoint +1");
        printf("\nInput Number:");
        gotoxy(14, 7);
        scanf("%d", &ca);
        if (ca < 1 || ca > 5) continue;
        switch (ca)
        {
        case 1: user.gold += 10000;
            break;
        case 2: user.attack += 1;
            break;
        case 3: user.hp += 10;
            break;
        case 4: user.mp += 10;
            break;
        case 5: user.wh += 1;
            break;
        }
        printf("\nComplete!");
        getch();
        break;
    }
    return;
}

void Battle()
{
    int i, j, k, l, time;
    FILE* fp24;
    while (1)
    {
        int i;
        char data;
        textcolor(15);
        clrscr();
        printf("  ┏━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━┓\n");
        printf("  ┃  A C T 1 ┃  A C T 2 ┃  A C T 3 ┃  A C T 4 ┃\n");
        printf("  ┣━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━╋━━━━━━━━━━┫\n");
        printf("  ┃1. Quest 1┃7. Quest 1┃13.Quest 1┃19.Quest 1┃\n");
        printf("  ┃2. Quest 2┃8. Quest 2┃14.Quest 2┃20.Quest 2┃\n");
        printf("  ┃3. Quest 3┃9. Quest 3┃15.Quest 3┃21.Quest 3┃\n");
        printf("  ┃4. Quest 4┃10 Quest 4┃16.Quest 4┃0. Exit   ┃\n");
        printf("  ┃5. Quest 5┃11 Quest 5┃17.Quest 5┃          ┃\n");
        printf("  ┃6. Quest 6┃12 Quest 6┃18.Quest 6┃          ┃\n");
        printf("  ┗━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━┛\n");
        printf("Select Quest Number(1~%2d):", user.wh);
        gotoxy(27, 11);
        scanf("%d", &l);
        if (l < 0 || l > user.wh)
        {
            printf("\n You can't go there....");
            getch();
            continue;
        }
        if (l == 1) fp24 = fopen("Quest1_1.dat", "rt");
        if (l == 2) fp24 = fopen("Quest1_2.dat", "rt");
        if (l == 3) fp24 = fopen("Quest1_3.dat", "rt");
        if (l == 4) fp24 = fopen("Quest1_4.dat", "rt");
        if (l == 5) fp24 = fopen("Quest1_5.dat", "rt");
        if (l == 6) fp24 = fopen("Quest1_6.dat", "rt");
        if (l == 7) fp24 = fopen("Quest2_1.dat", "rt");
        if (l == 8) fp24 = fopen("Quest2_2.dat", "rt");
        if (l == 9) fp24 = fopen("Quest2_3.dat", "rt");
        if (l == 10) fp24 = fopen("Quest2_4.dat", "rt");
        if (l == 11) fp24 = fopen("Quest2_5.dat", "rt");
        if (l == 12) fp24 = fopen("Quest2_6.dat", "rt");
        if (l == 13) fp24 = fopen("Quest3_1.dat", "rt");
        if (l == 14) fp24 = fopen("Quest3_2.dat", "rt");
        if (l == 15) fp24 = fopen("Quest3_3.dat", "rt");
        if (l == 16) fp24 = fopen("Quest3_4.dat", "rt");
        if (l == 17) fp24 = fopen("Quest3_5.dat", "rt");
        if (l == 18) fp24 = fopen("Quest3_6.dat", "rt");
        if (l == 19) fp24 = fopen("Quest4_1.dat", "rt");
        if (l == 20) fp24 = fopen("Quest4_2.dat", "rt");
        if (l == 21) fp24 = fopen("Quest4_3.dat", "rt");
        if (l == 0) break;
        if (l > 0 && l <= user.wh)
        {
            clrscr();
            printf("\n ");
            time = 30;
            while (fscanf(fp24, "%c", &data) != EOF)
            {
                if (kbhit()) time = 0;
                printf("%c", data);
                delay(time);
            }
            if (time == 0) getch();
            getch();
        }
        if (l == 1) Q1_1();
        if (l == 2) Q1_2();
        if (l == 3) Q1_3();
        if (l == 4) Q1_4();
        if (l == 5) Q1_5();
        if (l == 6) Q1_6();
        if (l == 7) Q2_1();
        if (l == 8) Q2_2();
        if (l == 9) Q2_3();
        if (l == 10) Q2_4();
        if (l == 11) Q2_5();
        if (l == 12) Q2_6();
        if (l == 13) Q3_1();
        if (l == 14) Q3_2();
        if (l == 15) Q3_3();
        if (l == 16) Q3_4();
        if (l == 17) Q3_5();
        if (l == 18) Q3_6();
        if (l == 19) Q4_1();
        if (l == 20) Q4_2();
        if (l == 21) Q4_3();
    }
    return;
}

void Potion()
{
    int l;
    printf("\n    M    E    N    U          YOURS ");
    printf("\n━━━━━━━━━━━━━━━━━━━━━");
    printf("\n  1.Light Healing Potion     [ %3d ]", user.item[0]);
    printf("\n  2.Light Mana Potion        [ %3d ]", user.item[1]);
    printf("\n  3.Super Healing Potion     [ %3d ]", user.item[2]);
    printf("\n  4.Super Mana Potion        [ %3d ]", user.item[3]);
    printf("\n  5.Great Healing Potion     [ %3d ]", user.item[4]);
    printf("\n  6.Great Mana Potion        [ %3d ]", user.item[5]);
    printf("\n  7.Full Helating Potion     [ %3d ]", user.item[6]);
    printf("\n  8.Full Mana Potion         [ %3d ]", user.item[7]);
    printf("\n  9.OUT  		         ");
    printf("\n━━━━━━━━━━━━━━━━━━━━━");
    printf("\nWhat you eat? :  ");
    scanf("%d", &l);
    if (l == 9) set();
    if (user.item[l - 1] != 0)
    {
        switch (l)
        {
        case 1: user.nhp += 25;
            if (user.nhp > user.hp) user.nhp = user.hp;
            user.item[0]--;
            break;
        case 2: user.nmp += 25;
            if (user.nmp > user.mp) user.nmp = user.mp;
            user.item[1]--;
            break;
        case 3: user.nhp += 50;
            if (user.nhp > user.hp) user.nhp = user.hp;
            user.item[2]--;
            break;
        case 4: user.nmp += 50;
            if (user.nmp > user.mp) user.nmp = user.mp;
            user.item[3]--;
            break;
        case 5: user.nhp += 100;
            if (user.nhp > user.hp) user.nhp = user.hp;
            user.item[4]--;
            break;
        case 6: user.nmp += 100;
            if (user.nmp > user.mp) user.nmp = user.mp;
            user.item[5]--;
            break;
        case 7: user.nhp += user.hp;
            user.item[6]--;
            break;
        case 8: user.nmp += user.nmp;
            user.item[7]--;
            break;
        }
        h_m();
    }
    else
    {
        printf("You don't have that potion!!!");
        getch();
        set();
    }
    return;
}

void Weapon_Store()
{
    int i, j, k, l;
    while (1)
    {
        clrscr();
        printf("             M    E    N    U     POWER  BONUS-HP  BONUS-MP   G O L D\n");
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        for (i = 0; i < count1; i++)
            printf("%d.%30s     [%4d]  [%4d]  [%4d]   [%4d]\n", i + 1, weapon[i].name, weapon[i].power,
                   weapon[i].hp_bonus, weapon[i].mp_bonus, weapon[i].cost);
        printf("%d.Out\n", count1 + 1);
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        printf("\nWhat do you Need? :    GOLD: %5d", user.gold);
        gotoxy(20, 5 + count1);
        scanf("%d", &l);
        if (l < 1 || l > count1 + 1) continue;
        if (l == count1 + 1) break;
        if (weapon[l - 1].cost > user.gold && l > 0 && l < count1 + 1)
        {
            printf("\n Need More Money");
            getch();
            continue;
        }
        else
        {
            user.gold -= weapon[l - 1].cost;
            user.hp += weapon[l - 1].hp_bonus;
            user.mp += weapon[l - 1].mp_bonus;
            user.attack += weapon[l - 1].power;
            clrscr();
            printf("\nH    P: %3d + %3d -> %3d", user.hp - weapon[l - 1].hp_bonus, weapon[l - 1].hp_bonus, user.hp);
            printf("\nM    P: %3d + %3d -> %3d", user.mp - weapon[l - 1].mp_bonus, weapon[l - 1].mp_bonus, user.mp);
            printf("\nATTACK: %3d + %3d -> %3d", user.attack - weapon[l - 1].power, weapon[l - 1].power, user.attack);
            getch();
        }
    }
    return;
}

void Defence_Store()
{
    int i, j, k, l;
    while (1)
    {
        clrscr();
        printf("             M    E    N    U    DEFENCE BONUS-HP  BONUS-MP   G O L D\n");
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        for (i = 0; i < count2; i++)
            printf("%d.%30s     [%4d]  [%4d]  [%4d]   [%4d]\n", i + 1, defence[i].name, defence[i].defence,
                   defence[i].hp, defence[i].mp, defence[i].cost);
        printf("%d.Out\n", count2 + 1);
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
        printf("\n*디펜스는 20 까지로 제한됩니다.");
        printf("\nWhat do you Need? :    GOLD: %5d", user.gold);
        gotoxy(20, 6 + count2);
        scanf("%d", &l);
        if (l < 1 || l > count2 + 1) continue;
        if (l == count2 + 1) break;
        if (defence[l - 1].cost > user.gold && l > 0 && l < count1 + 1)
        {
            printf("\n Need More Money");
            getch();
            continue;
        }
        else
        {
            user.gold -= defence[l - 1].cost;
            user.hp += defence[l - 1].hp;
            user.mp += defence[l - 1].mp;
            user.defence += defence[l - 1].defence;
            clrscr();
            printf("\nH    P : %3d + %3d -> %3d", user.hp - defence[l - 1].hp, defence[l - 1].hp, user.hp);
            printf("\nM    P : %3d + %3d -> %3d", user.mp - defence[l - 1].mp, defence[l - 1].mp, user.mp);
            printf("\nDEFENCE: %3d + %3d -> %3d", user.defence - defence[l - 1].defence, defence[l - 1].defence,
                   user.defence);
            getch();
        }
    }
    return;
}

void Item_store()
{
    int cost[8] = {50, 50, 100, 100, 500, 500, 1000, 1000};
    int i, j, k, l;
    while (1)
    {
        clrscr();
        printf("\n    M    E    N    U       GOLD      YOURS ");
        printf("\n━━━━━━━━━━━━━━━━━━━━━━");
        printf("\n  1.Light Healing Potion   [ 50 ]    [ %3d ]", user.item[0]);
        printf("\n  2.Light Mana Potion      [ 50 ]    [ %3d ]", user.item[1]);
        printf("\n  3.Super Healing Potion   [ 100]    [ %3d ]", user.item[2]);
        printf("\n  4.Super Mana Potion      [ 100]    [ %3d ]", user.item[3]);
        printf("\n  5.Great Healing Potion   [ 500]    [ %3d ]", user.item[4]);
        printf("\n  6.Great Mana Potion      [ 500]    [ %3d ]", user.item[5]);
        printf("\n  7.Full Helating Potion   [1000]    [ %3d ]", user.item[6]);
        printf("\n  8.Full Mana Potion       [1000]    [ %3d ]", user.item[7]);
        printf("\n  9.OUT  		         ");
        printf("\n━━━━━━━━━━━━━━━━━━━━━━");
        printf("\nWhat do you Need? :    GOLD: %5d", user.gold);
        gotoxy(20, 14);
        scanf("%d", &l);
        if (l <= 0 || l >= 10) continue;
        if (l == 9) break;
        if (cost[l - 1] > user.gold && l > 0 && l < 9)
        {
            clrscr();
            printf("\n Need More Money");
            getch();
            continue;
        }
        else
        {
            user.gold -= cost[l - 1];
            user.item[l - 1]++;
        }
    }
    return;
}

void Save_option()
{
    FILE* fp3;
    clrscr();
    user.code = (user.lv + user.cs + user.nhp + user.nmp + user.gold + user.exp) / user.lv + user.lv;
    fp3 = fopen("savedata.sav", "w+");
    fprintf(fp3, "%s", user.name);
    fprintf(fp3, "\n%d", user.lv);
    fprintf(fp3, "\n%d", user.cs);
    fprintf(fp3, "\n%d", user.hp);
    fprintf(fp3, "\n%d", user.nhp);
    fprintf(fp3, "\n%d", user.mp);
    fprintf(fp3, "\n%d", user.nmp);
    fprintf(fp3, "\n%d", user.exp);
    fprintf(fp3, "\n%d", user.attack);
    fprintf(fp3, "\n%d", user.gold);
    fprintf(fp3, "\n%d", user.defence);
    fprintf(fp3, "\n%d", user.wh);
    fprintf(fp3, "\n%d", user.code);
    fprintf(fp3, "\n%d", user.item[0]);
    fprintf(fp3, "\n%d", user.item[1]);
    fprintf(fp3, "\n%d", user.item[2]);
    fprintf(fp3, "\n%d", user.item[3]);
    fprintf(fp3, "\n%d", user.item[4]);
    fprintf(fp3, "\n%d", user.item[5]);
    fprintf(fp3, "\n%d", user.item[6]);
    fprintf(fp3, "\n%d", user.item[7]);
    fclose(fp3);
    printf("\n 세이브가 완료되었습니다<Enter>");
    getch();
    return;
}

void Condition()
{
    int i;
    clrscr();
    textcolor(15);
    printf("\n");
    printf("          N   a  m   e: %s\n", user.name);
    printf("          L e  v  e  l: %d\n", user.lv);
    printf("          H          P: %d/%d\n", user.nhp, user.hp);
    printf("          M          P: %d/%d\n", user.nmp, user.mp);
    printf("          Attack Point: %d ~ %d\n", user.attack - 2, user.attack + 2);
    printf("          DefencePoint: %d\n", user.defence);
    printf("          Need     Exp: %d \n", user.exp);
    printf("          G   O  L   D: %d \n", user.gold);
    textcolor(15);
    printf("     Can Private Magic: \n ");
    printf("\n━━━━━━━━━━━━┳━━━━━━━┳━━━━━┳━━━━━━━");
    textcolor(15);
    for (i = 0; i < 8; i++)
    {
        if (magic[i].lv <= user.lv)
        {
            printf("\n NAME: %16s ┃  Damage: %3d ┃ Mp: %4d ┃  Level: %3d", magic[i].name, magic[i].power, magic[i].ump,
                   magic[i].lv);
        }
    }
    textcolor(15);
    printf("\n━━━━━━━━━━━━┻━━━━━━━┻━━━━━┻━━━━━━━");
    printf("\n\n\n  < E N T E R >");
    getch();
    return;
}

void Insert_magic()
{
    FILE* fp2;
    int i;
    if (user.cs == 1) fp2 = fopen("Amamagic.dat", "rt");
    if (user.cs == 2) fp2 = fopen("Socmagic.dat", "rt");
    if (user.cs == 3) fp2 = fopen("Necmagic.dat", "rt");
    for (i = 0; i < 8; i++)
        fscanf(fp2, "%s %d %d %d", magic[i].name, &magic[i].power, &magic[i].ump, &magic[i].lv);
    fclose(fp2);
    return;
}

void Load()
{
    char d[100];
    char name[30];
    int code, lv, cs, nmp, defence, gold, mp, nhp, hp, exp, attack, wh;
    FILE* fp1;
    clrscr();
    fp1 = fopen("savedata.sav", "rt");
    fscanf(fp1, "%s", name);
    fscanf(fp1, "%d", &lv);
    fscanf(fp1, "%d", &cs);
    fscanf(fp1, "%d", &hp);
    fscanf(fp1, "%d", &nhp);
    fscanf(fp1, "%d", &mp);
    fscanf(fp1, "%d", &nmp);
    fscanf(fp1, "%d", &exp);
    fscanf(fp1, "%d", &attack);
    fscanf(fp1, "%d", &gold);
    fscanf(fp1, "%d", &defence);
    fscanf(fp1, "%d", &wh);
    fscanf(fp1, "%d", &code);
    fscanf(fp1, "%d", &user.item[0]);
    fscanf(fp1, "%d", &user.item[1]);
    fscanf(fp1, "%d", &user.item[2]);
    fscanf(fp1, "%d", &user.item[3]);
    fscanf(fp1, "%d", &user.item[4]);
    fscanf(fp1, "%d", &user.item[5]);
    fscanf(fp1, "%d", &user.item[6]);
    fscanf(fp1, "%d", &user.item[7]);

    strcpy(user.name, name);
    user.gold = gold;
    user.lv = lv;
    user.cs = cs;
    user.hp = hp;
    user.nhp = nhp;
    user.mp = mp;
    user.nmp = nmp;
    user.exp = exp;
    user.attack = attack;
    user.gold = gold;
    user.defence = defence;
    user.wh = wh;
    fclose(fp1);
    if (code == (user.lv + user.cs + user.nhp + user.nmp + user.gold + user.exp) / user.lv + user.lv)
    {
        printf("이런 치사한넘 .. 이런 게임을 에디트 하려 하다니... !!");
        getch();
        // exit(0);
    }
    return;
}

void Opening()
{
    FILE* fp0 = fopen("Opening.qwe", "rt");
    int ch, i;
    char data;
    randomize();
    clrscr();
    while (fscanf(fp0, "%c", &data) != EOF)
    {
        printf("%c", data);
        delay(50);
    }
    getch();

    clrscr();
    printf("\n\n\n\n\n");
    printf("\n           ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    printf("\n           ┃                                          ┃");
    printf("\n           ┃                                          ┃");
    printf("\n           ┃            Choose Your Character         ┃");
    printf("\n           ┃                                          ┃");
    printf("\n           ┃                 1. Amazon                ┃");
    printf("\n           ┃                 2. Sorceress             ┃");
    printf("\n           ┃                 3. Necromancer           ┃");
    printf("\n           ┃   Press Input number:                    ┃");
    printf("\n           ┃                                          ┃");
    printf("\n           ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
    gotoxy(37, 15);
    scanf("%d", &ch);
    user.gold = 0;
    if (ch == 1)
    {
        strcpy(user.name, "Amazon");
        user.cs = 1;
        user.lv = 1;
        user.nhp = user.hp = my_random(28) + 25;
        user.nmp = user.mp = my_random(15) + 5;
        user.attack = my_random(8) + 10;
        user.exp = my_random(8 * user.lv * user.lv) + 100;
        user.defence = 0;
        user.wh = 1;
    }
    if (ch == 2)
    {
        strcpy(user.name, "Sorceress");
        user.cs = 2;
        user.lv = 1;
        user.nhp = user.hp = my_random(20) + 10;
        user.nmp = user.mp = my_random(20) + 30;
        user.attack = my_random(8) + 5;
        user.exp = my_random(8 * user.lv * user.lv) + 100;
        user.defence = 0;
        user.wh = 1;
    }

    if (ch == 3)
    {
        strcpy(user.name, "Necromancer");
        user.cs = 3;
        user.lv = 1;
        user.nhp = user.hp = my_random(20) + 15;
        user.nmp = user.mp = my_random(20) + 25;
        user.attack = my_random(8) + 7;
        user.exp = my_random(8 * user.lv * user.lv) + 100;
        user.defence = 0;
        user.wh = 1;
    }
    user.gold = 500;
    for (i = 0; i < 8; i++)
        user.item[i] = 0;
    fclose(fp0);
    return;
}

void set()
{
    int re, input;
    randomize();
    while (1)
    {
        clrscr();
        if (monster.nhp <= 0)
        {
            printf("\n 캬오~~凸 우케케케케케 꾸르르르르 끄억");

            printf("\n 당신은 %d 만큼의 골드와 %d 만큼의 경험치를 얻습니다", monster.gold, monster.exp);
            getch();
            user.exp -= monster.exp;
            user.gold += monster.gold;
            l_m--;
            if (user.exp <= 0)
            {
                clrscr();
                textcolor(15);
                printf("\n Level Up!");

                switch (user.cs)
                {
                case 1: user.attack += my_random(1) + 1;
                    user.nmp = user.mp += my_random(4);
                    user.nhp = user.hp += my_random(5);
                    user.exp = my_random(80 * user.lv) + 100;
                    break;
                case 2: user.attack += my_random(1);
                    user.nmp = user.mp += my_random(9);
                    user.nhp = user.hp += my_random(3);
                    user.exp = my_random(80 * user.lv) + 100;
                    break;
                case 3: user.attack += my_random(2);
                    user.nmp = user.mp += my_random(7);
                    user.nhp = user.hp += my_random(8);
                    user.exp = my_random(80 * user.lv) + 100;
                    break;
                }
                user.lv++;
                getch();
            }

            break;
        }
        if (user.nhp <= 0)
        {
            printf("\n 으억! 이렇게 당하다니 이세계의 미래는.....");
            getch();
            clrscr();
            printf("\n Game Over");
            getch();
            exit(1);
        }
        textcolor(15);
        gotoxy(1, 1);
        printf("                               ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
        gotoxy(1, 2);
        printf("   1.Melee Attack              ┃   Name: %12s       %10s    ┃", user.name, monster.name);
        gotoxy(1, 3);
        printf("   2.Magic                     ┃   Lev :   %3d              %3d           ┃", user.lv, monster.lv);
        gotoxy(1, 4);
        printf("   3.Eat potion                ┃   H  P: %4d/%4d        %4d/%4d       ┃", user.nhp, user.hp,
               monster.nhp, monster.hp);
        gotoxy(1, 5);
        printf("   4.Run Away                  ┃   M  P: %4d/%4d        %4d/%4d       ┃", user.nmp, user.mp,
               monster.nmp, monster.mp);
        gotoxy(1, 6);
        printf("                               ┃   Att : %4d~%4d        %4d~%4d       ┃", user.attack - 2,
               user.attack + 2, monster.attack - 2, monster.attack + 2);
        gotoxy(1, 7);
        printf("                               ┃   Def :  %4d             %4d           ┃", user.defence,
               monster.defence);
        gotoxy(1, 8);
        printf("                               ┃   EXP :  %5d           %5d           ┃", user.exp, monster.exp);
        gotoxy(1, 9);
        printf("                               ┃   Gold:                  %5d           ┃", monster.gold);
        gotoxy(1, 10);
        printf("                               ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛  ");
        gotoxy(30, 11);
        printf("Leave Monster: %d", l_m);
        gotoxy(1, 11);
        printf("Battle Order(1~4): ");
        scanf("%d", &input);
        if (input < 1 || input > 4) continue;
        switch (input)
        {
        case 1: M_A();
            break;
        case 2: Mg();
            break;
        case 3: Potion();
            break;
        }
    }
    return;
}

void Q1_1()
{
    int i;
    if (user.lv >= 16)
    {
        printf("\n당신은 레벨이 높아 더이상 출입 불가능합니다.<Enter>");
        getch();
        return;
    }
    clrscr();
    monster.lv = 1;
    l_m = 29;
    for (i = 0; i < 10; i++)
    {
        strcpy(monster.name, "Skel_Hasu");
        monster.nhp = monster.hp = my_random(7) + 10;
        monster.attack = my_random(4) + 3;
        monster.nmp = monster.mp = my_random(7) + 1;
        monster.exp = my_random(4) + 1;
        monster.gold = my_random(6) + 1;
        monster.defence = 0;
        set();
    }
    for (i = 0; i < 10; i++)
    {
        strcpy(monster.name, "Skel_Mid");
        monster.mp = monster.nmp = monster.nhp = monster.nhp = monster.hp = my_random(14) + 7;
        monster.attack = my_random(5) + 2;
        monster.gold = my_random(5) + 3;
        monster.defence = 2;
        monster.exp = my_random(5) + 1;
        set();
    }
    for (i = 0; i < 9; i++)
    {
        strcpy(monster.name, "Skel_Gosu");
        monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(20) + 9;
        monster.attack = my_random(1) + 3;
        monster.gold = my_random(3) + 1;
        monster.defence = 3;
        monster.exp = my_random(7) + 2;
        set();
    }
    clrscr();
    printf("\n 뽀너스: Light Healing Potion +1");
    user.item[0] += 1;
    getch();
    monster.lv = 3;
    strcpy(monster.name, "Skel_Boss");
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(5) + 60;
    monster.attack = my_random(4) + 5;
    monster.gold = my_random(7) + 1;
    monster.defence = 1;
    monster.exp = my_random(35) + 7;
    set();
    if (user.wh == 1) user.wh++;
    return;
}

void Q1_2()
{
    l_m = 1;
    monster.lv = 3;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(10) + 150;
    strcpy(monster.name, "Iron_Fish");
    monster.attack = my_random(5) + 5;
    monster.gold = my_random(7) + 50;
    monster.defence = 2;
    monster.exp = my_random(7) + 22;
    set();
    monster.lv = 8;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(40) + 210;
    strcpy(monster.name, "Gold_Fish");
    monster.attack = my_random(6) + 5;
    monster.gold = my_random(7) + 45;
    monster.defence = 6;
    monster.exp = my_random(8) + 30;
    set();
    if (user.wh == 2) user.wh++;
    return;
}

void Q1_3()
{
    l_m = 0;
    monster.lv = 10;
    strcpy(monster.name, "Saladin");
    monster.attack = my_random(7) + 4;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(70) + 250;
    monster.gold = 100;
    monster.defence = 3;
    monster.exp = my_random(37) + 45;
    set();
    if (user.wh == 3) user.wh++;
    return;
}

void Q1_4()
{
    int i;
    monster.lv = 12;
    l_m = 2;
    for (i = 0; i < 2; i++)
    {
        strcpy(monster.name, "케인똘마니");
        monster.attack = my_random(6) + 10;
        monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(50) + 150;
        monster.gold = 50;
        monster.defence = 0;
        monster.exp = my_random(6) + 14;
        set();
        l_m = l_m;
    }
    strcpy(monster.name, "데까드케인");
    monster.attack = my_random(6) + 4;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(60) + 400;
    monster.gold = 60;
    monster.defence = 5;
    monster.exp = my_random(70) + 37;
    set();
    if (user.wh == 4) user.wh++;
    return;
}

void Q1_5()
{
    int i;
    l_m = 10;
    monster.lv = 11; //이거 왜 두개?
    monster.lv = 11;
    for (i = 0; i < 3; i++)
    {
        strcpy(monster.name, "각목사나이");
        monster.attack = my_random(6) + 2;
        monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(60) + 100;
        monster.gold = 10;
        monster.defence = 1;
        monster.exp = my_random(7) + 10;
        set();
    }
    for (i = 0; i < 4; i++)
    {
        strcpy(monster.name, "사시미군단");
        monster.attack = my_random(6) + 15;
        monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(20) + 10;
        monster.gold = 30;
        monster.defence = 2;
        monster.exp = my_random(7) + 10;
        set();
    }
    for (i = 0; i < 3; i++)
    {
        strcpy(monster.name, "장관급부하");
        monster.attack = my_random(6) + 11;
        monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(30) + 70;
        monster.gold = 10;
        monster.defence = 4;
        monster.exp = my_random(7) + 20;
        set();
    }
    if (user.wh == 5) user.wh++;
    return;
}

void Q1_6()
{
    if (user.wh > 6)
    {
        printf("\n 보스급 스테이지는 한번 이상 클리어가 불가능 합니다");
        getch();
        return;
    }
    clrscr();
    printf("\nAnddaerial: 니가 내 아그들 건드렸냐?");
    delay(1200);
    printf("\n%s: 그래 어쩔래? 니가 보태준거 있냐?", user.name);
    delay(1200);
    printf("\nAnddaerial: 없다. 죽고 싶냐 말투가 그게 머냐?");
    delay(1200);
    printf("\n%s: 후훗.. 간이 배밖으로 나왔나 보군 없애주마!!", user.name);
    delay(1200);
    printf("\nAnddaerial: 레벨도 %d 밖에 안되는 주제에 내 레벨은 %d 다!!", user.lv, monster.lv = user.lv + 10);
    delay(1200);
    printf("\n%s: 헉... 그래도 승부다!! -.-;; \n <Enter> ", user.name);
    getch();
    getch();

    l_m = 0;
    strcpy(monster.name, "안때리얼");
    monster.attack = my_random(6) + 14;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(300) + 770;
    monster.gold = 10;
    monster.defence = 3;
    monster.exp = my_random(400) + 200;
    set();
    if (user.wh == 6) user.wh++;
    return;
}

void Q2_1()
{
    monster.lv = 12;
    l_m = 5;
    strcpy(monster.name, "Turtle_1");
    monster.attack = my_random(20) + 6;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(100) + 150;
    monster.gold = my_random(50) + 50;
    monster.defence = 8;
    monster.exp = my_random(30) + 6;
    set();
    strcpy(monster.name, "Turtle_2");
    monster.attack = my_random(20) + 6;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(100) + 150;
    monster.gold = my_random(50) + 50;
    monster.defence = 6;
    monster.exp = my_random(30) + 6;
    set();
    strcpy(monster.name, "Turtle_3");
    monster.attack = my_random(20) + 6;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(100) + 150;
    monster.gold = my_random(50) + 50;
    monster.defence = 7;
    monster.exp = my_random(30) + 6;
    set();
    strcpy(monster.name, "Turtle_4");
    monster.attack = my_random(20) + 6;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(100) + 150;
    monster.gold = my_random(50) + 50;
    monster.defence = 4;
    monster.exp = my_random(30) + 6;
    set();
    strcpy(monster.name, "Turtle_5");
    monster.attack = my_random(30) + 6;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(100) + 150;
    monster.gold = my_random(30) + 10;
    monster.defence = 5;
    monster.exp = my_random(50) + 20;
    set();
    strcpy(monster.name, "동상 단군");
    monster.attack = my_random(10) + 16;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(100) + 250;
    monster.gold = my_random(50) + 150;
    monster.defence = 9;
    monster.exp = my_random(40) + 40;
    set();

    if (user.wh == 7) user.wh++;
    return;
}

void Q2_2()
{
    l_m = 4;
    strcpy(monster.name, "GoldHamster");
    monster.attack = my_random(20) + 8;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(100) + 150;
    monster.gold = my_random(50) + 10;
    monster.defence = 7;
    monster.exp = my_random(40) + 8;
    set();
    strcpy(monster.name, "Jangarian");
    monster.attack = my_random(20) + 10;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(100) + 250;
    monster.gold = my_random(50) + 10;
    monster.defence = 4;
    monster.exp = my_random(30) + 5;
    set();
    strcpy(monster.name, "기니피그");
    monster.attack = my_random(20) + 6;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(100) + 150;
    monster.gold = my_random(50) + 50;
    monster.defence = 6;
    monster.exp = my_random(30) + 10;
    set();
    strcpy(monster.name, "~시궁쥐~");
    monster.attack = my_random(46) + 16;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(100) + 350;
    monster.gold = my_random(50) + 10;
    monster.defence = 3;
    monster.exp = my_random(40) + 40;
    set();
    if (user.wh == 8) user.wh++;
    return;
}

void Q2_3()
{
    l_m = 4;
    strcpy(monster.name, "Boradolei");
    monster.attack = my_random(36) + 16;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(100) + 350;
    monster.gold = my_random(50) + 10;
    monster.defence = 8;
    monster.exp = my_random(40) + 30;
    set();
    strcpy(monster.name, "Ddubi");
    monster.attack = my_random(36) + 16;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(100) + 350;
    monster.gold = my_random(50) + 10;
    monster.defence = 4;
    monster.exp = my_random(40) + 30;
    set();
    strcpy(monster.name, "NaNa");
    monster.attack = my_random(36) + 16;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(100) + 350;
    monster.gold = my_random(50) + 10;
    monster.defence = 4;
    monster.exp = my_random(40) + 30;
    set();
    strcpy(monster.name, "Bo");
    monster.attack = my_random(36) + 16;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(100) + 350;
    monster.gold = my_random(50) + 10;
    monster.defence = 3;
    monster.exp = my_random(40) + 30;
    set();
    strcpy(monster.name, "BabyOfSun");
    monster.attack = my_random(10) + 8;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(100) + 1550;
    monster.gold = my_random(500) + 100;
    monster.defence = 6;
    monster.exp = my_random(40) + 130;
    set();
    if (user.wh == 9) user.wh++;
    return;
}


void Q2_4() { return; }// 우현
void Q2_5() { return; }
void Q2_6() { return; }

void Q3_1() { return; } // 준서
void Q3_2() { return; }
void Q3_3() { return; }
void Q3_4() { return; }
void Q3_5() { return; }
void Q3_6() { return; }

void Q4_1() { // 아정
    l_m = 4;
    for(int i=0;i<3;i++){
        strcpy(monster.name, "Griffin");
        monster.attack = my_random(100) + 100;
        monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(400) + 600;
        monster.gold = my_random(50) + 150;
        monster.defence = 650;
        monster.exp = my_random(300) + 420;
        set();
    }
    strcpy(monster.name, "Ttirael");
    monster.attack = my_random(300) + 200;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(400) + 1000;
    monster.gold = my_random(50) + 200;
    monster.defence = 740;
    monster.exp = my_random(380) + 440;
    set();
    if (user.wh == 19) user.wh++;
    return; } 
void Q4_2() {
    l_m = 5;
    for(int i=0;i<5;i++){
        strcpy(monster.name, "Nephalem");
        monster.attack = my_random(360) + 200;
        monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(400) + 570;
        monster.gold = my_random(220) + 100;
        monster.defence = 760;
        monster.exp = my_random(400) + 460;
        set();
    }
    if (user.wh == 20) user.wh++;
    return; }
void Q4_3() { 
    if (user.wh > 21)
    {
        printf("\n 보스급 스테이지는 한번 이상 클리어가 불가능 합니다");
        getch();
        return;
    }
    clrscr();
    textcolor(9);
    printf("\nDiablo: 너희가 감히 나를 잡을 수 있을 것이라고 생각하느냐.");
    delay(1200);
    textcolor(15);
    printf("\n%s: 디아블로, 나는 이미 당신의 수많은 부하를 죽이고 왔다.", user.name);
    delay(1200);
    textcolor(9);
    printf("\nDiablo: 너희의 그 모험이 얼마나 어리석은 짓이였는지 알려주마.");
    delay(1200);
    textcolor(15);
    printf("\n%s: ㅋㅋㅋㅋㅋ이제 곧 죽을 녀석의 말은 듣지 않는다.", user.name);
    delay(1200);
    textcolor(9);
    printf("\n%s: 현재 내 레벨은 %d, 너를 죽이기엔 충분한 경험을 쌓았지... 조용히 죽어라 디아블로!",user.name, user.lv);
    delay(1200);
    textcolor(15);
    printf("\nDiablo: 해볼테면 해봐라 나약한 인간... \n <Enter> ");
    getch();
    getch();

    l_m = 0;
    strcpy(monster.name, "DIABLO");
    monster.attack = my_random(400) + 850;
    monster.mp = monster.nmp = monster.mp = monster.nhp = monster.hp = my_random(1000) + 6500;        monster.gold = my_random(220) + 100;
    monster.defence = 700;
    monster.exp = my_random(400) + 1000;
    set();
    //if (user.wh == 21) user.wh++;
    return; }


void M_A()
{
    int cr, ud;
    randomize();
    ud = my_random(4) - 2 + user.attack - monster.defence;
    if (ud < 0) ud = 0;

    cr = my_random(40);
    if (cr == 3)
    {
        gotoxy(10, 13);
        printf("풀 파워~! 크리티컬 어택 %d만큼의 데미지!", ud * 3);
        monster.nhp = monster.nhp - (ud * 3);
        getch();
    }
    else
    {
        if (ud == 0)
        {
            gotoxy(10, 13);
            printf(" ~~ ㅋㅋ..");
        }
        else
        {
            gotoxy(10, 13);
            printf("당신은 %s에게 물리적인 힘으로 %d만큼의 충격을 가합니다....", monster.name, ud);
            monster.nhp -= ud;
        }
    }
    getch();
    if (monster.nhp > 0) h_m();
    return;
}

void h_m()
{
    int md, s;
    randomize();
    s = my_random(6);
    md = my_random(4) - 2 + monster.attack - user.defence;
    if (md <= 0) md = 1;

    printf("\n");
    switch (s)
    {
    case 0: printf("당신은 %s에게 명치를 가격 당하면서 %d 만큼의 충격을 받습니다...", monster.name, md);
        break;
    case 1: printf("%s가 당신의 목에 올라타 순식간에 목을 뒤로 꺽으면서 %d의 충격을 가합니다..", monster.name, md);
        break;
    case 2: printf("%s가 잔상을 남기며 사라지며 순식간에 앞으로 다가와 두 눈을 손가락으로 강력하게 찌르면서 %d의 데미지를 줍니다..", monster.name, md);
        break;
    case 3: printf("%s의 분신 3개가 나타나 각각 머리 가슴 배 를 구타하고 사라지며 %d만큼의 데미지를 줍니다..", monster.name, md);
        break;
    case 4: printf("무시무시한 %s의 손등이 번개같이 당신의 목 뒤를 후려 갈기면서 %d만큼의 데미지를 줍니다..", monster.name, md);
        break;
    case 5: printf("%s의 잔인한 죽음의 손길이 당신의 죽음을 재촉합니다..순식간에 %d만큼의 엄청난 충격이 몸으로 흡수됩니다...", monster.name, md * 2);
        md = md * 2;
        break;
    }
    user.nhp -= md;
    getch();
    return;
}

void Mg()
{
    int bonus, w, in, i, xx, s = 0;
    gotoxy(1, 13);
    printf("     Can Private Magic: \n ");
    printf("\n━━━━━━━━━━━━┳━━━━━━━┳━━━━━┳━━━━━━━");
    for (i = 0; i < 8; i++)
    {
        if (magic[i].lv <= user.lv)
        {
            printf("\n%2d.NAME: %12s   ┃ Damage: %3d  ┃ Mp: %3d  ┃  Level: %3d", s + 1, magic[i].name, magic[i].power,
                   magic[i].ump, magic[i].lv);
            s++;
        }
    }
    printf("\n━━━━━━━━━━━━┻━━━━━━━┻━━━━━┻━━━━━━━");
xx:
    printf("\n0.Cancel ,Magic Order(1~%d):", s);
    scanf(" %d", &in);

    if (in < 0 || in > s || magic[in - 1].ump > user.nmp) goto xx;
    if (in == 0)
    {
        printf("\n 그런것은 불가능 합니다.<Enter>");
        getch();
        set();
    }
    else
    {
        w = my_random(3);
        if (user.cs == 1) bonus = my_random(magic[in - 1].power + user.lv * 4);
        if (user.cs == 2) bonus = my_random(magic[in - 1].power + user.lv * 8);
        if (user.cs == 3) bonus = my_random(magic[in - 1].power + user.lv * 6);
        switch (w)
        {
        case 0: textcolor(15);
            printf("\n 피에 굶주린 자들이여 성스러운 %s 를 받아라~~~", magic[in - 1].name);
            break;
        case 1: textcolor(15);
            printf("\n 나의 주먹을 맛 보아라~~~~%s!!!!", magic[in - 1].name);
            break;
        case 2: textcolor(15);
            printf("\n %s!!! 하핫 아프지? ", magic[in - 1].name);
            break;
        }
        printf("\n 당신은 %s 에게 %d 만큼의 데미지를 가합니다", monster.name, magic[in - 1].power + bonus);
        monster.nhp -= (magic[in - 1].power + bonus);
        user.nmp -= magic[in - 1].ump;
        getch();
    }
    if (monster.nhp > 0) h_m();
    return;
}
