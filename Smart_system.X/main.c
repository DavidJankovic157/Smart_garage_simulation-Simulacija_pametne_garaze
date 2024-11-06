/* 
 * File:  Domaci alkotest(umesto senzora,logika se primenjuje na potenciometar)
 * Author: David Jankovic,Luka Urosevic
 
 * Created on 1.11.2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <p30fxxxx.h>
#include<outcompare.h>

#include "adc.h"
#include "timer2.h"


_FOSC(CSW_FSCM_OFF & XT_PLL4);//instruction takt je isti kao i kristal 10MHz
_FWDT(WDT_OFF);


unsigned int sirovi0,sirovi1,sirovi2,sirovi3,step;
unsigned int broj,broj1,broj2,tempRX;
unsigned int buz=0;
unsigned char buf[6]; //RS232
int i = 0;


/***************************************************************************
* Ime funkcije      : initUART1                                            *
* Opis              : inicjalizuje RS232 komunikaciju s 9600bauda          * 
* Parameteri        : Nema                                                 *
* Povratna vrednost : Nema                                                 *
***************************************************************************/

void initUART1(void)
{
U1BRG=0x0040;//baud rate 9600
U1MODEbits.ALTIO = 1;
IEC0bits.U1RXIE = 1;
U1STA&=0xfffc;
U1MODEbits.UARTEN=1;
U1STAbits.UTXEN=1;
}
void __attribute__((__interrupt__)) _U1RXInterrupt(void) 
{
    IFS0bits.U1RXIF = 0;
  //   tempRX=U1RXREG;
    buf[i] = U1RXREG;
    if(i < 6)
        i++;
    else
        i = 0;

} 

void __attribute__((__interrupt__)) _ADCInterrupt(void) 
{
							

										sirovi0=ADCBUF0; //RB10 -fotootpotnik
										sirovi1=ADCBUF1;//RB11- MQ3
										sirovi2=ADCBUF2; //od viska glava ne boli
										

    IFS0bits.ADIF = 0;

} 

/*********************************************************************
* Ime funkcije      : WriteUART1                            		 *
* Opis              : Funkcija upisuje podatke u registar U1TXREG,   *
*                     za slanje podataka    						 *
* Parameteri        : unsigned int data-podatak koji zelimo poslati  *
* Povratna vrednost : Nema                                           *
*********************************************************************/

void WriteUART1(unsigned int data)
{


	while (U1STAbits.TRMT==0);
    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}
/***********************************************************************
* Ime funkcije      : WriteUART1dec2string                     		   *
* Opis              : Funkcija salje 4-cifrene brojeve (cifru po cifru)*
* Parameteri        : unsigned int data-podatak koji zelimo poslati    *
* Povratna vrednost : Nema                                             *
************************************************************************/
void WriteUART1dec2string(unsigned int data)
{
	unsigned char temp;

	temp=data/1000;
	WriteUART1(temp+'0');
	data=data-temp*1000;
	temp=data/100;
	WriteUART1(temp+'0');
	data=data-temp*100;
	temp=data/10;
	WriteUART1(temp+'0');
	data=data-temp*10;
	WriteUART1(data+'0');
}

void __attribute__((__interrupt__)) _T2Interrupt(void)
{

   	TMR2 =0;
    IFS0bits.T2IF = 0;
}

void write_string(char* data) {
    int br = 0;
    while(data[br] != '\0') {
        WriteUART1(data[br]);
        br++;
    }
    WriteUART1(13);
}

void proveri_buf(char buf[]){
   if(i < 6){
        if(buf[0] == 'C' && buf[1] == 'O' && buf[2] == '2' && buf[3] == 'I' && buf[4] == 'N' && buf[5]=='F' && buf[6]=='O'){
            WriteUART1dec2string(sirovi1);
        WriteUART1(13);
        }
        else
            write_string("Netacan unos");
        i = 0; }
}
/*
 * 
 */
int main(int argc, char** argv) {
    
    for(broj1=0;broj1<10000;broj1++);
 
        TRISBbits.TRISB7=1;
        TRISBbits.TRISB10=1;
        ADPCFGbits.PCFG10=0;
        ADPCFGbits.PCFG7=0; 
                
        
		TRISBbits.TRISB0=1;
		TRISBbits.TRISB1=1;
        TRISBbits.TRISB2=1;
        TRISD=0x0000;
        TRISFbits.TRISF2=0;
        TRISFbits.TRISF3=0;
        
        step=4096/5;
		for(broj=0;broj<60000;broj++);


		initUART1();//inicijalizacija UART-a
 		ADCinit();//inicijalizacija AD konvertora

		ADCON1bits.ADON=1;//pocetak Ad konverzije 
        
        TRISDbits.TRISD0=0;//konfigurisemo kao izlaz za pwm
        
        PR2=833;//odredjuje frekvenciju po formuli
        OC1RS=20;//postavimo pwm
        OC1R=1000;//inicijalni pwm pri paljenju samo
        OC1CON =OC_IDLE_CON & OC_TIMER2_SRC & OC_PWM_FAULT_PIN_DISABLE& T2_PS_1_256;//konfiguracija pwma
        T2CONbits.TON=1;//ukljucujemo timer koji koristi
        
        
        
	while(1)
	{

     
 /***********************************************************************
*   Ako potenciometar ne ide od 0 do 4096 neophodno je sa terminala     *
*   procitati opseg  i podesiti step i maksimalnu vrednost.             *     
*   To se desava zbog habanja potenciometra.                            *
*                                                                       *
************************************************************************/
      
        
        //fotootpornik
          if(sirovi0>3*step){
            LATDbits.LATD3=1;
            LATDbits.LATD2=1;
            LATDbits.LATD1=1;
            LATDbits.LATD8=1;
            LATDbits.LATD9=1;
           
          }
            else{
            LATDbits.LATD3=0;
            LATDbits.LATD2=0;
            LATDbits.LATD1=0;
            LATDbits.LATD8=0;
            LATDbits.LATD9=0;
          
            }
        
        
     
       //mq3 i buzzer  
        if(sirovi1<2*step){
            OC1RS=0;
            LATFbits.LATF2=0;
            LATFbits.LATF3=0;
        }
        
        if( (sirovi1>2*step) && (sirovi1<3*step) ){
            
            OC1RS=666;
            
            
            LATFbits.LATF2=1;
            LATFbits.LATF3=0;
          }
           
        
       if(sirovi1>3*step ){
            
            OC1RS=167;
             LATFbits.LATF3=1;
             LATFbits.LATF2=0;
             
          }
       if(buf!=0)    
         proveri_buf(buf);
       
        
        
        
		for(broj1=0;broj1<1000;broj1++)
		for(broj2=0;broj2<3000;broj2++);

	}

    return (EXIT_SUCCESS);
}

