/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
 
#include "com.madara.WaitSettings_Glue.h"
#include "com.madara.WaitSettings_Impl.h"

/*
 * Class:     com_madara_WaitSettings
 * Method:    jni_getDefaultWaitSettings
 * Signature: (I)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_WaitSettings_jni_1getDefaultWaitSettings (JNIEnv *env, jclass clazz, jint which)
{
	void* ret;
	jni_WaitSettings_get_default_wait_settings_RI(&ret, which);
	return ret;
}

/*
 * Class:     com_madara_WaitSettings
 * Method:    jni_waitSettings
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_madara_WaitSettings_jni_1waitSettings__(JNIEnv * env, jobject obj)
{
	void* ret;
	jni_WaitSettings_RV(&ret);
	return ret;
}

/*
 * Class:     com_madara_WaitSettings
 * Method:    jni_waitSettings
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_WaitSettings_jni_1waitSettings__J (JNIEnv * env, jobject obj, jlong original)
{
	void* ret;
	jni_WaitSettings_RP(&ret, original);
	return ret;
}

/*
 * Class:     com_madara_WaitSettings
 * Method:    jni_setPollFrequency
 * Signature: (JD)V
 */
JNIEXPORT void JNICALL Java_com_madara_WaitSettings_jni_1setPollFrequency (JNIEnv *env, jobject obj, jlong cptr, jdouble pollFreq)
{
	jni_WaitSettings_set_poll_frequency_PD(cptr, pollFreq);
}

/*
 * Class:     com_madara_WaitSettings
 * Method:    jni_getPollFrequency
 * Signature: (J)D
 */
JNIEXPORT jdouble JNICALL Java_com_madara_WaitSettings_jni_1getPollFrequency (JNIEnv *env, jobject obj, jlong cptr)
{
	return jni_WaitSettings_get_poll_frequency_P(cptr);
}

/*
 * Class:     com_madara_WaitSettings
 * Method:    jni_setMaxWaitTime
 * Signature: (JD)V
 */
JNIEXPORT void JNICALL Java_com_madara_WaitSettings_jni_1setMaxWaitTime (JNIEnv *env, jobject obj, jlong cptr, jdouble maxWaitTime)
{
	jni_WaitSettings_set_max_wait_time_PD(cptr, maxWaitTime);
}

/*
 * Class:     com_madara_WaitSettings
 * Method:    jni_getMaxWaitTime
 * Signature: (J)D
 */
JNIEXPORT jdouble JNICALL Java_com_madara_WaitSettings_jni_1getMaxWaitTime (JNIEnv *env, jobject obj, jlong cptr)
{
	return jni_WaitSettings_get_max_wait_time(cptr);
}


