/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_madara_containers_IntegerVector */

#ifndef _Included_com_madara_containers_IntegerVector
#define _Included_com_madara_containers_IntegerVector
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_madara_containers_IntegerVector
 * Method:    jni_IntegerVector
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_madara_containers_IntegerVector_jni_1IntegerVector__
  (JNIEnv *, jobject);

/*
 * Class:     com_madara_containers_IntegerVector
 * Method:    jni_IntegerVector
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_containers_IntegerVector_jni_1IntegerVector__J
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_containers_IntegerVector
 * Method:    jni_freeIntegerVector
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_madara_containers_IntegerVector_jni_1freeIntegerVector
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_madara_containers_IntegerVector
 * Method:    jni_set
 * Signature: (JIJ)V
 */
JNIEXPORT void JNICALL Java_com_madara_containers_IntegerVector_jni_1set
  (JNIEnv *, jobject, jlong, jint, jlong);

/*
 * Class:     com_madara_containers_IntegerVector
 * Method:    jni_getName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_madara_containers_IntegerVector_jni_1getName
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_containers_IntegerVector
 * Method:    jni_setName
 * Signature: (JJJLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_madara_containers_IntegerVector_jni_1setName
  (JNIEnv *, jobject, jlong, jlong, jlong, jstring);

/*
 * Class:     com_madara_containers_IntegerVector
 * Method:    jni_get
 * Signature: (JI)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_containers_IntegerVector_jni_1get
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     com_madara_containers_IntegerVector
 * Method:    jni_toRecord
 * Signature: (JI)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_containers_IntegerVector_jni_1toRecord__JI
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     com_madara_containers_IntegerVector
 * Method:    jni_toRecord
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_containers_IntegerVector_jni_1toRecord__J
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif
