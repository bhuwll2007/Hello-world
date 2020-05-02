/***************************************************************//**
 * @defgroup RsmConfig RsmConfig API
 * @ingroup FaciLayer
 * @{
 *
 *  This module provides APIs to do the operate about RsmConfig.
 *
 * @file RsmConfig.h
 * @brief API file.
 *  Created on: 20200310
 *      Author: chaizhijun
 *******************************************************************/

#ifndef __RSMCONFIG_H_
#define __RSMCONFIG_H_
#include "./../Algorithm/Type.h"
#include "./../Include/BaseInterface_Include.h"
//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Macros & Constants
//------------------------------------------------------------------------------
/****************************************************************************
* 与激光雷达消息接口定义              start add by huangsai
*修改者：柴志军
*修改时间：20200310
*修改原因：结构体定义和成员变量定义不规范
*修改内容：按照万集科技编码规范，结构体定义：WJrect，成员变量定义：m_unXX
****************************************************************************/
typedef enum enumSub
{
    LaserRawData, /* 使用激光标志位 */
    GeodeticLatLon, /* 使用大地标志位 */
    LaserAndGeodetic /* 同时使用标志位 */
}enumSub_t;
typedef enum enumType
{
    unknow,
    vehicle,
    Truck,
    bus,
    person,
    Bicycle,
    Motorcycle
}enumtType_t;
typedef enum enumColor
{
    unknown,
    white,
    black,
    red,
    silvery,
    yellow,
    blue,
    colour
}enumColor_t;
typedef enum enumInformation
{
    laser,
    video,
    LaV
}enumInformation_t;
typedef struct rectLasetParticipants
{
    uint16_t m_usId;                       /* 交通参与者ID */
    uint8_t m_ucEnumtTrafficType;    /* 交通参与者类型 */
    uint8_t m_ucConfidenceLevel;            /* 置信度 */
    uint8_t m_ucEnumMyColor;         /* 交通参与者颜色 */
    uint8_t m_ucEnumFrom;      /* 信息来源 */
    uint8_t m_ucBUF[2];                     /* 扩展字段 */
    uint16_t m_usCoordinate_X;                 /* x轴坐标CM */
    uint16_t m_usCoordinate_Y;                /* y轴坐标CM */
    uint16_t m_usCoordinate_Z;                /* z轴坐标CM */
    uint16_t m_usSpeed;                       /* 速度CM/S */
    uint16_t m_usDirection;                    /* 运动方向 */
    uint16_t m_usLongth;                       /* 长度cm */
    uint16_t m_usWidth;                        /* 宽度cm */
    uint16_t m_usHeight;                       /* 高度cm */
    uint8_t m_ucExtension[4];                      /* 扩展字段 */
}rectLasetParticipants_t;
typedef struct rectGeoDeticParticipants
{
    uint16_t m_usId;                       /* 交通参与者ID */
    uint8_t m_ucEnumtTrafficType;    /* 交通参与者类型 */
    uint8_t m_ucConfidenceLevel;            /* 置信度 */
    uint8_t m_ucEnumMyColor;          /* 交通参与者颜色 */
    uint8_t m_ucEnumFrom;      /* 信息来源 */
    uint8_t m_ucBUF[2];                     /* 扩展字段 */
    uint32_t m_unLat;                      /* 经度 */
    uint32_t m_unLon;                      /* 纬度 */
    uint16_t m_usAltitude;                  /* 海拔 */
    uint16_t m_usSpeed;                    /* 速度CM/S */
    uint16_t m_usHeadAnagle;                /* 航向角 */
    uint16_t m_usLongth;                   /* 长度cm */
    uint16_t m_usWidth;                     /* 宽度cm */
    uint16_t m_usHeight;                    /* 高度cm */
    uint8_t m_ucExtension[4];                /* 扩展字段 */
}rectGeoDeticParticipants_t;
typedef struct rectLaGParticipants
{
    uint16_t m_usId;                       /* 交通参与者ID */
    uint8_t m_ucEnumtTrafficType;    /* 交通参与者类型 */
    uint8_t m_ucConfidenceLevel;            /* 置信度 */
    uint8_t m_ucEnumMyColor;          /* 交通参与者颜色 */
    uint8_t m_ucEnumFrom;      /* 信息来源 */
    uint8_t m_ucBUF[2];                     /* 扩展字段 */
    uint32_t m_unLat;                      /* 经度 */
    uint32_t m_unLon;                      /* 纬度 */
    uint16_t m_usAltitude;                  /* 海拔 */
    uint16_t m_usSpeed;                    /* 速度CM/S */
    uint16_t m_usHeadAnagle;                /* 航向角 */
    uint16_t m_usLongth;                    /* 长度cm */
    uint16_t m_usWidth;                     /* 宽度cm */
    uint16_t m_usHeight;                    /* 高度cm */
    uint16_t m_usCoordinate_X;                 /* x轴坐标CM */
    uint16_t m_usCoordinate_Y;                /* y轴坐标CM */
    uint16_t m_usCoordinate_Z;                /* z轴坐标CM */
    uint8_t m_ucExtension[2];                      /* 扩展字段 */
}rectLaGParticipants_t;
typedef struct rectMSGLaserToV2X
{
    uint16_t m_usStartByte;             /* 帧开始位，固定0xFF 0xFF */
    uint8_t m_ucSerNum;                  /* 数据帧序列号0x00～0x0F */
    uint8_t m_ucMainCmd;                 /* 主命令号 */
    uint8_t m_ucEnumSubCmd;         /* 子命令号 */
    uint8_t m_ucStatusByte;              /* 状态位：00H,此时无意义 */
    uint16_t m_usMessageLength;         /* 消息长度 */
    uint16_t m_usDevId;      /* 交通参与者ID */
    uint16_t m_usExtension;  /* 扩展字段 */
    uint32_t m_unFrame;      /* 点云数据帧号 */
    uint64_t m_ulTimeSec;    /* 时间戳 秒 */
    uint32_t m_unLat;        /* 激光器原点经度 */
    uint32_t m_unLon;        /* 激光器原点纬度 */
    uint16_t m_usAngle;      /* 激光器角度 */
    uint8_t m_ucParticipantsNum; /* 交通参与者数量 */
    uint8_t m_ucExtension[5];    /* 扩展字段 */
    union MsgContent
    {              /* 消息内容信息 */
        rectLasetParticipants_t sub1[100];      /* 激光器坐标系原始数据 */
        rectGeoDeticParticipants_t    sub2[100]; /* 大地坐标系原始数据 */
        rectLaGParticipants_t    sub3[100];     /* 大地激光同时输出 */
    }choice;
}rectMSGLaserToV2X_t;
#define INT64_TO_NET(val)	((uint64_t) ( \
      (((uint64_t) (val) &						\
	(uint64_t) (0x00000000000000ffU)) << 56) |	\
      (((uint64_t) (val) &						\
	(uint64_t) (0x000000000000ff00U)) << 40) |	\
      (((uint64_t) (val) &						\
	(uint64_t) (0x0000000000ff0000U)) << 24) |	\
      (((uint64_t) (val) &						\
	(uint64_t) (0x00000000ff000000U)) <<  8) |	\
      (((uint64_t) (val) &						\
	(uint64_t) (0x000000ff00000000U)) >>  8) |	\
      (((uint64_t) (val) &						\
	(uint64_t) (0x0000ff0000000000U)) >> 24) |	\
      (((uint64_t) (val) &						\
	(uint64_t) (0x00ff000000000000U)) >> 40) |	\
      (((uint64_t) (val) &						\
	(uint64_t) (0xff00000000000000U)) >> 56)))
/****************************************************************************
* 与激光雷达消息接口定义              end add by huangsai
****************************************************************************/

/**********************************************
*功能：释放激光雷达数据结构变量资源
*输入参数： void
*输出参数： 无
*函数返回值： 无
作者： 柴志军
生成时间：2020-03-10 版本号：V1.0.0
修改记录：无
************************************************/
extern void FG_RecvFromLaserMsgFree(void);

/**********************************************
*功能：获取接收到的激光雷达数据结构变量
*输入参数： 无
*输出参数：p_prectMSGLaserToV2Xt  激光雷达结构数据
*函数返回值>=0正常, <0:错误码；
作者： 柴志军
生成时间：2020-03-10 版本号：V1.0.0
修改记录：无
************************************************/
extern int FG_RecvFromLaserMsgGet(rectMSGLaserToV2X_t *p_prectMSGLaserToV2Xt);

/**********************************************
*功能：填充接收到的激光雷达数据结构变量
*输入参数： p_prectMSGLaserToV2Xt  激光雷达结构数据
*输出参数： 无
*函数返回值>=0正常, <0:错误码；
作者： 柴志军
生成时间：2020-03-10 版本号：V1.0.0
修改记录：无
************************************************/
extern int FG_RecvFromLaserMsgSet(rectMSGLaserToV2X_t *p_prectMSGLaserToV2Xt);

#endif /*__LASERDATA_H_*/

/**
 * @}
 */
