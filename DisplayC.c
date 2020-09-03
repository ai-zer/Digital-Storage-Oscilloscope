/*
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
#include <xc.h>          //Library Containing Macros for SFR and other stuff
#include <stdint.h>   //Used here for using uint8_t(which defines int of 1 byte)
 */
//Below Macros Define Various Control lines and data lines and Basic Commands
#define DataLine PORTD  
#define Output 0
#define Input 1
#define CmmndCntrlC TRISB
#define DataLineC TRISD
#define Display_Off 0b00111110
#define Display_On 0b00111111
#define PageI 0b10111000
#define ColumnI 0b01000000
#define Enable PORTBbits.RB5
#define RW PORTBbits.RB6
#define RS PORTBbits.RB7
#define CS1 PORTBbits.RB3       
#define CS2 PORTBbits.RB4
#define ZI 0b11000000
// FUnctions Declaration
void Display_Init(void); //Used for initialization of JHD12864e display
void Display_Cmnd(uint8_t ); // Used for sending commands to display
void Display_Data(uint8_t ); // Used for sending data to display
void Draw_Taxis(void);  // used for drawing time-axis
void Draw_Vaxis(void);   // used for drawing voltage-axis
void Page_Address(uint8_t );  // used for selecting page address
void Column_Address(uint8_t ); //used for selecting column address
void Chip1_Control(void); // used for activating chip 1 and de-activating chip 2
void Chip2_Control(void); //  // used for activating chip 2 and de-activating chip 1
#define _XTAL_FREQ 16000000  // Frequency of Crystal Oscillator used,need this 
                              // for calling delay function from xc.h library



void Display_Init()
{   CmmndCntrlC=Output;
    DataLineC=Output;
    CS1=1;
    CS2=1;
    Display_Cmnd(Display_Off);
    Display_Cmnd(PageI);
    Display_Cmnd(ColumnI);
    Display_Cmnd(ZI);
    Display_Cmnd(Display_On);
    Chip1_Control();
    Page_Address(0);
    Column_Address(0);
    
    
}
void Display_Cmnd(uint8_t cmmnd)
{   
    DataLine=cmmnd;
    RS=0;
    RW=0;
    Enable=0;
    __delay_us(3);
    Enable=1;
    __delay_us(3);
    Enable=0;
    __delay_us(3);
 }
void Display_Data(uint8_t data)
{
  DataLine=data;
  RS=1;
  RW=0;
  Enable=0;
    __delay_us(3);
  Enable=1;
  __delay_us(3);
  Enable=0; 
  __delay_us(3); 
}
 
void Page_Address(uint8_t page )
{
    page=(page|0b10111000);
    Display_Cmnd(page);
 }
void Column_Address(uint8_t column)
{
    column=(column|0b01000000);
    Display_Cmnd(column);
}
void Chip1_Control()
{   
    CS1=1;
    CS2=0;
}
void Chip2_Control()
{
    CS2=1;
    CS1=0;
}
void Draw_Taxis()
{   uint8_t taxis=0b00000001;
    for(uint8_t i=0;i<128;i++)
    {  if(i==64)
      {
        Chip2_Control();
      }
    Display_Data(taxis);
    }
 }
void Draw_Vaxis()
{   uint8_t vaxis=0b11111111;
    Chip2_Control();
    for (uint8_t i=0;i<8;i++)
    {   Column_Address(63);
        Page_Address(i);
        Display_Data(vaxis);
    }
    
}









   





