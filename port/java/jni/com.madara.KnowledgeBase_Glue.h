/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#ifndef _Included_com_madara_KnowledgeBase_Impl
#define _Included_com_madara_KnowledgeBase_Impl

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif


//===================================================================================
//KnowledgeBase
//===================================================================================
/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_KnowledgeBase
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1KnowledgeBase__
  (JNIEnv *, jobject);

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_KnowledgeBase
 * Signature: (Ljava/lang/String;ILjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1KnowledgeBase__Ljava_lang_String_2ILjava_lang_String_2
  (JNIEnv *, jobject, jstring, jint, jstring);

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_KnowledgeBase
 * Signature: (Ljava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1KnowledgeBase__Ljava_lang_String_2J
  (JNIEnv *, jobject, jstring, jlong);

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_KnowledgeBase
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1KnowledgeBase__J
  (JNIEnv *, jobject, jlong);
  
/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_evaluate
 * Signature: (JLjava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1evaluate__JLjava_lang_String_2J
  (JNIEnv *, jobject, jlong, jstring, jlong);

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_evaluate
 * Signature: (JJJ)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1evaluate__JJJ
  (JNIEnv *, jobject, jlong, jlong, jlong);

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_evaluateNoReturn
 * Signature: (JLjava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1evaluateNoReturn__JLjava_lang_String_2J
  (JNIEnv *, jobject, jlong, jstring, jlong);

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_evaluateNoReturn
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1evaluateNoReturn__JJJ
  (JNIEnv *, jobject, jlong, jlong, jlong);

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_compile
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1compile
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_defineFunction
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1defineFunction__JLjava_lang_String_2
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_defineFunction
 * Signature: (JLjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1defineFunction__JLjava_lang_String_2Ljava_lang_String_2
  (JNIEnv *, jobject, jlong, jstring, jstring);

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_defineFunction
 * Signature: (JLjava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1defineFunction__JLjava_lang_String_2J
  (JNIEnv *, jobject, jlong, jstring, jlong);
  
/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_clear
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1clear
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_get
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1get
  (JNIEnv *, jobject, jlong, jstring);
  
/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_set
 * Signature: (JLjava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1set
  (JNIEnv *, jobject, jlong, jstring, jlong);
  
/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_wait
 * Signature: (JLjava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1wait__JLjava_lang_String_2J
  (JNIEnv *, jobject, jlong, jstring, jlong);

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_wait
 * Signature: (JJJ)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1wait__JJJ
  (JNIEnv *, jobject, jlong, jlong, jlong);

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_waitNoReturn
 * Signature: (JLjava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1waitNoReturn__JLjava_lang_String_2J
  (JNIEnv *, jobject, jlong, jstring, jlong);

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_waitNoReturn
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1waitNoReturn__JJJ
  (JNIEnv *, jobject, jlong, jlong, jlong);  

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_freeKnowledgeBase
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1freeKnowledgeBase
  (JNIEnv *, jobject, jlong);
//===================================================================================
  
//===================================================================================
//CompiledExpression
//===================================================================================
/*
 * Class:     com_madara_KnowledgeBase_CompiledExpression
 * Method:    jni_freeCompiledExpression
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_00024CompiledExpression_jni_1freeCompiledExpression
  (JNIEnv *, jobject, jlong);  
//===================================================================================

#ifdef __cplusplus
}
#endif
#endif
