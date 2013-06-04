/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
 
#include "com.madara.KnowledgeRecord_Glue.h"
#include "com.madara.KnowledgeRecord_Impl.h"

/*
 * Class:     com_madara_KnowledgeRecord
 * Method:    jni_KnowledgeRecord
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeRecord_jni_1KnowledgeRecord__Ljava_lang_String_2 (JNIEnv * env, jobject obj, jstring data)
{
	const char *nativeData = (*env)->GetStringUTFChars(env, data, 0);
	
	void* ret;
	jni_KnowledgeRecord_impl_RS(&ret, nativeData);
	
	(*env)->ReleaseStringUTFChars(env, data, nativeData);
	
	return ret;
}

/*
 * Class:     com_madara_KnowledgeRecord
 * Method:    jni_KnowledgeRecord
 * Signature: (D)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeRecord_jni_1KnowledgeRecord__D (JNIEnv * env, jobject obj, jdouble data)
{
	void* ret;
	jni_KnowledgeRecord_impl_RD(&ret, data);
	return ret;
}

/*
 * Class:     com_madara_KnowledgeRecord
 * Method:    jni_KnowledgeRecord
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeRecord_jni_1KnowledgeRecord__J (JNIEnv * env, jobject obj, jlong data)
{
	void* ret;
	jni_KnowledgeRecord_impl_RJ(&ret, data);
	return ret;
}

/*
 * Class:     com_madara_KnowledgeRecord
 * Method:    jni_toLongValue
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeRecord_jni_1toLongValue (JNIEnv * env, jobject obj, jlong cptr)
{
	return jni_KnowledgeRecord_to_long_value_impl_P(cptr);
}

/*
 * Class:     com_madara_KnowledgeRecord
 * Method:    jni_toStringValue
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_madara_KnowledgeRecord_jni_1toStringValue (JNIEnv * env, jobject obj, jlong cptr)
{
	return (*env)->NewStringUTF(env, jni_KnowledgeRecord_to_string_value_impl_P(cptr));
}

/*
 * Class:     com_madara_KnowledgeRecord
 * Method:    jni_toDoubleValue
 * Signature: (J)D
 */
JNIEXPORT jdouble JNICALL Java_com_madara_KnowledgeRecord_jni_1toDoubleValue (JNIEnv * env, jobject obj, jlong cptr)
{
	return jni_KnowledgeRecord_to_double_value_impl_P(cptr);
}

/*
 * Class:     com_madara_KnowledgeRecord
 * Method:    jni_getType
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_madara_KnowledgeRecord_jni_1getType (JNIEnv *env, jobject obj, jlong cptr)
{
	return jni_KnowledgeRecord_get_type_P(cptr);
}

/*
 * Class:     com_madara_KnowledgeRecord
 * Method:    jni_freeKnowledgeRecord
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeRecord_jni_1freeKnowledgeRecord (JNIEnv * env, jobject obj, jlong cptr)
{
	jni_KnowledgeRecord_free_P(cptr);
}

/*
 * Class:     com_madara_KnowledgeList
 * Method:    jni_freeKnowledgeList
 * Signature: ([JI)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeList_jni_1freeKnowledgeList (JNIEnv *env, jobject obj, jlongArray records, jint length)
{
	jboolean jniNoCopy = JNI_FALSE;
	jlong* nativeRecords = (*env)->GetLongArrayElements(env, records, &jniNoCopy);
	int x;
	for (x = 0; x < length; x++)
	{
		jni_KnowledgeRecord_free_P(nativeRecords[x]);
	}
}

/*
 * Class:     com_madara_KnowledgeMap
 * Method:    jni_freeKnowledgeMap
 * Signature: ([JI)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeMap_jni_1freeKnowledgeMap (JNIEnv * env, jobject obj, jlongArray records, jint length)
{
	//KnowledgeMap#free is exactly the same as KnowledgeList#free, no reason to re-implement
	Java_com_madara_KnowledgeList_jni_1freeKnowledgeList(env, obj, records, length);
}


