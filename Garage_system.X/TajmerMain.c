#include <p30fxxxx.h>

#include "driverGLCD.h"
#include "tajmer.h"
#include "adc.h"

_FOSC(CSW_FSCM_OFF & HS3_PLL4);
_FWDT(WDT_OFF);


//---------------------------------
unsigned int brojac_ms,stoperica,ms,sekund,stanje,taster,p=0,strane=0;

unsigned int sirovi0,sirovi1,sirovi2,sirovi3,sirovi4;
unsigned int X, Y,x_vrednost, y_vrednost;//Promenljive za touch
unsigned int slika=0;

unsigned int temp0,temp1;//Promenljive za AD konverziju

#define DRIVE_A PORTCbits.RC13
#define DRIVE_B PORTCbits.RC14


void ConfigureTSPins(void)
{
	//ADPCFGbits.PCFG10=1;
	//ADPCFGbits.PCFG7=digital;

	//TRISBbits.TRISB10=0;
	TRISCbits.TRISC13=0;
    TRISCbits.TRISC14=0;
	
	//LATCbits.LATC14=0;
	//LATCbits.LATC13=0;
}

void Touch_Panel (void)
{
// vode horizontalni tranzistori
	DRIVE_A = 1;  
	DRIVE_B = 0;
    
     LATCbits.LATC13=1;
     LATCbits.LATC14=0;

	Delay_ms(500); //cekamo jedno vreme da se odradi AD konverzija
				
	// ocitavamo x	
	x_vrednost = temp0;//temp0 je vrednost koji nam daje AD konvertor na BOTTOM pinu		

	// vode vertikalni tranzistori
     LATCbits.LATC13=0;
     LATCbits.LATC14=1;
	DRIVE_A = 0;  
	DRIVE_B = 1;

	Delay_ms(500); //cekamo jedno vreme da se odradi AD konverzija
	
	// ocitavamo y	
	y_vrednost = temp1;// temp1 je vrednost koji nam daje AD konvertor na LEFT pinu	
	
//Ako ?elimo da nam X i Y koordinate budu kao rezolucija ekrana 128x64 treba skalirati vrednosti x_vrednost i y_vrednost tako da budu u opsegu od 0-128 odnosno 0-64
//skaliranje x-koordinate

    X=(x_vrednost-161)*0.03629;



//X= ((x_vrednost-AD_Xmin)/(AD_Xmax-AD_Xmin))*128;	
//vrednosti AD_Xmin i AD_Xmax su minimalne i maksimalne vrednosti koje daje AD konvertor za touch panel.


//Skaliranje Y-koordinate
	Y= ((y_vrednost-500)*0.020725);

//	Y= ((y_vrednost-AD_Ymin)/(AD_Ymax-AD_Ymin))*64;
}




/*******************************************************************
//funkcija za kasnjenje u milisekundama
*********************************************************************/
void Delay_ms (int vreme)//funkcija za kasnjenje u milisekundama
	{
		stoperica = 0;
		while(stoperica < vreme);
	}



void __attribute__ ((__interrupt__)) _T2Interrupt(void) // svakih 1ms
{

		TMR2 =0;
     ms=1;//fleg za milisekundu ili prekid;potrebno ga je samo resetovati u funkciji

	brojac_ms++;//brojac milisekundi
    stoperica++;//brojac za funkciju Delay_ms

    if (brojac_ms==1000)//sek
        {
          brojac_ms=0;
          sekund=1;//fleg za sekundu
		 } 
	IFS0bits.T2IF = 0; 
       
}



//ADC interrupt	
void __attribute__((__interrupt__)) _ADCInterrupt(void) 
{
							
	//podaci za touch
	sirovi0=ADCBUF0;//0
	sirovi1=ADCBUF1;//1
    
    

	temp0=sirovi0;
	temp1=sirovi1;
    

    IFS0bits.ADIF = 0;
} 

	// ------------------------------------------------------  
// GLCD Picture name: Welcome screen           
// ------------------------------------------------------  

unsigned char const Welcome_screen_bmp[1024] = {
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192, 48, 
  16,  8,  8,  8,  8, 16, 32,  0,  0,  0,192,112, 24,112,192,  0, 
   0,  0,248,  8,  8,  8,  8,  8,  8,240, 96,  0,  0,  0,192,112, 
  24,112,192,  0,  0,  0,192, 48, 16,  8,  8,  8,  8, 16, 32,  0, 
   0,248,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7, 24, 
  16, 32, 32, 33, 33, 17, 31,  0, 48, 30,  7,  4,  4,  4,  7, 30, 
  48,  0, 63,  1,  1,  1,  1,  3, 13, 24, 48, 32, 48, 30,  7,  4, 
   4,  4,  7, 30, 48,  0,  7, 24, 16, 32, 32, 33, 33, 17, 31,  0, 
   0, 63, 33, 33, 33, 33, 33, 33, 32,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0, 16,240,  0,  0,  0,  0,224, 
  48,240,  0,  0,  0,  0,240, 16,  0,240, 16, 16, 16, 16, 16, 16, 
  16,  0,  0,240,  0,  0,  0,  0,  0,  0,  0,192, 96, 32, 16, 16, 
  16, 16, 32, 64,  0,  0,128, 96, 32, 16, 16, 16, 16, 32, 96,128, 
   0,  0,240,112,128,  0,  0,  0,128,112,240,  0,  0,240, 16, 16, 
  16, 16, 16, 16, 16,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 15,112,112, 15,  0, 
   0,  0, 15,112,112, 15,  0,  0,  0,127, 66, 66, 66, 66, 66, 66, 
  64,  0,  0,127, 64, 64, 64, 64, 64,  0,  0, 15, 48, 96, 64, 64, 
  64, 64, 32, 24,  0,  0, 15, 48, 32, 64, 64, 64, 64, 32, 48, 15, 
   0,  0,127,  0,  1, 28, 96, 28,  1,  0,127,  0,  0,127, 66, 66, 
  66, 66, 66, 66, 64,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 
};

// ------------------------------------------------------  
// GLCD Picture name: Open screen.bmp             
// ------------------------------------------------------  

unsigned char const Open_screen_bmp[1024] = {
   0,254, 34, 34, 34, 34, 34,  0,  0,156, 18, 34, 34, 34,204,  0, 
   0,248,  4,  2,  2,  2,  6,140,  0,  0,  0,255,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  32, 35, 34, 34, 34, 34, 34, 32, 32, 33, 34, 34, 34, 34, 33, 32, 
  32, 32, 33, 34, 34, 34, 35, 33, 32, 32, 32, 63,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,124,130,  1,  1, 17, 
 146,116,  0,128,112, 46, 33, 46,112,128,  0,255, 17, 17, 17, 49, 
 209, 14,  0,128,112, 46, 33, 46,112,128,  0,124,130,  1,  1, 17, 
 146,116,  0,  0,255, 17, 17, 17, 17, 17,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,  0,  1,  1,129, 
 128,128,128,  1,  0,  0,  0,  0,  0,  1,  0,  1,  0,  0,  0,  0, 
   0,  1,  0,  1,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1,  1,  1, 
   0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,  0,  0,103,136, 
 136,136,136,115,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 64, 64, 64, 
 128,  0,  0,  0,192, 64, 64, 64, 64,128,  0,  0,192, 64, 64, 64, 
  64, 64,  0,  0,192,128,  0,  0,  0,  0,192,  0,  0,192, 64, 64, 
  64, 64, 64,  0,  0,192, 64, 64, 64, 64,128,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 31, 32, 64, 64, 64, 
  32, 31,  0,  0,127,  4,  4,  4,  4,  3,  0,  0,127, 68, 68, 68, 
  68, 68,  0,  0,127,  0,  3,  4, 24, 32,127,  0,  0,127, 68, 68, 
  68, 68, 68,  0,  0,127, 64, 64, 64, 64, 32, 31,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 
};

// ------------------------------------------------------  
// GLCD Picture name: Close screen            
// ------------------------------------------------------  

unsigned char const Close_screen_bmp[1024] = {
   0,254, 34, 34, 34, 34, 34,  0,  0,156, 18, 34, 34, 34,204,  0, 
   0,248,  4,  2,  2,  2,  6,140,  0,  0,  0,255,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  32, 35, 34, 34, 34, 34, 34, 32, 32, 33, 34, 34, 34, 34, 33, 32, 
  32, 32, 33, 34, 34, 34, 35, 33, 32, 32, 32, 63,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,248,  4,  2,  2, 34, 36,232, 
   0,  0,224, 92, 66, 92,224,  0,  0,254, 34, 34, 34, 98,162, 28, 
   0,  0,224, 92, 66, 92,224,  0,  0,248,  4,  2,  2, 34, 36,232, 
   0,  0,254, 34, 34, 34, 34, 34,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  2,  2,  1,  0, 
   0,  3,  0,  0,  0,  0,  0,  3,  0,  3,  0,  0,  0,  0,  1,  2, 
   0,  3,  0,  0,  0,  0,  0,  3,  0,  0,  1,  2,  2,  2,  1,  0, 
   0,  0,  3,  2,  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,255,  0,  0,206, 17, 17, 17, 
  17,230,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,128,128,129,  1,  1, 
   1,  0,128,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,  0,  0, 
   0,  0,  0,128,128,128,128,  0,  0,  0,128,128,128,128,128,128, 
   0,  0,128,128,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0, 62, 65,128,128,128, 65, 34, 
   0,  0,255,128,128,128,128,128,  0, 62, 65,128,128,128, 65, 62, 
   0,  0,103,136,136,136,136,115,  0,  0,255,136,136,136,136,136, 
   0,  0,255,128,128,128,128, 65, 62,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 
};


void main (void)
{
    
    
    TRISFbits.TRISF2=1;
	TRISFbits.TRISF3=1;
    
    TRISDbits.TRISD8=0;
    
   
	TRISDbits.TRISD9=0;//konfigurisemo kao izlaz motora

    
    ConfigureLCDPins();
	ConfigureTSPins();

	GLCD_LcdInit();

	GLCD_ClrScr();
   
	
	ADCinit();
	ConfigureADCPins();
	ADCON1bits.ADON=1;
    
    
Init_T2();

 
//Podesavanje PIR-a
    ADPCFGbits.PCFG7=1;//definisemo da pin B7 nije analogni vec digitalni
    TRISBbits.TRISB7=1;//definisemo pin kao ulazni
	while(1)
	{   
        Touch_Panel();
        
        switch(slika) {
  case 0:
 
     GLCD_DisplayPicture(Welcome_screen_bmp);
     
     
    break;
  case 1:
          GLCD_DisplayPicture(Open_screen_bmp);
          
    break;
        case 2:
            
         GLCD_DisplayPicture(Close_screen_bmp);
         
        break;
  default:
        GLCD_DisplayPicture(Welcome_screen_bmp);
        }
     
        
        
        
        
        while(PORTFbits.RF2==1){
            LATDbits.LATD9=1;
            Delay_ms(20);
            LATDbits.LATD9=0;
            Delay_ms(180);
            LATDbits.LATD9=1;
            slika=1;
        }
		
        while(PORTFbits.RF3==1){
            LATDbits.LATD9=1;
            Delay_ms(10);
            LATDbits.LATD9=0;
            Delay_ms(190);
            LATDbits.LATD9=1;
            slika=2;
        }
        
        //Parking senzor
        if(PORTBbits.RB7==1){
            LATDbits.LATD8=~LATDbits.LATD8;
            Delay_ms(500);
            LATDbits.LATD8=~LATDbits.LATD8;
            Delay_ms(750);
        }
        
        
       if ((X>2)&&(X<20)&& (Y>47)&&(Y<60))
       slika=0;     
        
        
       
        
            
        
        
			
	}//while(1)



}//main


