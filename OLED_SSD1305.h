
//---------------------------------------------------
//|  												|
//|   			DISPLAY_OLED_128x64					|
//|  												|
//|   Controller: SSD1305							|
//|   Interface: 8bits parallel 8080-series			|
//|   For MPLAB complier C18, PIC18F4520 
//|  												|
//---------------------------------------------------

//Define Constants for the Functions
#include "font.h"
#include <xc.h>
#include "delays.h"
#define CONTROL_PORT		PORTA
#define CONTROL_PORT_CONF	TRISA
#define DATA_PORT			PORTD
#define DATA_PORT_CONF		TRISD

//Define Constants for the SSD1305 controller
#define SCREEN_WIDTH	128

#define DISPLAY_OFF		0xAE //Display OFF (sleep mode) (RESET)
#define DISPLAY_ON		0xAF //Display ON in Normal Mode
#define SETDCLK			0xD5 //Generate DCLK from CLK
#define DCLK			0x80 //Obteined Osc Frec approx 400khz
#define SETMUX			0xA8 //Set mux ratio
#define MUX				0x3F //64 MUX
#define SETOFFSET		0xD3 //Move Image in screen
#define OFFSET			0x00 //No Offset
#define SETSTARTLINE	0x40 //Line #0
#define	RE_MAP0			0xA0 //Column adress 0 mapped to SEG0 (Direc Left to Right)
#define RE_MAP1			0xA1 //Column adress 131 mapped to SEG0 (Direc Right to Left)
#define COMSCANDIREC0	0xC0 //Direc Down to Up
#define COMSCANDIREC1	0xC8 //Direc Up to Down
#define SETCOMPINS		0xDA //alternative COM Pin Conf(enable/disable)
#define COMPINS			0x12 //Disable, alternative COM Pin Conf
#define SETCONTRAST		0x81 //1 out of 256 contrast steps
#define CONTRAST		0xff //Medium contrast, (segment courrent approx 350uA for Max Contrast)
#define SETPRECHARGE	0xD9 //Set duration of precharge period
#define PRECHARGE		0xF1 //Precharge period
#define SETVCOMH		0xDB //Adjusts the Vconh regulator output
#define VCOMH			0x00 //~0.43xVcc
#define ENTIREDISPLAY0	0xA4 //Output follows RAM content		
#define ENTIREDISPLAY1	0xA5 //Entire Display ON, Output ignore RAM content
#define NORMALDISPLAY	0xA6 //0 in RAM => OFF in Display ; 1 in RAM => ON in Display
#define INVERTDISPLAY	0xA7 //1 in RAM => OFF in Display ; 0 in RAM => ON in Display
#define SETMEMORY		0x20 //Memory Adressing
#define MEMORY			0x00 //Horizontal Adressing Mode
#define SETLOWCOLUMN	0x00 //Lower nibble=0
#define SETHIGHCOLUMN	0x10 //Higher nibble=0
#define SETSTARTCOLUMN	0x21 //Define Width Screen (0 - 127)
#define SETSTARTPAGE	0x22 //Define Widht Pages (0 - 7)
#define SETPAGE			0xB0 //Set GDDRAM Page start Adress 0
#define SETRMW			0xE0 //Set Read Modify Write Mode
#define CLRRMW			0xEE //Exit Read Modify Write Mode
#define NOPCOMMAND		0xE3 //No Operation Command

//Function Declarations
void OLED_InitializePorts(void);
void OLED_Initialize(void);
void OLED_WriteCommand(unsigned char);
void OLED_WriteData(unsigned char);
void OLED_ClearScreen(void);
void OLED_SetScreen(void);
void OLED_GOTOXY(unsigned char, unsigned char);
void OLED_Bitmap(const unsigned char *);
void OLED_WriteCharacter(unsigned char);
void OLED_Pixel_XY(unsigned char, unsigned char);
void OLED_WriteChar(char);
void OLED_WriteString(char *);
void GLCD_Rectangle(unsigned char , unsigned char , unsigned char , unsigned char );
void GLCD_Circle(unsigned char , unsigned char  ,unsigned char );
void GLCD_Line(unsigned int ,unsigned int ,unsigned int ,unsigned int );
void glcd_line(int , int , int , int );
unsigned char OLED_ReadData(void);
void GLCD_WriteString(char * );


void init(void);
void delay(unsigned int);
void l_delay(unsigned int);
void orientation_up_down_left_right(void);
void orientation_down_up_left_right(void);
void orientation_down_up_right_left(void);
void orientation_up_down_right_left(void);
void contrast_0_percent(void);
void disp_20(void);
void disp_40(void);
void disp_60(void);
void disp_80(void);
void disp_100(void);
void Splash_Screen();
void pset(unsigned int , unsigned int , unsigned int );
void update_display( void );
void DrawRectangle(unsigned char , unsigned char , unsigned char , unsigned char );
void menu();
void overfrequency();
void overcurrent();
void underfrequency(); 
void overvoltage();
void undervoltage();
void menuscreen();

//Functions
int abs(int);
void OLED_InitializePorts(void){
 CONTROL_PORT_CONF=0x00;
 DATA_PORT_CONF=0x00;
 DATA_PORT=0x00;
 
 CONTROL_PORT=0x1F; //OLED_RES=1
 Delay1KTCYx(16); //delay_1ms
 CONTROL_PORT=0x17; //OLED_RES=0
 Delay1KTCYx(160); //delay_10ms
 CONTROL_PORT=0x1F; //OLED_RES=1
 Delay1KTCYx(16); //delay_1ms
}

void OLED_Initialize(void){
 OLED_InitializePorts();
 OLED_WriteCommand(DISPLAY_OFF);
 OLED_WriteCommand(SETDCLK);
 OLED_WriteCommand(DCLK);
 OLED_WriteCommand(SETMUX);
 OLED_WriteCommand(MUX);
 OLED_WriteCommand(SETOFFSET);
 OLED_WriteCommand(OFFSET);
 OLED_WriteCommand(RE_MAP0);
 OLED_WriteCommand(SETSTARTLINE);
 OLED_WriteCommand(COMSCANDIREC0);
 OLED_WriteCommand(SETCOMPINS);
 OLED_WriteCommand(COMPINS);
 OLED_WriteCommand(SETCONTRAST);
 OLED_WriteCommand(CONTRAST);
 OLED_WriteCommand(SETPRECHARGE);
 OLED_WriteCommand(PRECHARGE);
 OLED_WriteCommand(SETVCOMH);
 OLED_WriteCommand(VCOMH);
 OLED_WriteCommand(ENTIREDISPLAY0);
 OLED_WriteCommand(NORMALDISPLAY);
 OLED_WriteCommand(SETMEMORY);
 OLED_WriteCommand(MEMORY);
 OLED_WriteCommand(SETLOWCOLUMN);
 OLED_WriteCommand(SETHIGHCOLUMN);
 OLED_WriteCommand(SETSTARTCOLUMN);
 OLED_WriteCommand(0x00);
 OLED_WriteCommand(SCREEN_WIDTH-1);
 OLED_WriteCommand(DISPLAY_ON);
}

void OLED_WriteCommand(unsigned char Command2Write){
 DATA_PORT=Command2Write;
 CONTROL_PORT=0x1F; //OLED_DC=1
 CONTROL_PORT=0x1B; //OLED_DC=0
 CONTROL_PORT=0x0B; //OLED_CS=0
 CONTROL_PORT=0x09; //OLED_WR=0
 Nop();
 CONTROL_PORT=0x0B; //OLED_WR=1
 CONTROL_PORT=0x0F; //OLED_DC=1
 CONTROL_PORT=0x1F; //OLED_CS=1
}

void OLED_WriteData(unsigned char Data2Write)
{
 DATA_PORT=Data2Write;
 CONTROL_PORT=0x1B; //OLED_DC=0
 CONTROL_PORT=0x1F; //OLED_DC=1
 CONTROL_PORT=0x0F; //OLED_CS=0
 CONTROL_PORT=0x0D; //OLED_WR=0
 Nop();
 CONTROL_PORT=0x0F; //OLED_WR=1
 CONTROL_PORT=0x0B; //OLED_DC=0
 CONTROL_PORT=0x1B; //OLED_CS=1
}

void OLED_ClearScreen(void){
 unsigned char page;
 unsigned char column;
 OLED_GOTOXY(0,0);
 for(page=0;page<8;page++){
  for(column=0;column<SCREEN_WIDTH;column++){
   OLED_WriteData(0x00);
  } 
 }
}

void OLED_SetScreen(void){
 unsigned char page;
 unsigned char column;
 OLED_GOTOXY(0,0);
 for(page=0;page<8;page++){
  for(column=0;column<SCREEN_WIDTH;column++){
   OLED_WriteData(0xFF);
  } 
 }
}


void OLED_GOTOXY(unsigned char posX, unsigned char posY){
 unsigned char x;
 unsigned char y;
 x=posX;
 y=posY;
 y=y/8;
 OLED_WriteCommand(SETSTARTCOLUMN);
 OLED_WriteCommand(x+4);
 OLED_WriteCommand(SCREEN_WIDTH+3);
 OLED_WriteCommand(SETPAGE+y);
}


void OLED_Pixel_XY(unsigned char posX, unsigned char posY){
 unsigned char x;
 unsigned char y;
 x=posX;
 y=posY;
 y=y%8;
 y=2<<y;
 OLED_GOTOXY(posX,posY);
 OLED_WriteData(y);
}


void OLED_Bitmap(const unsigned char *image)
{
 unsigned char page;
 unsigned char column;
 OLED_GOTOXY(0,0);
 for(page=0;page<8;page++){
  for(column=0;column<SCREEN_WIDTH;column++){
  OLED_WriteData(*(image++));
  } 
 }
}

void OLED_WriteCharacter(unsigned char num){
switch(num){
//0
case 0:
OLED_WriteData(0x7C);
OLED_WriteData(0xA2);
OLED_WriteData(0x92);
OLED_WriteData(0x8A);
OLED_WriteData(0x7C);
break;

//1
case 1:
OLED_WriteData(0x00);
OLED_WriteData(0x84);
OLED_WriteData(0xFE);
OLED_WriteData(0x80);
OLED_WriteData(0x00);
break;

//2
case 2:
OLED_WriteData(0x84);
OLED_WriteData(0xC2);
OLED_WriteData(0xA2);
OLED_WriteData(0x92);
OLED_WriteData(0x8C);
break;

//3
case 3:
OLED_WriteData(0x42);
OLED_WriteData(0x82);
OLED_WriteData(0x8A);
OLED_WriteData(0x96);
OLED_WriteData(0x62);
break;

//4
case 4:
OLED_WriteData(0x30);
OLED_WriteData(0x28);
OLED_WriteData(0x24);
OLED_WriteData(0xFE);
OLED_WriteData(0x20);
break;

//5
case 5:
OLED_WriteData(0x4E);
OLED_WriteData(0x8A);
OLED_WriteData(0x8A);
OLED_WriteData(0x8A);
OLED_WriteData(0x72);
break;

//6
case 6:
OLED_WriteData(0x78);
OLED_WriteData(0x94);
OLED_WriteData(0x92);
OLED_WriteData(0x92);
OLED_WriteData(0x60);
break;

//7
case 7:
OLED_WriteData(0x02);
OLED_WriteData(0xE2);
OLED_WriteData(0x12);
OLED_WriteData(0x0A);
OLED_WriteData(0x06);
break;

//8
case 8:
OLED_WriteData(0x6C);
OLED_WriteData(0x92);
OLED_WriteData(0x92);
OLED_WriteData(0x92);
OLED_WriteData(0x6C);
break;

//9
case 9:
OLED_WriteData(0x0C);
OLED_WriteData(0x92);
OLED_WriteData(0x92);
OLED_WriteData(0x52);
OLED_WriteData(0x3C);
break;

//b
case 10:
OLED_WriteData(0xFE);
OLED_WriteData(0x90);
OLED_WriteData(0x88);
OLED_WriteData(0x88);
OLED_WriteData(0x70);
break;

//a
case 11:
OLED_WriteData(0x40);
OLED_WriteData(0xA8);
OLED_WriteData(0xA8);
OLED_WriteData(0xA8);
OLED_WriteData(0xF0);
break;

//=
case 12:
OLED_WriteData(0x28);
OLED_WriteData(0x28);
OLED_WriteData(0x28);
OLED_WriteData(0x28);
OLED_WriteData(0x28);
break;

//-
case 13:
OLED_WriteData(0x10);
OLED_WriteData(0x10);
OLED_WriteData(0x10);
OLED_WriteData(0x10);
OLED_WriteData(0x10);
break;

//.
case 14:
OLED_WriteData(0x00);
OLED_WriteData(0xC0);
OLED_WriteData(0xC0);
OLED_WriteData(0x00);
OLED_WriteData(0x00);
break;

//_
case 15:
OLED_WriteData(0x80);
OLED_WriteData(0x80);
OLED_WriteData(0x80);
OLED_WriteData(0x80);
OLED_WriteData(0x80);
break;

//' '
case 16:
OLED_WriteData(0x00);
OLED_WriteData(0x00);
OLED_WriteData(0x00);
OLED_WriteData(0x00);
OLED_WriteData(0x00);
break;

//'!'
case 17:
OLED_WriteData(0x00);
OLED_WriteData(0x00);
OLED_WriteData(0x9E);
OLED_WriteData(0x00);
OLED_WriteData(0x00);
break;

}
OLED_WriteData(0x00);
OLED_WriteData(0x00);
}


unsigned char OLED_ReadByteFromROMMemory(char * ptr)
{
  return *(ptr);
}
//-------------------------------------------------------------------------------------------------
// Writes char to screen memory
//-------------------------------------------------------------------------------------------------
void OLED_WriteChar(char charToWrite)
{
char i;
charToWrite -= 32; 
for(i = 0; i < 5; i++) 
OLED_WriteData(OLED_ReadByteFromROMMemory((char *)(font5x8 + (5 * charToWrite) + i))); 
OLED_WriteData(0x00);
}
//-------------------------------------------------------------------------------------------------
// Write null-terminated string to screen memory
//-------------------------------------------------------------------------------------------------
void OLED_WriteString(char * stringToWrite)
{
while(*stringToWrite)
  OLED_WriteChar(*stringToWrite++);
}


//************************************GRAPHICS APIs**********************************

void glcd_line(int x1, int y1, int x2, int y2)
{
   signed int  x, y, addx, addy, dx, dy;
   signed long P;
   int i;
   dx = abs((signed int)(x2 - x1));
   dy = abs((signed int)(y2 - y1));
   x = x1;
   y = y1;

   if(x1 > x2)
      addx = -1;
   else
      addx = 1;
   if(y1 > y2)
      addy = -1;
   else
      addy = 1;

   if(dx >= dy)
   {
      P = 2*dy - dx;

      for(i=0; i<=dx; ++i)
      {
         OLED_Pixel_XY(x, y);

         if(P < 0)
         {
            P += 2*dy;
            x += addx;
         }
         else
         {
            P += 2*dy - 2*dx;
            x += addx;
            y += addy;
         }
      }
   }
   else
   {
      P = 2*dx - dy;

      for(i=0; i<=dy; ++i)
      {
         OLED_Pixel_XY(x, y);

         if(P < 0)
         {
            P += 2*dx;
            y += addy;
         }
         else
         {
            P += 2*dx - 2*dy;
            x += addx;
            y += addy;
         }
      }
   }
}
void DrawFrame( unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2 )
{
   glcd_line(x1,y1,x2,y1);
   glcd_line(x1,y2,x2,y2);
   glcd_line(x1,y1,x1,y2);
   glcd_line(x2,y1,x2,y2);
}



void blinking(unsigned char times){
unsigned char t1;
unsigned char t2;
t2=times;
for(t1=0;t1<t2;t1++){
 OLED_SetScreen();
 delay(10);
 OLED_ClearScreen();
 delay(10);
 }
}

void orientation_up_down_left_right(void){
OLED_WriteCommand(COMSCANDIREC0);
OLED_WriteCommand(RE_MAP1);
OLED_ClearScreen();
}

void orientation_down_up_left_right(void){
OLED_WriteCommand(COMSCANDIREC1);
OLED_WriteCommand(RE_MAP1);
}

void orientation_down_up_right_left(void){
OLED_WriteCommand(COMSCANDIREC1);
OLED_WriteCommand(RE_MAP0);
}

void orientation_up_down_right_left(void){
OLED_WriteCommand(COMSCANDIREC0);
OLED_WriteCommand(RE_MAP0);
}

void disp_0(void){
OLED_WriteCommand(SETCONTRAST);
OLED_WriteCommand(0x00);
}

void disp_20(void){
OLED_WriteCommand(SETCONTRAST);
OLED_WriteCommand(0x32);
}

void disp_40(void){
OLED_WriteCommand(SETCONTRAST);
OLED_WriteCommand(0x64);
}

void disp_60(void){
OLED_WriteCommand(SETCONTRAST);
OLED_WriteCommand(0x96);
}

void disp_80(void){
OLED_WriteCommand(SETCONTRAST);
OLED_WriteCommand(0xC8);
}

void disp_100(void){
OLED_WriteCommand(SETCONTRAST);
OLED_WriteCommand(0xFF);
}



void l_delay(unsigned int value){
unsigned char temp;
for(temp=0;temp<value;temp++){
Delay1TCY();
//_delay(10); 
}
}

void Splash_Screen()
{
    
OLED_ClearScreen();//CLEAR SSD1305 BUFFER
orientation_up_down_left_right();
OLED_Bitmap(YTEKLOGO);//PUT YTEK LOGO
delay(100);
OLED_ClearScreen();
}

void delay(unsigned int value){
unsigned char temp;
for(temp=0;temp<value;temp++){
Delay10KTCYx(10);
//_delay(10); 
}
}