/**
 * @defgroup Algorithm Algorithm API
 * @{
 *
 * 本模块提供V2X算法接口层的API接口.
 *
 * @}
 *
 */

/**
 * @ingroup Algorithm API
 * @{
 *
 * 本模块V2X算法接口层的API接口 - 算法参数结构体/常量相关.
 *
 * @file Appconfigure.h
 * @brief API file.
 *
 */
#ifndef ALGORITHM_APPCONFIGURE_H_
#define ALGORITHM_APPCONFIGURE_H_

///类型声明
typedef unsigned long long int	uint64_t;
typedef unsigned int 			uint32_t;
typedef unsigned short 			uint16_t;
typedef unsigned char 			uint8_t;
typedef long long int 			nint64_t;
typedef int 					nint32_t;
typedef short 					nint16_t;
typedef char 					nint8_t;

/// Parameter for MapMatch()
const static double CofficDis = 0.5;
const static double CofficHeading = 0.5;

/// Parameter for FG_WARN_LVL_JUDGE()
#define V2V_FCW_TTC 1
#define V2V_FCW_THW 2
#define V2V_ICW_TTC 3
#define V2V_MAY_DANGER 4

///Parameter for V2V_EVW_Section()
enum Emergency_VehcileClass
{
    TypeUnknown = 60,
    TYpeOther = 61,
    Fire_Light = 62,
    Fire_Heavy = 63,
    Fire_Paramedic = 64,
    Fire_Ambulance = 65,
    Police_Light = 66,
    Police_Heavy = 67,
    Other_Responder = 68,
    Other_Ambulance = 69
};
// -------------------- split line ------------------------
///
/// Define the OV location area
///
///   ----------------------------
///  |   6     |    2   |    7   |
///  		   ----------
///  |---------|   HV   |--------|
///  		   ----------
///  |    4    |    5   |    6   |
///  -----------------------------
///


///--|-----|-----|-----|---------/
///--|  6  |  2  |  7  |---------///////
///--|     |-----|     |---------///////
///------------------------------///////
///--|-----|- H -|-----|---------///////
///--|-----|-   -|-----|---------///////
///--|-----|- V -|-----|---------///////
///------------------------------///////
///--|     |-----|     |---------///////
///--|  8  |  3  |  9  |---------///////
///--|-----|-----|-----|---------///////



/**
 * @brief 车辆方位.
 */
enum VEHICLE_LOC
{
    LOC_DEFAULT = -1,

    FRONT_LOC = 1,  			/**< 前方. */
    FRONT_LOC_DIRECT = 2,  		/**< 正前方. */
    BACK_LOC = 3,				/**< 后方. */
    BACK_LOC_SIDE = 4,			/**< 侧后方. */


    FRONT_LOC_Left = 6,			/**< 左前方. */
    FRONT_LOC_Right = 7,		/**< 右前方. */
    BACK_LOC_Left,				/**< 左后方. */
    BACK_LOC_Right,				/**< 右后方. */
};
// -------------------- split line ------------------------

/**
 * @brief 车辆灯光信息.
 */
enum LIGHT_STATUS
{
    lowBeamHeadlinghtsOn = 0x00,
    highBeamHeadlinghtsOn = 0x02,
    leftTurnSignalOn = 0x04,
    rightTurnSignalOn = 0x08,
    hazardSignalOn = 0x010,
    automaticLightContorlOn = 0x20,
    daytimeRunningLightsOn = 0x40,
    fogLightOn = 0x80,
    ParKingLightsOn = 0x100
};
// -------------------- split line ------------------------

/**
 * @brief V2I应用类型.
 */
typedef enum APP_TYPE
{
    Type_DEFAULT = -1,
    Type_BSM = 1,
    Type_RSI = 2,
    Type_SPAT = 3
} APP_TYPE;

/**
 * @brief V2V事件类型.
 */
typedef enum WARN_CASE
{
    STR_DEFAULT = -1,		/**< 默认状态. */

    CVF_Warning = 1,		/**< 跟车过近. */
    FCW_Warning = 2,		/**< 前向碰撞预警. */
    EBW_Warning = 3,		/**< 紧急制动预警. */

    ICW_Warning = 4,		/**< 交叉路口碰撞预警. */
    LTA_Warning = 5,		/**< 左转辅助. */
    DNPW_Waring = 6,		/**< 逆向超车预警. */

    LCW_Warning = 7,		/**< 变道预警. */
    BSW_Warning = 8,		/**< 盲区预警. */

    AVW_Warning = 9,		/**< 异常车辆提醒. */
    SVW_Warning = 12,		/**< 静止车辆预警. */
    SLV_Warning = 13,		/**< 慢速车辆预警. */

    CLW_Warning = 10,		/**< 失控车辆预警. */

    EVW_Warning = 11		/**< 紧急车辆预警. */
} WARN_CASE;

/**
 * @brief 警告优先级.
 */
typedef enum WARN_PRIO
{
    PRIO_DEFAULT = 100,
    HIGH_PRIO = 1,
    MEDIUM_PRIO = 2,
    LOW_PRIO = 3,
    Lower_V2I_PRIO = 4
} WARN_PRIO;

/**
 * @brief 警告等级.
 */
typedef enum WARN_LVL
{
    LVL_DEFAULT = -1,
    LOW_LVL = 1,
    MEDIUM_LVL = 2,
    HIGH_LVL = 3
} WARN_LVL;


//---------------------------------------SPAT ZONE---------------------------------------------//
/* -------------------- split line ------------------------
/// Parameter for LightDirection_Detect()
const static double LightDir_Dis_Value = 30;  //判断下游节点相对为 横向偏移阈值

/// Parameter for SpeedGuide()
const static double WarnCalcTime = 30;

const static double SpeedLimit_High = 15;
const static double SpeedLimit_Low = 3;
*/

/**
 * @brief 红绿灯方向.
 */
enum LightDirection
{
    Turn_Left = 1,
    Go_straight = 2,
    Turn_Right = 3,
    Turn_Round = 4
};

/**
 * @brief 应用中表示信号灯状态的枚举类型.
 */
enum LightStatus
{
    App_LightState_unavailable	= 0,					/**< 未知状态. */
    App_LightState_dark	= 1,							/**< 信号灯未工作. */
    App_LightState_Red_Flash	= 2,					/**< 红闪. */
    App_LightState_Red	= 3,							/**< 红灯状态. */
    App_LightState_pre_Movement	= 4,					/**< 绿灯待行状态. */
    App_LightState_Green	= 5,						/**< 绿灯状态. */
    App_LightState_protected_Movement_Allowed	= 6,	/**< 受保护相位绿灯. */
    App_LightState_Yellow	= 7,						/**< 黄灯状态. */
    App_LightState_Yellow_Flash	= 8						/**< 黄闪. */
};

/**
 * @brief 当前V2I应用类型.
 */
typedef enum V2I_WARN_CASE
{

    V2I_STR_DEFAULT = -1,		/**< 默认状态. */
    IVS_Warning = (1 << 0),		/**< 车内标牌. */
    HLW_Warning = (1 << 1),		/**< 道路危险状况提示. */
    SLW_Warning = (1 << 2),		/**< 道路限速提醒. */

    TJW_Warning = (1 << 3),		/**< 前方拥堵提醒. */
    GLOSA_Warning = (1 << 4),	/**< 滤波车速引导. */
    RLVW_Waring = (1 << 5),		/**< 闯红灯预警. */

    VRUCW_Warning = (1 << 6)	/**< 弱势交通参与者碰撞预警. */

} V2I_WARN_CASE;

//---------------------------------------RSI ZONE---------------------------------------------//
/**
 * @brief 限速类型.
 */
typedef enum SpeedLimitMode
{
    SpeedNoLimit = 0,			/**< 无限速. */
    SpeedLowLimit = 1 << 0,		/**< 有最低限速. */
    SpeedHighLimit = 1 << 1,	/**< 有最高限速. */
    SpeedAllLimit = SpeedLowLimit | SpeedHighLimit	/**< 有最高和最低限速. */

} e_SpeedLimitMode;

#endif /* ALGORITHM_APPCONFIGURE_H_ */

/**
 * @}
 */
