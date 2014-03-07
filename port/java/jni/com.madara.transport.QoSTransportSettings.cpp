/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#include "com.madara.transport.QoSTransportSettings.h"
#include "madara/transport/QoS_Transport_Settings.h"

#include <iostream>

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_QoSTransportSettings
 * Signature: ()J
 */
MADARA_Export jlong JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1QoSTransportSettings__
  (JNIEnv * env, jclass cls)
{
    return (jlong) new Madara::Transport::QoS_Transport_Settings();
}

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_QoSTransportSettings
 * Signature: (J)J
 */
MADARA_Export jlong JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1QoSTransportSettings__J
  (JNIEnv * env, jclass cls, jlong old)
{
    return (jlong) new Madara::Transport::QoS_Transport_Settings(*(Madara::Transport::QoS_Transport_Settings*)old);
}

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_freeQoSTransportSettings
 * Signature: (J)V
 */
MADARA_Export void JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1freeQoSTransportSettings
  (JNIEnv * env, jclass cls, jlong cptr)
{
    Madara::Transport::QoS_Transport_Settings* settings = (Madara::Transport::QoS_Transport_Settings*)cptr;
    if (settings)
        delete settings;
}

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_addRebroadcastFilter
 * Signature: (JILcom/madara/transport/TransportFilter;)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1addRebroadcastFilter__JILcom_madara_transport_TransportFilter_2
  (JNIEnv * env, jclass cls, jlong cptr, jint type, jobject func)
{
    Madara::Transport::QoS_Transport_Settings* settings = (Madara::Transport::QoS_Transport_Settings*)cptr;
    settings->add_rebroadcast_filter(type, func);
}

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_addRebroadcastFilter
 * Signature: (JLcom/madara/transport/AggregateFilter;)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1addRebroadcastFilter__JLcom_madara_transport_AggregateFilter_2
  (JNIEnv * env, jclass cls, jlong cptr, jobject func)
{
    Madara::Transport::QoS_Transport_Settings* settings = (Madara::Transport::QoS_Transport_Settings*)cptr;
    settings->add_rebroadcast_filter(func);
}

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_addSendFilter
 * Signature: (JILcom/madara/transport/TransportFilter;)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1addSendFilter__JILcom_madara_transport_TransportFilter_2
  (JNIEnv * env, jclass cls, jlong cptr, jint type, jobject func)
{
    Madara::Transport::QoS_Transport_Settings* settings = (Madara::Transport::QoS_Transport_Settings*)cptr;
    settings->add_send_filter(type, func);
}

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_addSendFilter
 * Signature: (JLcom/madara/transport/AggregateFilter;)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1addSendFilter__JLcom_madara_transport_AggregateFilter_2
  (JNIEnv * env, jclass cls, jlong cptr, jobject func)
{
    Madara::Transport::QoS_Transport_Settings* settings = (Madara::Transport::QoS_Transport_Settings*)cptr;
    settings->add_send_filter(func);
}

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_addReceiveFilter
 * Signature: (JILcom/madara/transport/TransportFilter;)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1addReceiveFilter__JILcom_madara_transport_TransportFilter_2
  (JNIEnv * env, jclass cls, jlong cptr, jint type, jobject func)
{
    Madara::Transport::QoS_Transport_Settings* settings = (Madara::Transport::QoS_Transport_Settings*)cptr;
    settings->add_receive_filter(type, func);
}

/*
 * Class:     com_madara_transport_QoSTransportSettings
 * Method:    jni_addReceiveFilter
 * Signature: (JLcom/madara/transport/AggregateFilter;)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_QoSTransportSettings_jni_1addReceiveFilter__JLcom_madara_transport_AggregateFilter_2
  (JNIEnv * env, jclass cls, jlong cptr, jobject func)
{
    Madara::Transport::QoS_Transport_Settings* settings = (Madara::Transport::QoS_Transport_Settings*)cptr;
    settings->add_receive_filter(func);
}