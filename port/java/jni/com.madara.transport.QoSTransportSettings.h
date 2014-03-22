/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#ifndef _Included_com_madara_transport_QoSTransportSettings
#define _Included_com_madara_transport_QoSTransportSettings

#include <jni.h>
#include "madara/MADARA_export.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_QoSTransportSettings
 * Signature: ()J
 */
MADARA_Export jlong JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1QoSTransportSettings__
  (JNIEnv *, jclass);

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_QoSTransportSettings
 * Signature: (J)J
 */
MADARA_Export jlong JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1QoSTransportSettings__J
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_freeQoSTransportSettings
 * Signature: (J)V
 */
MADARA_Export void JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1freeQoSTransportSettings
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_addRebroadcastFilter
 * Signature: (JILcom/madara/transport/TransportFilter;)V
 */
MADARA_Export void JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1addRebroadcastFilter__JILcom_madara_transport_TransportFilter_2
  (JNIEnv *, jclass, jlong, jint, jobject);

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_addRebroadcastFilter
 * Signature: (JLcom/madara/transport/AggregateFilter;)V
 */
MADARA_Export void JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1addRebroadcastFilter__JLcom_madara_transport_AggregateFilter_2
  (JNIEnv *, jclass, jlong, jobject);

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_addSendFilter
 * Signature: (JILcom/madara/transport/TransportFilter;)V
 */
MADARA_Export void JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1addSendFilter__JILcom_madara_transport_TransportFilter_2
  (JNIEnv *, jclass, jlong, jint, jobject);

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_addSendFilter
 * Signature: (JLcom/madara/transport/AggregateFilter;)V
 */
MADARA_Export void JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1addSendFilter__JLcom_madara_transport_AggregateFilter_2
  (JNIEnv *, jclass, jlong, jobject);

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_addReceiveFilter
 * Signature: (JILcom/madara/transport/TransportFilter;)V
 */
MADARA_Export void JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1addReceiveFilter__JILcom_madara_transport_TransportFilter_2
  (JNIEnv *, jclass, jlong, jint, jobject);

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_addReceiveFilter
 * Signature: (JLcom/madara/transport/AggregateFilter;)V
 */
MADARA_Export void JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1addReceiveFilter__JLcom_madara_transport_AggregateFilter_2
  (JNIEnv *, jclass, jlong, jobject);

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_setRebroadcastTtl
 * Signature: (JI)V
 */
MADARA_Export void JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1setRebroadcastTtl
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_getRebroadcastTtl
 * Signature: (J)I
 */
MADARA_Export jint JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1getRebroadcastTtl
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_enableParticipantTtl
 * Signature: (JI)V
 */
MADARA_Export void JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1enableParticipantTtl
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_getParticipantTtl
 * Signature: (J)I
 */
MADARA_Export jint JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1getParticipantTtl
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_setSendBandwidthLimit
 * Signature: (JI)V
 */
MADARA_Export void JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1setSendBandwidthLimit
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_getSendBandwidthLimit
 * Signature: (J)I
 */
MADARA_Export jint JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1getSendBandwidthLimit
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_setTotalBandwidthLimit
 * Signature: (JI)V
 */
MADARA_Export void JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1setTotalBandwidthLimit
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_getSendTotalBandwidthLimit
 * Signature: (J)I
 */
MADARA_Export jint JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1getTotalBandwidthLimit
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_setDeadline
 * Signature: (JI)V
 */
MADARA_Export void JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1setDeadline
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_getDeadline
 * Signature: (J)I
 */
MADARA_Export jint JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1getDeadline
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif
