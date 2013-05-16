/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
 
#include "com.madara.MadaraVariables_Glue.h"
#include "com.madara.MadaraVariables_Impl.h"

/*
 * Class:     com_madara_MadaraVariables
 * Method:    jni_evaluate
 * Signature: (JJJ)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_MadaraVariables_jni_1evaluate__JJJ (JNIEnv * env, jobject obj, jlong cptr, jlong expression, jlong evalSettings)
{
	void* ret;
	jni_MadaraVariables_evaluate_impl_RPPP(&ret, cptr, expression, evalSettings);
	return ret;
}

/*
 * Class:     com_madara_MadaraVariables
 * Method:    jni_compile
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_MadaraVariables_jni_1compile (JNIEnv *env, jobject obj, jlong cptr, jstring expression)
{
	const char *nativeExpression = (*env)->GetStringUTFChars(env, expression, 0);

	void* ret;
	jni_MadaraVariables_compile_impl_RPS(&ret, cptr, nativeExpression);
	
	(*env)->ReleaseStringUTFChars(env, expression, nativeExpression);
	
	return ret;
}

/*
 * Class:     com_madara_MadaraVariables
 * Method:    jni_get
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_MadaraVariables_jni_1get (JNIEnv * env, jobject obj, jlong cptr, jstring var)
{
	const char *nativeVar = (*env)->GetStringUTFChars(env, var, 0);
	void* ret;
	jni_MadaraVariables_get_impl_RPS(&ret, cptr, nativeVar);
	(*env)->ReleaseStringUTFChars(env, var, nativeVar);
	return ret;
}

/*
 * Class:     com_madara_MadaraVariables
 * Method:    jni_set
 * Signature: (JLjava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_com_madara_MadaraVariables_jni_1set (JNIEnv *env, jobject obj, jlong cptr, jstring var, jlong record)
{
	const char *nativeVar = (*env)->GetStringUTFChars(env, var, 0);
	jni_MadaraVariables_set_impl_PSP(cptr, nativeVar, record);
	(*env)->ReleaseStringUTFChars(env, var, nativeVar);
}



