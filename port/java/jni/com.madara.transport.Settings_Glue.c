/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#include "com.madara.transport.Settings_Glue.h"
#include "com.madara.transport.Settings_Impl.h"

#include <stdio.h>

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_Settings
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_madara_transport_Settings_jni_1Settings__(JNIEnv *env, jobject obj)
{
	void* ret;
	jni_Settings_impl_RV(&ret);
	return ret;
}

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_Settings
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_transport_Settings_jni_1Settings__J (JNIEnv *env, jobject obj, jlong cptr)
{
	void* ret;
	jni_Settings_impl_RP(&ret, cptr);
	return ret;
}

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_setDomains
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_Settings_jni_1setDomains (JNIEnv *env, jobject obj, jlong cptr, jstring domains)
{
	const char *nativeDomains = (*env)->GetStringUTFChars(env, domains, 0);
	
	jni_Settings_set_domains_impl_PS(cptr, nativeDomains);
	
	(*env)->ReleaseStringUTFChars(env, domains, nativeDomains);
}

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_getDomains
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_madara_transport_Settings_jni_1getDomains (JNIEnv * env, jobject obj, jlong cptr)
{
	return (*env)->NewStringUTF(env, jni_Settings_get_domains_impl_P(cptr));
}

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_setQueueLength
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_Settings_jni_1setQueueLength (JNIEnv * env, jobject obj, jlong cptr, jint queueLength)
{
	jni_Settings_set_queue_length_impl_PI(cptr, queueLength);
}

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_getQueueLength
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_madara_transport_Settings_jni_1getQueueLength (JNIEnv * env, jobject obj, jlong cptr)
{
	return jni_Settings_get_queue_length_impl_P(cptr);
}

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_setDeadline
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_Settings_jni_1setDeadline (JNIEnv * env, jobject obj, jlong cptr, jint deadline)
{
	jni_Settings_set_deadline_impl_PI(cptr, deadline);
}

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_getDeadline
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_madara_transport_Settings_jni_1getDeadline (JNIEnv * env, jobject obj, jlong cptr)
{
	return jni_Settings_get_deadline_impl_P(cptr);
}

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_setType
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_Settings_jni_1setType (JNIEnv * env, jobject obj, jlong cptr, jint type)
{
	jni_Settings_set_type_impl_PI(cptr, type);
}

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_getType
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_madara_transport_Settings_jni_1getType (JNIEnv *env, jobject obj, jlong cptr)
{
	return jni_Settings_get_type_impl_P(cptr);
}

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_setReliability
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_Settings_jni_1setReliability (JNIEnv * env, jobject obj, jlong cptr, jint reliability)
{
	jni_Settings_set_reliability_impl_PI(cptr, reliability);
}

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_getReliability
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_madara_transport_Settings_jni_1getReliability (JNIEnv * env, jobject obj, jlong cptr)
{
	return jni_Settings_get_reliability_impl_P(cptr);
}

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_setId
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_Settings_jni_1setId (JNIEnv *env, jobject obj, jlong cptr, jint id)
{
	jni_Settings_set_id_impl_PI(cptr, id);
}

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_getId
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_madara_transport_Settings_jni_1getId (JNIEnv *env, jobject obj, jlong cptr)
{
	return jni_Settings_get_id_impl_P(cptr);
}

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_setProcesses
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_Settings_jni_1setProcesses (JNIEnv *env, jobject obj, jlong cptr, jint processes)
{
	jni_Settings_set_processes_impl_PI(cptr, processes);
}

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_getProcesses
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_madara_transport_Settings_jni_1getProcesses (JNIEnv *env, jobject obj, jlong cptr)
{
	return jni_Settings_get_processes_impl_P(cptr);
}

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_setOnDataReceivedLogic
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_Settings_jni_1setOnDataReceivedLogic (JNIEnv *env, jobject obj, jlong cptr, jstring onDataReceivedLogic)
{
	const char *nativeOnDataReceivedLogic = (*env)->GetStringUTFChars(env, onDataReceivedLogic, 0);
	
	jni_Settings_set_on_data_received_logic_impl_PS(cptr, nativeOnDataReceivedLogic);
	
	(*env)->ReleaseStringUTFChars(env, onDataReceivedLogic, nativeOnDataReceivedLogic);
}

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_getOnDataReceivedLogic
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_madara_transport_Settings_jni_1getOnDataReceivedLogic (JNIEnv *env, jobject obj, jlong cptr)
{
	return (*env)->NewStringUTF(env, jni_Settings_get_on_data_received_logic_impl_P(cptr));
}

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_setHosts
 * Signature: (J[Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_madara_transport_Settings_jni_1setHosts (JNIEnv *env, jobject obj, jlong cptr, jobjectArray hosts)
{
	int hostsLen = (*env)->GetArrayLength(env, hosts);
	
	const char* cArray[hostsLen];
	
	int x = 0;
	for (x; x < hostsLen; x++)
	{
		jstring jhost = (jstring)(*env)->GetObjectArrayElement(env, hosts, x);
		const char* curHost = (*env)->GetStringUTFChars(env, jhost, 0);
		cArray[x] = strdup(curHost);
		(*env)->ReleaseStringUTFChars(env, jhost, curHost);
	}
	
	jni_Settings_set_hosts_impl_PAI(cptr, cArray, hostsLen);
}

/*
 * Class:     com_madara_transport_Settings
 * Method:    jni_getHosts
 * Signature: (J)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_com_madara_transport_Settings_jni_1getHosts (JNIEnv *env, jobject obj, jlong cptr)
{
	int len;
	jobjectArray ret;
	const char** cArray = jni_Settings_get_hosts_PI(cptr, &len);
	
	ret= (jobjectArray)(*env)->NewObjectArray(env, len, (*env)->FindClass(env, "java/lang/String"), (*env)->NewStringUTF(env, ""));
	
	int x = 0;
	for (x; x < len; x++)
	{
		(*env)->SetObjectArrayElement(env, ret, x, (*env)->NewStringUTF(env, cArray[x])); 
	}
	
	return ret;
}



