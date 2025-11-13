#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <fcntl.h>
#endif


void Q1_1();
void Q1_2();
void Q1_3();
void Q1_4();
void Q1_5();
void Q1_6();
void Q2_1();
void Q2_2();
void Q2_3();
void Q2_4();
void Q2_5();
void Q2_6();
void Q3_1();
void Q3_2();
void Q3_3();
void Q3_4();
void Q3_5();
void Q3_6();
void Q4_1();
void Q4_2();
void Q4_3();
void h_m();
int set(); // 전투의 결과를 알려주도록 Set() 반환값을 void -> int로 변경
void Opening();
void Load();
void Insert_magic();
void Insert_weapon();
void Insert_defence();
int Play_1();
void Item_store();
void Defence_Store();
void Save_option();
void Condition();
void Weapon_Store();
void Battle();
void cheatcenter();
void M_A();
void Mg();
int my_random(int n);
int scani();
void state_handler();
void set_monster(const char* name, int lv,
                 int hp_min, int hp_max,
                 int mp_min, int mp_max,
                 int att_min, int att_max,
                 int def,
                 int gold_min, int gold_max,
                 int exp_min, int exp_max);
void game_over(void);

void flush_stdin(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
    }
}

void clrscr()
{
    // 화면 clear
    printf("\033[2J\033[1;1H");
}

void textcolor(int color)
{
    // 색깔바꾸기
    if(color<20){
        printf("\033[%dm", color + 30); //텍스트의 전경색 계산
    }else{
        printf("\033[%dm", color + 70); //텍스트의 밝은 전경색 계산
    }
// Black	30	40
// Red	    31	41
// Green	32	42
// Yellow	33	43
// Blue	    34	44
// Magenta	35	45
// Cyan	    36	46
// White	37	47
// Default	39	49

// Bright Black	90	100
// Bright Red	91	101
// Bright Green	92	102
// Bright Yellow93	103
// Bright Blue	94	104
// Bright Magenta   95	105
// Bright Cyan	96	106
// Bright White	97	107
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
    // TODO: 제출 시 원상 복구
    usleep(ms * 1);
}

#if !defined(_WIN32)
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
#endif

int my_random(int n)
{
    if (n <= 0) return 0;
    return rand() % n;
}


void randomize()
{
    srand(time(NULL));
}

int scani()
{
    char buffer[64]; // 입력을 임시로 저장할 버퍼
    int i; // 문자 검사 인덱스
    int digits; // 입력된 숫자 자리수
    int value; // 최종 반환할 값

    while (1) // 0~99 범위의 숫자가 들어올 때까지 반복
    {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) // 잘못된 입력 처리
        {
            printf("\n입력 오류가 발생했습니다.");
            return -1;
        }

        i = 0;
        while (buffer[i] == ' ' || buffer[i] == '\t')
            i++; // 선행 공백 제거

        digits = 0;
        value = 0;
        while (buffer[i] >= '0' && buffer[i] <= '9' && digits < 2) // 두자릿수까지만 입력받음
        {
            value = value * 10 + (buffer[i] - '0');
            i++;
            digits++;
        }

        while (buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\r')
            i++; // 후행 공백 및 CR 제거

        if ((buffer[i] == '\n' || buffer[i] == '\0') && digits > 0)
            return value; // 0~99 범위 값 반환. 세자릿수부터는 조건에 걸림
        else
            printf("\n0 ~ 99 사이의 숫자만 입력해 주세요."); // 입력 재요청
    }
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
    int nhp; // 남은 HP
    int nmp; // 남은 MP
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
    #ifdef _WIN32 
        system("chcp 65001");
    #endif
    randomize();
    cheat = 0;
    while (1)
    {
        clrscr();
        textcolor(7);
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
        a = scani(); // scani: 메인 메뉴 입력을 두자리 정수로 제한
        if (a > 0 && a <= 3) break;
    }
    if (a == 1) Opening();
    if (a == 2) Load();
    if (a == 3) exit(0);
    Insert_magic();
    Insert_weapon();
    Insert_defence();
    state_handler(); //Play_1();
    return 0;
}

void state_handler(){
    int Func_set = 0;
    while(Func_set != -1){
        switch (Func_set){
            case 0: Func_set = Play_1(); break; // Play_1()은 사용자 입력을 q로 반환하는 int 함수로 변경. exit == q
            case 1: Condition(); Func_set = 0; break;
            case 2: Item_store(); Func_set = 0; break;
            case 3: Weapon_Store(); Func_set = 0; break;
            case 4: Defence_Store(); Func_set = 0; break;
            case 5: Battle(); Func_set = 0; break;
            case 6: Save_option(); Func_set = 0; break;
            case 7: Func_set = -1; break;
            case 99: cheatcenter(); Func_set = 0; break; // 원래 1008인데 일단 99로 설정
            default: Func_set = -1;
        }
    }
}

void Insert_weapon()
{
    int i;
    FILE* fp13 = fopen("WEAPON.QWE", "r");
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
    FILE* fp56 = fopen("DEFENCE.QWE", "rt"); // 리눅스 환경에서는 대소문자 구분을 해야함. 대문자로 수정
    fscanf(fp56, "%d", &count2);
    for (i = 0; i < count2; i++)
        fscanf(fp56, "%s %d %d %d %d", defence[i].name, &defence[i].defence, &defence[i].hp, &defence[i].mp,
               &defence[i].cost);
    fclose(fp56);
    return;
}

int Play_1()
{
    int i, q;
    clrscr();
    //user.nhp = user.hp; //배틀 중에 변경된 체력과 마나로 유지하기 위해 초기화 로직을 지움.
    //user.nmp = user.mp;
    /*
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
    */
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
    q = scani(); // scanf -> scani
    /*
    if (q == 2) Item_store();
    if (q == 4) Defence_Store();
    if (q == 6) Save_option();
    if (q == 7) exit(0);
    if (q == 1) Condition();
    if (q == 3) Weapon_Store();
    if (q == 5) Battle();
    if (q == 99) cheatcenter(); // 원래 1008인데 일단 99로 설정
    Play_1();
    */
    return q;
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
        ca = scani(); // scanf -> scani
        if (ca < 1 || ca > 5) continue;
        switch (ca)
        {
        case 1: user.gold += 1000000; // TODO: 10000으로 수정
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
    char *QuestFileName[22] = {"", "QUEST1_1.DAT", "QUEST1_2.DAT", "QUEST1_3.DAT", "QUEST1_4.DAT", "QUEST1_5.DAT", "QUEST1_6.DAT",
                                "QUEST2_1.DAT", "QUEST2_2.DAT", "QUEST2_3.DAT", "QUEST2_4.DAT", "QUEST2_5.DAT", "QUEST2_6.DAT",
                                "QUEST3_1.DAT", "QUEST3_2.DAT", "QUEST3_3.DAT", "QUEST3_4.DAT", "QUEST3_5.DAT", "QUEST3_6.DAT", 
                                "QUEST4_1.DAT", "QUEST4_2.DAT", "QUEST4_3.DAT"};
    void (*QuestFuncName[22])() = {NULL, Q1_1, Q1_2, Q1_3, Q1_4, Q1_5, Q1_6,
                                   Q2_1, Q2_2, Q2_3, Q2_4, Q2_5, Q2_6,
                                   Q3_1, Q3_2, Q3_3, Q3_4, Q3_5, Q3_6,
                                   Q4_1, Q4_2, Q4_3};
    FILE* fp24;
    while (1)
    {
        int i;
        char data;
        textcolor(7);
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
        l = scani(); // scanf -> scani
        if (l < 0 || l > user.wh)
        {
            printf("\n You can't go there....");
            getch();
            continue;
        }
        if (l == 0) break;
        fp24 = fopen(QuestFileName[l], "rt");

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
        QuestFuncName[l]();
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
    l = scani(); // scanf -> scani
    if (l == 9)
    {
        //set(); 필요없음
        return;
    }
    if (l < 1 || l > 8)
    {
        printf("잘못된 입력입니다.");
        getch();
        return;
    }
    if (user.item[l - 1] == 0)
    {
        printf("You don't have that potion!!!");
        getch();
        //set();
        return;
    }

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
    case 7: user.nhp = user.hp; // 기존에는 현재 체력을 최대 체력만큼 덧셈 연산을 했다. 오버플로우 가능성
        user.item[6]--;
        break;
    case 8: user.nmp = user.mp; // 기존에는 현재 마나를 두배로 만드는 연산을 했다. 오버플로우 가능성
        user.item[7]--;
        break;
    }
    h_m();
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
        l = scani(); // scanf -> scani
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
            if (user.nhp > user.hp) user.nhp = user.hp; // 현재 체력이 최대 체력을 넘는 경우, 최대 보정
            user.mp += weapon[l - 1].mp_bonus;
            user.attack += weapon[l - 1].power;
            clrscr();
            printf("\nH    P: %3d + %3d -> %3d", user.hp - weapon[l - 1].hp_bonus, weapon[l - 1].hp_bonus, user.hp);
            printf("\nM    P: %3d + %3d -> %3d", user.mp - weapon[l - 1].mp_bonus, weapon[l - 1].mp_bonus, user.mp);
            printf("\nATTACK: %3d + %3d -> %3d", user.attack - weapon[l - 1].power, weapon[l - 1].power, user.attack);
            getch();
            if (l==10 && user.nhp <= 0)
            {
                clrscr();
                printf("\n아아.. 당신은 검의 힘을 견뎌내지 못하고 ");
                textcolor(13);textcolor(21);printf("주화입마");
                textcolor(9);textcolor(19);
                printf("에 빠져 죽었습니다.\n");
                getch();
                game_over();
            }
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
        l = scani(); // scanf -> scani
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
            if(user.defence > 20){ // Play_1()의 디펜스 하락 로직 상점에서 방어구 구매 시점으로 이동
                user.defence = 20;
                clrscr();
                printf("\n디펜스가 20이상이 되면 자동으로 디펜스가 하락됩니다.");
                getch();
                clrscr();
            }
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
        l = scani(); // scanf -> scani
        if (l <= 0 || l >= 10) continue;
        if (l == 9) break;
        if (cost[l - 1] > user.gold) // && l > 0 && l < 9 전처리 있으므로 삭제
        {
            clrscr();
            printf("\n Need More Money");
            getch();
            continue;
        }
        else if(user.item[l-1] >= 20){ // Play_1의 물약 개수 검증 상점에 구현
            clrscr();
            printf("\n You can't buy item more than 20");
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
    fp3 = fopen("SAVEDATA.SAV", "w+");
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
    // flush_stdin();
    textcolor(7);
    printf("\n");
    printf("          N   a  m   e: %s\n", user.name);
    printf("          L e  v  e  l: %d\n", user.lv);
    printf("          H          P: %d/%d\n", user.nhp, user.hp);
    printf("          M          P: %d/%d\n", user.nmp, user.mp);
    printf("          Attack Point: %d ~ %d\n", user.attack - 2, user.attack + 2);
    printf("          DefencePoint: %d\n", user.defence);
    printf("          Need     Exp: %d \n", user.exp);
    printf("          G   O  L   D: %d \n", user.gold);
    textcolor(7);
    printf("     Can Private Magic: \n ");
    printf("\n━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━━━━━");
    textcolor(7);
    for (i = 0; i < 8; i++)
    {
        if (magic[i].lv <= user.lv)
        {
            printf("\n NAME: %16s ┃  Damage: %3d ┃ Mp: %4d ┃  Level: %3d", magic[i].name, magic[i].power, magic[i].ump,
                   magic[i].lv);
        }
    }
    textcolor(7);
    printf("\n━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━━━━━");
    printf("\n\n\n  < E N T E R >");
    getch();
    return;
}

void Insert_magic()
{
    FILE* fp2;
    int i;
    if (user.cs == 1) fp2 = fopen("AMAMAGIC.DAT", "rt");
    if (user.cs == 2) fp2 = fopen("SOCMAGIC.DAT", "rt");
    if (user.cs == 3) fp2 = fopen("NECMAGIC.DAT", "rt");
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
    fp1 = fopen("SAVEDATA.SAV", "rt");
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
    if (code != (user.lv + user.cs + user.nhp + user.nmp + user.gold + user.exp) / user.lv + user.lv)
    {
        printf("이런 치사한넘 .. 이런 게임을 에디트 하려 하다니... !!");
        getch();
        // TODO: 제출 시 원상 복구
        // exit(0);
    }
    return;
}

void Opening()
{
    FILE* fp0 = fopen("OPENING.QWE", "rt");
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
    /*
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
    */
    //ch = scani();
    user.gold = 0;
    while(1){ // 기존에 다른 정수 입력 시 세그폴트 문제를 해결(예외처리)
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
        ch = scani(); // scanf -> scani
        switch(ch){
            case 1:
                strcpy(user.name, "Amazon");
                user.cs = 1;
                user.lv = 1;
                user.nhp = user.hp = my_random(28) + 25;
                user.nmp = user.mp = my_random(15) + 5;
                user.attack = my_random(8) + 10;
                user.exp = my_random(8 * user.lv * user.lv) + 100;
                user.defence = 0;
                user.wh = 1;
                break;
            case 2:
                strcpy(user.name, "Sorceress");
                user.cs = 2;
                user.lv = 1;
                user.nhp = user.hp = my_random(20) + 10;
                user.nmp = user.mp = my_random(20) + 30;
                user.attack = my_random(8) + 5;
                user.exp = my_random(8 * user.lv * user.lv) + 100;
                user.defence = 0;
                user.wh = 1;
                break;
            case 3:
                strcpy(user.name, "Necromancer");
                user.cs = 3;
                user.lv = 1;
                user.nhp = user.hp = my_random(20) + 15;
                user.nmp = user.mp = my_random(20) + 25;
                user.attack = my_random(8) + 7;
                user.exp = my_random(8 * user.lv * user.lv) + 100;
                user.defence = 0;
                user.wh = 1;
                break;
            default:
                continue;
        }
        break;
    }
    /*
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
    */
    user.gold = 500;
    for (i = 0; i < 8; i++)
        user.item[i] = 0;
    fclose(fp0);
    return;
}

void game_over(void)
{
    clrscr();
    printf("\n 으억! 이렇게 당하다니 이세계의 미래는.....");
    getch();
    clrscr();
    printf("\n Game Over ");
    getch();
    exit(0); // exit(1)에서 exit(0)으로 수정
}

int set()
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
            if(l_m>0){l_m--;};
            if (user.exp <= 0)
            {
                clrscr();
                textcolor(7);
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
            game_over();
        }
        textcolor(7);
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
        input = scani(); // scanf -> scani
        if (input < 1 || input > 4) continue;
        switch (input)
        {
        case 1: M_A();
            break;
        case 2: Mg();
            break;
        case 3: Potion();
            break;
        case 4: return 0; // Run Away 구현
        }
    }
    return 1;
}

void set_monster(const char* name, int lv,
                 int hp_min, int hp_max,
                 int mp_min, int mp_max,
                 int att_min, int att_max,
                 int def,
                 int gold_min, int gold_max,
                 int exp_min, int exp_max)
{
    strcpy(monster.name, name);
    monster.lv = lv;
    monster.nhp = monster.hp = my_random(hp_max - hp_min + 1) + hp_min;
    monster.nmp = monster.mp = my_random(mp_max - mp_min + 1) + mp_min;
    monster.attack = my_random(att_max - att_min + 1) + att_min;
    monster.defence = def;
    monster.gold = my_random(gold_max - gold_min + 1) + gold_min;
    monster.exp = my_random(exp_max - exp_min + 1) + exp_min;
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
    // monster.lv = 1;
    l_m = 29;
    for (i = 0; i < 10; i++)
    {
        set_monster("Skel_Hasu", 1, 10, 17, 1, 8,
                    3, 7, 0, 1, 7, 1, 5);
        if(set() == 0) return;
    }
    for (i = 0; i < 10; i++)
    {
        set_monster("Skel_Mid", 1, 7, 20, 7, 20,
                    2, 7, 2, 3, 8, 1, 6);
        if(set() == 0) return;
    }
    for (i = 0; i < 9; i++)
    {
        set_monster("Skel_Gosu", 1, 9, 30, 9, 30,
                    3, 4, 3, 1, 4, 2, 9);
        if(set() == 0) return;
    }
    clrscr();
    printf("\n 뽀너스: Light Healing Potion +1");
    user.item[0] += 1;
    getch();
    // monster.lv = 3;
    strcpy(monster.name, "Skel_Boss");
    set_monster("Skel_Boss", 3, 60, 65, 60, 65,
                5, 9, 1, 1, 8, 7, 42);
    if(set() == 0) return;
    if (user.wh == 1) user.wh++;
    return;
}

void Q1_2()
{
    l_m = 1;
    set_monster("Iron_Fish", 3, 150, 160, 150, 160,
                5, 10, 2, 50, 56, 22, 28);
    if(set() == 0) return;
    set_monster("Gold_Fish", 8, 210, 250, 210, 250,
                5, 10, 6, 45, 51, 30, 37);
    if(set() == 0) return;
    if (user.wh == 2) user.wh++;
    return;
}

void Q1_3()
{
    l_m = 0;
    set_monster("Saladin", 10, 250, 320, 250, 320,
                4, 10, 3, 100, 100, 45, 81);
    if(set() == 0) return;
    if (user.wh == 3) user.wh++;
    return;
}

void Q1_4()
{
    int i;
    // monster.lv = 12;
    l_m = 2;
    for (i = 0; i < 2; i++)
    {
        set_monster("케인똘마니", 12, 150, 200, 150, 200,
                    10, 16, 0, 50, 50, 14, 20);
        if(set() == 0) return;
        //l_m = l_m; // 쓸데없는 배정문 삭제
    }
    set_monster("데까드케인", 12, 400, 460, 400, 460,
                4, 9, 5, 60, 60, 37, 107);
    if(set() == 0) return;
    if (user.wh == 4) user.wh++;
    return;
}

void Q1_5()
{
    int i;
    l_m = 10;
    // monster.lv = 11; //이거 왜 두개?
    // monster.lv = 11;
    for (i = 0; i < 3; i++)
    {
        set_monster("각목사나이", 11, 100, 160, 100, 160,
                    2, 7, 1, 10, 10, 10, 16);
        if(set() == 0) return;
    }
    for (i = 0; i < 4; i++)
    {
        set_monster("사시미군단", 11, 10, 30, 10, 30,
                    15, 20, 2, 30, 30, 10, 16);
        if(set() == 0) return;
    }
    for (i = 0; i < 3; i++)
    {
        set_monster("장관급부하", 11, 70, 100, 70, 100,
                    11, 16, 4, 10, 10, 20, 26);
        if(set() == 0) return;
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
    set_monster("안때리얼", user.lv + 10, 770, 1070, 770, 1070,
                14, 20, 3, 10, 10, 200, 600);
    if(set() == 0) return;
    if (user.wh == 6) user.wh++;
    return;
}

void Q2_1()
{
    // monster.lv = 12;
    l_m = 5;
    set_monster("Turtle_1", 12, 150, 250, 150, 250,
                6, 26, 8, 50, 100, 6, 35);
    if(set() == 0) return;
    set_monster("Turtle_2", 12, 150, 250, 150, 250,
                6, 26, 6, 50, 100, 6, 35);
    if(set() == 0) return;
    set_monster("Turtle_3", 12, 150, 250, 150, 250,
                6, 26, 7, 50, 100, 6, 35);
    if(set() == 0) return;
    set_monster("Turtle_4", 12, 150, 250, 150, 250,
                6, 26, 4, 50, 100, 6, 35);
    if(set() == 0) return;
    set_monster("Turtle_5", 12, 150, 250, 150, 250,
                6, 36, 5, 10, 39, 20, 70);
    if(set() == 0) return;
    set_monster("동상 단군", 12, 250, 350, 250, 350,
                16, 26, 9, 150, 200, 40, 80);
    if(set() == 0) return;

    if (user.wh == 7) user.wh++;
    return;
}

void Q2_2()
{
    l_m = 4;
    set_monster("GoldHamster", 9, 150, 250, 150, 250,
                8, 28, 7, 10, 60, 8, 47);
    if(set() == 0) return;
    set_monster("Jangarian", 9, 250, 350, 250, 350,
                10, 30, 4, 10, 60, 5, 34);
    if(set() == 0) return;
    set_monster("기니피그", 9, 150, 250, 150, 250,
                            6, 26, 6, 50, 100, 10, 40);
    if(set() == 0) return;
    set_monster("~시궁쥐~", 9, 350, 450, 350, 450,
                            16, 62, 3, 10, 60, 40, 80);
    if(set() == 0) return;
    if (user.wh == 8) user.wh++;
    return;
}

void Q2_3()
{
    l_m = 4;
    set_monster("Boradolei", 10, 350, 450, 350, 450,
                            16, 52, 3, 10, 60, 30, 70);
    if(set() == 0) return;
    set_monster("Ddubi", 10, 350, 450, 350, 450,
                        16, 52, 3, 10, 60, 30, 70);
    if(set() == 0) return;
    set_monster("NaNa", 10, 350, 450, 350, 450,
                        16, 52, 3, 10, 60, 30, 70);
    if(set() == 0) return;
    set_monster("Bo", 10, 350, 450, 350, 450,
                        16, 52, 3, 10, 60, 30, 70);
    if(set() == 0) return;
    set_monster("BabyOfSun", 10, 1550, 1650, 1550, 1650,
                    8, 18, 6, 100, 600, 130, 170);
    if(set() == 0) return;
    if (user.wh == 9) user.wh++;
    return;
}


void Q2_4()
{
    int i;
    if (user.lv >= 25)
    {
        printf("\n당신은 레벨이 높아 더이상 출입 불가능합니다.<Enter>");
        getch();
        return;
    }
    clrscr();
    l_m = 14;
    for (i = 0; i < 6; i++)
    {
        set_monster("고지방 감자튀김", 15, 30, 40, 10, 15,
                    10, 14, 3, 10, 15, 15, 20);
        if(set() == 0) return;
    }
    for (i = 0; i < 3; i++)
    {
        set_monster("유당 콜라", 16, 32, 42, 14, 19,
                    12, 16, 3, 13, 18, 18, 23);
        if(set() == 0) return;
    }
    for (i = 0; i < 3; i++)
    {
        set_monster("치즈 슬라임", 17, 80, 90, 15, 20,
                    10, 15, 4, 10, 15, 15, 20);
        if(set() == 0) return;
    }
    set_monster("불타는 치즈버거 괴물", 18, 140, 150, 30, 35,
                18, 21, 4, 10, 15, 15, 20);
    if(set() == 0) return;
    clrscr();
    printf("\n 뽀너스: Light Healing Potion +1");
    user.item[0] += 1;
    getch();
    clrscr();
    printf("\n롯데리우스: 리아 올뉴 한우 콰트로 치즈 맥시멈 쉐프의 초이스 단일 상품 29,900원!");
    delay(1200);
    printf("\n%s: 그걸 먹을 이유가 있나?", user.name);
    delay(1200);
    printf("\n롯데리우스: 알빠노?\n<ENTER>");
    getch();

    l_m = 0;
    set_monster("롯데리우스", 20, 220, 230, 40, 45,
                25, 26, 5, 80, 160, 180, 230);
    if(set() == 0) return;
    clrscr();
    printf("\n 참깨빵위에 순쇠고기 패티 두 장...");
    printf("\n\n 뽀너스: Super Healing Potion +1");
    user.item[2] += 1;
    getch();
    if (user.wh == 10) user.wh++;
    return;
} // 우현

void Q2_5()
{
    int i;
    if (user.lv >= 26)
    {
        printf("\n당신은 레벨이 높아 더이상 출입 불가능합니다.<Enter>");
        getch();
        return;
    }
    clrscr();
    l_m = 14;
    for (i = 0; i < 6; i++)
    {
        set_monster("치어 좀비", 16, 60, 80, 15, 25,
                    12, 16, 2, 18, 19, 25, 35);
        if(set() == 0) return;
    }
    for (i = 0; i < 4; i++)
    {
        set_monster("200dB 악단", 17, 70, 90, 14, 19,
                    14, 18, 3, 15, 40, 25, 50);
        if(set() == 0) return;
    }
    for (i = 0; i < 3; i++)
    {
        set_monster("블랙 치어리더", 18, 120, 160, 25, 40,
                    16, 22, 4, 20, 25, 30, 45);
        if(set() == 0) return;
    }
    set_monster("응원단의 악령", 19, 180, 220, 40, 60,
                20, 25, 5, 50, 70, 80, 120);
    if(set() == 0) return;
    clrscr();
    printf("\n치어데몬: 자~! 목소리 안 들려요! 더 크게!!!");
    delay(500);
    printf("\n칭칭언니 준비됐어요? 출발합시다~ 라일러 라일러~");
    delay(1200);
    getch();
    getch();

    set_monster("[응원단장] 치어 데몬", 20, 420, 500, 70, 100,
        20, 28, 6, 90, 160, 200, 300);
    if(set() == 0) return;
    clrscr();
    printf("\n응원봉이 부러지고, 지옥 응원가가 멈췄다...");
    printf("\n당신의 귀에는 아직도 메아리가 남아 있다.");
    printf("\n보상: Super Healing Potion +1");
    user.item[2] += 1;
    getch();
    if (user.wh == 11) user.wh++;
    return;
}

void Q2_6()
{
    int i;
    if (user.lv >= 26)
    {
        printf("\n당신은 레벨이 높아 더이상 출입 불가능합니다.<Enter>");
        getch();
        return;
    }
    clrscr();
    l_m = 14;
    for (i = 0; i < 5; i++)
    {
        set_monster("Frozen Stew Chunk", 19, 80, 100, 20,
                    30, 14, 18, 2, 10, 25, 30, 50);
        if(set() == 0) return;
    }
    for (i = 0; i < 4; i++)
    {
        set_monster("Frozen Cucumber", 19, 70, 90, 25,
                    35, 15, 19, 3, 15, 30, 35, 55);
        if(set() == 0) return;
    }
    for (i = 0; i < 3; i++)
    {
        set_monster("Spirit of Frozen Kimchi", 20, 120, 160, 40,
                    50, 17, 23, 4, 25, 45, 50, 80);
        if(set() == 0) return;
    }
    if(set() == 0) return;
    clrscr();
    delay(500);
    printf("\n듀리엘: 흐윽... 따뜻한 인간의 체온이라니... 불쾌하군...");
    delay(1000);
    getch();
    getch();
    getch();
    set_monster("냉장고 듀리엘", 22, 450, 600, 100, 130,
                24, 32, 6, 0, 0, 0, 0);
    if(set() == 0) return;
    clrscr();
    printf("\n듀리엘: 네 영혼을 급속 냉동 보관해주지..!!");
    delay(500);
    printf("\n냉기야, 퍼져라!!");
    delay(1200);
    getch();
    getch();

    set_monster("냉장고 듀리엘", 22, 550, 650, 100, 130,
                26, 32, 6, 400, 600, 760, 900);
    if(set() == 0) return;
    clrscr();
    printf("\n듀리엘이 녹아내린다... 하수구의 냉기가 사라진다.");
    printf("\n\n보상: Great Healing Potion +1");
    printf("\n보상: Great Mana Potion +1");
    user.item[4] += 1;
    user.item[5] += 1;
    getch();

    if (user.wh == 12) user.wh++;
    return;
}

// Act 3-1: rebellious squids from QUEST3_1.DAT
void Q3_1()
{
    l_m = 5;

    set_monster("Ink_Rebel", 18, 420, 640, 420, 640,
                70, 120, 45, 120, 200, 140, 220);
    if(set() == 0) return;
    set_monster("Salted_Veteran", 18, 480, 730, 480, 730,
                80, 120, 55, 130, 190, 150, 220);
    if(set() == 0) return;
    set_monster("Sashimi_Berserker", 18, 530, 789, 530, 789,
                95, 150, 60, 150, 240, 170, 260);
    if(set() == 0) return;
    set_monster("Boiled_Enforcer", 18, 570, 870, 570, 870,
                100, 160, 65, 170, 250, 190, 290);
    if(set() == 0) return;
    set_monster("King_Squid", 18, 900, 1300, 900, 1300,
                120, 190, 80, 220, 339, 210, 330);
    if(set() == 0) return;

    if (user.wh == 13) user.wh++;
    return;
}

// Act 3-2: crab republic from QUEST3_2.DAT
void Q3_2()
{
    l_m = 6;

    set_monster("Crab_Militia", 20, 520, 780, 520, 780,
                85, 139, 70, 150, 240, 180, 270);
    if(set() == 0) return;
    set_monster("Shell_Guardian", 20, 560, 880, 560, 880,
                90, 150, 90, 170, 280, 190, 290);
    if(set() == 0) return;
    set_monster("Tax_Collector", 20, 600, 940, 600, 940,
                95, 165, 95, 200, 320, 210, 310);
    if(set() == 0) return;
    set_monster("Claw_Senator", 20, 640, 1000, 640, 1000,
                100, 180, 110, 220, 360, 220, 330);
    if(set() == 0) return;
    set_monster("Republic_Hammer", 20, 700, 1120, 700, 1120,
                115, 195, 120, 260, 400, 230, 350);
    if(set() == 0) return;
    set_monster("President_Gestin", 20, 1100, 1600, 1100, 1600,
                150, 240, 140, 280, 460, 260, 400);
    if(set() == 0) return;

    if (user.wh == 14) user.wh++;
    return;
}

// Act 3-3: demon chefs from QUEST3_3.DAT
void Q3_3()
{
    l_m = 4;

    set_monster("Demonic_LineCook", 22, 640, 1000, 640, 1000,
                110, 180, 100, 210, 330, 210, 330);
    if(set() == 0) return;
    set_monster("Sous_Chef", 22, 680, 1060, 680, 1060,
                120, 200, 115, 240, 360, 220, 350);
    if(set() == 0) return;
    set_monster("Sashimi_Wraith", 22, 720, 1140, 720, 1140,
                130, 220, 120, 250, 400, 240, 380);
    if(set() == 0) return;
    textcolor(4);
    printf("\n셰프 마고로시: \"신선함의 비결은... 방금 죽은 손님이죠.\"");
    textcolor(7);
    set_monster("Chef_Magoroshi", 22, 900, 1420, 900, 1420,
                170, 280, 140, 260, 440, 280, 440);
    if(set() == 0) return;

    if (user.wh == 15) user.wh++;
    return;
}

// Act 3-4: awakened whales from QUEST3_4.DAT
void Q3_4()
{
    l_m = 3;

    set_monster("Rotwave_Whale", 24, 900, 1420, 900, 1420,
                140, 240, 150, 260, 420, 260, 430);
    if(set() == 0) return;
    set_monster("Sonic_Leviathan", 24, 980, 1540, 980, 1540,
                160, 270, 160, 300, 480, 280, 460);
    if(set() == 0) return;
    set_monster("Blood_Whale", 24, 1300, 1100, 1300, 1100,
                190, 330, 190, 340, 560, 320, 540);
    if(set() == 0) return;

    if (user.wh == 16) user.wh++;
    return;
}

// Act 3-5: soul angler from QUEST3_5.DAT
void Q3_5()
{
    l_m = 4;

    set_monster("Hooked_Spirit", 25, 880, 1400, 880, 1400,
                150, 260, 160, 280, 440, 290, 410);
    if(set() == 0) return;
    set_monster("Line_Binder", 25, 950, 1510, 950, 1510,
                160, 280, 170, 300, 480, 300, 480);
    if(set() == 0) return;
    set_monster("Soul_Net", 25, 1000, 1600, 1000, 1600,
                170, 300, 180, 320, 510, 320, 540);
    if(set() == 0) return;
    textcolor(4);
    printf("\n조낚귀: \"낚싯줄은 이미 네 심장에 닿아 있다.\"");
    textcolor(7);
    set_monster("JoNakGwi", 25, 1400, 2100, 1400, 2100,
                200, 350, 200, 360, 540, 350, 570);
    if(set() == 0) return;

    if (user.wh == 17) user.wh++;
    return;
}

// Act 3-6: Mephisto encounter from QUEST3_6.DAT
void Q3_6()
{
    if (user.wh > 18)
    {
        printf("\n 보스급 스테이지는 한번 이상 클리어가 불가능 합니다");
        getch();    
        return;
    }

    clrscr();
    textcolor(4);
    printf("\n메피스토: \"어서 오세요. 오늘은 손님이 회가 되는 날입니다.\"");
    delay(1200);
    textcolor(7);
    printf("\n%s: 이 수조에서 사람 머리가 떠다니는 걸 보니 입맛이 싹 달아나는군.", user.name);
    delay(1200);
    textcolor(4);
    printf("\n메피스토: 불결함과 청결, 둘 다 내 것이다. 넌 마지막 재료다.");
    delay(1200);
    textcolor(7);
    printf("\n%s: 네 앞치마에 내 피는 묻지 않을거다. 칼을 내려놔라.", user.name);
    delay(1200);
    textcolor(4);
    printf("\n메피스토: 그럼 직접 썰어주지... \n <Enter> ");
    getch();
    getch();

    l_m = 0;
    set_monster("MEPHISTO", user.lv + 8, 4200, 4900, 4200, 4900,
                600, 850, 320, 500, 720, 650, 950);
    if(set() == 0) return;

    if (user.wh == 18) user.wh++;
    return;
}

void Q4_1() { // 아정
    l_m = 5;
    for(int i=0;i<5;i++){
       set_monster("악마 공무원", 27, 1100, 2150, 1100, 2150, 300, 525, 300, 540, 870, 525, 855);
        if(set() == 0) return;
    }
    if (user.wh == 19) user.wh++;
    return; }
void Q4_2() {
    l_m = 4;

    for(int i=0;i<3;i++){
        set_monster("악마 공무원", 29, 1520, 2780, 1520, 2780, 360, 630, 360, 650, 1050, 630, 1230);
        if(set() == 0) return;
    }
    set_monster("곽팀장", 29, 1700, 3100, 1700, 3100, 400, 700, 400, 710, 1142, 700, 1130);
        if(set() == 0) return;
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
    textcolor(7);
    printf("\n%s: 결제? 웃기고 있네ㅋㅋㅋ창원시가 그렇게 쉽게 넘어갈거 같냐?", user.name);
    delay(1200);
    textcolor(4);
    printf("\nDiablo: 이미 창원시는 나의 행정력에 무릎을 꿇었다.");
    delay(1200);
    textcolor(4);
    printf("\nDiablo: 이제 네 놈의 죽이고 네 놈의 피로 결제를 해야겠다.");
    delay(1200);
    textcolor(7);
    printf("\n%s: ㅋㅋㅋㅋㅋ이제 곧 죽을 녀석의 말은 듣지 않는다.", user.name);
    delay(1200);
    textcolor(4);
    printf("\n%s: 이제 창원의 행정을 바로 잡을 시간이다. 디아블로!",user.name);
    delay(1200);
    textcolor(7);
    printf("\nDiablo: 해볼테면 해봐라 나약한 인간... \n <Enter> ");
    getch();
    getch();

    l_m = 0;
    set_monster("DIABLO", 35, 1660, 2490, 1660, 2490, 480, 830, 480, 0, 0, 0, 0); //페이즈1
    if(set() == 0) return;
    if(monster.nhp < 1){ 
        clrscr();
        textcolor(7);
        printf("\n%s: 이제 끝이다 디아블로!", user.name);
        delay(1200);
        textcolor(4);
        printf("\nDiablo: 하하하...생각보다 강하군...흐흥흥흥... \n 내가 각성하면 어떻게 될지 보자고!!", user.name);
        delay(1200);
        getch();
    }
        clrscr();
        set_monster("DIABLO[각성]", 35, 3330, 4990, 3330, 4990, 720, 1250, 570, 1140, 1840, 1110, 1810); //페이즈2
        if(set() == 0) return;
    if (user.wh == 21) user.wh++;
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
    printf("\n━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━━━━━");
    for (i = 0; i < 8; i++)
    {
        if (magic[i].lv <= user.lv)
        {
            printf("\n%2d.NAME: %12s   ┃ Damage: %3d  ┃ Mp: %3d  ┃  Level: %3d", s + 1, magic[i].name, magic[i].power,
                   magic[i].ump, magic[i].lv);
            s++;
        }
    }
    printf("\n━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━┻━━━━━━━━━━┻━━━━━━━━━━━━━━");
xx:
    printf("\n0.Cancel ,Magic Order(1~%d):", s);
    in = scani(); // scanf -> scani

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
        case 0: textcolor(7);
            printf("\n 피에 굶주린 자들이여 성스러운 %s 를 받아라~~~", magic[in - 1].name);
            break;
        case 1: textcolor(7);
            printf("\n 나의 주먹을 맛 보아라~~~~%s!!!!", magic[in - 1].name);
            break;
        case 2: textcolor(7);
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
