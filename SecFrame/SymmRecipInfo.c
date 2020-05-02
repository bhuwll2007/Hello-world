/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "SecMsg"
 * 	found in "SecMsg.asn"
 * 	`asn1c -gen-PER`
 */

#include "SymmRecipInfo.h"

asn_TYPE_member_t asn_MBR_SymmRecipInfo_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct SymmRecipInfo, hashAlg),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_HashAlgorithm,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"hashAlg"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SymmRecipInfo, recipientId),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_HashedId8,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"recipientId"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct SymmRecipInfo, encKey),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_SymmetricCipherText,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"encKey"
		},
};
static const ber_tlv_tag_t asn_DEF_SymmRecipInfo_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_SymmRecipInfo_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* hashAlg */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* recipientId */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* encKey */
};
asn_SEQUENCE_specifics_t asn_SPC_SymmRecipInfo_specs_1 = {
	sizeof(struct SymmRecipInfo),
	offsetof(struct SymmRecipInfo, _asn_ctx),
	asn_MAP_SymmRecipInfo_tag2el_1,
	3,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_SymmRecipInfo = {
	"SymmRecipInfo",
	"SymmRecipInfo",
	&asn_OP_SEQUENCE,
	asn_DEF_SymmRecipInfo_tags_1,
	sizeof(asn_DEF_SymmRecipInfo_tags_1)
		/sizeof(asn_DEF_SymmRecipInfo_tags_1[0]), /* 1 */
	asn_DEF_SymmRecipInfo_tags_1,	/* Same as above */
	sizeof(asn_DEF_SymmRecipInfo_tags_1)
		/sizeof(asn_DEF_SymmRecipInfo_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_SymmRecipInfo_1,
	3,	/* Elements count */
	&asn_SPC_SymmRecipInfo_specs_1	/* Additional specs */
};

