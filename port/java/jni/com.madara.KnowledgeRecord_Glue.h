/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#ifndef _Included_com_madara_KnowledgeRecord_Glue
#define _Included_com_madara_KnowledgeRecord_Glue

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_madara_KnowledgeRecord
 * Method:    jni_KnowledgeRecord
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeRecord_jni_1KnowledgeRecord__Ljava_lang_String_2
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_madara_KnowledgeRecord
 * Method:    jni_KnowledgeRecord
 * Signature: (D)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeRecord_jni_1KnowledgeRecord__D
  (JNIEnv *, jobject, jdouble);

/*
 * Class:     com_madara_KnowledgeRecord
 * Method:    jni_KnowledgeRecord
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeRecord_jni_1KnowledgeRecord__J
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_KnowledgeRecord
 * Method:    jni_toLongValue
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeRecord_jni_1toLongValue
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_KnowledgeRecord
 * Method:    jni_toStringValue
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_madara_KnowledgeRecord_jni_1toStringValue
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_KnowledgeRecord
 * Method:    jni_toDoubleValue
 * Signature: (J)D
 */
JNIEXPORT jdouble JNICALL Java_com_madara_KnowledgeRecord_jni_1toDoubleValue
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_KnowledgeRecord
 * Method:    jni_getType
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_madara_KnowledgeRecord_jni_1getType
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_KnowledgeRecord
 * Method:    jni_freeKnowledgeRecord
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeRecord_jni_1freeKnowledgeRecord
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif
