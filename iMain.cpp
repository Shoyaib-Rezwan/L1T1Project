#include "iGraphics.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define ScreenWidth 1000
#define ScreenHeight 600
// GameState 0: Intro page, GameState 1: double name Input, GameState 2: About, GameState 3: Score,GameSate 4: Gamemode, GameState 5 : Single name Input, GameState 6 : main game,gamestate 7: Computer vs player, GameState 8: shuffling window, GameState 9-11 : rules,GameState 12: Pause
short int GameState = 0, NameIndex = 0, IntroIndex = 0, Sound = 1, NameState = 0, GameModeIndex = 2, SelectTime;
int isPlay = 0; // tracks whether the GameOver.wav has played for a single time or not;
int timer1;		// for cross-tick
int timer2;		// for change
int timer4;		// for time count of player 1
int timer5;		// for time count of player 2
int timer6;		// for the computer turn
int timer4on = 0, timer5on = 0;
int scorePlayer1 = 0;	// score count for player 1
int scorePlayer2 = 0;	// score count for player 2
int ScoringRate1 = 100; // scoring rate for player 1
int ScoringRate2 = 100; // scoring rate for player 2
int penalty1;			// calcualtes the penalty for the 2nd player
int penalty2;			// calcualtes the penalty for the 2nd player
char Time[2][10];		// tracks time for both players
char Score[2][10];		// Tracks score for both players
int shuffle = 0;
char name[2][30],
	Intro[2][30] = {
		"Picture//Intro1.bmp",
		"Picture//Intro2.bmp"};
char GameMode[14][30] = {
	"Picture//GameMode0.bmp",
	"Picture//GameMode1.bmp",
	"Picture//GameMode2.bmp",
	"Picture//GameMode3.bmp",
	"Picture//GameMode4.bmp",
	"Picture//GameMode5.bmp",
	"Picture//GameMode6.bmp",
	"Picture//GameMode7.bmp",
	"Picture//GameMode8.bmp",
	"Picture//GameMode9.bmp",
	"Picture//GameMode10.bmp",
	"Picture//GameMode11.bmp",
	"Picture//GameMode12.bmp",
	"Picture//GameMode13.bmp"};
char Files[5][50] = {
	"Text//ComputerVsPlayer.txt",
	"Text//20Cards3min.txt",
	"Text//20Cards5min.txt",
	"Text//30Cards3min.txt",
	"Text//30Cards5min.txt"};
/*Variables for GameState 6*/
int move1 = -1, move2 = -1, GameArrow = 1, CardNum, MaxCard, mx, my; // CardNum is to track the remianing card numbers and MaxCard keeps the number of highest card number
short int min1, sec1, min2, sec2;
/*Variables for tracking whether it is a turn of computer or not*/
int computer = 0;
/*Variables for ScoreUpdate()*/
char MaxScorer[100], CurrentMaxScorer[100];
int maxScore, CurrentMaxScore;
int compvsply = 0, mode203 = 0, mode205 = 0, mode303 = 0, mode305 = 0, mode20 = 0, mode30 = 0; // this variables determine that the player is playing which mode
/*Variables for name Input*/
char Nameinp[30] = "Picture//Name_Input.bmp";
int pause = 0;
char Animals[15][30] = {
	"Animals//Animal 1.bmp",
	"Animals//Animal 2.bmp",
	"Animals//Animal 3.bmp",
	"Animals//Animal 4.bmp",
	"Animals//Animal 5.bmp",
	"Animals//Animal 6.bmp",
	"Animals//Animal 7.bmp",
	"Animals//Animal 8.bmp",
	"Animals//Animal 9.bmp",
	"Animals//Animal 10.bmp",
	"Animals//Animal 11.bmp",
	"Animals//Animal 12.bmp",
	"Animals//Animal 13.bmp",
	"Animals//Animal 14.bmp",
	"Animals//Animal 15.bmp"};
struct card
{
	int cardX = 0;		  // card's x-cordinate
	int cardY = 0;		  // card's y-coordinate
	int found = 0;		  // checking for the card has been clicked or not
	int animal = 0;		  // stores animal's position
	int animal_found = 0; // tracing for animal match
	int Tick = 0;
	int Cross = 0;
} CardInfo[30];
void iGameBoard(void) // Initializes the properties of the game board for 30 cards
{
	// reset the cardInfo structure
	for (int i = 0; i < MaxCard; i++)
	{
		CardInfo[i].animal = CardInfo[i].animal_found = CardInfo[i].found = CardInfo[i].cardX = CardInfo[i].cardY = CardInfo[i].Cross = CardInfo[i].Tick = 0;
	}
	srand(time(NULL)); // this generates random number based on the current time
	// assaigning animals for 1st 15 cards
	for (int i = 0; i < MaxCard / 2; i++)
	{
		int random, check;
		do
		{
			check = 1;
			random = rand();
			random = random % (MaxCard / 2);
			for (int j = 0; j < i; j++)
			{
				if (random == CardInfo[j].animal)
				{
					check = 0;
					break;
				}
			}
		} while (check == 0);
		CardInfo[i].animal = random;
	}
	// assigning animals for last 15 cards
	for (int i = 0; i < MaxCard / 2; i++)
	{
		int random, check;
		do
		{
			check = 1;
			random = rand();
			random = random % (MaxCard / 2);
			for (int j = MaxCard / 2; j < MaxCard / 2 + i; j++)
			{
				if (random == CardInfo[j].animal)
				{
					check = 0;
				}
			}
		} while (check == 0);
		CardInfo[i + MaxCard / 2].animal = random;
	}
	int prod = 0;
	for (int i = 0; i < MaxCard; i++)
	{
		CardInfo[i].cardX = 15 + 100 * (prod);
		if (prod == 9)
		{
			prod = 0;
		}
		else
		{
			prod++;
		}
		CardInfo[i].cardY = 150 + (i / 10) * 150;
		// fprintf(fptr, "%d\t%d\n", CardInfo[i].cardX, CardInfo[i].cardY);
	}
}
void iGameBoard2(void) // Initializes the properties of the game board with 20 cards(We have to use this because rand function doesn't work with variables)
{
	// reset the cardInfo structure
	for (int i = 0; i < 20; i++)
	{
		CardInfo[i].animal = CardInfo[i].animal_found = CardInfo[i].cardX = CardInfo[i].cardY = CardInfo[i].Cross = CardInfo[i].found = CardInfo[i].Tick = 0;
		CardInfo[i].cardX = 150, CardInfo[i].cardY = 150;
	}

	srand(time(NULL)); // this generates random number based on the current time
	// assaigning animals for 1st 15 cards
	for (int i = 0; i < 10; i++)
	{
		int random, check;
		do
		{
			check = 1;
			random = rand() % 10;
			for (int j = 0; j < i; j++)
			{
				if (random == CardInfo[j].animal)
				{
					check = 0;
					break;
				}
			}
		} while (check == 0);
		CardInfo[i].animal = random;
	}
	// assigning animals for last 15 cards
	for (int i = 0; i < 10; i++)
	{
		int random, check;
		do
		{
			check = 1;
			random = rand() % 10;
			for (int j = 10; j < 10 + i; j++)
			{
				if (random == CardInfo[j].animal)
				{
					check = 0;
				}
			}
		} while (check == 0);
		CardInfo[i + 10].animal = random;
	}
	for (int i = 0; i < 6; i++) // cordinates for first 6 cards
	{
		CardInfo[i].cardX = 50 + 160 * i;
		CardInfo[i].cardY = 150;
	}
	for (int i = 0; i < 8; i++) // cordintes for the second 8 cards
	{
		CardInfo[i + 6].cardX = 15 + 120 * i;
		CardInfo[i + 6].cardY = 300;
	}
	for (int i = 0; i < 6; i++) // cordintes for the last 6 cards
	{
		CardInfo[i + 14].cardX = 50 + 160 * i;
		CardInfo[i + 14].cardY = 450;
	}
}
void DrawGameBoard(void) // draws the game board for game State 6
{
	iSetColor(15, 15, 15);
	iFilledRectangle(0, 0, ScreenWidth, 140);
	iSetColor(255, 255, 255);
	iFilledRectangle(50, 40, 200, 50);
	iSetColor(255, 255, 255);
	iFilledRectangle(750, 40, 200, 50);
	iShowBMP(260, 40, "Picture//Player1.bmp");
	iShowBMP(410, 40, "Picture//Shuffle_Button.bmp");
	if (GameState == 6)
		iShowBMP(690, 40, "Picture//Player2.bmp");
	else
		iShowBMP(690, 40, "Picture//computer.bmp");
	iText(100, 20, name[0], GLUT_BITMAP_TIMES_ROMAN_24);
	iText(800, 20, name[1], GLUT_BITMAP_TIMES_ROMAN_24);
	iSetColor(0, 255, 0);
	if (min1 < 1) // time color will be red if remaining time is less than 1 min
	{
		iSetColor(255, 0, 0);
	}
	iText(100, 100, Time[0], GLUT_BITMAP_TIMES_ROMAN_24);
	iSetColor(0, 255, 0);
	if (min2 < 1) // time color will be red if remaining time is less than 1 min
	{
		iSetColor(255, 0, 0);
	}
	if (timer5on)
	{
		iText(800, 100, Time[1], GLUT_BITMAP_TIMES_ROMAN_24);
	}
	/*Showing score of both players*/
	iSetColor(0, 0, 255);
	iText(100, 60, Score[0], GLUT_BITMAP_TIMES_ROMAN_24);
	iText(800, 60, Score[1], GLUT_BITMAP_TIMES_ROMAN_24);
	if (GameArrow == 1)
	{
		if (min1 != 0 || sec1 != 0)
		{
			iShowBMP(320, 40, "Picture//LeftArrow.bmp");
		}
		else
		{
			iShowBMP(630, 40, "Picture//RightArrow.bmp");
		}
	}
	if (GameArrow == 2)
	{
		if (min2 != 0 || sec2 != 0)
		{
			iShowBMP(630, 40, "Picture//RightArrow.bmp");
		}
		else
		{
			iShowBMP(320, 40, "Picture//LeftArrow.bmp");
		}
	}
	for (int i = 0; i < MaxCard; i++)
	{
		if (CardInfo[i].found == 0) // shows cards
		{
			iShowBMP(CardInfo[i].cardX, CardInfo[i].cardY, "Picture//Card.bmp");
		}
		else if (CardInfo[i].animal_found == 0) // shows animals when a card is clicked on
		{
			iShowBMP(CardInfo[i].cardX, CardInfo[i].cardY, Animals[CardInfo[i].animal]);
		}
		if (CardInfo[i].Tick)
		{
			iShowBMP2(CardInfo[i].cardX, CardInfo[i].cardY, "Picture//Tick.bmp", 0);
		}
		if (CardInfo[i].Cross)
		{
			iShowBMP2(CardInfo[i].cardX, CardInfo[i].cardY, "Picture//Cross.bmp", 0);
		}
	}
	/*Showing window after game over*/
	if ((min1 == 0 && min2 == 0 && sec1 == 0 && sec2 == 0) || CardNum == 0)
	{
		iShowBMP(0, 0, "Picture//GameOver.bmp");
		iSetColor(0, 255, 0);
		iText(420, 570, CurrentMaxScorer, GLUT_BITMAP_TIMES_ROMAN_24);
		if (CurrentMaxScore != maxScore)
		{
			iSetColor(255, 215, 0);
			iText(420, 520, "Has Won", GLUT_BITMAP_TIMES_ROMAN_24);
		}
		else
		{
			iSetColor(255, 0, 0);
			iText(400, 520, "Has Won and Made a new HIGH SCORE!!!", GLUT_BITMAP_TIMES_ROMAN_24);
		}
	}
}
void TurnComputer(void)
{
	srand(time(NULL));
	if (move1 == -1 && move2 == -1 && isPlay == 0 && GameState == 7 && (min2 > 0 || sec2 > 0))
	{
		PlaySound("Music//click.wav", NULL, SND_ASYNC);
		move1 = rand();
		move1 = move1 % MaxCard;
		while (CardInfo[move1].found == 1 || CardInfo[move1].animal_found == 1)
		{
			move1 = rand();
			move1 = move1 % MaxCard;
		}
		CardInfo[move1].found = 1;
	}
	else if (isPlay == 0 && GameState == 7 && (min2 > 0 || sec2 > 0))
	{
		PlaySound("Music//click.wav", NULL, SND_ASYNC);
		move2 = rand();
		move2 = move2 % MaxCard;
		while (CardInfo[move2].found == 1 || CardInfo[move2].animal_found == 1)
		{
			move2 = rand();
			move2 = move2 % MaxCard;
		}
		CardInfo[move2].found = 1;
		iPauseTimer(timer6);
		iResumeTimer(timer1);
	}
}
void Cross_tick()
{
	if (min1 == 0 && sec1 == 0)
	{
		timer4on = 0;
	}
	else if (min2 == 0 && sec2 == 0)
	{
		timer4on = 1;
	}
	if (move2 != -1 && CardInfo[move1].animal != CardInfo[move2].animal && isPlay == 0)
	{
		CardInfo[move1].Cross = CardInfo[move2].Cross = 1;
		PlaySound("Music//Wrong.wav", NULL, SND_ASYNC);
		/*Modifying Score deduce the scoring rate by 10 unit for every wrong move*/
		/*The scoring rate will not be less than 50*/
		if (timer4on && ScoringRate1 > 50)
		{
			ScoringRate1 -= 10;
		}
		if (!timer4on && ScoringRate2 > 50)
		{
			ScoringRate2 -= 10;
		}
	}
	if (move2 != -1 && CardInfo[move1].animal == CardInfo[move2].animal && isPlay == 0)
	{
		CardInfo[move1].Tick = CardInfo[move2].Tick = 1;
		PlaySound("Music//Correct.wav", NULL, SND_ASYNC);
		/*Modifying Score*/
		if (timer4on)
		{
			scorePlayer1 += ScoringRate1;
			sprintf(Score[0], "%04d", scorePlayer1);
			ScoringRate1 += 10; // increasing the scoring rate by 10 for each correct move
		}
		else
		{
			scorePlayer2 += ScoringRate2;
			sprintf(Score[1], "%04d", scorePlayer2);
			ScoringRate2 += 10;
		}
		CardNum -= 2;
	}
	iPauseTimer(timer1);
	iResumeTimer(timer2);
	if ((timer4on || (min1 == 0 && sec1 == 0)))
	{
		iPauseTimer(timer4);
		iResumeTimer(timer5);
		timer5on = 1;
		timer4on = 0;
	}
	else if (!timer4on || (min2 == 0 && sec2 == 0))
	{
		timer4on = 1;
		iResumeTimer(timer4);
		iPauseTimer(timer5);
	}
	if ((min1 != 0 || sec1 != 0) && (min2 == 0 && sec2 == 0))
	{
		timer4on = 1;
		iResumeTimer(timer4);
		iPauseTimer(timer5);
	}
	// changing the pointer arrow to the player who will give the next 2 moves
	GameArrow = 3 - GameArrow;
	// Handling the cases when one player has reached hos time limit
}
void Change(void)
{
	if (move2 != -1)
	{
		if (CardInfo[move1].animal != CardInfo[move2].animal)
		{
			CardInfo[move1].found = 0;
			CardInfo[move2].found = 0;
		}
		else
		{
			CardInfo[move1].animal_found = CardInfo[move2].animal_found = CardInfo[move1].found = CardInfo[move2].found = 1;
		}
		CardInfo[move1].Cross = 0;
		CardInfo[move2].Cross = 0;
		CardInfo[move1].Tick = 0;
		CardInfo[move2].Tick = 0;
		move1 = -1;
		move2 = -1;
		if (GameState == 7)
		{
			computer = 1 - computer;
			if (computer || (min1 <= 0 && sec1 <= 0 && (min2 > 0 || sec2 > 0)))
				iResumeTimer(timer6);
		}
		iPauseTimer(timer2);
	}
}
void NameInput(unsigned char key)
{
	if (key != '\b' && key != '\r')
	{
		name[NameState][NameIndex] = key;
		NameIndex++;
		name[NameState][NameIndex] = '\0';
	}
	else if (key == '\b' && NameIndex)
	{
		NameIndex--;
		name[NameState][NameIndex] = '\0';
	}
	else if (key == '\r')
	{
		if (NameState == 0)
		{
			NameState++;
			NameIndex = 0;
			strcpy(Nameinp, "Picture//Name_Input_1.bmp");
		}

		else
		{
			GameState = (GameState == 1) ? 6 : 7;
			if (GameState == 7)
			{
				/*We have taken input of the */
				strcpy(name[0], name[1]);
				strcpy(name[1], "Computer");
			}
			iResumeTimer(timer4);
			timer4on = 1;
		}
	}
}
void ScoreUpdate(void)
{
	/*Handling the draw as we don't want a draw*/
	if (scorePlayer1 == scorePlayer2)
	{
		scorePlayer1 -= penalty1;
		scorePlayer2 -= penalty2;
	}
	FILE *fptr;
	char FileName[100];
	if (compvsply)
		strcpy(FileName, Files[0]);
	if (mode203)
		strcpy(FileName, Files[1]);
	if (mode205)
		strcpy(FileName, Files[2]);
	if (mode303)
		strcpy(FileName, Files[3]);
	if (mode305)
		strcpy(FileName, Files[4]);
	fptr = fopen(FileName, "r");
	fscanf(fptr, "%[^\n]%d", MaxScorer, &maxScore);
	if (scorePlayer1 > scorePlayer2)
	{
		CurrentMaxScore = scorePlayer1;
		strcpy(CurrentMaxScorer, name[0]);
	}
	else
	{
		CurrentMaxScore = scorePlayer2;
		strcpy(CurrentMaxScorer, name[1]);
	}
	if (CurrentMaxScore >= maxScore)
	{
		maxScore = CurrentMaxScore;
		if (maxScore == scorePlayer1)
		{
			strcpy(MaxScorer, name[0]);
		}
		else
		{
			strcpy(MaxScorer, name[1]);
		}
	}
	fclose(fptr);
	fptr = fopen(FileName, "w");
	fprintf(fptr, "%s\n%d", MaxScorer, maxScore);
	fclose(fptr);
}
void ClearScores(void)
{
	FILE *fptr = NULL;
	for (int i = 0; i < 5; i++)
	{
		fptr = fopen(Files[i], "w");
		fprintf(fptr, "<No_Name>\n0");
		fclose(fptr);
	}
}
void TimePlayer1(void) // time period for player 1
{
	if (60 * min1 + sec1 < 60 * min2 + sec2) // whoever lags in time will have a penalty of 10 points
	{
		penalty2 = 0;
		penalty1 = 10;
	}
	else if (60 * min1 + sec1 > 60 * min2 + sec2)
	{
		penalty2 = 10;
		penalty1 = 0;
	}
	if (sec1 > 0)
	{
		sec1--;
	}
	else if (min1 > 0)
	{
		min1--;
		sec1 = 59;
	}
	sprintf(Time[0], "%d : %02d", min1, sec1);
	if (min1 == 0 && sec1 == 0)
	{
		timer5on = 1;
		timer4on = 0;
		iResumeTimer(timer5);
		iResumeTimer(timer6);
		iPauseTimer(timer4);
		if (GameState == 7)
		{
			TurnComputer();
		}
	}
	if (!isPlay && (((min1 <= 0 && sec1 <= 0) && (min2 <= 0 && sec2 <= 0)) || CardNum == 0))
	{
		PlaySound(0, 0, 0);
		PlaySound("Music//GameOver.wav", NULL, SND_ASYNC);
		isPlay = 1;
		ScoreUpdate();
		iPauseTimer(timer4);
		iPauseTimer(timer6);
	}
}
void TimePlayer2(void) // time period for player 2
{
	if (60 * min1 + sec1 < 60 * min2 + sec2) // whoever lags in time will have a penalty of 10 points
	{
		penalty2 = 0;
		penalty1 = 10;
	}
	else if (60 * min1 + sec1 > 60 * min2 + sec2)
	{
		penalty2 = 10;
		penalty1 = 0;
	}
	if (sec2 > 0)
	{
		sec2--;
	}
	else if (min2 > 0)
	{
		min2--;
		sec2 = 59;
	}
	sprintf(Time[1], "%d : %02d", min2, sec2);
	if (min2 == 0 && sec2 == 0)
	{
		computer = 0;
		timer4on = 1;
		timer5on = 0;
		iResumeTimer(timer4);
		iPauseTimer(timer6);
		iPauseTimer(timer5);
	}
	if (!isPlay && (((min1 <= 0 && sec1 <= 0) && (min2 <= 0 && sec2 <= 0)) || CardNum == 0))
	{
		PlaySound(0, 0, 0);
		PlaySound("Music//GameOver.wav", NULL, SND_ASYNC);
		isPlay = 1;
		ScoreUpdate();
		iPauseTimer(timer5);
		iPauseTimer(timer6);
	}
}
void DrawScoreBoard(void)
{
	char Name[100];
	int Scores;
	FILE *fptr = fopen(Files[0], "r");
	fscanf(fptr, "%[^\n]%d", Name, &Scores);
	fclose(fptr);
	iSetColor(0, 0, 255);
	iText(281, 445, Name, GLUT_BITMAP_TIMES_ROMAN_24);
	itoa(Scores, Name, 10);
	iText(619, 445, Name, GLUT_BITMAP_TIMES_ROMAN_24);
	fptr = fopen(Files[1], "r");
	fscanf(fptr, "%[^\n]%d", Name, &Scores);
	fclose(fptr);
	iText(281, 380, Name, GLUT_BITMAP_TIMES_ROMAN_24);
	itoa(Scores, Name, 10);
	iText(619, 380, Name, GLUT_BITMAP_TIMES_ROMAN_24);
	fptr = fopen(Files[2], "r");
	fscanf(fptr, "%[^\n]%d", Name, &Scores);
	fclose(fptr);
	iText(281, 305, Name, GLUT_BITMAP_TIMES_ROMAN_24);
	itoa(Scores, Name, 10);
	iText(619, 305, Name, GLUT_BITMAP_TIMES_ROMAN_24);
	fptr = fopen(Files[3], "r");
	fscanf(fptr, "%[^\n]%d", Name, &Scores);
	fclose(fptr);
	iText(281, 235, Name, GLUT_BITMAP_TIMES_ROMAN_24);
	itoa(Scores, Name, 10);
	iText(619, 235, Name, GLUT_BITMAP_TIMES_ROMAN_24);
	fptr = fopen(Files[4], "r");
	fscanf(fptr, "%[^\n]%d", Name, &Scores);
	fclose(fptr);
	iText(281, 160, Name, GLUT_BITMAP_TIMES_ROMAN_24);
	itoa(Scores, Name, 10);
	iText(619, 160, Name, GLUT_BITMAP_TIMES_ROMAN_24);
}
void iDraw()
{
	// place your drawing codes here
	iClear();
	if (GameState == 0)
	{
		iShowBMP(0, 0, Intro[IntroIndex]);
	}
	// drawing in game state 1
	if (GameState == 1)
	{
		iShowBMP(0, 0, Nameinp);
		iSetColor(255, 255, 255);
		iText(45, 473, name[0], GLUT_BITMAP_TIMES_ROMAN_24);
		iText(45, 293, name[1], GLUT_BITMAP_TIMES_ROMAN_24);
	}
	if (GameState == 2)
	{
		iShowBMP(0, 0, "Picture//About.bmp");
	}
	if (GameState == 3)
	{
		iShowBMP(0, 0, "Picture//score.bmp");
		DrawScoreBoard();
	}
	if (GameState == 4)
	{
		iShowBMP(0, 0, GameMode[GameModeIndex]);
		iSetColor(0, 0, 0);
		iText(0, 10, "Press left arrow to go back.", GLUT_BITMAP_TIMES_ROMAN_24);
	}
	if (GameState == 6 || GameState == 7)
	{
		DrawGameBoard();
	}
	if (GameState == 5)
	{
		iShowBMP(0, 0, "Picture//Name_Input_2.bmp");
		iSetColor(255, 255, 255);
		iText(45, 293, name[1], GLUT_BITMAP_TIMES_ROMAN_24);
	}
	if (GameState == 8)
	{

		shuffle++;
		if (shuffle <= 20)
			iShowBMP(0, 0, "Picture//Shuffle1.bmp");
		else if (shuffle <= 40)
			iShowBMP(0, 0, "Picture//Shuffle2.bmp");
		else if (shuffle <= 60)
			iShowBMP(0, 0, "Picture//Shuffle3.bmp");
		else if (shuffle <= 80)
			iShowBMP(0, 0, "Picture//Shuffle1.bmp");
		else if (shuffle <= 100)
			iShowBMP(0, 0, "Picture//Shuffle2.bmp");
		else if (shuffle <= 120)
			iShowBMP(0, 0, "Picture//Shuffle3.bmp");

		else
		{
			if (compvsply == 1)
			{
				GameState = 7;
			}
			else
				GameState = 6;
		}
	}
	if (GameState == 9)
	{
		iShowBMP(0, 0, "Picture//About1.bmp");
	}
	if (GameState == 10)
	{
		iShowBMP(0, 0, "Picture//About2.bmp");
	}
	if (GameState == 11)
	{
		iShowBMP(0, 0, "Picture//About3.bmp");
	}
	if (GameState == 12)
	{
		iShowBMP(0, 0, "Picture//Pause.bmp");
	}

} /*
	  function iMouseMove() is called when the user presses and drags the mouse.
	  (mx, my) is the position where the mouse pointer is.
	  */
void iMouseMove(int mx, int my)
{
}
/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/

void Shuffle(void)
{
	int Map[MaxCard / 2];
	memset(Map, 0, MaxCard / 2 * sizeof(int));
	for (int i = 0; i < MaxCard / 2; i++)
	{
		if (CardInfo[i].animal_found == 1)
		{
			Map[CardInfo[i].animal] = 1;
		}
	}
	char temp_animal[CardNum / 2][30];
	int j = 0;
	for (int i = 0; i < MaxCard / 2; i++)
	{
		if (Map[i] == 0)
		{
			strcpy(temp_animal[j], Animals[i]);
			j++;
		}
	}
	for (int i = 0; i < CardNum / 2; i++)
	{
		strcpy(Animals[i], temp_animal[i]);
	}
}
void RefreshGame(void)
{
	GameState = 0, NameIndex = 0, IntroIndex = 0, Sound = 1, NameState = 0, GameModeIndex = 2;
	isPlay = 0;
	timer4on = 0, timer5on = 0;
	scorePlayer1 = 0;
	scorePlayer2 = 0;
	ScoringRate1 = 100;
	ScoringRate2 = 100;
	shuffle = 0;
	move1 = -1, move2 = -1, GameArrow = 1;
	compvsply = 0, mode203 = 0, mode205 = 0, mode303 = 0, mode305 = 0, mode20 = 0, mode30 = 0;

	iPauseTimer(timer1);
	iPauseTimer(timer2);
	iPauseTimer(timer4);
	iPauseTimer(timer5);
	iPauseTimer(timer6);
	strcpy(Nameinp, "Picture//Name_Input.bmp");
	pause = 0;
	computer = 0;
	for (int i = 0; i < 30; i++)
	{
		CardInfo[i].animal = CardInfo[i].animal_found = CardInfo[i].found = CardInfo[i].cardX = CardInfo[i].cardY = CardInfo[i].Cross = CardInfo[i].Tick = 0;
	}
	scorePlayer1 = 0;
	scorePlayer2 = 0;
	sprintf(Score[0], "%04d", scorePlayer1);
	sprintf(Score[1], "%04d", scorePlayer2);
}
void iKeyboard(unsigned char key)
{
	if (GameState == 1 || GameState == 5)
	{
		NameInput(key);
	}
	if ((GameState == 6 || GameState == 7 || GameState == 12) && key == ' ')
	{
		if (timer4on && pause == 0)
		{
			iPauseTimer(timer4);
			pause = 1;
			GameState = 12;
		}
		else if (timer5on && pause == 0 && GameState == 6)
		{
			iPauseTimer(timer5);
			pause = 1;
			GameState = 12;
		}
		else if (timer4on && pause == 1)
		{
			iResumeTimer(timer4);
			pause = 0;
			GameState = (compvsply) ? 7 : 6;
		}
		else if (timer5on && pause == 1 && GameState == 6)
		{
			iResumeTimer(timer5);
			pause = 0;
			GameState = (compvsply) ? 7 : 6;
		}
	}
}
void iMouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (Sound)
		{
			PlaySound("Music//click.wav", NULL, SND_ASYNC);
		}
		FILE *fptr = fopen("cursor.txt", "a");
		fprintf(fptr, "%d\t%d\n", mx, my);
		fclose(fptr);
		//  mouse activities in GameState 0
		if (GameState == 0)
		{

			Sleep(200);
			if (mx >= 1 && mx <= 74 && my >= 2 && my <= 74) // sound
			{
				IntroIndex = 1 - IntroIndex;
				Sound = (Sound > 0) ? 0 : 1;
			}
			if (mx >= 303 && mx <= 707 && my >= 302 && my <= 376) // start
			{
				GameState = 4;
				mx = 0, my = 0;
			}
			if (mx >= 321 && mx <= 680 && my >= 122 && my <= 184) // about
			{
				GameState = 2;
				mx = 0, my = 0;
			}
			if (mx >= 320 && mx <= 679 && my >= 209 && my <= 271) // score
			{
				GameState = 3;
				mx = 0, my = 0;
			}
			if (mx >= 322 && mx <= 680 && my >= 31 && my <= 96) // rules
			{
				GameState = 9;
				mx = 0, my = 0;
			}
			if (mx >= 930 && mx <= 976 && my >= 8 && my <= 66) // exit
			{
				exit(0);
			}
		}
		// Mouse activities in GameState 1
		if (GameState == 1 || GameState == 5)
		{
			Sleep(500);
			if (mx >= 21 && mx <= 141 && my >= 162 && my <= 202) // exit the game while pressing the the quit button
			{
				exit(0);
			}
			if (mx >= 276 && mx <= 569 && my >= 162 && my <= 202) // preesing the proceed button
			{
				if (GameState == 1)
					GameState = 6;
				else
				{
					GameState = 7;
					/*We have taken input of the */
					strcpy(name[0], name[1]);
					strcpy(name[1], "Computer");
				}
				mx = 0, my = 0; // this is for safety measure otherwise two different symbols at two different gamestates will be clicked at a time.
				iResumeTimer(timer4);
				timer4on = 1;
			}
			if (mx >= 13 && mx <= 86 && my >= 10 && my <= 80) // pressing back button
			{
				GameState = 4;
				mx = 0, my = 0;
			}
		}
		// Mouse Activities in Game State 2
		if (GameState == 2)
		{
			Sleep(500);
			if (mx >= 13 && mx <= 86 && my >= 10 && my <= 80)
			{
				GameState = 0;
				mx = 0, my = 0;
			}
		}
		// Mouse Activities in Game State 3
		if (GameState == 3)
		{
			Sleep(500);
			// FILE *fptr = fopen("cursor.txt", "a");
			// fprintf(fptr, "%d %d\n", mx, my);
			if (mx >= 13 && mx <= 86 && my >= 10 && my <= 80) // preesing back
			{
				GameState = 0;
				mx = 0, my = 0;
			}
			// fclose(fptr);
			if (mx >= 332 && mx <= 573 && my >= 20 && my <= 67) // preesing Clear scoreboard
			{
				ClearScores();
			}
		}
		// Mouse Activities in Game State 4
		if (GameState == 4)
		{
			Sleep(200);
			if (mx >= 125 && mx <= 418 && my >= 386 && my <= 445 && GameModeIndex >= 2) // computer vs Player
			{
				GameModeIndex = 3;
				mx = 0, my = 0;
				// SelectTime = 3;
				min1 = min2 = 3;
				sec1 = sec2 = 0;
				CardNum = MaxCard = 30;
				compvsply = 0, mode203 = 0, mode205 = 0, mode303 = 0, mode305 = 0, mode20 = 0, mode30 = 0;
				compvsply = 1;
			}
			if (mx >= 126 && mx <= 418 && my >= 274 && my <= 330 && GameModeIndex >= 2) // 2 players
			{
				GameModeIndex = 6;
				mx = 0, my = 0;
			}
			if (mx >= 392 && mx <= 650 && my >= 201 && my <= 242 && GameModeIndex >= 6) // 20 Cards
			{
				GameModeIndex = 8;
				mx = 0, my = 0;
				CardNum = MaxCard = 20;
				compvsply = 0, mode203 = 0, mode205 = 0, mode303 = 0, mode305 = 0, mode20 = 0, mode30 = 0;
				mode20 = 1;
			}
			if (mx >= 392 && mx <= 650 && my >= 114 && my <= 155 && GameModeIndex >= 6) // 30 Cards
			{
				GameModeIndex = 11;
				mx = 0, my = 0;
				CardNum = MaxCard = 30;
				compvsply = 0, mode203 = 0, mode205 = 0, mode303 = 0, mode305 = 0, mode20 = 0, mode30 = 0;
				mode30 = 1;
			}
			if (mx >= 780 && mx <= 974 && my >= 227 && my <= 272 && GameModeIndex >= 8) // 3 Minutes
			{
				GameModeIndex = (GameModeIndex >= 8 && GameModeIndex <= 10) ? 9 : 12;
				mx = 0, my = 0;
				min1 = min2 = 3;
				sec1 = sec2 = 0;
				compvsply = 0, mode203 = 0, mode205 = 0, mode303 = 0, mode305 = 0;
				if (mode20)
					mode203 = 1;
				else
					mode303 = 1;
			}
			if (mx >= 780 && mx <= 974 && my >= 82 && my <= 122 && GameModeIndex >= 8) // 5 Minutes
			{
				GameModeIndex = (GameModeIndex >= 8 && GameModeIndex <= 10) ? 10 : 13;
				mx = 0, my = 0;
				min1 = min2 = 5;
				sec1 = sec2 = 0;
				compvsply = 0, mode203 = 0, mode205 = 0, mode303 = 0, mode305 = 0;
				if (mode20)
					mode205 = 1;
				else
					mode305 = 1;
			}
			if (mx >= 365 && mx <= 655 && my >= 19 && my <= 57 && (GameModeIndex == 3 || GameModeIndex >= 9)) // proceed button
			{
				GameState = (GameModeIndex == 3) ? 5 : 1;
				NameState = (GameModeIndex == 3) ? 1 : 0;
				strcpy(name[0], "");
				strcpy(name[1], "");
				NameIndex = 0;
				mx = 0, my = 0;
				if (CardNum == 30)
				{
					iGameBoard();
				}
				if (CardNum == 20)
				{
					iGameBoard2();
				}
			}
		}
		if (GameState == 6)
		{

			for (int i = 0; i < MaxCard; i++)
			{
				if (mx >= CardInfo[i].cardX && mx <= CardInfo[i].cardX + 71 && my >= CardInfo[i].cardY && my <= CardInfo[i].cardY + 104 && CardInfo[i].found != 1 && (move1 == -1 || move2 == -1))
				{

					CardInfo[i].found = 1;
					if (move1 == -1)
					{
						move1 = i;
					}
					else
					{
						move2 = i;
						iResumeTimer(timer1);
					}
					break;
				}
			}
			if (mx >= 410 && mx <= 555 && my >= 43 && my <= 77)
			{
				shuffle = 0;
				Shuffle();
				MaxCard = CardNum;
				iGameBoard();
				GameState = 8;
			}
			if (mx >= 315 && mx <= 422 && my >= 113 && my <= 139) // gameover yes
			{
				RefreshGame();
			}
			if (mx >= 602 && mx <= 681 && my >= 113 && my <= 139) // gameover no
			{
				exit(0);
			}
		}
		if (GameState == 7)
		{
			for (int i = 0; i < MaxCard; i++)
			{
				if (mx >= CardInfo[i].cardX && mx <= CardInfo[i].cardX + 71 && my >= CardInfo[i].cardY && my <= CardInfo[i].cardY + 104 && CardInfo[i].found != 1 && (move1 == -1 || move2 == -1) && (computer == 0 || min2 <= 0 && sec2 <= 0))
				{

					CardInfo[i].found = 1;
					if (move1 == -1)
					{
						move1 = i;
					}
					else
					{
						move2 = i;
						iResumeTimer(timer1);
					}
					break;
				}
			}
			if (mx >= 410 && mx <= 555 && my >= 43 && my <= 77 && !computer) // shuffle
			{
				shuffle = 0;
				Shuffle();
				MaxCard = CardNum;
				iGameBoard();
				GameState = 8;
			}
			if (mx >= 315 && mx <= 422 && my >= 113 && my <= 139) // gameover yes
			{
				RefreshGame();
			}
			if (mx >= 602 && mx <= 681 && my >= 113 && my <= 139) // gameover no
			{
				exit(0);
			}
		}
		if (GameState == 9)
		{
			if (mx >= 13 && mx <= 86 && my >= 10 && my <= 80) // pressing back button
			{
				GameState = 0;
				mx = 0, my = 0;
			}
			if (mx >= 914 && mx <= 983 && my >= 10 && my <= 80) // pressing next button
			{
				GameState = 10;
				mx = 0, my = 0;
			}
		}
		if (GameState == 10)
		{
			if (mx >= 13 && mx <= 86 && my >= 10 && my <= 80) // pressing back button
			{
				GameState = 9;
				mx = 0, my = 0;
			}
			if (mx >= 914 && mx <= 983 && my >= 10 && my <= 80) // pressing next button
			{
				GameState = 11;
				mx = 0, my = 0;
			}
		}
		if (GameState == 11)
		{
			if (mx >= 13 && mx <= 86 && my >= 10 && my <= 80) // pressing back button
			{
				GameState = 10;
				mx = 0, my = 0;
			}
			if (mx >= 914 && mx <= 983 && my >= 10 && my <= 80) // pressing next button
			{
				GameState = 0;
				mx = 0, my = 0;
			}
		}
		if (GameState == 12)
		{
			if ((mx >= 400 && mx <= 600 && my >= 200 && my <= 242) || (mx >= 372 && mx <= 627 && my >= 303 && my <= 351))
			{
				iKeyboard(' ');
			}
			if ((mx >= 399 && mx <= 602 && my >= 141 && my <= 182) || (mx >= 452 && mx <= 507 && my >= 469 && my <= 531))
			{
				RefreshGame();
			}
			if (mx >= 819 && mx <= 900 && my >= 354 && my <= 861)
			{
				exit(0);
			}
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		// place your codes here
	}
}
/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/

void iSpecialKeyboard(unsigned char key)
{

	if (key == GLUT_KEY_END)
	{
		exit(0);
	}
	if (GameState == 4 && key == GLUT_KEY_LEFT)
	{
		GameState = 0;
	}
	// place your codes for other keys here
}
/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/

int main()
{
	// place your own initialization codes here.
	timer1 = iSetTimer(1200, Cross_tick);
	timer2 = iSetTimer(1200, Change);
	timer4 = iSetTimer(1000, TimePlayer1);
	timer5 = iSetTimer(1000, TimePlayer2);
	timer6 = iSetTimer(1500, TurnComputer);
	iPauseTimer(timer1);
	iPauseTimer(timer2);
	iPauseTimer(timer4);
	iPauseTimer(timer5);
	iPauseTimer(timer6);
	sprintf(Score[0], "%04d", scorePlayer1);
	sprintf(Score[1], "%04d", scorePlayer2);
	iInitialize(ScreenWidth, ScreenHeight, "Word Maching Game: Match The Animals");
	return 0;
}
