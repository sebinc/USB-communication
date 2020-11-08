#ifndef MAIN_C
#define MAIN_C
// Global includes
#include <htc.h>
// Local includes
#include "usb.h"
#include "HardwareProfile.h"
#include "usb_function_hid.h"
#include "genericHID.h"
//*******DELAY INITIALIZATION************
void delayMs();
//*******PIC CONFIGURATION WORDS**************
// Config word 1 (Oscillator configuration)
// 20Mhz crystal input scaled to 48Mhz and configured for USB operation
__CONFIG(1, USBPLL & IESODIS & FCMDIS & HSPLL & CPUDIV1 & PLLDIV5);
// Config word 2
__CONFIG(2, VREGEN & PWRTDIS & BOREN & BORV20 & WDTDIS & WDTPS32K);
// Config word 3
__CONFIG(3, PBDIGITAL & LPT1DIS & MCLREN);
// Config word 4
__CONFIG(4, XINSTDIS & STVREN & LVPDIS & ICPORTDIS & DEBUGDIS);
// Config word 5, 6 and 7 (protection configuration)
__CONFIG(5, UNPROTECT);
__CONFIG(6, UNPROTECT);
__CONFIG(7, UNPROTECT);
// local prototypes

static void InitialiseSystem(void);
void ProcessIO(void);
volatile unsigned char count;
volatile unsigned char n;
void interrupt ISR();





// ***********Main function*************
void main(void)
{
InitialiseSystem();

while(1)
{
// Check bus status and service USB interrupts.
USBDeviceTasks();
// user-specific tasks.
ProcessIO();
}
}



// *********PIC REGISTER INITIALIZATION***********
static void InitialiseSystem(void)
{
ADCON1 = 0x0F; // Default all pins to digital
OSCCON=0x00;
TRISB = 0xFE;
TRISD=0x00;
PORTB = 0x00;
PORTD = 0x00;
USBEN=1;
FSEN=1;
UPUEN=1;
UTRDIS=1;
SUSPND=0;
IPEN=1;
GIE=1;
PEIE=1;

CCP1CON=0b00001100;
CCP2CON=0b00001100;//enable ccp module
CCPR1L=10;
PR2=0xFF;
T2CON=0b00000001;//timer2 run
TMR2ON=1;
TRISC2=0;


#if defined(USE_SELF_POWER_SENSE_IO)
tris_self_power = INPUT_PIN;
#endif
// Initialize the variable holding the handle for the last
// transmission
USBOutHandle = 0;
USBInHandle = 0;
blinkStatusValid = TRUE;
void delayMs();
USBDeviceInit();
}
// Process input and output




void delayMs()
{
int i,j;
for(i=0;i<=300;i++)
{
for(j=0;j<=50;j++);
}}



void ProcessIO(void)
{
// If we are not in the configured state just return
if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;



if(!HIDTxHandleBusy(USBOutHandle)) //checking input data
{
if( ReceivedDataBuffer[0]==0xFF)
{
PORTD = ReceivedDataBuffer[ 1];
CCPR1L=ReceivedDataBuffer[ 2];
ReceivedDataBuffer[0]=0x00;
}
else
{
PORTD=0x00;
}
USBOutHandle = HIDRxPacket(HID_EP,(BYTE*)&ReceivedDataBuffer,64);
}
}



#endif