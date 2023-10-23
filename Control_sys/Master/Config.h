/*
 * Config.h
 *
 * Created: 10/3/2023 10:58:33 AM
 *  Author: Abdulrhman Nasser
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_
//#define first_time  //Uncomment this line for security overwrite

#define led1_bit 1
#define led2_bit 2
#define led3_bit 3
#define ac_status_bit 4
#define ac_temp_bit 5


#define Wlcm_msg1 "Welcome to Smart"
#define Wlcm_msg2 "Home Project"
#define wlcm_msg_time 700
#define admin_pass_status 0x50
#define guest_pass_status 0x51
#define admin_pass_address 0x52
#define guest_pass_address 0x56
#define blocked_status 0x60
#define blocked_time 20
#define blocked_msg1 "Out of trials"
#define blocked_msg2 "Wait for: "
#define No_pass_Set 0
#define admin_pass_NSet 1
#define guest_pass_NSet 2
#define both_pass_Set 3
#define show_pass_time 100
#define hidden_char '*'
#define choose_mode_str1 "Select mode"
#define choose_mode_str2 "0:Admin 1:Guest"
#define admin_mode '0'
#define guest_mode '1'
#define Number_of_trials 3
#define Conseq_trials 10
#define Conseq_trials_status 0x10

#define sys_en_status 0x12
#define sys_hack_check1 0x110
#define sys_hack_check2	0x270
#define sys_hack_check3	0x380
#define sys_hack_val 0x78

#endif /* CONFIG_H_ */