#
#DEVMODE_DSRC = n
#ifeq ($(DEVMODE_DSRC),y)
#  ARCH:=arm
#  ARCHITECTURE:=/home/wanji/wanji_bin/gcc-4.9-2016.02-x86_64_arm-linux-gnueabi/bin/arm-linux-gnueabi
#  CROSS_COMPILE:=$(ARCHITECTURE)-
#endif
#ifeq ($(DEVMODE_DSRC),n)
#  ARCH:=arm
#  ARCHITECTURE:=/home/wanji/wanji_bin/gcc-4.9-2016.02-x86_64_arm-linux-gnueabi/bin/arm-linux-gnueabi
#  CROSS_COMPILE:=$(ARCHITECTURE)-
#endif
#
#export ARCH
#export CROSS_COMPILE
#
#
#ifeq ($(DEVMODE_DSRC),y)
#	prefix=/home/wanji/wanji_bin/asn1c
#else
#	prefix=/home/wanji/wanji_bin/asn1c
#endif
#
#EXTRA_CFLAGS := -I$(prefix)/share/asn1c/
##LDFLAGS :=-L${prefix}/lib -lsqlite3 
#
#DEBUG = n
#ifeq ($(DEBUG),y)
#	DEBUG_CFLAGS := -O -g -DDEBUG
#else
#	DEBUG_CFLAGS := -O2
#endif
#
#CFLAGS := -Wall #-Werror -MD
#CFLAGS += $(DEBUG_CFLAGS)
#
#export EXTRA_CFLAGS
#export LDFLAGS
#export CFLAGS

APP_DIR := .
ASN_DIR := .
#ASN_MODULE_SRCS=	\
#	BasicSafetyMessage.c	\
#	AccelerationSet4Way.c	\
#	Acceleration.c	\
#	VerticalAcceleration.c	\
#	YawRate.c	\
#	Speed.c	\
#	Heading.c	\
#	CoarseHeading.c	\
#	SteeringWheelAngle.c	\
#	MotionConfidenceSet.c	\
#	HeadingConfidence.c	\
#	SpeedConfidence.c	\
#	SteeringWheelAngleConfidence.c	\
#	Latitude.c	\
#	Longitude.c	\
#	Elevation.c	\
#	PositionConfidenceSet.c	\
#	PositionConfidence.c	\
#	ElevationConfidence.c	\
#	Position3D.c	\
#	SemiMajorAxisAccuracy.c	\
#	SemiMinorAxisAccuracy.c	\
#	SemiMajorAxisOrientation.c	\
#	PositionalAccuracy.c	\
#	PositionOffsetLLV.c	\
#	OffsetLL-B12.c	\
#	OffsetLL-B14.c	\
#	OffsetLL-B16.c	\
#	OffsetLL-B18.c	\
#	OffsetLL-B22.c	\
#	OffsetLL-B24.c	\
#	Position-LL-24B.c	\
#	Position-LL-28B.c	\
#	Position-LL-32B.c	\
#	Position-LL-36B.c	\
#	Position-LL-44B.c	\
#	Position-LL-48B.c	\
#	Position-LLmD-64b.c	\
#	PositionOffsetLL.c	\
#	VerticalOffset.c	\
#	VertOffset-B07.c	\
#	VertOffset-B08.c	\
#	VertOffset-B09.c	\
#	VertOffset-B10.c	\
#	VertOffset-B11.c	\
#	VertOffset-B12.c	\
#	DSecond.c	\
#	DYear.c	\
#	DMonth.c	\
#	DDay.c	\
#	DHour.c	\
#	DMinute.c	\
#	DTimeOffset.c	\
#	DDateTime.c	\
#	MinuteOfTheYear.c	\
#	TimeMark.c	\
#	TimeOffset.c	\
#	TimeConfidence.c	\
#	MapData.c	\
#	LaneList.c	\
#	Lane.c	\
#	LaneID.c	\
#	LaneWidth.c	\
#	ConnectsToList.c	\
#	Connection.c	\
#	ConnectingLane.c	\
#	AllowedManeuvers.c	\
#	LaneAttributes.c	\
#	LaneSharing.c	\
#	LaneTypeAttributes.c	\
#	LaneAttributes-Vehicle.c	\
#	LaneAttributes-Crosswalk.c	\
#	LaneAttributes-Bike.c	\
#	LaneAttributes-Sidewalk.c	\
#	LaneAttributes-Barrier.c	\
#	LaneAttributes-Striping.c	\
#	LaneAttributes-TrackedVehicle.c	\
#	LaneAttributes-Parking.c	\
#	LinkList.c	\
#	Link.c	\
#	MovementList.c	\
#	Movement.c	\
#	NodeList.c	\
#	Node.c	\
#	DescriptiveName.c	\
#	NodeReferenceID.c	\
#	RoadRegulatorID.c	\
#	NodeID.c	\
#	PointList.c	\
#	RoadPoint.c	\
#	SpeedLimitList.c	\
#	RegulatorySpeedLimit.c	\
#	SpeedLimitType.c	\
#	MessageFrame.c	\
#	MsgCount.c	\
#	RoadSideInformation.c	\
#	RTEList.c	\
#	RTEData.c	\
#	EventSource.c	\
#	EventType.c	\
#	RSITimeDetails.c	\
#	ReferencePathList.c	\
#	ReferencePath.c	\
#	RTSList.c	\
#	RTSData.c	\
#	Description.c	\
#	ReferenceLinkList.c	\
#	ReferenceLink.c	\
#	ReferenceLanes.c	\
#	SignType.c	\
#	PathPointList.c	\
#	Radius.c	\
#	RSIPriority.c	\
#	RoadsideSafetyMessage.c	\
#	ParticipantList.c	\
#	ParticipantData.c	\
#	ParticipantType.c	\
#	SourceType.c	\
#	SPAT.c	\
#	IntersectionStateList.c	\
#	IntersectionState.c	\
#	IntersectionStatusObject.c	\
#	PhaseList.c	\
#	Phase.c	\
#	PhaseStateList.c	\
#	PhaseState.c	\
#	LightState.c	\
#	TimeChangeDetails.c	\
#	UTCTiming.c	\
#	TimeCountingDown.c	\
#	PhaseID.c	\
#	BrakeSystemStatus.c	\
#	BrakePedalStatus.c	\
#	BrakeAppliedStatus.c	\
#	BrakeBoostApplied.c	\
#	TractionControlStatus.c	\
#	AntiLockBrakeStatus.c	\
#	StabilityControlStatus.c	\
#	AuxiliaryBrakeStatus.c	\
#	VehicleClassification.c	\
#	BasicVehicleClass.c	\
#	FuelType.c	\
#	VehicleEmergencyExtensions.c	\
#	ResponseType.c	\
#	SirenInUse.c	\
#	LightbarInUse.c	\
#	VehicleSafetyExtensions.c	\
#	PathHistory.c	\
#	FullPositionVector.c	\
#	GNSSstatus.c	\
#	PathHistoryPointList.c	\
#	PathHistoryPoint.c	\
#	PathPrediction.c	\
#	Confidence.c	\
#	RadiusOfCurvature.c	\
#	VehicleSize.c	\
#	VehicleWidth.c	\
#	VehicleLength.c	\
#	VehicleHeight.c	\
#	TransmissionState.c	\
#	VehicleEventFlags.c	\
#	ExteriorLights.c
#
#ASN_MODULE_HDRS=	\
#	BasicSafetyMessage.h	\
#	AccelerationSet4Way.h	\
#	Acceleration.h	\
#	VerticalAcceleration.h	\
#	YawRate.h	\
#	Speed.h	\
#	Heading.h	\
#	CoarseHeading.h	\
#	SteeringWheelAngle.h	\
#	MotionConfidenceSet.h	\
#	HeadingConfidence.h	\
#	SpeedConfidence.h	\
#	SteeringWheelAngleConfidence.h	\
#	Latitude.h	\
#	Longitude.h	\
#	Elevation.h	\
#	PositionConfidenceSet.h	\
#	PositionConfidence.h	\
#	ElevationConfidence.h	\
#	Position3D.h	\
#	SemiMajorAxisAccuracy.h	\
#	SemiMinorAxisAccuracy.h	\
#	SemiMajorAxisOrientation.h	\
#	PositionalAccuracy.h	\
#	PositionOffsetLLV.h	\
#	OffsetLL-B12.h	\
#	OffsetLL-B14.h	\
#	OffsetLL-B16.h	\
#	OffsetLL-B18.h	\
#	OffsetLL-B22.h	\
#	OffsetLL-B24.h	\
#	Position-LL-24B.h	\
#	Position-LL-28B.h	\
#	Position-LL-32B.h	\
#	Position-LL-36B.h	\
#	Position-LL-44B.h	\
#	Position-LL-48B.h	\
#	Position-LLmD-64b.h	\
#	PositionOffsetLL.h	\
#	VerticalOffset.h	\
#	VertOffset-B07.h	\
#	VertOffset-B08.h	\
#	VertOffset-B09.h	\
#	VertOffset-B10.h	\
#	VertOffset-B11.h	\
#	VertOffset-B12.h	\
#	DSecond.h	\
#	DYear.h	\
#	DMonth.h	\
#	DDay.h	\
#	DHour.h	\
#	DMinute.h	\
#	DTimeOffset.h	\
#	DDateTime.h	\
#	MinuteOfTheYear.h	\
#	TimeMark.h	\
#	TimeOffset.h	\
#	TimeConfidence.h	\
#	MapData.h	\
#	LaneList.h	\
#	Lane.h	\
#	LaneID.h	\
#	LaneWidth.h	\
#	ConnectsToList.h	\
#	Connection.h	\
#	ConnectingLane.h	\
#	AllowedManeuvers.h	\
#	LaneAttributes.h	\
#	LaneSharing.h	\
#	LaneTypeAttributes.h	\
#	LaneAttributes-Vehicle.h	\
#	LaneAttributes-Crosswalk.h	\
#	LaneAttributes-Bike.h	\
#	LaneAttributes-Sidewalk.h	\
#	LaneAttributes-Barrier.h	\
#	LaneAttributes-Striping.h	\
#	LaneAttributes-TrackedVehicle.h	\
#	LaneAttributes-Parking.h	\
#	LinkList.h	\
#	Link.h	\
#	MovementList.h	\
#	Movement.h	\
#	NodeList.h	\
#	Node.h	\
#	DescriptiveName.h	\
#	NodeReferenceID.h	\
#	RoadRegulatorID.h	\
#	NodeID.h	\
#	PointList.h	\
#	RoadPoint.h	\
#	SpeedLimitList.h	\
#	RegulatorySpeedLimit.h	\
#	SpeedLimitType.h	\
#	MessageFrame.h	\
#	MsgCount.h	\
#	RoadSideInformation.h	\
#	RTEList.h	\
#	RTEData.h	\
#	EventSource.h	\
#	EventType.h	\
#	RSITimeDetails.h	\
#	ReferencePathList.h	\
#	ReferencePath.h	\
#	RTSList.h	\
#	RTSData.h	\
#	Description.h	\
#	ReferenceLinkList.h	\
#	ReferenceLink.h	\
#	ReferenceLanes.h	\
#	SignType.h	\
#	PathPointList.h	\
#	Radius.h	\
#	RSIPriority.h	\
#	RoadsideSafetyMessage.h	\
#	ParticipantList.h	\
#	ParticipantData.h	\
#	ParticipantType.h	\
#	SourceType.h	\
#	SPAT.h	\
#	IntersectionStateList.h	\
#	IntersectionState.h	\
#	IntersectionStatusObject.h	\
#	PhaseList.h	\
#	Phase.h	\
#	PhaseStateList.h	\
#	PhaseState.h	\
#	LightState.h	\
#	TimeChangeDetails.h	\
#	UTCTiming.h	\
#	TimeCountingDown.h	\
#	PhaseID.h	\
#	BrakeSystemStatus.h	\
#	BrakePedalStatus.h	\
#	BrakeAppliedStatus.h	\
#	BrakeBoostApplied.h	\
#	TractionControlStatus.h	\
#	AntiLockBrakeStatus.h	\
#	StabilityControlStatus.h	\
#	AuxiliaryBrakeStatus.h	\
#	VehicleClassification.h	\
#	BasicVehicleClass.h	\
#	FuelType.h	\
#	VehicleEmergencyExtensions.h	\
#	ResponseType.h	\
#	SirenInUse.h	\
#	LightbarInUse.h	\
#	VehicleSafetyExtensions.h	\
#	PathHistory.h	\
#	FullPositionVector.h	\
#	GNSSstatus.h	\
#	PathHistoryPointList.h	\
#	PathHistoryPoint.h	\
#	PathPrediction.h	\
#	Confidence.h	\
#	RadiusOfCurvature.h	\
#	VehicleSize.h	\
#	VehicleWidth.h	\
#	VehicleLength.h	\
#	VehicleHeight.h	\
#	TransmissionState.h	\
#	VehicleEventFlags.h	\
#	ExteriorLights.h
#
#
#ASN_MODULE_HDES+=ENUMERATED.h
#ASN_MODULE_SRCS+=ENUMERATED.c
#ASN_MODULE_HDES+=INTEGER.h
#ASN_MODULE_HDES+=NativeEnumerated.h
#ASN_MODULE_HDES+=IA5String.h
#ASN_MODULE_SRCS+=IA5String.c
#ASN_MODULE_SRCS+=INTEGER.c
#ASN_MODULE_SRCS+=NativeEnumerated.c
#ASN_MODULE_HDES+=NativeInteger.h
#ASN_MODULE_SRCS+=NativeInteger.c
#ASN_MODULE_HDES+=asn_SEQUENCE_OF.h
#ASN_MODULE_SRCS+=asn_SEQUENCE_OF.c
#ASN_MODULE_HDES+=asn_SET_OF.h
#ASN_MODULE_SRCS+=asn_SET_OF.c
#ASN_MODULE_HDES+=constr_CHOICE.h
#ASN_MODULE_SRCS+=constr_CHOICE.c
#ASN_MODULE_HDES+=constr_SEQUENCE.h
#ASN_MODULE_SRCS+=constr_SEQUENCE.c
#ASN_MODULE_HDES+=constr_SEQUENCE_OF.h
#ASN_MODULE_SRCS+=constr_SEQUENCE_OF.c
#ASN_MODULE_HDES+=constr_SET_OF.h
#ASN_MODULE_SRCS+=constr_SET_OF.c
#ASN_MODULE_HDES+=asn_application.h
#ASN_MODULE_HDES+=asn_system.h
#ASN_MODULE_HDES+=asn_codecs.h
#ASN_MODULE_HDES+=asn_internal.h
#ASN_MODULE_HDES+=OCTET_STRING.h
#ASN_MODULE_SRCS+=OCTET_STRING.c
#ASN_MODULE_HDES+=BIT_STRING.h
#ASN_MODULE_SRCS+=BIT_STRING.c
#ASN_MODULE_SRCS+=asn_codecs_prim.c
#ASN_MODULE_HDES+=asn_codecs_prim.h
#ASN_MODULE_HDES+=ber_tlv_length.h
#ASN_MODULE_SRCS+=ber_tlv_length.c
#ASN_MODULE_HDES+=ber_tlv_tag.h
#ASN_MODULE_SRCS+=ber_tlv_tag.c
#ASN_MODULE_HDES+=ber_decoder.h
#ASN_MODULE_SRCS+=ber_decoder.c
#ASN_MODULE_HDES+=der_encoder.h
#ASN_MODULE_SRCS+=der_encoder.c
#ASN_MODULE_HDES+=constr_TYPE.h
#ASN_MODULE_SRCS+=constr_TYPE.c
#ASN_MODULE_HDES+=constraints.h
#ASN_MODULE_SRCS+=constraints.c
#ASN_MODULE_HDES+=xer_support.h
#ASN_MODULE_SRCS+=xer_support.c
#ASN_MODULE_HDES+=xer_decoder.h
#ASN_MODULE_SRCS+=xer_decoder.c
#ASN_MODULE_HDES+=xer_encoder.h
#ASN_MODULE_SRCS+=xer_encoder.c
#ASN_MODULE_HDES+=per_support.h
#ASN_MODULE_SRCS+=per_support.c
#ASN_MODULE_HDES+=per_decoder.h
#ASN_MODULE_SRCS+=per_decoder.c
#ASN_MODULE_HDES+=per_encoder.h
#ASN_MODULE_SRCS+=per_encoder.c
#ASN_MODULE_HDES+=per_opentype.h
#ASN_MODULE_SRCS+=per_opentype.c
#

ASN_MODULE_SRCS+=Acceleration.c
ASN_MODULE_SRCS+=AccelerationSet4Way.c
ASN_MODULE_SRCS+=AllowedManeuvers.c
ASN_MODULE_SRCS+=AntiLockBrakeStatus.c
ASN_MODULE_SRCS+=ANY.c
ASN_MODULE_SRCS+=asn_application.c
ASN_MODULE_SRCS+=asn_bit_data.c
ASN_MODULE_SRCS+=asn_codecs_prim.c
ASN_MODULE_SRCS+=asn_internal.c
ASN_MODULE_SRCS+=asn_random_fill.c
ASN_MODULE_SRCS+=asn_SEQUENCE_OF.c
ASN_MODULE_SRCS+=asn_SET_OF.c
ASN_MODULE_SRCS+=AuxiliaryBrakeStatus.c
ASN_MODULE_SRCS+=BasicSafetyMessage.c
ASN_MODULE_SRCS+=BasicVehicleClass.c
ASN_MODULE_SRCS+=ber_decoder.c
ASN_MODULE_SRCS+=ber_tlv_length.c
ASN_MODULE_SRCS+=ber_tlv_tag.c
ASN_MODULE_SRCS+=BIT_STRING.c
ASN_MODULE_SRCS+=BIT_STRING_oer.c
ASN_MODULE_SRCS+=BMPString.c
ASN_MODULE_SRCS+=BOOLEAN.c
ASN_MODULE_SRCS+=BrakeAppliedStatus.c
ASN_MODULE_SRCS+=BrakeBoostApplied.c
ASN_MODULE_SRCS+=BrakePedalStatus.c
ASN_MODULE_SRCS+=BrakeSystemStatus.c
ASN_MODULE_SRCS+=CoarseHeading.c
ASN_MODULE_SRCS+=Confidence.c
ASN_MODULE_SRCS+=ConnectingLane.c
ASN_MODULE_SRCS+=Connection.c
ASN_MODULE_SRCS+=ConnectsToList.c
ASN_MODULE_SRCS+=constraints.c
ASN_MODULE_SRCS+=constr_CHOICE.c
ASN_MODULE_SRCS+=constr_CHOICE_oer.c
ASN_MODULE_SRCS+=constr_SEQUENCE.c
ASN_MODULE_SRCS+=constr_SEQUENCE_oer.c
ASN_MODULE_SRCS+=constr_SEQUENCE_OF.c
ASN_MODULE_SRCS+=constr_SET.c
ASN_MODULE_SRCS+=constr_SET_OF.c
ASN_MODULE_SRCS+=constr_SET_OF_oer.c
ASN_MODULE_SRCS+=constr_TYPE.c
#ASN_MODULE_SRCS+=converter-example.c
ASN_MODULE_SRCS+=DDateTime.c
ASN_MODULE_SRCS+=DDay.c
ASN_MODULE_SRCS+=der_encoder.c
ASN_MODULE_SRCS+=Description.c
ASN_MODULE_SRCS+=DescriptiveName.c
ASN_MODULE_SRCS+=DHour.c
ASN_MODULE_SRCS+=DMinute.c
ASN_MODULE_SRCS+=DMonth.c
ASN_MODULE_SRCS+=DSecond.c
ASN_MODULE_SRCS+=DTimeOffset.c
ASN_MODULE_SRCS+=DYear.c
ASN_MODULE_SRCS+=Elevation.c
ASN_MODULE_SRCS+=ElevationConfidence.c
ASN_MODULE_SRCS+=ENUMERATED.c
ASN_MODULE_SRCS+=EventSource.c
ASN_MODULE_SRCS+=EventType.c
ASN_MODULE_SRCS+=ExteriorLights.c
ASN_MODULE_SRCS+=FuelType.c
ASN_MODULE_SRCS+=FullPositionVector.c
ASN_MODULE_SRCS+=GeneralizedTime.c
ASN_MODULE_SRCS+=GeneralString.c
ASN_MODULE_SRCS+=GNSSstatus.c
ASN_MODULE_SRCS+=GraphicString.c
ASN_MODULE_SRCS+=Heading.c
ASN_MODULE_SRCS+=HeadingConfidence.c
ASN_MODULE_SRCS+=IA5String.c
ASN_MODULE_SRCS+=INTEGER.c
ASN_MODULE_SRCS+=INTEGER_oer.c
ASN_MODULE_SRCS+=IntersectionState.c
ASN_MODULE_SRCS+=IntersectionStateList.c
ASN_MODULE_SRCS+=IntersectionStatusObject.c
ASN_MODULE_SRCS+=ISO646String.c
ASN_MODULE_SRCS+=Lane.c
ASN_MODULE_SRCS+=LaneAttributes-Barrier.c
ASN_MODULE_SRCS+=LaneAttributes-Bike.c
ASN_MODULE_SRCS+=LaneAttributes-Crosswalk.c
ASN_MODULE_SRCS+=LaneAttributes-Parking.c
ASN_MODULE_SRCS+=LaneAttributes-Sidewalk.c
ASN_MODULE_SRCS+=LaneAttributes-Striping.c
ASN_MODULE_SRCS+=LaneAttributes-TrackedVehicle.c
ASN_MODULE_SRCS+=LaneAttributes-Vehicle.c
ASN_MODULE_SRCS+=LaneAttributes.c
ASN_MODULE_SRCS+=LaneID.c
ASN_MODULE_SRCS+=LaneList.c
ASN_MODULE_SRCS+=LaneSharing.c
ASN_MODULE_SRCS+=LaneTypeAttributes.c
ASN_MODULE_SRCS+=LaneWidth.c
ASN_MODULE_SRCS+=Latitude.c
ASN_MODULE_SRCS+=LightbarInUse.c
ASN_MODULE_SRCS+=LightState.c
ASN_MODULE_SRCS+=Link.c
ASN_MODULE_SRCS+=LinkList.c
ASN_MODULE_SRCS+=Longitude.c
ASN_MODULE_SRCS+=MapData.c
ASN_MODULE_SRCS+=MessageFrame.c
ASN_MODULE_SRCS+=MinuteOfTheYear.c
ASN_MODULE_SRCS+=MotionConfidenceSet.c
ASN_MODULE_SRCS+=Movement.c
ASN_MODULE_SRCS+=MovementList.c
ASN_MODULE_SRCS+=MsgCount.c
ASN_MODULE_SRCS+=NativeEnumerated.c
ASN_MODULE_SRCS+=NativeEnumerated_oer.c
ASN_MODULE_SRCS+=NativeInteger.c
ASN_MODULE_SRCS+=NativeInteger_oer.c
ASN_MODULE_SRCS+=NativeReal.c
ASN_MODULE_SRCS+=Node.c
ASN_MODULE_SRCS+=NodeID.c
ASN_MODULE_SRCS+=NodeList.c
ASN_MODULE_SRCS+=NodeReferenceID.c
ASN_MODULE_SRCS+=NULL.c
ASN_MODULE_SRCS+=NumericString.c
ASN_MODULE_SRCS+=ObjectDescriptor.c
ASN_MODULE_SRCS+=OBJECT_IDENTIFIER.c
ASN_MODULE_SRCS+=OCTET_STRING.c
ASN_MODULE_SRCS+=OCTET_STRING_oer.c
ASN_MODULE_SRCS+=oer_decoder.c
ASN_MODULE_SRCS+=oer_encoder.c
ASN_MODULE_SRCS+=oer_support.c
ASN_MODULE_SRCS+=OffsetLL-B12.c
ASN_MODULE_SRCS+=OffsetLL-B14.c
ASN_MODULE_SRCS+=OffsetLL-B16.c
ASN_MODULE_SRCS+=OffsetLL-B18.c
ASN_MODULE_SRCS+=OffsetLL-B22.c
ASN_MODULE_SRCS+=OffsetLL-B24.c
ASN_MODULE_SRCS+=OPEN_TYPE.c
ASN_MODULE_SRCS+=OPEN_TYPE_oer.c
ASN_MODULE_SRCS+=ParticipantData.c
ASN_MODULE_SRCS+=ParticipantList.c
ASN_MODULE_SRCS+=ParticipantType.c
ASN_MODULE_SRCS+=PathHistory.c
ASN_MODULE_SRCS+=PathHistoryPoint.c
ASN_MODULE_SRCS+=PathHistoryPointList.c
ASN_MODULE_SRCS+=PathPointList.c
ASN_MODULE_SRCS+=PathPrediction.c
ASN_MODULE_SRCS+=pdu_collection.c
ASN_MODULE_SRCS+=per_decoder.c
ASN_MODULE_SRCS+=per_encoder.c
ASN_MODULE_SRCS+=per_opentype.c
ASN_MODULE_SRCS+=per_support.c
ASN_MODULE_SRCS+=Phase.c
ASN_MODULE_SRCS+=PhaseID.c
ASN_MODULE_SRCS+=PhaseList.c
ASN_MODULE_SRCS+=PhaseState.c
ASN_MODULE_SRCS+=PhaseStateList.c
ASN_MODULE_SRCS+=PointList.c
ASN_MODULE_SRCS+=Position-LL-24B.c
ASN_MODULE_SRCS+=Position-LL-28B.c
ASN_MODULE_SRCS+=Position-LL-32B.c
ASN_MODULE_SRCS+=Position-LL-36B.c
ASN_MODULE_SRCS+=Position-LL-44B.c
ASN_MODULE_SRCS+=Position-LL-48B.c
ASN_MODULE_SRCS+=Position-LLmD-64b.c
ASN_MODULE_SRCS+=Position3D.c
ASN_MODULE_SRCS+=PositionalAccuracy.c
ASN_MODULE_SRCS+=PositionConfidence.c
ASN_MODULE_SRCS+=PositionConfidenceSet.c
ASN_MODULE_SRCS+=PositionOffsetLL.c
ASN_MODULE_SRCS+=PositionOffsetLLV.c
ASN_MODULE_SRCS+=PrintableString.c
ASN_MODULE_SRCS+=Radius.c
ASN_MODULE_SRCS+=RadiusOfCurvature.c
ASN_MODULE_SRCS+=REAL.c
ASN_MODULE_SRCS+=ReferenceLanes.c
ASN_MODULE_SRCS+=ReferenceLink.c
ASN_MODULE_SRCS+=ReferenceLinkList.c
ASN_MODULE_SRCS+=ReferencePath.c
ASN_MODULE_SRCS+=ReferencePathList.c
ASN_MODULE_SRCS+=RegulatorySpeedLimit.c
ASN_MODULE_SRCS+=RELATIVE-OID.c
ASN_MODULE_SRCS+=ResponseType.c
ASN_MODULE_SRCS+=RoadPoint.c
ASN_MODULE_SRCS+=RoadRegulatorID.c
ASN_MODULE_SRCS+=RoadSideInformation.c
ASN_MODULE_SRCS+=RoadsideSafetyMessage.c
ASN_MODULE_SRCS+=RSIPriority.c
ASN_MODULE_SRCS+=RSITimeDetails.c
ASN_MODULE_SRCS+=RTEData.c
ASN_MODULE_SRCS+=RTEList.c
ASN_MODULE_SRCS+=RTSData.c
ASN_MODULE_SRCS+=RTSList.c
ASN_MODULE_SRCS+=SemiMajorAxisAccuracy.c
ASN_MODULE_SRCS+=SemiMajorAxisOrientation.c
ASN_MODULE_SRCS+=SemiMinorAxisAccuracy.c
ASN_MODULE_SRCS+=SignType.c
ASN_MODULE_SRCS+=SirenInUse.c
ASN_MODULE_SRCS+=SourceType.c
ASN_MODULE_SRCS+=SPAT.c
ASN_MODULE_SRCS+=Speed.c
ASN_MODULE_SRCS+=SpeedConfidence.c
ASN_MODULE_SRCS+=SpeedLimitList.c
ASN_MODULE_SRCS+=SpeedLimitType.c
ASN_MODULE_SRCS+=StabilityControlStatus.c
ASN_MODULE_SRCS+=SteeringWheelAngle.c
ASN_MODULE_SRCS+=SteeringWheelAngleConfidence.c
ASN_MODULE_SRCS+=T61String.c
ASN_MODULE_SRCS+=TeletexString.c
ASN_MODULE_SRCS+=TimeChangeDetails.c
ASN_MODULE_SRCS+=TimeConfidence.c
ASN_MODULE_SRCS+=TimeCountingDown.c
ASN_MODULE_SRCS+=TimeMark.c
ASN_MODULE_SRCS+=TimeOffset.c
ASN_MODULE_SRCS+=TractionControlStatus.c
ASN_MODULE_SRCS+=TransmissionState.c
ASN_MODULE_SRCS+=UniversalString.c
ASN_MODULE_SRCS+=UTCTime.c
ASN_MODULE_SRCS+=UTCTiming.c
ASN_MODULE_SRCS+=UTF8String.c
ASN_MODULE_SRCS+=VehicleClassification.c
ASN_MODULE_SRCS+=VehicleEmergencyExtensions.c
ASN_MODULE_SRCS+=VehicleEventFlags.c
ASN_MODULE_SRCS+=VehicleHeight.c
ASN_MODULE_SRCS+=VehicleLength.c
ASN_MODULE_SRCS+=VehicleSafetyExtensions.c
ASN_MODULE_SRCS+=VehicleSize.c
ASN_MODULE_SRCS+=VehicleWidth.c
ASN_MODULE_SRCS+=VerticalAcceleration.c
ASN_MODULE_SRCS+=VerticalOffset.c
ASN_MODULE_SRCS+=VertOffset-B07.c
ASN_MODULE_SRCS+=VertOffset-B08.c
ASN_MODULE_SRCS+=VertOffset-B09.c
ASN_MODULE_SRCS+=VertOffset-B10.c
ASN_MODULE_SRCS+=VertOffset-B11.c
ASN_MODULE_SRCS+=VertOffset-B12.c
ASN_MODULE_SRCS+=VideotexString.c
ASN_MODULE_SRCS+=VisibleString.c
ASN_MODULE_SRCS+=xer_decoder.c
ASN_MODULE_SRCS+=xer_encoder.c
ASN_MODULE_SRCS+=xer_support.c
ASN_MODULE_SRCS+=YawRate.c

ASN_MODULE_HDES+=Acceleration.h
ASN_MODULE_HDES+=AccelerationSet4Way.h
ASN_MODULE_HDES+=AllowedManeuvers.h
ASN_MODULE_HDES+=AntiLockBrakeStatus.h
ASN_MODULE_HDES+=ANY.h
ASN_MODULE_HDES+=asn_application.h
ASN_MODULE_HDES+=asn_bit_data.h
ASN_MODULE_HDES+=asn_codecs.h
ASN_MODULE_HDES+=asn_codecs_prim.h
ASN_MODULE_HDES+=asn_internal.h
ASN_MODULE_HDES+=asn_ioc.h
ASN_MODULE_HDES+=asn_random_fill.h
ASN_MODULE_HDES+=asn_SEQUENCE_OF.h
ASN_MODULE_HDES+=asn_SET_OF.h
ASN_MODULE_HDES+=asn_system.h
ASN_MODULE_HDES+=AuxiliaryBrakeStatus.h
ASN_MODULE_HDES+=BasicSafetyMessage.h
ASN_MODULE_HDES+=BasicVehicleClass.h
ASN_MODULE_HDES+=ber_decoder.h
ASN_MODULE_HDES+=ber_tlv_length.h
ASN_MODULE_HDES+=ber_tlv_tag.h
ASN_MODULE_HDES+=BIT_STRING.h
ASN_MODULE_HDES+=BMPString.h
ASN_MODULE_HDES+=BOOLEAN.h
ASN_MODULE_HDES+=BrakeAppliedStatus.h
ASN_MODULE_HDES+=BrakeBoostApplied.h
ASN_MODULE_HDES+=BrakePedalStatus.h
ASN_MODULE_HDES+=BrakeSystemStatus.h
ASN_MODULE_HDES+=CoarseHeading.h
ASN_MODULE_HDES+=Confidence.h
ASN_MODULE_HDES+=ConnectingLane.h
ASN_MODULE_HDES+=Connection.h
ASN_MODULE_HDES+=ConnectsToList.h
ASN_MODULE_HDES+=constraints.h
ASN_MODULE_HDES+=constr_CHOICE.h
ASN_MODULE_HDES+=constr_SEQUENCE.h
ASN_MODULE_HDES+=constr_SEQUENCE_OF.h
ASN_MODULE_HDES+=constr_SET.h
ASN_MODULE_HDES+=constr_SET_OF.h
ASN_MODULE_HDES+=constr_TYPE.h
ASN_MODULE_HDES+=DDateTime.h
ASN_MODULE_HDES+=DDay.h
ASN_MODULE_HDES+=der_encoder.h
ASN_MODULE_HDES+=Description.h
ASN_MODULE_HDES+=DescriptiveName.h
ASN_MODULE_HDES+=DHour.h
ASN_MODULE_HDES+=DMinute.h
ASN_MODULE_HDES+=DMonth.h
ASN_MODULE_HDES+=DSecond.h
ASN_MODULE_HDES+=DTimeOffset.h
ASN_MODULE_HDES+=DYear.h
ASN_MODULE_HDES+=Elevation.h
ASN_MODULE_HDES+=ElevationConfidence.h
ASN_MODULE_HDES+=ENUMERATED.h
ASN_MODULE_HDES+=EventSource.h
ASN_MODULE_HDES+=EventType.h
ASN_MODULE_HDES+=ExteriorLights.h
ASN_MODULE_HDES+=FuelType.h
ASN_MODULE_HDES+=FullPositionVector.h
ASN_MODULE_HDES+=GeneralizedTime.h
ASN_MODULE_HDES+=GeneralString.h
ASN_MODULE_HDES+=GNSSstatus.h
ASN_MODULE_HDES+=GraphicString.h
ASN_MODULE_HDES+=Heading.h
ASN_MODULE_HDES+=HeadingConfidence.h
ASN_MODULE_HDES+=IA5String.h
ASN_MODULE_HDES+=INTEGER.h
ASN_MODULE_HDES+=IntersectionState.h
ASN_MODULE_HDES+=IntersectionStateList.h
ASN_MODULE_HDES+=IntersectionStatusObject.h
ASN_MODULE_HDES+=ISO646String.h
ASN_MODULE_HDES+=Lane.h
ASN_MODULE_HDES+=LaneAttributes-Barrier.h
ASN_MODULE_HDES+=LaneAttributes-Bike.h
ASN_MODULE_HDES+=LaneAttributes-Crosswalk.h
ASN_MODULE_HDES+=LaneAttributes-Parking.h
ASN_MODULE_HDES+=LaneAttributes-Sidewalk.h
ASN_MODULE_HDES+=LaneAttributes-Striping.h
ASN_MODULE_HDES+=LaneAttributes-TrackedVehicle.h
ASN_MODULE_HDES+=LaneAttributes-Vehicle.h
ASN_MODULE_HDES+=LaneAttributes.h
ASN_MODULE_HDES+=LaneID.h
ASN_MODULE_HDES+=LaneList.h
ASN_MODULE_HDES+=LaneSharing.h
ASN_MODULE_HDES+=LaneTypeAttributes.h
ASN_MODULE_HDES+=LaneWidth.h
ASN_MODULE_HDES+=Latitude.h
ASN_MODULE_HDES+=LightbarInUse.h
ASN_MODULE_HDES+=LightState.h
ASN_MODULE_HDES+=Link.h
ASN_MODULE_HDES+=LinkList.h
ASN_MODULE_HDES+=Longitude.h
ASN_MODULE_HDES+=MapData.h
ASN_MODULE_HDES+=MessageFrame.h
ASN_MODULE_HDES+=MinuteOfTheYear.h
ASN_MODULE_HDES+=MotionConfidenceSet.h
ASN_MODULE_HDES+=Movement.h
ASN_MODULE_HDES+=MovementList.h
ASN_MODULE_HDES+=MsgCount.h
ASN_MODULE_HDES+=NativeEnumerated.h
ASN_MODULE_HDES+=NativeInteger.h
ASN_MODULE_HDES+=NativeReal.h
ASN_MODULE_HDES+=Node.h
ASN_MODULE_HDES+=NodeID.h
ASN_MODULE_HDES+=NodeList.h
ASN_MODULE_HDES+=NodeReferenceID.h
ASN_MODULE_HDES+=NULL.h
ASN_MODULE_HDES+=NumericString.h
ASN_MODULE_HDES+=ObjectDescriptor.h
ASN_MODULE_HDES+=OBJECT_IDENTIFIER.h
ASN_MODULE_HDES+=OCTET_STRING.h
ASN_MODULE_HDES+=oer_decoder.h
ASN_MODULE_HDES+=oer_encoder.h
ASN_MODULE_HDES+=oer_support.h
ASN_MODULE_HDES+=OffsetLL-B12.h
ASN_MODULE_HDES+=OffsetLL-B14.h
ASN_MODULE_HDES+=OffsetLL-B16.h
ASN_MODULE_HDES+=OffsetLL-B18.h
ASN_MODULE_HDES+=OffsetLL-B22.h
ASN_MODULE_HDES+=OffsetLL-B24.h
ASN_MODULE_HDES+=OPEN_TYPE.h
ASN_MODULE_HDES+=ParticipantData.h
ASN_MODULE_HDES+=ParticipantList.h
ASN_MODULE_HDES+=ParticipantType.h
ASN_MODULE_HDES+=PathHistory.h
ASN_MODULE_HDES+=PathHistoryPoint.h
ASN_MODULE_HDES+=PathHistoryPointList.h
ASN_MODULE_HDES+=PathPointList.h
ASN_MODULE_HDES+=PathPrediction.h
ASN_MODULE_HDES+=per_decoder.h
ASN_MODULE_HDES+=per_encoder.h
ASN_MODULE_HDES+=per_opentype.h
ASN_MODULE_HDES+=per_support.h
ASN_MODULE_HDES+=Phase.h
ASN_MODULE_HDES+=PhaseID.h
ASN_MODULE_HDES+=PhaseList.h
ASN_MODULE_HDES+=PhaseState.h
ASN_MODULE_HDES+=PhaseStateList.h
ASN_MODULE_HDES+=PointList.h
ASN_MODULE_HDES+=Position-LL-24B.h
ASN_MODULE_HDES+=Position-LL-28B.h
ASN_MODULE_HDES+=Position-LL-32B.h
ASN_MODULE_HDES+=Position-LL-36B.h
ASN_MODULE_HDES+=Position-LL-44B.h
ASN_MODULE_HDES+=Position-LL-48B.h
ASN_MODULE_HDES+=Position-LLmD-64b.h
ASN_MODULE_HDES+=Position3D.h
ASN_MODULE_HDES+=PositionalAccuracy.h
ASN_MODULE_HDES+=PositionConfidence.h
ASN_MODULE_HDES+=PositionConfidenceSet.h
ASN_MODULE_HDES+=PositionOffsetLL.h
ASN_MODULE_HDES+=PositionOffsetLLV.h
ASN_MODULE_HDES+=PrintableString.h
ASN_MODULE_HDES+=Radius.h
ASN_MODULE_HDES+=RadiusOfCurvature.h
ASN_MODULE_HDES+=REAL.h
ASN_MODULE_HDES+=ReferenceLanes.h
ASN_MODULE_HDES+=ReferenceLink.h
ASN_MODULE_HDES+=ReferenceLinkList.h
ASN_MODULE_HDES+=ReferencePath.h
ASN_MODULE_HDES+=ReferencePathList.h
ASN_MODULE_HDES+=RegulatorySpeedLimit.h
ASN_MODULE_HDES+=RELATIVE-OID.h
ASN_MODULE_HDES+=ResponseType.h
ASN_MODULE_HDES+=RoadPoint.h
ASN_MODULE_HDES+=RoadRegulatorID.h
ASN_MODULE_HDES+=RoadSideInformation.h
ASN_MODULE_HDES+=RoadsideSafetyMessage.h
ASN_MODULE_HDES+=RSIPriority.h
ASN_MODULE_HDES+=RSITimeDetails.h
ASN_MODULE_HDES+=RTEData.h
ASN_MODULE_HDES+=RTEList.h
ASN_MODULE_HDES+=RTSData.h
ASN_MODULE_HDES+=RTSList.h
ASN_MODULE_HDES+=SemiMajorAxisAccuracy.h
ASN_MODULE_HDES+=SemiMajorAxisOrientation.h
ASN_MODULE_HDES+=SemiMinorAxisAccuracy.h
ASN_MODULE_HDES+=SignType.h
ASN_MODULE_HDES+=SirenInUse.h
ASN_MODULE_HDES+=SourceType.h
ASN_MODULE_HDES+=SPAT.h
ASN_MODULE_HDES+=Speed.h
ASN_MODULE_HDES+=SpeedConfidence.h
ASN_MODULE_HDES+=SpeedLimitList.h
ASN_MODULE_HDES+=SpeedLimitType.h
ASN_MODULE_HDES+=StabilityControlStatus.h
ASN_MODULE_HDES+=SteeringWheelAngle.h
ASN_MODULE_HDES+=SteeringWheelAngleConfidence.h
ASN_MODULE_HDES+=T61String.h
ASN_MODULE_HDES+=TeletexString.h
ASN_MODULE_HDES+=TimeChangeDetails.h
ASN_MODULE_HDES+=TimeConfidence.h
ASN_MODULE_HDES+=TimeCountingDown.h
ASN_MODULE_HDES+=TimeMark.h
ASN_MODULE_HDES+=TimeOffset.h
ASN_MODULE_HDES+=TractionControlStatus.h
ASN_MODULE_HDES+=TransmissionState.h
ASN_MODULE_HDES+=UniversalString.h
ASN_MODULE_HDES+=UTCTime.h
ASN_MODULE_HDES+=UTCTiming.h
ASN_MODULE_HDES+=UTF8String.h
ASN_MODULE_HDES+=VehicleClassification.h
ASN_MODULE_HDES+=VehicleEmergencyExtensions.h
ASN_MODULE_HDES+=VehicleEventFlags.h
ASN_MODULE_HDES+=VehicleHeight.h
ASN_MODULE_HDES+=VehicleLength.h
ASN_MODULE_HDES+=VehicleSafetyExtensions.h
ASN_MODULE_HDES+=VehicleSize.h
ASN_MODULE_HDES+=VehicleWidth.h
ASN_MODULE_HDES+=VerticalAcceleration.h
ASN_MODULE_HDES+=VerticalOffset.h
ASN_MODULE_HDES+=VertOffset-B07.h
ASN_MODULE_HDES+=VertOffset-B08.h
ASN_MODULE_HDES+=VertOffset-B09.h
ASN_MODULE_HDES+=VertOffset-B10.h
ASN_MODULE_HDES+=VertOffset-B11.h
ASN_MODULE_HDES+=VertOffset-B12.h
ASN_MODULE_HDES+=VideotexString.h
ASN_MODULE_HDES+=VisibleString.h
ASN_MODULE_HDES+=xer_decoder.h
ASN_MODULE_HDES+=xer_encoder.h
ASN_MODULE_HDES+=xer_support.h
ASN_MODULE_HDES+=YawRate.h


# This file may be used as an input for make(3)
# Remove the lines below to convert it into a pure .am file
TARGET = libmsgframe.so

EXTRA_CFLAGS += -I $(ASN_DIR)

OBJS := ${ASN_MODULE_SRCS:.c=.o}

################################################################################
# Build Rules
################################################################################
CC := $(CROSS_COMPILE)gcc

all: $(TARGET)

$(TARGET): ${OBJS}
	$(CC) -shared $(CFLAGS) -o $(TARGET) ${OBJS} ${LDFLAGS}

.SUFFIXES:
.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ $(EXTRA_CFLAGS)
clean:
	rm -f $(TARGET)
	#rm -f $(OBJS)
	find ./ -name "*.o" |xargs rm -rf
	find ./ -name "*.d" |xargs rm -rf
