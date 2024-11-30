#ifndef __pwm_h__
#define __pwm_h__
void pwm_init(void);
void pwm_setcompare1(int compare);
void pwm_setcompare2(int compare);
void pwm_setcompare3(int compare);
void pwm_setcompare4(int compare);
void pwm_setprescaler(int prescaler);
#endif
