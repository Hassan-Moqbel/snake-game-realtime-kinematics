#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define STARTLENGTH 5


COORD ncoord;
int SNAKESTARTX = 20;
int SNAKESTARTY = 25;
int XBORDER;
int YBORDER;
int length = STARTLENGTH;
int bend_no;
int len;
char key;
int life = 3;
int level;
char speed;
void Print_welcome();
void Start();
void GotoXY(int x, int y);
void Move();
void Right();
void Left();
void Down();
void Up();
void Bend();
void Erase();
void Loses_Management();
void Food();
int Score();
void Delay();
char Menu();
void Print_maze(int slevel);
void record();
struct coordinate
{
	int x;
	int y;
	int direction;
};
typedef struct coordinate coordinate;
coordinate head, bend[500], food, poison[4], body[30];
int main()
{
	Print_welcome();
	Start();
	return 0;
}
void Move()
{
	int a, i;
	Score();
	if (length >= 30)
	{
		level++;
		Start();
	}
	do
	{
		fflush(stdin);
		len = 0;
		if (speed == 'E' || speed == 'e')
		{
			Delay();
			Delay();
			Delay();
		}
		else if (speed == 'H' || speed == 'h')
			Delay();
		Erase();
		if (head.direction == RIGHT)
		{
			ncoord.X = head.x + 1;
			ncoord.Y = head.y;
			Right();
		}
		else if (head.direction == LEFT)
		{
			ncoord.X = head.x - 1;
			ncoord.Y = head.y;
			Left();
		}
		else if (head.direction == DOWN)
		{
			ncoord.X = head.x;
			ncoord.Y = head.y + 1;
			Down();
		}
		else if (head.direction == UP)
		{
			ncoord.X = head.x;
			ncoord.Y = head.y - 1;
			Up();
		}
		Loses_Management();
	} while (!kbhit());
	a = getch();
	if (a == 'M' || a == 'm')
		Menu();
	key = getch();
	if ((key == RIGHT && head.direction != LEFT && head.direction != RIGHT) || (key == LEFT && head.direction != RIGHT && head.direction != LEFT) || (key == UP && head.direction != DOWN && head.direction != UP) || (key == DOWN && head.direction != UP && head.direction != DOWN))
	{
		bend_no++;
		bend[bend_no] = head;
		head.direction = key;
		Move();
	}
	else
	{
		printf("\a");
		Move();
	}
}
///////////////////////////////////////////////////////////////////////////////////////// ////////////////////////
void GotoXY(int x, int y)
{
	HANDLE a;
	COORD b;
	fflush(stdout);
	b.X = x;
	b.Y = y;
	a = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(a, b);
}
///////////////////////////////////////////////////////////////////////////////////////// ///////////////////////////////
void Down()
{
	int i;
	for (i = 0; i <= (head.y - bend[bend_no].y) && len < length; i++)
	{
		GotoXY(head.x, head.y - i);
		{
			if (len == 0)
				printf("v");
			else
				printf("*");
		}
		body[len].x = head.x;
		body[len].y = head.y - i;
		len++;
	}
	Bend();
	head.y++;
}
void Delay()
{
	long double i;
	for (i = 0; i <= (10000000); i++)
		;
}
void Loses_Management()
{
	char a;
	int reti;
	HANDLE display = GetStdHandle(STD_OUTPUT_HANDLE);
	ReadConsoleOutputCharacter(display, &a, 1, ncoord, &reti);
	if (a == 35)
	{
		if (life <= 0)
			goto ENDGAME;
		life--;
	}
	else if (a == 64)
	{
		length++;
		Food();
	}
	else if (a != 32 && a != 64)
	{
		life--;
		if (life >= 0)
		{
			Print_maze(level);
			Move();
		}
		else
		{
		ENDGAME:
			system("cls");
			printf("All lives completed\nBetter Luck Next Time!!!\nPress any key to quit
the game\n");
            record();
            exit(0);
		}
	}
}
void Food()
{
	char a;
	int reti;
	COORD fcoord;
	HANDLE display = GetStdHandle(STD_OUTPUT_HANDLE);
	food.x = rand() % XBORDER;
	if (food.x == 0)
		food.x = 1;
	food.y = rand() % YBORDER;
	if (food.y <= 10)
		food.y += 11;
	ReadConsoleOutputCharacter(display, &a, 1, fcoord, &reti);
	while (a != 32)
	{
		if (food.x >= XBORDER / 2)
			food.x--;
		else
			food.x++;
		if (food.y >= YBORDER / 2)
			food.y--;
		else
			food.y++;
		fcoord.X = food.x;
		fcoord.Y = food.y;
		ReadConsoleOutputCharacter(display, &a, 1, fcoord, &reti);
	}
	GotoXY(food.x, food.y);
	printf("%c", 64);
}
void Left()
{
	int i;
	for (i = 0; i <= (bend[bend_no].x - head.x) && len < length; i++)
	{
		GotoXY((head.x + i), head.y);
		{
			if (len == 0)
				printf("<");
			else
				printf("*");
		}
		body[len].x = head.x + i;
		body[len].y = head.y;
		len++;
	}
	Bend();
	head.x--;
}
void Right()
{
	int i;
	for (i = 0; i <= (head.x - bend[bend_no].x) && len < length; i++)
	{
		body[len].x = head.x - i;
		body[len].y = head.y;
		GotoXY(body[len].x, body[len].y);
		{
			if (len == 0)
				printf(">");
			else
				printf("*");
		}
		len++;
	}
	Bend();
	head.x++;
}
void Bend()
{
	int i, j, diff;
	for (i = bend_no; i >= 0 && len < length; i--)
	{
		if (bend[i].x == bend[i - 1].x)
		{
			diff = bend[i].y - bend[i - 1].y;
			if (diff < 0)
				for (j = 1; j <= (-diff); j++)
				{
					body[len].x = bend[i].x;
					body[len].y = bend[i].y + j;
					GotoXY(body[len].x, body[len].y);
					printf("*");
					len++;
					if (len == length)
						break;
				}
			else if (diff > 0)
				for (j = 1; j <= diff; j++)
				{
					body[len].x = bend[i].x;
					body[len].y = bend[i].y - j;
					GotoXY(body[len].x, body[len].y);
					printf("*");
					len++;
					if (len == length)
						break;
				}
		}
		else if (bend[i].y == bend[i - 1].y)
		{
			diff = bend[i].x - bend[i - 1].x;
			if (diff < 0)
				for (j = 1; j <= (-diff) && len < length; j++)
				{
					body[len].x = bend[i].x + j;
					body[len].y = bend[i].y;
					GotoXY(body[len].x, body[len].y);
					printf("*");
					len++;
					if (len == length)
						break;
				}
			else if (diff > 0)
				for (j = 1; j <= diff && len < length; j++)
				{
					body[len].x = bend[i].x - j;
					body[len].y = bend[i].y;
					GotoXY(body[len].x, body[len].y);
					printf("*");
					len++;
					if (len == length)
						break;
				}
		}
	}
}
void Print_welcome()
{
	GotoXY(10, 10);
	printf("\tWelcome to the mini Snake game.(press any key to continue)\n");
	getch();
	system("cls");
	GotoXY(10, 10);
	printf("\tGame instructions:\n");
	printf("\n-> Use arrow keys to move the snake.\n");
	printf("\n-> Everytime you eat  food (@) the length of the snake will be increased by
1 element and thus the score.\n");
    printf("\n-> Here you are provided with three lives. Your life will decrease as you
hit the wall or snake's body or poison (#).\n");
    printf("\n-> You can pause the game in the middle by pressing any key. To continue
the paused game press any other key once again.\n");
    printf("\n\nPress any key to play the game...");
    getch();
}
void record()
{
	char plname[20], nplname[20], cha, c;
	int i, j, px;
	FILE *info;
	info = fopen("record.txt", "a+");
	getch();
	system("cls");
	printf("Enter your name\n");
	scanf("%[^\n]", plname);
	//************************
	for (j = 0; plname[j] != '\0'; j++)
	{
		nplname[0] = toupper(plname[0]);
		if (plname[j - 1] == ' ')
		{
			nplname[j] = toupper(plname[j]);
			nplname[j - 1] = plname[j - 1];
		}
		else
			nplname[j] = plname[j];
	}
	nplname[j] = '\0';
	//*****************************
	fprintf(info, "Player Name :%s\n", nplname);
	time_t mytime;
	mytime = time(NULL);
	fprintf(info, "Played Date:%s", ctime(&mytime));
	//**************************
	fprintf(info, "Score:%d\n", px = Score());
	system("cls");
	for (i = 0; i <= 50; i++)
		fprintf(info, "%c", '_');
	fprintf(info, "\n");
	fclose(info);
	printf("Wanna see past records press 'y'\n");
	cha = getch();
	system("cls");
	if (cha == 'y')
	{
		info = fopen("record.txt", "r");
		do
		{
			putchar(c = getc(info));
		} while (c != EOF);
	}
	fclose(info);
	cha = getch();
}
int Score()
{
	int score;
	GotoXY(20, 8);
	score = length - 5;
	printf("SCORE : %d", score);
	GotoXY(50, 8);
	printf("Life : %d", life);
	return score;
}
void Up()
{
	int i;
	for (i = 0; i <= (bend[bend_no].y - head.y) && len < length; i++)
	{
		GotoXY(head.x, head.y + i);
		{
			if (len == 0)
				printf("^");
			else
				printf("*");
		}
		body[len].x = head.x;
		body[len].y = head.y + i;
		len++;
	}
	Bend();
	head.y--;
}
void Erase()
{
	int i, j;
	for (i = 0; i <= length; i++)
	{
		GotoXY(body[i].x, body[i].y);
		printf(" ");
	}
}
void Start()
{
	system("cls");
	if (level == 0)
	{
		GotoXY(36, 14);
		printf("Choose the level of hardness");
		GotoXY(36, 15);
		printf("Easy 'E' \t\t Hard 'H'  ");
		GotoXY(36, 16);
		speed = 0;
		while (!(speed == 'e' || speed == 'E' || speed == 'h' || speed == 'H'))
			speed = getch();
	}
	system("cls");
	if (level == 3)
	{
		GotoXY(10, 10);
		printf("Congratulations!!!");
		GotoXY(10, 12);
		printf("You have won the game!");
		record();
		exit(0);
	}
	GotoXY(36, 14);
	printf(" Welcome to level %d", level + 1);
	printf("\n\t\t Press any key to play the game...");
	getch();
	system("cls");
	Print_maze(level);
	Move();
}
char Menu()
{
	int i, j;
	char a;
	GotoXY(90, 16);
	printf("Menu");
	GotoXY(90, 17);
	printf("press the corresponding letter to each");
	GotoXY(90, 18);
	printf("option to select it");
	GotoXY(90, 19);
	printf("Start a new game:  'N'");
	GotoXY(90, 20);
	printf("Exit:   'E'");
	GotoXY(90, 21);
	printf("Exit Menu:  'C'");
	GotoXY(90, 22);
	a = getch();
	//For starting a new game
	if (a == 78 || a == 110)
	{
		GotoXY(90, 23);
		printf("Are you sure you want to ");
		GotoXY(90, 24);
		printf("start a new game? ");
		GotoXY(90, 25);
		printf("Yes : 'Y'    No : 'N'");
		a = getch();
		if (a == 78 || a == 110)
		{
		flag:
			GotoXY(90, 23);
			for (i = 90; i < 128; i++)
			{
				for (j = 22; j < 26; j++)
				{
					GotoXY(i, j);
					printf(" ");
				}
			}
			Menu();
		}
		else if (a == 89 || a == 121)
		{
			level = 0;
			Start();
		}
		else
			goto flag;
	}
	//for erasing the menu
	else if (a == 99 || a == 67)
	{
		GotoXY(90, 16);
		for (i = 90; i < 131; i++)
		{
			for (j = 16; j < 30; j++)
			{
				GotoXY(i, j);
				printf(" ");
			}
		}
		return;
	}
	//for exiting
	else if (a == 69 || a == 101)
	{
		system("cls");
		exit(0);
	}
	else
		Menu();
}
void Print_maze(int slevel)
{
	int i;
	head.x = SNAKESTARTX;
	head.y = SNAKESTARTY;
	head.direction = RIGHT;
	XBORDER = 79;
	YBORDER = 38;
	bend_no = 0;
	bend[0] = head;
	poison[0].x = 61;
	poison[1].x = 43;
	poison[2].x = 8;
	poison[3].x = 35;
	poison[0].y = 15;
	poison[1].y = 21;
	poison[2].y = 27;
	poison[3].y = 32;
	char
		maze0[] = "//////////////////////////////////////////////////////////////////////////////// /\n|                                                                               |\n|  
				  |\n |
				  |\n |
				  |\n |
				  |\n |
				  |\n |
				  |\n |
				  |\n |
				  |\n |
				  |\n |
				  |\n |
				  |\n |
				  |\n | |\n |
				  |\n |
				  |\n |
				  |\n |
				  |\n |
				  |\n |
				  |\n |
				  |\n |
				  |\n |
				  |\n |
				  |\n |
				  |\n | |\n | |\n |
				  |\n | ////////////////////////////////////////////////////////////////////////////////\n" ;
				  char
					  maze1[] = "////////////////////////////////////////////////////////////////////////////////"
								"  /\n|             |                                    |          |                 |\n|  
								| | |				/////    |\n|       |
								|					//////////////////////////      |    |     |            |    |\n|       |     |
								| | | | | |\n | | | ///////////       |             |    |     |////////    |    |\n|             |
								| | | | | | | |\n | | |
								| | | | | | |\n | ///////////       |      |
								| | |			  /////////    |    |    |\n|                      |      |      |
								| | | | |\n | | | |
								| | | |\n |				  ////////////     |      |                    |
								| | | |\n | | | |		  /////////////       |////////     |
								| |\n | | | | | |\n | | | ///////////      |      |                       |    |\n|     |    |                                |      |       ///////     |///|    |\n|
								|						  //////////////////////////      |      |       |           |        |\n|          |                                |      |       |           |        |\n|      ////|
								| | |					  ////////        |\n|     |        /////////////////            |              |   |                |\n|     |
								| |						  ////////   |                |\n|     |     ////////////////////////        |      |           |     /////      |\n|     |
								| | | | | |\n | |
								| | | | | |\n | | ///////////////////
								| |				  ///////         |      |\n|     |                       |     |
								| | |\n | | | | |
								| | |\n | | ////////////////|     |       |
											///////              |\n|                                           |

								|\n | ////////////////////////////////////////////////////////////////////////////////\n" ;
								GotoXY(90, 13);
	printf("Press 'M' to access the menu:");
	GotoXY(0, 10);
	switch (slevel)
	{
	case 0:
		printf("%s", maze0);
		break;
	case 1:
		printf("%s", maze1);
		break;
	case 2:
	{
		printf("%s", maze1);
		for (i = 0; i < 4; i++)
		{
			GotoXY(poison[i].x, poison[i].y);
			printf("%c", 35);
		}
		break;
	}
	}
	Food();
}
