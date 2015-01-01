#ifndef __HW_SIMULATOR_UI_H__
#define __HW_SIMULATOR_UI_H__

void    hw_simulator_ui_init();
int     hw_simulator_ui_poll();
void    hw_simulator_ui_set_led_state(int led, int state);

#define NO_OPERATION        0
#define BUTTON_1_PRESSED    1
#define BUTTON_1_RELEASED   2
#define BUTTON_2_PRESSED    3
#define BUTTON_2_RELEASED   4
#define BUTTON_3_PRESSED    5
#define BUTTON_3_RELEASED   6
#define QUIT               -1

#endif
