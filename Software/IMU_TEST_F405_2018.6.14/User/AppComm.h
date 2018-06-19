/**
  ******************************************************************************
  * @file    AppComm.h
  * @brief
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 ZEROTECH
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_COMM_H
#define __APP_COMM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DrvCom.h"


typedef struct
{
  char status;
  float angle;
  float hight;
  float velN;
  float velE;
  float velD;
  float megnetic;
  char starNum;
} Recv_Plane_Data_Type;

typedef enum
{
  Reply_No_Ack_Serial_Number_No_Repeat,       //回复的数据 无ACK-SN 不重复
  Reply_Ack_Serial_Number_No_Repeat,          //回复的数据有ACK-SN 不重复

  Period_Send_No_Ack_Seial_Number_No_Repeat,      //周期性主动发送的数据 不重复 无ACK-SN
  Event_Send_No_Ack_Seial_Number_Repeat,      //事件主动发送的数据 重复 无ACK
  Event_Send_No_Ack_Seial_Number_No_Repeat,       //事件主动发送的数据 重复 无ACK

} Pack_Type_Enum;

typedef enum
{
  TEST_IDLE = 0,
  TESTING = 1,
  COMM_ERR,
  TEST_OK,
  TEST_ERR
} testStateType;

typedef enum
{    
  Register_Gimbal_Version_R    = 0x33,
  Register_Gimbal_Test_Mode_WR = 0x36,

    Register_App_Voice_W      = 0x40,     
    Register_App_PothoStatus_W  = 0x41,
    
    Register_PID_Parameter_WR    = 0x50,
    Register_Show_Data           = 0x51,
    Register_Calibrate_Imu_W     = 0x52,
    Register_Motor_Debug_W       = 0x53,
    Register_Encoder_Drift_W     = 0x54,
    Register_Show_motorDebugData = 0x55,
    Register_Capture_Position_WR = 0x56,
    Register_Plane_Data_WR       = 0x58,

  Register_Gimbal_Trace_W          = 0x60,
  Register_Gimbal_Profiles_User_WR    = 0x61,
  Register_Gimbal_Profiles_Set_WR   = 0x62,
  Register_App_Calibrate_Imu_W      = 0x63,
  Register_App_Restore_Parameter_W = 0x64,
  Register_App_Return_Center_W = 0x65,
  Register_App_FollowMode_WR        = 0x66,
  Register_APP_FullView_W        = 0x67,
  Register_App_Remote_Move_W      = 0x68,
  Register_App_Photo_W      = 0x69,
  Register_App_Focus_W      = 0x6A,
  Register_App_Motor_Tuning_WR      = 0x6B,
  Register_App_Motor_PowerOff      = 0x6C,
  Register_APP_FullViewVideo_W      = 0x6D,
  Register_APP_DelayVideo_W    = 0x6E,
  Register_User_Data_WR        = 0x6F,


  Register_PC_Test_Start  = 0x70,
  Register_PC_Test_Imu   = 0x71,
  Register_PC_Test_Motor  = 0x72,
  Register_PC_Test_Key = 0x73,
  Register_PC_Capture_Motor_Theta0 = 0x74,
  Register_PC_Calibrate_Imu = 0x75,
  Register_PC_Capture_Encoder_Zero = 0x76,
  Register_PC_Fast_Calibrate = 0x77,
  Register_PC_Test_ShowData = 0x7F,

  Register_Gimbal_BootMode = 0x45,
  Register_Gimbal_UpdateCode   = 0xC1,


} MsgCmd_Enum;



typedef enum
{
  Stablizer_Mode = 0x30,
  Unstablizer_Mode = 0x31,
  Debug_Mode = 0x32,
} SysWorkMode_Enum;

typedef enum
{
  Buffer_Kp_Yaw = 0,
  Buffer_Ki_Yaw = 12,
  Buffer_Kd_Yaw = 2,    //2

  Buffer_Kp_Roll = 3,   //3
  Buffer_Ki_Roll = 29,
  Buffer_Kd_Roll = 34,

  Buffer_Kp_Pitch = 22,
  Buffer_Ki_Pitch = 18,
  Buffer_Kd_Pitch = 20,

  Buffer_BWLF_Yaw = 15, //15
  Buffer_BWLF_Roll = 16,    //16
  Buffer_BWLF_Pitch = 17,   //17
} PID_Buffer_Index_Enum;


typedef struct
{
  u8 kp_yaw;
  u8 ki_yaw;
  u8 kd_yaw;
  u8 BWLF_yaw;

  u8 kp_roll;
  u8 ki_roll;
  u8 kd_roll;
  u8 BWLF_roll;

  u8 kp_pitch;
  u8 ki_pitch;
  u8 kd_pitch;
  u8 BWLF_pitch;
} Gcs_Data_Type;


typedef struct
{
  uint8_t cmd;
  uint8_t sub_cmd;
//  unsigned short *pAckSn;     //应答式序列号地址
  uint8_t pAckSn[2];      //应答式序列号地址
  uint8_t *pData;
  uint8_t error;
  uint8_t data_len;
  Pack_Type_Enum pack_type;

} Protocol_Type;

extern float userFloatData[12];
extern u8 packageTxMsgBuf[256];


extern void AppComm_Init(void);

extern void AppComm_SendDebugData(u8* buf, u16 len);



extern void AppShowDebugData(void);
extern void AppShowData(void);



#ifdef __cplusplus
}
#endif

#endif /* __APP_COMM_H */

/************************ (C) COPYRIGHT ZEROTECH *****END OF FILE****/
