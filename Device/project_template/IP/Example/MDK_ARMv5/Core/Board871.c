#include "Board871.h"

void Board871_Initialize(struct Board871 * self) {
  LED_Init();

  self -> board871_log_content = (char * ) malloc(BOARD871_LOG_CONTENT * sizeof(char));
  if (!self -> board871_log_content) {
    Toggle_LED_1();
    while (1);
  }

  BC660K_Initialize( & self -> bc660k);
  //	LC76F_Initialize(&self->lc76f);
  //	MC3416_Initialize(&self->mc3416);
}