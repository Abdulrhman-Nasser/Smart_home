/*
 * Timers.h
 *
 * Created: 6/10/2022 9:16:18 AM
 *  Author: Abdulrhman Nasser
 */


#ifndef TIMERS_H_
#define TIMERS_H_
void vInit_Timer0_NormalMode();
void vInit_Timer0_CTC();
void Timer0_NonPWM_wave();
void Timer0_fastPWM_wave();
void Timer0_pashcorrectPWM_wave();
void vInit_Timer2_NormalMode();

#endif /* TIMERS_H_ */
