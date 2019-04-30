#include"thermometer.h"

/* Defines 7segment character map - MC14489 compatible */
/* 0 1 2 3 4 5 6 7 8 9 A B C D E F */
/* <space> c H h J L n o P r U u y - = <centigrad> */
const char seg[]={0x3F,0x06,0x5b,0x4F,0x66,0x6D,0x7D,0x07,
	  	   		  0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
				  0x00,0x58,0x76,0x74,0x1E,0x38,0x54,0x5C,
				  0x73,0x50,0x3E,0x1E,0x6E,0x40,0x48,0x63};

/* Keyboard is scanned in the same rate as display, contants are in number
   of scanning cycles */
#define kbrd_timeo 10 /* Keyboard detection constant */
#define kbrd_long_timeo 200 /* Keyboard longpress/repeat timeout constant */
#define kbrd_repeat 40 /* Keyboard repeat constant */
#define autorepeat 0xFF /* Which buttons could be autorepeated */

/* Keyboard related variables */
unsigned char kbrd[8]={0,0,0,0,0,0,0,0}; /* timeouts */
unsigned int kbrd_tim[8]={0,0,0,0,0,0,0,0}; /* keypress durations */
unsigned char keypress=0;		  /* keypess map */
unsigned char keys=0; 			  /* keys map */
unsigned char keypresslng=0;	  /* long keypess map */
unsigned char keyslng=0; 		  /* long keys map */

/* Display related vars */
unsigned char disp[8]={0,0,0,0,0,0,0,0};	  /* Video memory */
unsigned char disp_pos=0;
int disp_cycle=0;

const char decod0[]={1,2,4,8};
const char decod1[]={16,32,64,128};
const char columns[]={0xC0,0x88,0x48,0xC8};


#pragma interrupt_handler disp_rfsh
void disp_rfsh(void)	
{
	unsigned char val;
    unsigned char keyscan,d,e;

	TSC&=~0x80;

    asm("cli");
	 
	DDRB=0; /* All rows off */
	PTD|=0xC8;	 /* All columns off */
    PTA|=0x40;

/* calculates data to be displayed */	
    d=disp[disp_pos];
    e=disp[disp_pos+4];
	val=(((disp_cycle&0x80)?decod0[disp_cycle&3]:decod1[disp_cycle&3])&e)?0:d;

	keyscan=PTB;
	DDRB=0xFF;
	PTB=~val;

/* Selects column */	
    PTD=(PTD&0x37)|(columns[disp_pos]); /* Select display column */
    if(disp_pos==3) PTA&=~0x40; else PTA|=0x40;
	
	if(!disp_pos)
	{
	   disp_cycle++;
/* Column counter & keyboard scanner */	
       e=1;
       for(d=0;d<8;d++)
   	   {
          if(kbrd[d])kbrd[d]--;
	      else 
		  { 
		     keys&=~e;
			 keyslng&=~e;
			 kbrd_tim[d]=0; 
		  }
          if((~keyscan)&e)
       	  {
	         if(!kbrd[d]) keypress|=e;
         	 kbrd[d]=kbrd_timeo;
         	 keys|=e;
			 kbrd_tim[d]++;
			 if(kbrd_tim[d]==kbrd_long_timeo) keypresslng|=e;
			 if(kbrd_tim[d]>=kbrd_long_timeo) keyslng|=e;
			 if(kbrd_tim[d]>=(kbrd_long_timeo+kbrd_repeat))
			 {
			    if(autorepeat&e) keypress|=e;
				kbrd_tim[d]-=(kbrd_repeat-1);
		     } 
      	  }
      	  e<<=1;
	   }
	}

	disp_pos=(disp_pos+1)&3;
    
}			  


/* Initializes Display & kbd IO ports (PTB, PTA4-7, PTA2,3),
 * Initializes Timer 2 to generate overflow interrupt for disp RFSH*/
void display_init(void)
{
	PTB=0xFF; /* All row signals off */
 	DDRB=0xFF; /* Row output */

	PTD=0xC8; /* All column signals off */
	DDRD|=0xC8;
    PTA=0x40;
    DDRA=0x40;
    PTAPUE=0x80;
    
	TSC=0x70; /* BUSCLK/1 */
	TMODH=0x07; /* TMOD=0x07FF */
	TMODL=0xFF; /* division by 0x800 ~122Hz */
	TSC&=~0x20; /* start */
}

