/**
 * @file Protocol.h
 * @author catt
 * @version 1.0
 * @date 2017-12-26
 * @brief XDS protocol definitions.
 * @details XDS protocol definitions.
 */

#ifndef PROTOCOL_PROTOCOL_H_
#define PROTOCOL_PROTOCOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

#define PROTOCOL_MESSAGE_HEAD   0xFEFE  /**< General Message Header Flag */
#define PROTOCOL_OPTION_MAXLEN  1024    /**< Maximum length of option or attachment data */
#define PROTOCOL_FIXHEAD_LEN    16

typedef enum ProtocolVersion_e
{
    E_PROTOCOL_VERSION_BAD = 0,
    E_PROTOCOL_VERSION_01  = 1,
}ProtocolVersion;

typedef enum OpCode_e
{
    E_OP_CLI2SER_OPEN_REQ       = 1,
    E_OP_CLI2SER_CLOSE_REQ      = 2,
    E_OP_CLI2SER_READ_REQ       = 3,
    E_OP_CLI2SER_WRITE_REQ      = 4,
    E_OP_CLI2SER_GETOPT_REQ     = 5,
    E_OP_CLI2SER_SETOPT_REQ     = 6,
    E_OP_CLI2SER_SUB_REQ        = 7,
    E_OP_CLI2SER_UNSUB_REQ      = 8,
    E_OP_CLI2SER_KEEP_ALIVE_RSP = 9,
    E_OP_CLI2SER_RESERVED1      = 10,
    E_OP_CLI2SER_RESERVED2      = 11,

    E_OP_CLI2SER_SER2CLI_CUTOFF = 100,

    E_OP_SER2CLI_OPEN_RSP       = 101,
    E_OP_SER2CLI_CLOSE_RSP      = 102,
    E_OP_SER2CLI_READ_RSP       = 103,
    E_OP_SER2CLI_WRITE_RSP      = 104,
    E_OP_SER2CLI_GETOPT_RSP     = 105,
    E_OP_SER2CLI_SETOPT_RSP     = 106,
    E_OP_SER2CLI_SUB_RSP        = 107,
    E_OP_SER2CLI_UNSUB_RSP      = 108,
    E_OP_SER2CLI_KEEP_ALIVE_REQ = 109,
    E_OP_SER2CLI_PUSH_IND       = 110,
    E_OP_SER2CLI_PUSH_KA_REQ    = 111,  /**< Business data carry a HeartBeat request */
}OpCode;

typedef enum OptionType_e
{
    E_OPTION_FRAMEWORK_BASE  = 0,
    E_OPTION_MONITOR_ADDRESS = 0,
    E_OPTION_ERROR_REASON,
    E_OPTION_AFFIX_CONTENT,
    E_OPTION_CRYPTO_EABLE,
    E_OPTION_CERTIFICATE,

    E_OPTION_SUBSERVICE_BASE = 127,
}OptionType;

typedef enum RspCodeC2S_e
{
    E_RSPC2S_NORSP   = 0, /**< No need Response */
    E_RSPC2S_RSPNOW  = 1, /**< Response at once */
    E_RSPC2S_RSPLAZY = 2, /**< Response lazy */
}RspCodeC2S;

typedef enum RspCodeS2C_e
{
    E_RSPS2C_FRAMEWORK_BASE  = 0,
    E_RSPS2C_SUCCESS         = 0,
    E_RSPS2C_UNKNOWN_VERSION,
    E_RSPS2C_NOEXIST_SERVICE,
    E_RSPS2C_SERVICE_REOPEN,
    E_RSPS2C_NOTEVEN_OPEN,
    E_RSPS2C_OPTION_ILLEGAL,
    E_RSPS2C_UNIQUEID_ILLEGAL,
    E_RSPS2C_HEADLEN_ILLEGAL,
    E_RSPS2C_MONITOR_ADDR_ILLEGAL,
    E_RSPS2C_MAX_CLIENT_NUM,
    E_RSPS2C_OPERATION_UNREALIZED,
    E_RSPS2C_UNKONWN_OPCODE,
    /* frame work error ... */


    E_RSPS2C_SERVICE_BASE   = 127,
    /* sub service error ... */
}RspCodeS2C;

typedef struct ProtocolFixHead_s
{
    uint16_t m_HeadFlag;
    uint16_t m_CheckSum;

    uint16_t m_PacketLen;
    uint8_t  m_Version : 4;
    uint8_t  m_HeadLen : 4;
    uint8_t  m_SeqNum;
}ProtocolFixHead;

typedef struct ProtocolHead_s
{
    uint16_t m_HeadFlag;
    uint16_t m_CheckSum;

    uint16_t m_PacketLen;
    uint8_t  m_Version : 4;
    uint8_t  m_HeadLen : 4;
    uint8_t  m_SeqNum;

    uint8_t  m_OpCode;
    uint8_t  m_RspCode;
    uint16_t m_UniqueID;    /* serviceID */

    uint16_t m_MessageID;
    uint16_t m_OptionLen;
}ProtocolHead;

typedef struct ProtocolOption_s
{
    uint16_t m_Type;
    uint16_t m_Len;
}ProtocolOption;

#ifdef __cplusplus
}
#endif

#endif /* PROTOCOL_PROTOCOL_H_ */
