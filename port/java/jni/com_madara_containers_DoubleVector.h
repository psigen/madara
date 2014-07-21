/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_madara_containers_DoubleVector */

#ifndef _Included_com_madara_containers_DoubleVector
#define _Included_com_madara_containers_DoubleVector
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_madara_containers_DoubleVector
 * Method:    jni_DoubleVector
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_madara_containers_DoubleVector_jni_1DoubleVector__
  (JNIEnv *, jobject);

/*
 * Class:     com_madara_containers_DoubleVector
 * Method:    jni_DoubleVector
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_containers_DoubleVector_jni_1DoubleVector__J
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_containers_DoubleVector
 * Method:    jni_freeDoubleVector
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_madara_containers_DoubleVector_jni_1freeDoubleVector
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_madara_containers_DoubleVector
 * Method:    jni_set
 * Signature: (JID)V
 */
JNIEXPORT void JNICALL Java_com_madara_containers_DoubleVector_jni_1set
  (JNIEnv *, jobject, jlong, jint, jdouble);

/*
 * Class:     com_madara_containers_DoubleVector
 * Method:    jni_getName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_madara_containers_DoubleVector_jni_1getName
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_containers_DoubleVector
 * Method:    jni_setName
 * Signature: (JJJLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_madara_containers_DoubleVector_jni_1setName
  (JNIEnv *, jobject, jlong, jlong, jlong, jstring);

/*
 * Class:     com_madara_containers_DoubleVector
 * Method:    jni_get
 * Signature: (JI)D
 */
JNIEXPORT jdouble JNICALL Java_com_madara_containers_DoubleVector_jni_1get
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     com_madara_containers_DoubleVector
 * Method:    jni_toRecord
 * Signature: (JI)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_containers_DoubleVector_jni_1toRecord__JI
  (JNIEnv *, jobject, jlong, jint);

/*
 * Class:     com_madara_containers_DoubleVector
 * Method:    jni_toRecord
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_containers_DoubleVector_jni_1toRecord__J
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_containers_DoubleVector
 * Method:    jni_toArray
 * Signature: (J)[Ljava/lang/Object;
 */
JNIEXPORT jobjectArray JNICALL Java_com_madara_containers_DoubleVector_jni_1toArray
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_containers_DoubleVector
 * Method:    jni_size
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_containers_DoubleVector_jni_1size
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_containers_DoubleVector
 * Method:    jni_resize
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_madara_containers_DoubleVector_jni_1resize
  (JNIEnv *, jobject, jlong, jlong);

#ifdef __cplusplus
}
#endif
#endif
