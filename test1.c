/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.0 Professional
Automatic Program Generator
© Copyright 1998-2010 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 10.10.2014
Author  : 
Company : 
Comments: 


Chip type               : ATtiny2313
AVR Core Clock frequency: 4,000000 MHz
Memory model            : Tiny
External RAM size       : 0
Data Stack size         : 32

PortB.1  + step
PortB.0  - steps
PindB.2  OnOff
PortD.0-PortD.3  motor coils

1rpm = 60 steps 
min 
100 rpm = divisor 2441
max
1000 rpm = divisor 244


*****************************************************/

#include <tiny2313.h>
#include <delay.h>
unsigned char i;
bit startstop=0;

void OneRpm ()
{
 for (i=1; i<8; i++) 
  {
   switch(i)
     {                      
      case 1 : PORTD = 0b0000001;       //Choose winding depending on the step   
      break;
      case 2 : PORTD = 0b0000011;       //make half-step     
      break;
      case 3 : PORTD = 0b0000010;    
      break;
      case 4 : PORTD = 0b0000110; 
      break;
      case 5 : PORTD = 0b0000100; 
      break; 
      case 6 : PORTD = 0b0001100; 
      break; 
      case 7 : PORTD = 0b0001000;  
      break;
      case 8 : PORTD = 0b0001001;       //do four steps    
      break;
     }
 }  
}

// Timer1 output compare A interrupt service routine
interrupt [TIM1_COMPA] void timer1_compa_isr(void)
  { if (startstop==0)
     {
    PORTD=0x00;     //stop
     ;
     }
    else           // start
     {
      for (i=1; i<15; i++)         // 1RPM= 4steps*15
      {
       OneRpm ();
      }
     } 
   }
    
// Declare your global variables here

void main(void)
{
// Declare your local variables here

// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=0x80;
CLKPR=0x00;
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

// Input/Output Ports initialization
// Port A initialization
// Func2=In Func1=In Func0=In 
// State2=T State1=T State0=T 
PORTA=0x00;
DDRA=0x00;

// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=P State6=P State5=P State4=P State3=P State2=P State1=P State0=P 
PORTB=0xFF;
DDRB=0x00;

// Port D initialization
// Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out 
// State6=0 State5=0 State4=0 State3=0 State2=0 State1=0 State0=0 
PORTD=0x00;
DDRD=0x7F;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=0xFF
// OC0A output: Disconnected
// OC0B output: Disconnected
TCCR0A=0x00;
TCCR0B=0x0D;
TCNT0=0x00;
OCR0A=0x00;
OCR0B=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 3,906 kHz
// Mode: CTC top=OCR1A
// OC1A output: Discon.
// OC1B output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: On
// Compare B Match Interrupt: Off
TCCR1A=0x00;
TCCR1B=0x00;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x09;    //  Start
OCR1AL=0x89;    //  100 rpm
OCR1BH=0x00;
OCR1BL=0x00;

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// Interrupt on any change on pins PCINT0-7: Off
GIMSK=0x00;
MCUCR=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x40;

// Universal Serial Interface initialization
// Mode: Disabled
// Clock source: Register & Counter=no clk.
// USI Counter Overflow Interrupt: Off
USICR=0x00;

// USART initialization
// USART disabled
UCSRB=0x00;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
DIDR=0x00;

// Global enable interrupts
#asm("sei")

while (1)
      { 
               if (PINB.2==0)
         {
          if (startstop==1)     // off motor  stop
             {
              startstop=0;  
                delay_us(200);            
              }
          else
             {     // on motor 
              startstop=1;
                delay_us(200);
             }   
          } 
          
       if (OCR1A<2441)
        {
       OCR1A=2441;    //min 100 RPM
         }
       else 
        {
          if (PINB.0==0)
            {
             OCR1A+=244;  // -100RPM
              delay_us(200);
          }
          
          if (OCR1A>244)    //max 1000RPM
             {
             OCR1A=244;
             }
          else
             {
              if (PINB.1==0)
                 {OCR1A-=244;   //+100RPM
                 delay_us(200);
                 }
           }
        }   
         
      } 
      
      
}
