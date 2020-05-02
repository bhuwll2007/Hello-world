/**
 * @defgroup CANUtil CANUtil API
 * @ingroup BaseInterface API
 * @{
 *
 * 本模块提供对CAN数据进行操作的API接口.
 *
 * @file CAN.h
 * @brief API file.
 *
 */

#ifndef __CAN_H_
#define __CAN_H_
#include "./../DataSet.h"
#include "./../MsgQueue/MsgQueue.h"

/**
 * @brief VehicleSize.
 */
typedef struct __VehicleSize1 {
	//车宽
	uint16_t width;
	//车长
	uint16_t length;
	//车高
	uint8_t height;
	nint8_t Release1[3];
} VehicleSize_t1;

/**
 * @brief BrakeSystemStatus.
 */
typedef struct BrakeSystemStatus1 {
	uint8_t	brakePadel;
	uint8_t	wheelBrakes;
	uint8_t	traction;
	uint8_t	abs;
	uint8_t	scs;
	uint8_t	brakeBoost;
	uint8_t	auxBrakes;
	nint8_t Release1;
} BrakeSystemStatus_t1;

/**
 * @brief AccelerationSet4Way.
 */
typedef struct AccelerationSet4Way1 {
	nint16_t Long;
	nint16_t lat;
	nint8_t vert;
	nint8_t Release1;
	nint16_t yaw;
} AccelerationSet4Way_t1;

/**
 * @brief MotionConfidenceSet.
 */
typedef struct MotionConfidenceSet1 {
	uint8_t speedCfd;
	uint8_t headingCfd;
	uint8_t steerCfd;
	nint8_t Release1;//预留字段
} MotionConfidenceSet_t1;

/**
 * @brief CAN总线数据结构.
 */
typedef struct _CANDATA
{
	nint32_t lat;//纬度
	nint32_t lng;//经度
	nint32_t alt;//海拔/高度

	nint8_t transmission;//档位
	nint8_t Release1;//预留字段   //(CAN状态)0--正常、-1 --异常
	nint16_t speed;//速度

	nint16_t heading;//航向角
	nint16_t angle;//方向盘转角
//	nint8_t angle;//方向盘转角
//	nint8_t Release2;//预留字段

	MotionConfidenceSet_t1 motionCfd;//运行状态精度

	AccelerationSet4Way_t1 accelSet;//车辆加速度

	BrakeSystemStatus_t1 brakes;//刹车系统状态

	VehicleSize_t1 size;//车辆尺寸

	uint16_t vehicleClass;//车辆类型
	uint8_t vehicleLight[2];//车灯状态

	nint8_t Release3[10];//预留字段

//	nint8_t TurnLeftSwitchSt;
//	nint8_t TurnRightSwitchSt;
//	nint32_t TotalOdmeter;
//	nint8_t EPS_Lampst;
}__attribute__ ((packed))  CANData;

/**
 * @brief 接收CAN数据线程.
 *
 * 接受CAN数据线程，并按照定义解析；
 *
 * @param[in] p  线程输入参数.
 */
void Thread_RecvCANData(void *p);

/**
 * @brief CAN_Open CAN模块初始化.
 *
 * 打开获取CAN数据的串口，并开启接收CAN数据线程；
 *
 * @param[in] pCanName  CAN接口名称.
 *
 * @return  执行状态，0 执行成功，非0 执行失败.
 */
extern int CAN_Open(char *pCanName);

/**
 * @brief CAN_GetData 读取CAN数据接口
 *
 * 读取当前最新的CAN数据，CAN数据格式按照指定的数据格式传出；
 *
 * @param[in] fix	最新获取的CAN信息的指针
 * @return  执行状态，0 执行成功，非0 执行失败.
 */
extern int CAN_GetData(CANData * fix);
/**
 * @brief CAN_SendData 发送CAN数据.
 *
 * 往sock端口中发送数据，数据内容为str，数据长度为len；
 *
 * @param[in] sock  数据通讯端口句柄.
 * @param[in] str   数据内容.
 * @param[in] len   数据长度.
 * @return  执行状态，0 执行成功，非0 执行失败.
 */
extern int CAN_SendData(int sock,char* str,int len);
 /**
 * @brief 关闭CAN模块.
 *
 * 关闭CAN数据获取模块，即关闭CAN串口，停止CAN数据接收线程，清空相关变量内容；
 *
 * @return  执行状态，0 执行成功，非0 执行失败.
 */
extern int CAN_Close();

#endif //#ifndef __CAN_H_


/**
 * @}
 */
