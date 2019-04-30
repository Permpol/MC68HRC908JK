#include<iojl.h>

extern unsigned char disp[8];	  /* Video memory */
extern const char seg[];

extern unsigned char keypress;		  /* keypess map */
extern unsigned char keys; 			  /* keys map */
extern unsigned char keypresslng;	  /* long keypess map */
extern unsigned char keyslng; 		  /* long keys map */

extern signed char temp_conv[];  /* ADC -> temperature conversion table */

void display_init(void);
void disp_rfsh(void);