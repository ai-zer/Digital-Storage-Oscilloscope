
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
// Functions Declaration
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









   





