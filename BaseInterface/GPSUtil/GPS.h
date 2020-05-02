/**
 * @defgroup GPSUtil GPSUtil API
 * @ingroup BaseInterface API
 * @{
 *
 * 本模块提供对GPS数据进行操作的API接口.
 *
 * @file GPS.h
 * @brief API file.
 *
 */

#ifndef __GPS_H_
#define __GPS_H_
#include "./../DataSet.h"
#include "./../ConfUtil/Config.h"

/**
 * @brief Represents a location.
 */
typedef struct
{
    /**
     *  Represents latitude in degrees*10000000.
     */
    int		latitude;
    /**
     *  Represents longitude in degrees*10000000.
     */
    int		longitude;
    /**
     * Represents altitude in decimetres above the WGS 84 reference ellipsoid.
     */
    int		altitude;
    /**
     * Represents speed in meters per second.
     */
    double	speed;
    /**
     * Represents heading in degrees.
     */
    int		bearing;
    /**
     * Represents expected accuracy in meters.
     */
    int		accuracy;
//	/**
//	 * Timestamp for the location fix.
//	 */
//	GnssUtcTime     timestamp;
} GpsLocation;

/**
 * @brief GPS模块初始化.
 *
 * @brief 打开获取GPS数据的串口，并开启接收GPS数据线程；
 * @brief 默认开发 /etc/V2X_APP.ini文件中的[GPSUtil] -> GPSName 的值,
 * @brief 注意，GPS串口名称在/etc/V2X_APP.ini文件中的[GPSUtil]-GPSName中获取，
 * @brief 如果没有该字段或者配置文件，请手动添加。
 *
 * @return  执行状态，0 执行成功，非0 执行失败.
 */
extern int GPS_Open();

/**
 * @brief 获取当前GPS数据.
 *
 * @brief 读取当前最新的GPS数据，GPS数据格式按照指定的数据格式传出；
 *
 * @param[out] fix  最新获取的GPS信息的指针.
 *
 * @return  执行状态，0 执行成功，非0 执行失败.
 */
extern int GPS_GetData(GpsLocation * fix);

/**
 * @brief 关闭GPS模块.
 *
 * 关闭GPS数据获取模块，即关闭GPS串口，停止GPS数据接收线程，清空相关变量内容；
 *
 * @return  执行状态，0 执行成功，非0 执行失败.
 */
extern int GPS_Close();


#endif //#ifndef __GPS_H_
/**
 * @}
 */
