/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "BASE"
 * 	found in "BASE.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_SequenceOfRectangularRegion_H_
#define	_SequenceOfRectangularRegion_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct RectangularRegion;

/* SequenceOfRectangularRegion */
typedef struct SequenceOfRectangularRegion {
	A_SEQUENCE_OF(struct RectangularRegion) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SequenceOfRectangularRegion_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SequenceOfRectangularRegion;
extern asn_SET_OF_specifics_t asn_SPC_SequenceOfRectangularRegion_specs_1;
extern asn_TYPE_member_t asn_MBR_SequenceOfRectangularRegion_1[1];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "RectangularRegion.h"

#endif	/* _SequenceOfRectangularRegion_H_ */
#include <asn_internal.h>
