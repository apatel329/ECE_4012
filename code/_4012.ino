#include <PololuLedStrip.h>

PololuLedStrip<12> ledStrip;
PololuLedStrip<13> ledStrip1; 

#define LED_COUNT 30
rgb_color colors[LED_COUNT];
int i = 0;
int j = 0;
rgb_color blank;
rgb_color GREEN;
rgb_color RED;
rgb_color BLUE;

/* Defining Global Variables */

/* Move*/
unsigned int Whitemove = 1;
unsigned int Blackmove = 0;
/* Move*/

/* Pawn Variables*/
unsigned long long int AllWhitePieces = 0xFFFF000000000000LL;
unsigned long long int WhitePawns = 0x00FF000000000000LL;
unsigned long long int BlackPawns = 0x000000000000FF00LL;
unsigned long long int AllBlackPieces = 0x000000000000FFFFLL;
unsigned long long int move1 = 0x0000000000000000LL;
unsigned long long int move2 = 0x0000000000000000LL;
unsigned long long int FinalPawns = 0x0000000000000000LL;
int FinalPawnsLED[64];
int FinalPawnLEDs[8][8];
/* Pawn Variables*/

//Input array for 4x4
int Inputarray[16];
int Inputarray3d[4][4];

/* Function Prototypes */
void Convbin2array(int theValue, int array[]);
void Convarray22darray(int array1d[64], int array2d[8][8]);
void Convarray22darray16(int array1d[16], int array2d[4][4]);
void Pawns(void);
void Output(rgb_color PLAYERCOLOR, int data[8][8]);
//For 4x4 prototype
void Input(void);

void setup()
{
  blank.red = 0;
  blank.green = 0;
  blank.blue = 0; 
  GREEN.red = 0;
  GREEN.green = 255;
  GREEN.blue = 0;  
  BLUE.red = 0;
  BLUE.green = 0;
  BLUE.blue = 255;
  RED.red = 255;
  RED.green = 0;
  RED.blue = 0;
  
  for(uint16_t i = 0; i < LED_COUNT; i++)
  {
    colors[i] = blank;
  }
  ledStrip.write(colors, LED_COUNT);  
  ledStrip1.write(colors, LED_COUNT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  //Digital pins set to input
  pinMode(3, INPUT);
  //Might have to set Pullups
  pinMode(2, INPUT);
  //Might have to set Pullups
}
void loop()
{
  while(1)
  {
    Input();
    Convarray22darray16(Inputarray,Inputarray3d);
//    DetectPiece(); have to also take into account whose move it is.
//    Pawns();
//    Convbin2array(FinalPawns, FinalPawnsLED);
//    Convarray22darray(FinalPawnsLED,FinalPawnLEDs);
    Output(GREEN,FinalPawnLEDs);
  }
  delay(100);
}


/* Functions*/ 

void Convbin2array(int theValue, int array[])
{
  for (i = 0; i < 64; ++i) 
  {
    array[i] = theValue & (1 << i) ? 1 : 0;
  }
}

void Convarray22darray(int array1d[64], int array2d[8][8])
{
  for(j = 0; j < 8; j++)
  {
    for(i = 0; i < 8; i++)
    {
      array2d[j][i] = array1d[i + 8*j];
    }
  }
}

void Convarray22darray16(int array1d[16], int array2d[4][4])
{
  for(j = 0; j < 4; j++)
  {
    for(i = 0; i < 4; i++)
    {
      array2d[j][i] = array1d[i + 4*j];
    }
  }
}

void Pawns(void)
{
  if(Whitemove)
  {
    move1 = (WhitePawns >> 8) & (!AllWhitePieces);
    move1 = move1 & (!AllBlackPieces);
    //Need to account for killing a black piece
    move2 = ((move1 & 0x00FF000000000000LL) >> 8) & (!AllWhitePieces);
    move2 = move2 & (!AllBlackPieces);
    FinalPawns = move1 | move2;
  }
  else if(Blackmove)
  {
    move1 = (BlackPawns << 8) & (!AllBlackPieces);
    move1 = move1 & (!AllWhitePieces);
    //Need to account for killing a black piece
    move2 = ((move1 & 0x000000000000FF00LL) << 8) & (!AllBlackPieces);
    move2 = move2 & (!AllWhitePieces);
    FinalPawns = move1 | move2;    
  }
}



void Output(rgb_color PLAYERCOLOR, int data[8][8])
{
  for(j = 0; j < 8; j++)
  {
    for(i = 0; i < 8; i++)
    {
      if(data[j][i] == 1)
      {
        colors[i] = PLAYERCOLOR;
      }
      else if(data[j][i] == 2)
      {
        colors[i] = RED;
      }
    }
    if(j == 0) {ledStrip.write(colors, LED_COUNT);}
    else if(j == 1) {ledStrip1.write(colors, LED_COUNT);}
  }
}

void Input(void)
{
  uint16_t temp1 = 0;
  uint16_t temp2 = 0;
  uint16_t temp3 = 0;
  uint16_t temp4 = 0;
  uint16_t temp = 0;
  temp1 = ((digitalRead(A2) << 2) | (digitalRead(A1) << 1) | digitalRead(A0));
  if(temp1 == 7)
  {
    temp1 = 0;
  }
  if(digitalRead(2))
  {
    temp2 = 7;
  }
  else temp2 = 0;
  temp3 = ((digitalRead(A5) << 2) | (digitalRead(A4) << 1) | digitalRead(A3)) + 8;
  if(temp3 == 15)
  {
    temp3 = 0;
  }
  if(digitalRead(3))
  {
    temp4 = 15;
  }
  else temp4 = 0;
  temp = (temp1 + temp2) + (temp3 + temp4);
  if(digitalRead(A0) && digitalRead(A1) && digitalRead(A2) && digitalRead(A3) && digitalRead(A4) && digitalRead(A5) && (!digitalRead(2)) && (!digitalRead(3)))
  {
    for(i = 0; i < 16; i++)
      Inputarray[i] = 0;
  }
  else
  {
    Inputarray[temp] = 1;
  }
}



