/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#include "com.madara.EvalSettings_Glue.h"
#include "com.madara.EvalSettings_Impl.h"

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getDefaultEvalSettings
 * Signature: (I)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_EvalSettings_jni_1getDefaultEvalSettings(JNIEnv *env, jclass clazz, jint which)
{
	void* ret;
	jni_getDefaultEvalSettings_impl_RI(&ret, which);
	return ret;
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_evalSettings
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_madara_EvalSettings_jni_1evalSettings__(JNIEnv *env, jobject obj)
{
	void* ret;
	jni_EvalSettings_impl_RV(&ret);
	return ret;
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_evalSettings
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_EvalSettings_jni_1evalSettings__J(JNIEnv *env, jobject obj, jlong cptr)
{
	void* ret;
	jni_EvalSettings_impl_RP(&ret, cptr);
	return ret;
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_setDelaySendingModifieds
 * Signature: (JZ)V
 */
JNIEXPORT void JNICALL Java_com_madara_EvalSettings_jni_1setDelaySendingModifieds (JNIEnv * env, jobject obj, jlong cptr, jboolean delaySendingModifieds)
{
	jni_EvalSettings_set_delay_sending_modifieds_impl_PB(cptr, delaySendingModifieds);
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getDelaySendingModifieds
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_madara_EvalSettings_jni_1getDelaySendingModifieds (JNIEnv * env, jobject obj, jlong cptr)
{
	return (jboolean) jni_EvalSettings_get_delay_sending_modifieds_impl_P(cptr);	
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_setPrePrintStatement
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_madara_EvalSettings_jni_1setPrePrintStatement (JNIEnv * env, jobject obj, jlong cptr, jstring prePrintStatement)
{
	const char *nativePrePrint = (*env)->GetStringUTFChars(env, prePrintStatement, 0);
	
	jni_EvalSettings_set_pre_print_statement_impl_PS(cptr, nativePrePrint);
	
	(*env)->ReleaseStringUTFChars(env, prePrintStatement, nativePrePrint);
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getPrePrintStatement
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_madara_EvalSettings_jni_1getPrePrintStatement (JNIEnv * env, jobject obj, jlong cptr)
{
	return (*env)->NewStringUTF(env, jni_EvalSettings_get_pre_print_statement_impl_P(cptr));
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_setPostPrintStatement
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_madara_EvalSettings_jni_1setPostPrintStatement (JNIEnv *env, jobject obj, jlong cptr, jstring postPrintStatement)
{
	const char *nativePostPrint = (*env)->GetStringUTFChars(env, postPrintStatement, 0);
	
	jni_EvalSettings_set_post_print_statement_impl_PS(cptr, nativePostPrint);
	
	(*env)->ReleaseStringUTFChars(env, postPrintStatement, nativePostPrint);
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getPostPrintStatement
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_madara_EvalSettings_jni_1getPostPrintStatement (JNIEnv * env, jobject obj, jlong cptr)
{
	return (*env)->NewStringUTF(env, jni_EvalSettings_get_post_print_statement_impl_P(cptr));
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_setAlwaysOverwrite
 * Signature: (JZ)V
 */
JNIEXPORT void JNICALL Java_com_madara_EvalSettings_jni_1setAlwaysOverwrite (JNIEnv * env, jobject obj, jlong cptr, jboolean alwaysOverwrite)
{
	jni_EvalSettings_set_always_overwrite_impl_PB(cptr, alwaysOverwrite);
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getAlwaysOverwrite
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_madara_EvalSettings_jni_1getAlwaysOverwrite (JNIEnv * env, jobject obj, jlong cptr)
{
	return (jboolean) jni_EvalSettings_get_always_overwrite_impl_P(cptr);
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_setTreatGlobalsAsLocals
 * Signature: (JZ)V
 */
JNIEXPORT void JNICALL Java_com_madara_EvalSettings_jni_1setTreatGlobalsAsLocals (JNIEnv *env, jobject obj, jlong cptr, jboolean treatGlobalsAsLocals)
{
	jni_EvalSettings_set_treat_globals_as_locals_impl_PB(cptr, treatGlobalsAsLocals);
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getTreatGlobalsAsLocals
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_madara_EvalSettings_jni_1getTreatGlobalsAsLocals (JNIEnv * env, jobject obj, jlong cptr)
{
	return (jboolean) jni_EvalSettings_get_treat_globals_as_locals_impl_P(cptr);
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_setDefaultClockIncrement
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_madara_EvalSettings_jni_1setClockIncrement (JNIEnv *env, jobject obj, jlong cptr, jlong defaultClockIncrement)
{
	jni_EvalSettings_set_clock_increment_impl_PJ(cptr, defaultClockIncrement);
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getDefaultClockIncrement
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_EvalSettings_jni_1getClockIncrement (JNIEnv *env, jobject obj, jlong cptr)
{
	return (jlong) jni_EvalSettings_get_clock_increment(cptr);
}
  
  
  
