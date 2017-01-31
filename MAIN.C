#include <xc.h>
#include "OLED_SSD1305.h"


void init(void)
{
    Delay10KTCYx(100);
  
    ADCON0bits.ADON = 0;            //turn off the ADC
    ADCON1=0x0F;                    //All pins digital

    OLED_Initialize();              //INBITIALIZE SSD1305 CONTROLLER
    OLED_ClearScreen();             //CLEAR SSD1305 BUFFER
    OLED_WriteCommand(SETCONTRAST);
    OLED_WriteCommand(255);         //CONTRAST FULL

   
}

void main()


{

    init();
     OLED_GOTOXY(100,55);
     OLED_WriteString("SSD1305 Running");
    
    while(1)
    {
    }



}
