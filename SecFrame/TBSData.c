/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "SecMsg"
 * 	found in "SecMsg.asn"
 * 	`asn1c -gen-PER`
 */

#include "TBSData.h"

static int
memb_data_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	const OCTET_STRING_t *st = (const OCTET_STRING_t *)sptr;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	
	if(1 /* No applicable constraints whatsoever */) {
		(void)st; /* Unused variable */
		/* Nothing is here. See below */
	}
	
	return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
}

static int
memb_extHash_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	const OCTET_STRING_t *st = (const OCTET_STRING_t *)sptr;
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	size = st->size;
	
	if((size == 32)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_memb_data_constr_3 CC_NOTUSED = {
	{ 0, 0 },
	-1	/* (SIZE(0..MAX)) */};
static asn_per_constraints_t asn_PER_memb_data_constr_3 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_SEMI_CONSTRAINED,	-1, -1,  0,  0 }	/* (SIZE(0..MAX)) */,
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_extHash_constr_4 CC_NOTUSED = {
	{ 0, 0 },
	32	/* (SIZE(32..32)) */};
static asn_per_constraints_t asn_PER_memb_extHash_constr_4 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 0,  0,  32,  32 }	/* (SIZE(32..32)) */,
	0, 0	/* No PER value map */
};
asn_TYPE_member_t asn_MBR_TBSData_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct TBSData, headerInfo),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_HeaderInfo,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"headerInfo"
		},
	{ ATF_POINTER, 2, offsetof(struct TBSData, data),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_OCTET_STRING,
		0,
		{ &asn_OER_memb_data_constr_3, &asn_PER_memb_data_constr_3,  memb_data_constraint_1 },
		0, 0, /* No default value */
		"data"
		},
	{ ATF_POINTER, 1, offsetof(struct TBSData, extHash),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_OCTET_STRING,
		0,
		{ &asn_OER_memb_extHash_constr_4, &asn_PER_memb_extHash_constr_4,  memb_extHash_constraint_1 },
		0, 0, /* No default value */
		"extHash"
		},
};
static const int asn_MAP_TBSData_oms_1[] = { 1, 2 };
static const ber_tlv_tag_t asn_DEF_TBSData_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_TBSData_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* headerInfo */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* data */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* extHash */
};
asn_SEQUENCE_specifics_t asn_SPC_TBSData_specs_1 = {
	sizeof(struct TBSData),
	offsetof(struct TBSData, _asn_ctx),
	asn_MAP_TBSData_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_TBSData_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_TBSData = {
	"TBSData",
	"TBSData",
	&asn_OP_SEQUENCE,
	asn_DEF_TBSData_tags_1,
	sizeof(asn_DEF_TBSData_tags_1)
		/sizeof(asn_DEF_TBSData_tags_1[0]), /* 1 */
	asn_DEF_TBSData_tags_1,	/* Same as above */
	sizeof(asn_DEF_TBSData_tags_1)
		/sizeof(asn_DEF_TBSData_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_TBSData_1,
	3,	/* Elements count */
	&asn_SPC_TBSData_specs_1	/* Additional specs */
};

