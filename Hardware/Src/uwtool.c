#include "uwtool.h"

//按键，不能同时按多个，无消抖
bool KEY_Scan(bool tf, bool *flag)
{
    if(tf&&*flag==false) {*flag = true;return true;} //若开启连按则将flag赋值为false
    else if(!tf&&*flag==true) *flag = false;

    return false;
}

//位置式PID初始化：*pid_temp结构体变量地址
void PID_P_init(struct PID_P *pid_temp, float p, float i, float d)
{
    pid_temp->SetSpeed = 0.0;
    pid_temp->ActualSpeed = 0.0;
    pid_temp->err = 0.0;
    pid_temp->err_last = 0.0;
    pid_temp->voltage = 0.0;
    pid_temp->integral = 0.0;
    pid_temp->Kp = p;
    pid_temp->Ki = i;
    pid_temp->Kd = d;
}
//位置式PID实现
float PID_P_realize(struct PID_P *pid_temp, float aim, float now)
{
    pid_temp->SetSpeed = aim;
    pid_temp->ActualSpeed = now;
    pid_temp->err = pid_temp->SetSpeed - pid_temp->ActualSpeed;
    pid_temp->integral += pid_temp->err;
    pid_temp->voltage = pid_temp->Kp * pid_temp->err + pid_temp->Ki * pid_temp->integral + pid_temp->Kd * (pid_temp->err - pid_temp->err_last);
    pid_temp->err_last = pid_temp->err;
    pid_temp->ActualSpeed = pid_temp->voltage * 1.0f;
    return pid_temp->ActualSpeed;
}

//增量式PID初始化：*pid_temp结构体变量地址
void PID_I_init(struct PID_I *pid_temp, float p, float i, float d)
{
    pid_temp->SetSpeed = 0.0;
    pid_temp->ActualSpeed = 0.0;
    pid_temp->err = 0.0;
    pid_temp->err_last = 0.0;
    pid_temp->err_next = 0.0;
    pid_temp->Kp = p;
    pid_temp->Ki = i;
    pid_temp->Kd = d;
}
//增量式PID实现
float PID_I_realize(struct PID_I *pid_temp, float aim, float now)
{
    pid_temp->SetSpeed = aim;
    pid_temp->ActualSpeed = now;
    pid_temp->err = pid_temp->SetSpeed - pid_temp->ActualSpeed;
    float incrementSpeed = pid_temp->Kp * (pid_temp->err - pid_temp->err_next) + pid_temp->Ki * pid_temp->err + pid_temp->Kd * (pid_temp->err - 2 * pid_temp->err_next + pid_temp->err_last);
    pid_temp->ActualSpeed += incrementSpeed; 
    pid_temp->err_last = pid_temp->err_next;
    pid_temp->err_next = pid_temp->err;
    return pid_temp->ActualSpeed;
}

//计次函数：times进入后面函数的次数，*flag使能指针
//注意：还有问题！！！times设定次数需要调整
bool count(uint8_t times,uint8_t *flag)
{
    if((*flag)>0) (*flag)++;
    if((*flag)==times+2) (*flag) = 0;
    return (bool)(*flag);
}

//数数函数：times第几次时进入后面的函数，*flag使能指针
//未测试
bool counting(uint8_t times,uint8_t *flag)
{
    if((*flag)>=times) (*flag) = 0;
    else (*flag)++;
    return !((bool)(*flag));
}

//滑动中值滤波：*SMF_temp结构体变量地址，data需要滤波的数据
//注意：若数据更新较慢，应用滤波后可能会有比较严重的滞后
void sliding_median_filter(struct SMF *SMF_temp,uint16_t data)
{
    int i, j, temp;
    uint16_t arr[SMF_SS];

    SMF_temp->arr_data[SMF_temp->times++] = data; //将采集到的数据放入数组
    if(!SMF_temp->en&&SMF_temp->times>=SMF_SS) SMF_temp->en = true; //达到样本量开启输出
    if(SMF_temp->times>=SMF_SS) SMF_temp->times = 0; //达到样本量后重新计数
    memcpy(arr,SMF_temp->arr_data,sizeof(arr)); //从数组中复制到临时数组

    //冒泡排序
    for (i = 0; i < SMF_SS - 1; i++)
        for (j = 0; j < SMF_SS - 1 - i; j++)
            if (arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }

    if(SMF_temp->en) SMF_temp->data = (arr[4]+arr[5])/2.0f; //存储数据
}

//滑动均值滤波：*SMF_temp结构体变量地址，data需要滤波的数据
//未测试
void moving_average_filter(struct MAF *MAF_temp,uint16_t data)
{
    uint8_t i;
    uint16_t temp;

    MAF_temp->arr_data[MAF_temp->times++] = data;
    if(MAF_temp->times>=MAF_SS) MAF_temp->times = 0;
    for(i=0;i<MAF_SS;i++)
        temp += MAF_temp->arr_data[i];

    MAF_temp->data = temp*1.0f/MAF_SS;
}

//简单卡尔曼滤波
#define Kk_calc(x,y) (x)/(x+y) //卡尔曼增益
//卡尔曼滤波初始化
void Kalman_Init(struct KalmanFilter *kalmanFilter, float FirstMeaValue, float E_mea, float FirstEstValue, float E_est)
{
	kalmanFilter->x_est = FirstEstValue;
	kalmanFilter->x_mea = FirstMeaValue;
	kalmanFilter->e_est = E_est;
	kalmanFilter->e_mea = E_mea;
	kalmanFilter->Kk = Kk_calc(kalmanFilter->e_est, kalmanFilter->e_mea);
}
//卡尔曼滤波数据更新
void Kalman_Update(struct KalmanFilter *kalmanFilter, float newMeaValue)
{
	float temp = kalmanFilter->e_est;
	kalmanFilter->x_est = kalmanFilter->x_est + kalmanFilter->Kk * (newMeaValue - kalmanFilter->x_est);
	kalmanFilter->x_mea = newMeaValue;
	kalmanFilter->Kk = Kk_calc(kalmanFilter->e_est, kalmanFilter->e_mea);
	kalmanFilter->e_est = (1 - kalmanFilter->Kk) * temp;
}

//三阶行列式结果
float third_order_determinant(float a11,float a12,float a13,float a21,float a22,float a23,float a31,float a32,float a33)
{
    return a11*a22*a33+a12*a23*a31+a13*a21*a32-a31*a22*a13-a32*a23*a11-a21*a12*a33;
}
//解三元一次方程组
void SOTLE(float x1,float y1,float z1,float x2,float y2,float z2,float x3,float y3,float z3,float *a,float *b,float *c)
{
    float d = third_order_determinant(x1,y1,1,x2,y2,1,x3,y3,1);
    *a = third_order_determinant(z1,y1,1,z2,y2,1,z3,y3,1)/d;
    *b = third_order_determinant(x1,z1,1,x2,z2,1,x3,z3,1)/d;
    *c = third_order_determinant(x1,y1,z1,x2,y2,z2,x3,y3,z3)/d;
}
