#include <stdio.h> // 표준 입출력
#include <stdlib.h> // 일반 유틸리티 함수
#include <string.h> // 문자열 처리
#include <unistd.h> // POSIX 운영체제 API(터미널 관련 함수에 사용)
#include <termios.h> // 터미널 I/O 인터페이스(입력 모드 변경에 사용)
#include <fcntl.h> // 파일 제어
#include <stdarg.h> // 가변 인자 목록 처리
#include <signal.h> // kill 시그널 처리 라이브러리

void handle_sigint(int signo) {
	// exit() 호출해서 atexit()에 등록한 함수도 실행되게 함.
	exit(0);
}

void clrscr() { // 화면을 지우고 커서를 (1, 1)로 이동하는 함수
    printf("\033[2J\033[1;1H");
    fflush(stdout);
}

void gotoxy(int x, int y) { // 커서를 (x, y) 좌표로 이동시키는 함수
    printf("\033[%d;%dH", y, x);
    fflush(stdout);
}

void textcolor(int color) { // 텍스트 색상 설정 함수(구현)
    // Linux에서는 컬러를 ANSI escape code로 처리할 수 있음. 여기서는 무시.
    if(color<8){
        printf("\033[%dm", color + 30); //텍스트의 전경색 계산
    }else{
        printf("\033[%dm", color + 82); //텍스트의 밝은 전경색 계산
    }
    
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
#define UP 'w'      // 위쪽 화살표 대신 'w' 사용(구현 완료)
#define DOWN 'z'    // 아래쪽 화살표 대신 'z' 사용(구현 완료)


void mon(void); // 메뉴와 그림판 경계를 화면에 출력하는 함수 (구현 완료)
void make(void); // 사용자가 그림을 그리는 주요 루프를 포함하는 함수 (구현 완료)
void filesave(int nowx,int nowy); // 파일 저장 여부를 묻고 저장 함수를 호출하는 함수
void copy_file(char *src, char *dest); // 파일 복사용 편의 함수
void filewrite1(void); // 파일 저장 함수(따옴표와 함께 저장) (구현 완료)
void filewrite2(void); // 파일 저장(따옴표 없이 저장) (구현 완료)
void fileread1(FILE *fp, char *buff, int savex, int savey); // 파일 읽기 함수(filewrite1() 저장 형식)
void fileread2(FILE *fp, char *buff, int savex, int savey); // 파일 읽기 함수(filewrite2() 저장 형식)
void checkFileType(); // 최근 저장한 파일이 있는지 확인하고, 저장 방식에 따라 다르게 로드
void prxy(int x,int y,char *msg); // 특정 위치에 문자열을 출력하는 함수
void cls(void); // 화면을 clrscr()의 Wrapper 함수
// 사용자가 그린 그림의 내용을 저장하는 25*25 2차원 문자 배열. 초기에는 공백으로 채워짐
char picture[25][25]=
   {{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
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
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}};

int longx=10,longy=10; // 사용자가 입력할 그림판의 가로 세로 크기 저장 변수
char last_saved_filename[100] = ""; // 최근 저장 파일 이름
int last_save_type = 0; // 1: quote, 2: plain
int main()
{
	atexit(clrscr);
	signal(SIGINT, handle_sigint);

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

		case '1': // *
			// prxy(nowx, nowy, "*");
			picture[picy][picx] = '*';
			break;
		case '2': // 0
			// prxy(nowx, nowy, "0");
			picture[picy][picx] = '0';
			break;
		case '3': // o
			// prxy(nowx, nowy, "o");
			picture[picy][picx] = 'o';
			break;

		case '4': // 공백
			// prxy(nowx, nowy, " ");
			picture[picy][picx] = ' ';
			break;

		case 'q': // 저장하고 나가기
			filesave(nowx, nowy);
			exit(0);

		case 'x': // 나가기
			exit(0);

		case 's': // 저장
			filesave(nowx, nowy);
			break;

		case 'l': // 불러오기
            checkFileType();
			cls();
			mon();
			break;

		case 'r': // 그림판 초기화
			for (int i = 0; i < longy; i++) for (int j = 0; j < longx; j++) picture[i][j] = ' ';
			break;

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
void copy_file(char *src, char *dest) { // 파일 src의 내용을 파일 dest로 복사
    FILE *in, *out;
    in = fopen(src, "r");
    if (!in) return;
    out = fopen(dest, "w");
    if (!out) { fclose(in); return; }

    int ch;
    while ((ch = fgetc(in)) != EOF)
        fputc(ch, out);

    fclose(in);
    fclose(out);
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

    fprintf(fp, "SIZE %d %d 1\n", longx, longy); // 저장 시점의 그림판 크기와 filetype 저장
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
    copy_file(filename, "latest_file.txt");
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

    fprintf(fp, "SIZE %d %d 2\n", longx, longy);
    putc('{',fp);
    putc('\n', fp);
    for(tempy=0;tempy<longy;tempy++)
    {
    len = 0;
    for(tempx=0;tempx<longx;tempx++)
    {
        buff2[len++] = picture[tempy][tempx];
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
    copy_file(filename, "latest_file.txt");
    prxy(45,22,"                         ");
}
void fileread1(FILE *fp, char *filename, int savex, int savey){
    //FILE *fp;
    //char filename[10], buff[20];
    char go;
    int tempx=0,tempy=0,c;
    //prxy(45,22,"File Name:");
    //scanf("%s",filename);
    //sprintf(buff,"%s",filename);
    //if((fp=fopen(buff,"rt"))==NULL){prxy(45,20,"File open error");exit(0);}
    //fscanf(fp, "SIZE %d %d", &savex, &savey); // 저장된 텍스트 파일의 그림판 크기 불러옴
    //while (getchar() != '\n'); // 개행 문자 제거

    if(savex != longx || savey != longy){ // 만약 그림판 크기가 저장된 크기와 다르면 사용자 입력을 받음(그림판 크기 변경 여부)
        prxy(45,20,"saved size is different from current size. want to resize? (y/n)");
        go = getch();
        prxy(45,20,"                                        ");
        switch(go){
            case 'Y':
            case 'y': longx = savex;longy = savey;break;
            case 'N':
            case 'n': break;
            default : prxy(45,20,"please type \'y\' or \'n\'");break; // 예외처리를 해야하나? 고민중
        }
        prxy(45,20,"                                        ");
    }
    for (int y = 0; y < 25; y++){ // 그림판 초기화
        for (int x = 0; x < 25; x++)
            picture[y][x] = ' ';
    }

    while((c = fgetc(fp)) != EOF){ // EOF 접근 시 탈출
        if (c == '\'') { // 여는 따옴표 접근시
            c = fgetc(fp); // 실제 문자 읽기
            if (tempy < longy && tempx < longx) { // 클리핑 기능 추가: 그림판 범위를 벗어나면 기록 X
                picture[tempy][tempx] = c;
            }
            tempx++;
            if (tempx >= savex) {
                if(tempy >= savey){
                    break;
                }else{
                    tempx = 0;
                    tempy++;
                }
            }
            fgetc(fp); // 닫는 따옴표 처리
        }else{
            continue; // 중괄호, 쉼표, 공백 처리
        }
    }
    fclose(fp);
    prxy(45,22,"                         ");
}
void fileread2(FILE *fp, char *buff, int savex, int savey){
    //FILE *fp;
    //char filename[10], buff[20];
    char go;
    int tempx=0,tempy=0,c;
    //prxy(45,22,"File Name:");
    //scanf("%s",filename);
    //sprintf(buff,"%s",filename);
    //if((fp=fopen(buff,"rt"))==NULL){prxy(45,20,"File open error");exit(0);}
    //fscanf(fp, "SIZE %d %d", &savex, &savey); // 저장된 텍스트 파일의 그림판 크기 불러옴
    //if((fp=fopen(buff,"rt"))==NULL){prxy(45,20,"File open error");exit(0);}

    //while (getchar() != '\n');

    if(savex != longx || savey != longy){ 
        prxy(45,22,"saved size is different from current size. want to resize? (y/n)");
        go = getch();
        prxy(45,22,"                                        ");
        switch(go){
            case 'Y':
            case 'y': longx = savex;longy = savey;break;
            case 'N':
            case 'n': break;
            default : prxy(45,20,"please type \'y\' or \'n\'");break;
        }
        prxy(45,22,"                                        ");
    }
    for (int y = 0; y < 25; y++){
        for (int x = 0; x < 25; x++)
            picture[y][x] = ' ';
    }
    
    while((c = fgetc(fp)) != EOF){ // EOF 접근 시 탈출
        if(c == '\"'){ // 여는 큰따옴표에 접근 할 때까지 문자 호출
            while((c = fgetc(fp)) != '\"'){ // 닫는 큰따옴표에 접근 할 때까지 문자 기록
                if (tempy < longy && tempx < longx) {
                picture[tempy][tempx] = c;
                }
                tempx++;
                if (tempx >= savex) {
                    if(tempy >= savey){
                        break;
                    }else{
                        tempx = 0;
                        tempy++;
                    }
                }
            }
        }else{
            continue;
        }
    }
    fclose(fp);
    prxy(45,22,"                         ");
}
void checkFileType(){
    FILE *fp;
    char filename[10], buff[20], go;
    int filetype, savex, savey;
    if((fp = fopen("latest_file.txt", "r")) != NULL){ // 최근 저장한 파일이 있으면 이를 불러올지 물어봄
        prxy(45,22,"load latest file? (y,n)");
        go = getch();
        prxy(45,22,"                                        ");
        if(go == 'y' || go == 'Y'){ // 최근 저장한 파일 불러오기
            prxy(45,22,"                                        ");
            fscanf(fp, "SIZE %d %d %d", &savex, &savey, &filetype); // 저장된 텍스트 파일의 그림판 크기 불러옴
            // while (getchar() != '\n'); // 개행 문자 제거
            if(filetype == 1){
                fileread1(fp, buff, savex, savey);
            }else{
                fileread2(fp, buff, savex, savey);
            }
        }else{ // 최근 저장한 파일이 아닌 다른 파일 불러오기. 예외처리 할지 고민중
            prxy(45,22,"File Name:");
            scanf("%s",filename);
            sprintf(buff,"%s",filename);
            if((fp=fopen(buff,"rt"))==NULL){prxy(45,20,"File open error\n");exit(0);} // 입력된 filename의 파일 불러옴.
            fscanf(fp, "SIZE %d %d %d", &savex, &savey, &filetype);
            if(filetype == 1){ // filetype에 따라 맞는 함수 호출(1: quote, 2: plain)
                fileread1(fp, buff, savex, savey);
            }else if(filetype == 2){
                fileread2(fp, buff, savex, savey);
            }else{
                prxy(45,22,"File Type error"); // File type error 처리. (부분구현)
                exit(0);
            }
        }
    }
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
            gotoxy(x-1, y);
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
 	prxy(48,14," r . reset");
 	prxy(48,15," s . save");
	prxy(48,16," l . load"); // file load 기능 추가
 	prxy(48,17," q . save & exit");
 	prxy(48,18," x . exit");
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
