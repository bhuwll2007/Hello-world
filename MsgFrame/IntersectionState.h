/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "SPATIntersectionState"
 * 	found in "SPATIntersectionState.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_IntersectionState_H_
#define	_IntersectionState_H_


#include <asn_application.h>

/* Including external dependencies */
#include "NodeReferenceID.h"
#include "IntersectionStatusObject.h"
#include "MinuteOfTheYear.h"
#include "DSecond.h"
#include "TimeConfidence.h"
#include "PhaseList.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* IntersectionState */
typedef struct IntersectionState {
	NodeReferenceID_t	 intersectionId;
	IntersectionStatusObject_t	 status;
	MinuteOfTheYear_t	*moy	/* OPTIONAL */;
	DSecond_t	*timeStamp	/* OPTIONAL */;
	TimeConfidence_t	*timeConfidence	/* OPTIONAL */;
	PhaseList_t	 phases;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} IntersectionState_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_IntersectionState;
extern asn_SEQUENCE_specifics_t asn_SPC_IntersectionState_specs_1;
extern asn_TYPE_member_t asn_MBR_IntersectionState_1[6];

#ifdef __cplusplus
}
#endif

#endif	/* _IntersectionState_H_ */
#include <asn_internal.h>
