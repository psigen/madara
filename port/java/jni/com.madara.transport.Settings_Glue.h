/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#ifndef _Included_com_madara_transport_Settings_Glue
#define _Included_com_madara_transport_Settings_Glue

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

//===================================================================================
//Settings
//===================================================================================

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_Settings
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_madara_transport_Settings_jni_1Settings__
  (JNIEnv *, jobject);

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_Settings
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_transport_Settings_jni_1Settings__J
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_setDomains
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_Settings_jni_1setDomains
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_getDomains
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_madara_transport_Settings_jni_1getDomains
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_setQueueLength
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_Settings_jni_1setQueueLength
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_getQueueLength
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_madara_transport_Settings_jni_1getQueueLength
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_setDeadline
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_Settings_jni_1setDeadline
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_getDeadline
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_madara_transport_Settings_jni_1getDeadline
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_setType
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_Settings_jni_1setType
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_getType
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_madara_transport_Settings_jni_1getType
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_setReliability
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_Settings_jni_1setReliability
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_getReliability
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_madara_transport_Settings_jni_1getReliability
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_setId
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_Settings_jni_1setId
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_getId
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_madara_transport_Settings_jni_1getId
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_setProcesses
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_Settings_jni_1setProcesses
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_getProcesses
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_madara_transport_Settings_jni_1getProcesses
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_setOnDataReceivedLogic
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_Settings_jni_1setOnDataReceivedLogic
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_getOnDataReceivedLogic
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_madara_transport_Settings_jni_1getOnDataReceivedLogic
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_setHosts
 * Signature: (J[Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_Settings_jni_1setHosts
  (JNIEnv *, jobject, jlong, jobjectArray);

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_getHosts
 * Signature: (J)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_com_madara_transport_Settings_jni_1getHosts
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif
