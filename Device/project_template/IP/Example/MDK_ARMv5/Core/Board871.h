#ifndef BOARD871_
#define BOARD871_

/* Includes */
#include "ht32.h"

#include "ht32_board.h"

#include "stdlib.h"

#include "string.h"

#include "Setting.h"

#include "Collections.h"

#include "BC660K.h"

#include "LC76F.h"

#include "MC3416.h"


/* Extern variables and functions */
extern void LED_Init(void);
extern void Toggle_LED_1(void);
extern void Toggle_LED_2(void);

extern void Initialize_Log(void);
extern void Write_Char_Log(u16 character);
extern void Write_String_Log(char * input_string);

/* Struct initialization */
static struct Board871 {
  char * board871_log_content;

  struct BC660K bc660k;
  struct LC76F lc76f;
  struct MC3416 mc3416;
}
Board871;

/* Function prototypes */
void Board871_Initialize(struct Board871 * self);

#endif /* BOARD871_ */