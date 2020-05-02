/*
 * MIBUtil.c
 *
 *  Created on: Aug 22, 2019
 *      Author: wanji-hxy
 */
#include "MIBUtil.h"

tDSMMIB g_DSMMIB;

static tApplicationMIB_list g_MIBlist_head;
static int g_MIBlist_node_cnt = 0;

int DSM_MIB_Setup(uint16_t aid[], int nAidNum)
{
	//设置DSM最大消息长度
	g_DSMMIB.SystemMIB.LocalInfo.DSMMaxLength = 2048;

	/** AID链表创建 */
	INIT_LIST_HEAD(&g_MIBlist_head.list);

	/** 往链表添加注册aid */
	int i;
	for (i = 0; i < nAidNum; i++)
	{
		g_DSMMIB.pApplicationMIB_lsit = (tApplicationMIB_list *) calloc(1,
				sizeof(tApplicationMIB_list));

		g_DSMMIB.pApplicationMIB_lsit->UserServiceInfo.ApplicatonIdentifier =
				aid[i];

		list_add_tail(&g_DSMMIB.pApplicationMIB_lsit->list,
				&g_MIBlist_head.list);
		g_MIBlist_node_cnt++;
	}
	return 0;
}

/** 链表查找AID(从头往尾查找) */
int DSM_AID_Check(uint16_t Aid)
{
	struct list_head *pos;
	tApplicationMIB_list *p;

	list_for_each(pos, &g_MIBlist_head.list)
	{
		p = list_entry(pos, tApplicationMIB_list, list);
		if (p->UserServiceInfo.ApplicatonIdentifier == Aid)
		{
#if NETLAYER_PRINTLOG
			printf("found AID in list\n");
#endif
			return 0;
		}
	}
#if NETLAYER_PRINTLOG
	printf("This AID is not in MIB\n");
#endif
	return -1;
}
