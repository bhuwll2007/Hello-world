/*
 * TcpClient.h
 *
 *  Created on: Jul 15, 2019
 *      Author: wanji-hxy
 */

#ifndef APPLYLAYER_V2X_TRANSF_TCPCLIENT_H_
#define APPLYLAYER_V2X_TRANSF_TCPCLIENT_H_


#include "./../DataSet/DataSet.h"
#include "./../../Include/FaciLayer_Include.h"
#include "./../../Include/BaseInterface_Include.h"


int Tcp_InitTcpClientSoc(int *pSocket);
int Tcp_InitTcpCltCntSer(char p_acIp[], int p_nPort, int lsocked);
int Tcp_TcpCltSnd2Data(int sockfd, char p_abuf[], int p_nBufSize);
int Tcp_TcpCltRcvData(int sockfd, char p_abuf[], int *p_nBufSize);
int Tcp_ClsSoc(int sockfd);

#endif /* APPLYLAYER_V2X_TRANSF_TCPCLIENT_H_ */
