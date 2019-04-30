#include"thermometer.h"

//struct nv ee;


void dispput(signed char val)
{
   unsigned char i;
   
   if(val<0) {
      disp[3]=0x40;
      val=-val;
   } else disp[3]=0;
     
   for(i=0;i<3;i++) {
      disp[i]=seg[val%10];
	  val/=10;
   }   
}

const unsigned int digvals[]={1,10,100,1000};

void HC08Setup(void)
{
   CONFIG1=0x17; // STOP enabled, LVI & COP disabled
   CONFIG2=0x00; // IRQ pullup enabled
}

void main(void)
{
   unsigned char i;

   display_init();

   asm("cli");
   
   for(i=0;i<4;i++)
   {
	  disp[i+4]=0;
   }

   ADSCR=0x29; // ADC - continuous conversion of PTD2
   ADICLK=0x00; // ADC - BUSCLK/1 (BUSCLK ~ 1MHz)
   
   while(1)
   {
      dispput(temp_conv[255-ADR]);
   }
}