/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "RSI"
 * 	found in "RSI.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_RTSData_H_
#define	_RTSData_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include "SignType.h"
#include "RSIPriority.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct PositionOffsetLLV;
struct Description;
struct RSITimeDetails;
struct ReferencePathList;
struct ReferenceLinkList;

/* RTSData */
typedef struct RTSData {
	long	 rtsId;
	SignType_t	 signType;
	struct PositionOffsetLLV	*signPos	/* OPTIONAL */;
	struct Description	*description	/* OPTIONAL */;
	struct RSITimeDetails	*timeDetails	/* OPTIONAL */;
	RSIPriority_t	*priority	/* OPTIONAL */;
	struct ReferencePathList	*referencePaths	/* OPTIONAL */;
	struct ReferenceLinkList	*referenceLinks	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RTSData_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RTSData;
extern asn_SEQUENCE_specifics_t asn_SPC_RTSData_specs_1;
extern asn_TYPE_member_t asn_MBR_RTSData_1[8];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "PositionOffsetLLV.h"
#include "Description.h"
#include "RSITimeDetails.h"
#include "ReferencePathList.h"
#include "ReferenceLinkList.h"

#endif	/* _RTSData_H_ */
#include <asn_internal.h>
