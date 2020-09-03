
// PIC18F4520 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1H
#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)
//Some Register
#pragma config DEBUG=OFF
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
/* Things TO keep in mind while programming ADC in PIC18F452   
Minimum TAD for A/D converter is 0.7 us
Conversion time is 12 TAD
Minimum acquisition time is 1.4 us
Minimum time of 2 TAD before next acquisition starts  
 */ 
#include <xc.h>
#include <stdint.h>
#include "DisplayC.c"
#include <math.h>
#define Go ADCON0bits.GO
#define _XTAL_FREQ 16000000
#define T0_On  T0CONbits.TMR0ON
unsigned int adc_val[127];
uint8_t row;
uint8_t page;
uint8_t pixel;
void Adc_InitWaveform(void); //Initializes ADC
void Adc_Sampling(unsigned int twidth);
void Timer0_Initialization(void);
void Timer0(unsigned int);
void Adc_Todisplay(void);
void Adc_Topixel(unsigned int * );
void main()
{   
    Display_Init();
    Draw_Taxis();
    Draw_Vaxis();
    Adc_InitWaveform();
    Timer0_Initialization();
    Adc_Sampling(20000);
    Adc_Todisplay();
 }
void Adc_InitWaveform()
{ 
    ADCON0=0b00000001; // Selects Channel 0 and ADC Module is enabled
    ADCON1=0b00001110;// Selects Voltage Ref as VDD(for positive) and Vss
                      //for negative.Also only AN0 is selected is Analog,rest
                      // is digital
    ADCON2=0b10010101; // Right justified format,TAD is 1 us and acquisition time
                      // is 2 us
 }
void Adc_Sampling(unsigned int twidth)
{   
  
    for (int i=0;i<127;i++)  //takes 23.25us between each interval if no timer 
    {                        //delay present
      Go=1;
      asm("hereA: btfsc ADCON0,1");
      asm("bra hereA");
      adc_val[i]=(ADRESH<<8)+ADRESL;
      Timer0(twidth-93);  
    }
}
void Timer0_Initialization()
{  unsigned int bla;
   T0CON=0b00011000;// Timer clock is CLK0,16 bit,High to low,No prescaler
  }
void Timer0(unsigned int twidth)
{   twidth=0xffff-twidth+25;
    TMR0H=twidth>>8;
    TMR0L=twidth;
    INTCONbits.TMR0IF=0;
    T0_On=1;
    asm("here: btfss INTCON,2");
    asm("bra here");
    T0_On=0;
    
}
void Adc_Todisplay()
{   Chip2_Control();
    for(uint8_t i=0;i<127;i++)
    {  
        Adc_Topixel(&adc_val[i]);
        Page_Address(page);
        if(i==63)
        {
          Chip1_Control();
        }
        if(i<63)
        {
            Column_Address(62-i);
        }
        else
        {
            Column_Address(126-i);
        }
        Display_Data(pixel);
    }
        
}
void Adc_Topixel(unsigned int *adc)
{  
   float volt=(5*(*adc))/1023.0;
   row=round(((volt*62)/5)+1);
   page=row/8;
   pixel=row%8;
   pixel=1<<pixel;
}
 



   




