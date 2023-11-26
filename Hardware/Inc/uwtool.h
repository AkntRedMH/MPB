#ifndef UW_TOOL_H
#define UW_TOOL_H

#include "stm32f1xx_hal.h"
#include "stdbool.h"
#include "string.h"

#define lim(x,min,max) ((x<=min)?min:(x>=max)?max:x) //限幅
#define abs_(x) (x>0?x:-x) //绝对值
#define ykxb(x,ax,ay,bx,by) 1.0f*(by-ay)/(bx-ax)*(x-ax)+ay //一次函数
#define lykxb(x,ax,ay,bx,by) lim(1.0f*(by-ay)/(bx-ax)*(x-ax)+ay,ay<by?ay:by,ay>by?ay:by) //有限幅的一次函数

bool KEY_Scan(bool tf, bool *flag);

struct PID_P
{
    float SetSpeed; //定义设定值
    float ActualSpeed; //定义实际值
    float err; //定义偏差值
    float err_last; //定义上一个偏差值
    float Kp, Ki, Kd; //定义比例、积分、微分系数
    float voltage; //定义电压值（控制执行器的变量）
    float integral; //定义积分值
};
void PID_P_init(struct PID_P *pid_temp, float p, float i, float d);
float PID_P_realize(struct PID_P *pid_temp, float aim, float now);

struct PID_I
{
    float SetSpeed; //定义设定值
    float ActualSpeed; //定义实际值
    float err; //定义偏差值
    float err_next; //定义上一个偏差值
    float err_last; //定义最上前的偏差值
    float Kp,Ki,Kd; //定义比例、积分、微分系数
};
void PID_I_init(struct PID_I *pid_temp, float p, float i, float d);
float PID_I_realize(struct PID_I *pid_temp, float aim, float now);

bool count(uint8_t times,uint8_t *flag);
bool counting(uint8_t times,uint8_t *flag);

#define SMF_SS 10//样本量
struct SMF
{
    bool en; //使能
    uint8_t times; //计次
    uint16_t arr_data[SMF_SS];
    float data;
};
void sliding_median_filter(struct SMF *SMF_temp,uint16_t data);

#define MAF_SS 10 //样本量
struct MAF
{
    uint8_t times;
    uint16_t arr_data[MAF_SS];
    float data;
};
void moving_average_filter(struct MAF *MAF_temp,uint16_t data);

struct KalmanFilter
{
	float x_mea; //测量值
	float x_est; //估计值
	float e_mea; //测量误差
	float e_est; //估计误差
	float Kk; //卡尔曼增益
};
void Kalman_Init(struct KalmanFilter *kalmanFilter, float FirstMeaValue, float E_mea, float FirstEstValue, float E_est);
void Kalman_Update(struct KalmanFilter *kalmanFilter, float newMeaValue);

float third_order_determinant(float a11,float a12,float a13,float a21,float a22,float a23,float a31,float a32,float a33);
void SOTLE(float x1,float y1,float z1,float x2,float y2,float z2,float x3,float y3,float z3,float *a,float *b,float *c);

#endif
