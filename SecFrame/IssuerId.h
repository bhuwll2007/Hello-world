/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "CER"
 * 	found in "CER.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_IssuerId_H_
#define	_IssuerId_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NULL.h>
#include "CertificateDigest.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum IssuerId_PR {
	IssuerId_PR_NOTHING,	/* No components present */
	IssuerId_PR_self,
	IssuerId_PR_certificateDigest
	/* Extensions may appear below */
	
} IssuerId_PR;

/* IssuerId */
typedef struct IssuerId {
	IssuerId_PR present;
	union IssuerId_u {
		NULL_t	 self;
		CertificateDigest_t	 certificateDigest;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} IssuerId_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_IssuerId;
extern asn_CHOICE_specifics_t asn_SPC_IssuerId_specs_1;
extern asn_TYPE_member_t asn_MBR_IssuerId_1[2];
extern asn_per_constraints_t asn_PER_type_IssuerId_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _IssuerId_H_ */
#include <asn_internal.h>
