/**
 * @file xds_application.h
 * @author cwy
 * @version 1.0
 * @date 2018-3-4
 * @brief Interface function prototype declaration.
 */

#ifndef XDS_APPLICATION_H_
#define XDS_APPLICATION_H_

#include <inttypes.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *@brief Open sub service.
 *@param[in] ssid Sub Service ID
 *@param[in] flag Reserved
 *@return    -1 for failure, > 0 for handle
 */
extern int v2x_open(int ssid, void* flag);

/**
 *@brief Close sub service.
 *@param[in] hdl Service handle, obtained from the v2x_open interface
 */
extern void v2x_close(int hdl);

/**
 *@brief Read message actively with handle and MSGID
 *@param[in]  hdl   Service handle, obtained from the v2x_open interface
 *@param[in]  msgid Specifically defined by sub services
 *@param[out] buf   Buffer prepared by user, message will be copied here
 *@param[in]  len   Length of the user buffer
 *@return     -1 for failure, > 0 for message length
 */
extern int v2x_read(int hdl, uint16_t msgid, void* buf, uint16_t len);

/**
 *@brief Subscribe message with handle and MSGID
 *@param[in] hdl   Service handle, obtained from the v2x_open interface
 *@param[in] msgid Specifically defined by sub services
 *@return    -1 for failure, 0 for success
 */
extern int v2x_sub(int hdl, uint16_t msgid);

/**
 *@brief UnSubscribe message with handle and MSGID
 *@param[in] hdl   Service handle, obtained from the v2x_open interface
 *@param[in] msgid Specifically defined by sub services
 *@return    -1 for failure, 0 for success
 */
extern int v2x_unsub(int hdl, uint16_t msgid);

/**
 *@brief Receive message passively(needs v2x_sub first)
 *@param[in]  hdl  Service handle, obtained from the v2x_open interface
 *@param[out] buf  Buffer prepared by user, message will be copied here
 *@param[in]  len  Length of the user buffer
 *@param[in]  flag Reserved
 *@return     -1 for failure, > 0 for message length
 */
extern int v2x_recv(int hdl, void* buf, uint16_t len, int flag);

/**
 *@brief Called after v2x_recv to detach additional messages
 *@param[in]  hdl  Service handle, obtained from the v2x_open interface
 *@param[in]  type Specifically defined by sub services
 *@param[out] buf  Buffer prepared by user, message will be copied here
 *@param[in]  len  Length of the user buffer
 *@return     -1 for failure, > 0 for additional message length
 */
extern int v2x_recv_detach(int hdl, uint16_t type, void* buf, uint16_t len);

/**
 *@brief Called after v2x_recv to detach MSGID
 *@param[in] hdl Service handle, obtained from the v2x_open interface
 *@return    -1 for failure, > 0 for MSGID
 */
extern int v2x_recv_msgid(int hdl);

/**
 *@brief Send Message
 *@param[in]  hdl  Service handle, obtained from the v2x_open interface
 *@param[in] buf  Buffer prepared by user, the content will be sent
 *@param[in]  len  Length of the user buffer
 *@param[in]  flag Reserved
 *@return     -1 for failure, > 0 Length of data successfully sent
 */
extern int v2x_send(int hdl, const void* buf, uint16_t len, int flag);

/**
 *@brief Called before v2x_send to attach additional message
 *@param[in]  hdl  Service handle, obtained from the v2x_open interface
 *@param[in]  type Specifically defined by sub services
 *@param[in] buf  Buffer prepared by user, the content will be attached
 *@param[in]  len  Length of the user buffer
 *@return     -1 for failure,  > 0 Length of data successfully attached
 */
extern int v2x_send_attach(int hdl, uint16_t type, void* buf, uint16_t len);

/**
 *@brief Called before v2x_send to attach MSGID(for some situations where a MSGID is required)
 *@param[in] hdl Service handle, obtained from the v2x_open interface
 *@return    -1 for failure, 0 for success
 */
extern int v2x_send_msgid(int hdl, uint16_t msgid);

/**
 *@brief Get the properties of the Sub service. Specific functions are defined by sub services
 *@param[in]  hdl Service handle, obtained from the v2x_open interface
 *@param[in]  cmd Specifically defined by sub services
 *@param[out] buf Buffer prepared by user, the properties will be copied here
 *@param[in]  len Length of the user buffer
 *@return     -1 for failure, 0 for success
 */
extern int v2x_getopt(int hdl, uint16_t cmd, void* buf, uint16_t len);

/**
 *@brief Set the properties of the Sub service. Specific functions are defined by sub services
 *@param[in]  hdl Service handle, obtained from the v2x_open interface
 *@param[in]  cmd Specifically defined by sub services
 *@param[in] buf Buffer prepared by user, the properties will be sent
 *@param[in]  len Length of the user buffer
 *@return     -1 for failure, 0 for success
 */
extern int v2x_setopt(int hdl, uint16_t cmd, const void* buf, uint16_t len);

/**
 *@brief Get the error number of the last called function.
 *@return error number
 */
extern int v2x_errno();

/**
 *@brief Get the error description of the last called function.
 *@return error description
 */
extern const char* v2x_strerror();

#include "service/GNSS.h"
#include "service/DSMP.h"
#include "service/PDCP.h"
#include "service/Modem.h"
#include "service/Utils.h"
//#include "service/TrafficLight.h"
#include "service/StackTime.h"

#ifdef __cplusplus
}
#endif


#endif /* XDS_APPLICATION_H_ */
