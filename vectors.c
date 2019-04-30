#include"thermometer.h"

#pragma interrupt_handler isrDummy
void isrDummy(void) 
	{
	}


	
#pragma abs_address:0xffde

void (* const _vectab[])(void) = {
	isrDummy,				/*15 ADC           	*/
	isrDummy, 				/*14 KBI           	*/
	isrDummy,				/*13            	*/
	isrDummy,				/*12            	*/
	isrDummy,				/*11            	*/
	isrDummy,				/*10            	*/
	isrDummy,				/*9             	*/
	isrDummy,				/*8     			*/
	isrDummy,				/*7             	*/
	isrDummy,				/*6              	*/
	disp_rfsh,				/*5 TIM1 OVR      	*/
	isrDummy,				/*4 TIM1 channel 1	*/
	isrDummy,				/*3 TIM1 channel 0	*/
	isrDummy,				/*2 not used       	*/
	isrDummy,				/*1 IRQ          	*/
	isrDummy				/*0 SWI            	*/
/*	RESET defined in crt08.o */
	};
