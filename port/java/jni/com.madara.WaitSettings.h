/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#ifndef _Included_com_madara_WaitSettings
#define _Included_com_madara_WaitSettings

#include <jni.h>
#include "madara/MADARA_export.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_madara_WaitSettings
 * Method:    jni_getDefaultWaitSettings
 * Signature: (I)J
 */
MADARA_Export jlong JNICALL Java_com_madara_WaitSettings_jni_1getDefaultWaitSettings
  (JNIEnv *, jclass, jint);

/*
 * Class:     com_madara_WaitSettings
 * Method:    jni_waitSettings
 * Signature: ()J
 */
MADARA_Export jlong JNICALL Java_com_madara_WaitSettings_jni_1waitSettings__
  (JNIEnv *, jobject);

/*
 * Class:     com_madara_WaitSettings
 * Method:    jni_waitSettings
 * Signature: (J)J
 */
MADARA_Export jlong JNICALL Java_com_madara_WaitSettings_jni_1waitSettings__J
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_WaitSettings
 * Method:    jni_setPollFrequency
 * Signature: (JD)V
 */
MADARA_Export void JNICALL Java_com_madara_WaitSettings_jni_1setPollFrequency
  (JNIEnv *, jobject, jlong, jdouble);

/*
 * Class:     com_madara_WaitSettings
 * Method:    jni_getPollFrequency
 * Signature: (J)D
 */
MADARA_Export jdouble JNICALL Java_com_madara_WaitSettings_jni_1getPollFrequency
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_WaitSettings
 * Method:    jni_setMaxWaitTime
 * Signature: (JD)V
 */
MADARA_Export void JNICALL Java_com_madara_WaitSettings_jni_1setMaxWaitTime
  (JNIEnv *, jobject, jlong, jdouble);

/*
 * Class:     com_madara_WaitSettings
 * Method:    jni_getMaxWaitTime
 * Signature: (J)D
 */
MADARA_Export jdouble JNICALL Java_com_madara_WaitSettings_jni_1getMaxWaitTime
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif
