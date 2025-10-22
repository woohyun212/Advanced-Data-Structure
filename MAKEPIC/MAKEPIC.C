#include <stdio.h> // 표준 입출력
#include <stdlib.h> // 일반 유틸리티 함수
#include <string.h> // 문자열 처리
#include <unistd.h> // POSIX 운영체제 API(터미널 관련 함수에 사용)
#include <termios.h> // 터미널 I/O 인터페이스(입력 모드 변경에 사용)
#include <fcntl.h> // 파일 제어
#include <stdarg.h> // 가변 인자 목록 처리

void clrscr() { // 화면을 지우고 커서를 (1, 1)로 이동하는 함수
    printf("\033[2J\033[1;1H");
    fflush(stdout);
}

void gotoxy(int x, int y) { // 커서를 (x, y) 좌표로 이동시키는 함수
    printf("\033[%d;%dH", y, x);
    fflush(stdout);
}

void textcolor(int color) { // 텍스트 색상 설정 함수(미구현)
    // Linux에서는 컬러를 ANSI escape code로 처리할 수 있음. 여기서는 무시.
}

int getch(void) { // 키보드에서 문자 입력 시 즉시 문자를 반환하는 함수(termios 구조체 사용)
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO); // ICANON: 정규 모드, ECHO: 입력 문자 출력
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

void putch(char c) { // 문자 하나를 출력하고 버퍼를 비움
    putchar(c);
    fflush(stdout);
}

void cprintf(const char *format, ...) { // 가변 인자를 받아 형식화된 출력 수행. 이후 버퍼 비움
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    fflush(stdout);
}

// 화살표 키 대신 일반 키로 변경
#define whereX 3 // 커서 시작 위치 (3, 3)
#define whereY 3
#define LEFT 'a'    // 왼쪽 화살표 대신 'a' 사용
#define RIGHT 'd'   // 오른쪽 화살표 대신 'd' 사용
#define UP 'w'      // 위쪽 화살표 대신 'w' 사용(미구현)
#define DOWN 'z'    // 아래쪽 화살표 대신 'z' 사용(미구현)


void mon(void); // 메뉴와 그림판 경계를 화면에 출력하는 함수 (미완성)
void make(void); // 사용자가 그림을 그리는 주요 루프를 포함하는 함수 (미완성)
void filesave(int nowx,int nowy); // 파일 저장 여부를 묻고 저장 함수를 호출하는 함수
void filewrite1(void); // 파일 저장 함수(따옴표와 함께 저장) (구현 완료)
void filewrite2(void); // 파일 저장(따옴표 없이 저장) (구현 완료)
void prxy(int x,int y,char *msg); // 특정 위치에 문자열을 출력하는 함수
void cls(void); // 화면을 clrscr()의 Wrapper 함수
// 사용자가 그린 그림의 내용을 저장하는 25*25 2차원 문자 배열. 초기에는 공백으로 채워짐
char picture[25][25]=
   {{'0','0','0',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {'0','0','0',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {'0','0','0',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}};

int longx=10,longy=10; // 사용자가 입력할 그림판의 가로 세로 크기 저장 변수
int main()
{
    FILE *fp;
    int allxy[101],lastxy;
    char readline,ch,filename,sel;
    cls();
	textcolor(10);
	prxy(10,7,"You can make pictures with \"*,0,o\"");
	textcolor(9);
	prxy(25,3," P I C T U R E   M A K E R "); // 오탈자 수정
	textcolor(11);
	prxy(45,10,"How many count for x(1~20):");
	scanf("%d",&longx);
    prxy(45,11,"How many count for y(1~20):");
	scanf("%d",&longy);
    /*lastxy=longx*longy;*/
    textcolor(15);
    cls();
    mon();
    gotoxy(whereX-1,whereY);
    make();
    return 0;
}

void make()
{
	int nowx = whereX, nowy = whereY, picx = 0, picy = 0;
	char go, yn;
	while (1/*(go=getch())!='!'*/)
	{
		go = getch();
		switch (go)
		{
		case LEFT: if (nowx == whereX)
			{
				prxy(45, 20, "You can't go there(LEFT)        ");
				// gotoxy(nowx, nowy);
			}
			else
			{
				nowx = nowx - 2;
				// gotoxy(nowx, nowy);
				picx--;
			}
			break;
		case RIGHT: if (nowx == (whereX - 2 + (longx * 2)))
			{
				prxy(45, 20, "You can't go there(RIGHT)       ");
				// gotoxy(nowx, nowy);
			}
			else
			{
				nowx = nowx + 2;
				// gotoxy(nowx, nowy);
				picx++;
			}
			break;
		case UP: if (nowy == whereY)
			{
				prxy(45, 20, "You can't go there(UP)        ");
				// gotoxy(nowx, nowy);
			}
			else
			{
				nowy = nowy - 1;
				// gotoxy(nowx, nowy);
				picy--;
			}
			break;
		case DOWN:
			if (nowy == (whereY - 1 + (longy * 1)))
			{
				prxy(45, 20, "You can't go there(DOWN)       ");
				// gotoxy(nowx, nowy);
			}
			else
			{
				nowy = nowy + 1;
				// gotoxy(nowx, nowy);
				picy++;
			}
			break;

		case '1':	// *
			// prxy(nowx, nowy, "*");
			picture[picy][picx] = '*';
			break;
		case '2':	// 0
			// prxy(nowx, nowy, "0");
			picture[picy][picx] = '0';
			break;
		case '3':	// o
			// prxy(nowx, nowy, "o");
			picture[picy][picx] = 'o';
			break;
		case '4':	// 공백
			// prxy(nowx, nowy, " ");
			picture[picy][picx] = ' ';
			break;
		case 'q':
			filesave(nowx, nowy);
			exit(0);
			// 저장하고 나가기
		case 'x': exit(0);
			// 나가기
		case 's':
			filesave(nowx, nowy);
			// 저장
		default: gotoxy(nowx, nowy);
			/*putch(go);*/
			break;
		}
		mon();
		// gotoxy(48, 3);
		// cprintf("%d, %d, %c",picx, picy, picture[picy][picx]);
		gotoxy(nowx, nowy);
	}
}

void filesave(int nowx, int nowy)
{
	int yn;
	prxy(45, 22, "Save with \" \' \" or now (y/n/q): ");
	yn = getch();
	prxy(45, 22, "                                        ");
	if (yn == 'y')
	{
		filewrite1();
		gotoxy(nowx, nowy);
	}
	else if (yn == 'n')
	{
		filewrite2();
		gotoxy(nowx, nowy);
	}
	else { gotoxy(nowx, nowy); }
}

void filewrite1()
{
     FILE *fp;
     char filename[10],buff[20],buff2[100];
     int tempx,tempy,len; // buff2 index용 len 변수 추가
     prxy(45,22,"File Name:");
     scanf("%s",filename);
     sprintf(buff,"%s",filename);
     if((fp=fopen(buff,"w+t"))==NULL){prxy(45,20,"File open error");exit(0);}
     putc('{',fp);
     putc('\n', fp);
     for(tempy=0;tempy<longy;tempy++) // row마다 반복
     {
        len = 0; // 버퍼(buff2) 초기화 - 저장하는 문자열이 고정 값이므로 내용을 비우지 않고 index만 초기화한다.
        for(tempx=0;tempx<longx;tempx++) // column마다 반복, 한 row만큼의 문자열을 buff2에 저장
        {
            buff2[len++] = '\'';
            buff2[len++] = picture[tempy][tempx];
            buff2[len++] = '\'';
            // 마지막 요소가 아니면 ', ' 추가
            if (tempx < longx - 1) {
                buff2[len++] = ',';
                buff2[len++] = ' ';
            }
        }
        buff2[len] = '\0'; // 문자열 끝 널 처리
        fprintf(fp, " {%s}", buff2); // {'*', '0', 'o', ' '} 형태로 파일에 저장
        if(tempy < longy - 1){
            putc(',', fp);
        }
        putc('\n', fp);
     }
     putc('}',fp);putc(';',fp);
     fclose(fp);
	 prxy(45,22,"                         ");
}

void filewrite2()
{
     FILE *fp;
     char filename[10],buff[20],buff2[100];
     int tempx,tempy,len;
     prxy(45,22,"File Name:");
     scanf("%s",filename);
     sprintf(buff,"%s",filename);
     if((fp=fopen(buff,"w+t"))==NULL){prxy(45,20,"File open error");exit(0);}
     putc('{',fp);
     putc('\n', fp);
     for(tempy=0;tempy<longy;tempy++)
     {
        len = 0;
        for(tempx=0;tempx<longx;tempx++)
        {
            buff2[len++] = picture[tempy][tempx];
            if (tempx < longx - 1) {
                buff2[len++] = ',';
                buff2[len++] = ' ';
            }
        }
        buff2[len] = '\0';
        fprintf(fp, " \"%s\"", buff2); // // "*, 0,  , o" 형태로 파일에 저장
        if(tempy < longy - 1){
            putc(',', fp);
        }
        putc('\n', fp);
     }
     putc('}',fp);putc(';',fp);
	 fclose(fp);
	 prxy(45,22,"                         ");
}

void mon() /* 메뉴 화면 출력 */
{
    int x,y,tempy,myx,myy;
    myx=(2*longx)+whereX;
    myy=longy+whereX;
    tempy=myy;
    for(x=whereX;x<myx;x=x+2)
    {
        int picX, picY; //picture 배열의 좌표값
        picX=(x-whereX)/2; 
        for(y=whereY;y<myy;y++)
        {
            picY=y-whereY;
            gotoxy(x, y);
            //picture 배열에 그림이 있다면 출력, 없다면 격자 출력
            if(picture[picY][picX]!=' ')
            {
                putch(picture[picY][picX]);
            }else{
                putch(',');
            }
            putch(' '); //x축 2칸 출력 간격 유지
        }
    }
         textcolor(11);
 	prxy(48,5,"--- M E N U ---");
     textcolor(10);
 	prxy(48,6," 1 . put \"*\"");
 	prxy(48,7," 2 . put \"0\"");
 	prxy(48,8," 3 . put \"o\"");
 	prxy(48,9," 4 . put \" \"");
 	prxy(48,10," w . move up");
 	prxy(48,11," z . move down");
 	prxy(48,12," a . move left");
 	prxy(48,13," d . move right");
 	prxy(48,14," s . save");
 	prxy(48,15," q . save & exit");
 	prxy(48,16," x . exit");
     textcolor(15);
}
void cls(void)
{
    clrscr();
}
void prxy(int x,int y,char *msg)
{
    gotoxy(x,y);
	cprintf("%s",msg);
}
