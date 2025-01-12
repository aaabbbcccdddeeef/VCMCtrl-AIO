#include "AM26C32.h"
#include "main.h"

#include "tim.h"


/* 调试 */
#include "stdio.h"
#define CNTZERO 10000
#define ENCODERPLUSE 8000;

struct AM26C32Encoder{
    int32_t cnt;    //编码器脉冲数 0 - ENCODERPLUSE
    float position; //位置量 0 - 10mm
};

typedef struct AM26C32Encoder VCMEncoder;
typedef struct VCMEncoder *PVCMEncoder;

VCMEncoder gVCMEncoder = {
        .cnt = 0,
        .position = 0,
};

/**
  * @brief 编码器初始化
  *
  */
void VCMEncoderInit(void)
{
	//编码器Encode功能启动
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    __HAL_TIM_SET_COUNTER(&htim2, CNTZERO);
    gVCMEncoder.cnt = 0;
    gVCMEncoder.position = 0.0;
}

/**
  * @brief 获得编码器的脉冲数
  * 
  */
int32_t VCMEncoderGetCnt(void)
{
    gVCMEncoder.cnt += __HAL_TIM_GET_COUNTER(&htim2) - CNTZERO;
    __HAL_TIM_SET_COUNTER(&htim2, CNTZERO);
    if(gVCMEncoder.cnt < 0){
        gVCMEncoder.cnt += ENCODERPLUSE;
    }
    gVCMEncoder.cnt = gVCMEncoder.cnt % ENCODERPLUSE;
    printf("CNT:%d\n", gVCMEncoder.cnt);
    return gVCMEncoder.cnt;
}

/**
  * @brief 获得位置数据 - 工程值
  */
float VCMEncoderGetPosition(void)
{
    gVCMEncoder.position = (float)10.0 * (float)VCMEncoderGetCnt() / (float)8000.0;
    if(gVCMEncoder.position < 0){
        gVCMEncoder.position = 0;
    }
    return gVCMEncoder.position;
}



