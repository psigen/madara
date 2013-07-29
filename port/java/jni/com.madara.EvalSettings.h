/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#ifndef _Included_com_madara_EvalSettings_Glue
#define _Included_com_madara_EvalSettings_Glue

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

//===================================================================================
//EvalSettings
//===================================================================================

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getDefaultEvalSettings
 * Signature: (I)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_EvalSettings_jni_1getDefaultEvalSettings
  (JNIEnv *, jclass, jint);

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_evalSettings
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_madara_EvalSettings_jni_1evalSettings__
  (JNIEnv *, jobject);

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_evalSettings
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_EvalSettings_jni_1evalSettings__J
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_setDelaySendingModifieds
 * Signature: (JZ)V
 */
JNIEXPORT void JNICALL Java_com_madara_EvalSettings_jni_1setDelaySendingModifieds
  (JNIEnv *, jobject, jlong, jboolean);

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getDelaySendingModifieds
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_madara_EvalSettings_jni_1getDelaySendingModifieds
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_setPrePrintStatement
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_madara_EvalSettings_jni_1setPrePrintStatement
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getPrePrintStatement
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_madara_EvalSettings_jni_1getPrePrintStatement
  (JNIEnv *, jobject, jlong);
  
/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_setPostPrintStatement
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_madara_EvalSettings_jni_1setPostPrintStatement
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getPostPrintStatement
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_madara_EvalSettings_jni_1getPostPrintStatement
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_setAlwaysOverwrite
 * Signature: (JZ)V
 */
JNIEXPORT void JNICALL Java_com_madara_EvalSettings_jni_1setAlwaysOverwrite
  (JNIEnv *, jobject, jlong, jboolean);

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getAlwaysOverwrite
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_madara_EvalSettings_jni_1getAlwaysOverwrite
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_setTreatGlobalsAsLocals
 * Signature: (JZ)V
 */
JNIEXPORT void JNICALL Java_com_madara_EvalSettings_jni_1setTreatGlobalsAsLocals
  (JNIEnv *, jobject, jlong, jboolean);

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getTreatGlobalsAsLocals
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_madara_EvalSettings_jni_1getTreatGlobalsAsLocals
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_setDefaultClockIncrement
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_madara_EvalSettings_jni_1setClockIncrement
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getDefaultClockIncrement
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_EvalSettings_jni_1getClockIncrement
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif
