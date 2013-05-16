/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#include "com.madara.KnowledgeBase_Glue.h"
#include "com.madara.KnowledgeBase_Impl.h"


static jclass knowledgeBaseClass = 0;
static jmethodID callbackMethod = 0;

//===================================================================================
//KnowledgeBase
//===================================================================================
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1KnowledgeBase__ (JNIEnv * env, jobject obj)
{
	void* ret;
	jni_KnowledgeBase_impl_RV(&ret);
	return ret;
}

JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1KnowledgeBase__Ljava_lang_String_2ILjava_lang_String_2 (JNIEnv * env, jobject obj, jstring host, jint transport, jstring domain)
{

	const char *nativeHost = (*env)->GetStringUTFChars(env, host, 0);
	const char *nativeDomain = (*env)->GetStringUTFChars(env, domain, 0);

	void* ret;
	jni_KnowledgeBase_impl_RSIS(&ret, nativeHost, transport, nativeDomain);
	
	(*env)->ReleaseStringUTFChars(env, host, nativeHost);
	(*env)->ReleaseStringUTFChars(env, domain, nativeDomain);

	return ret;
}

JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1KnowledgeBase__Ljava_lang_String_2J (JNIEnv *env, jobject obj, jstring host, jlong settings)
{
	const char *nativeHost = (*env)->GetStringUTFChars(env, host, 0);
	
	void* ret;
	jni_KnowledgeBase_impl_RSP(&ret, nativeHost, settings);
	
	(*env)->ReleaseStringUTFChars(env, host, nativeHost);

	return ret;
}

JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1KnowledgeBase__J (JNIEnv * env, jobject obj, jlong original)
{
	void* ret;
	jni_KnowledgeBase_impl_RP(&ret, original);
	return ret;
}


/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_evaluate
 * Signature: (JLjava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1evaluate__JLjava_lang_String_2J (JNIEnv *env, jobject obj, jlong cptr, jstring expression, jlong evalSettings)
{
	const char *nativeExpression = (*env)->GetStringUTFChars(env, expression, 0);
	
	void* ret;
	jni_KnowledgeBase_evaluate_impl_RPSP(&ret, cptr, nativeExpression, evalSettings);
	
	(*env)->ReleaseStringUTFChars(env, expression, nativeExpression);
	
	return ret;
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_evaluate
 * Signature: (JJJ)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1evaluate__JJJ (JNIEnv *env, jobject obj, jlong cptr, jlong expression, jlong evalSettings)
{
	void* ret;
	jni_KnowledgeBase_evaluate_impl_RPPP(&ret, cptr, expression, evalSettings);
	return ret;
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_evaluateNoReturn
 * Signature: (JLjava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1evaluateNoReturn__JLjava_lang_String_2J (JNIEnv * env, jobject obj, jlong cptr, jstring expression, jlong evalSettings)
{
	const char *nativeExpression = (*env)->GetStringUTFChars(env, expression, 0);
	jni_KnowledgeBase_evaluate_no_return_impl_PSP(cptr, nativeExpression, evalSettings);
	(*env)->ReleaseStringUTFChars(env, expression, nativeExpression);
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_evaluateNoReturn
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1evaluateNoReturn__JJJ (JNIEnv *env, jobject obj, jlong cptr, jlong expression, jlong evalSettings)
{
	jni_KnowledgeBase_evaluate_no_return_impl_PPP(cptr, expression, evalSettings);
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_compile
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1compile (JNIEnv *env, jobject obj, jlong cptr, jstring expression)
{
	const char *nativeExpression = (*env)->GetStringUTFChars(env, expression, 0);

	void* ret;
	jni_KnowledgeBase_compile_impl_RPS(&ret, cptr, nativeExpression);
	
	(*env)->ReleaseStringUTFChars(env, expression, nativeExpression);
	
	return ret;
}


typedef struct
{
	JNIEnv *env;
	jlong cptr;
} callback_struct;

void c_callback(void** ret, const char* name, void** args, unsigned int argsLen, void* vars, void* callBackStruct)
{
	callback_struct* cbs = (callback_struct*)callBackStruct;
	JNIEnv *env = cbs->env;
	jlong cptr = cbs->cptr;
	
	//Build the array for the args
	jlongArray argsArray = (*env)->NewLongArray(env, argsLen);
	(*env)->SetLongArrayRegion(env, argsArray, 0, argsLen, args);

	*ret = (*env)->CallStaticLongMethod(env, knowledgeBaseClass, callbackMethod, (*env)->NewStringUTF(env, name), cptr, argsArray, vars);
}


/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_defineFunction
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1defineFunction__JLjava_lang_String_2 (JNIEnv *env, jobject obj, jlong cptr, jstring name)
{
	if (!knowledgeBaseClass)
	{
		knowledgeBaseClass = (jclass)(*env)->NewGlobalRef(env, (*env)->GetObjectClass(env, obj));
		callbackMethod = (*env)->GetStaticMethodID(env, knowledgeBaseClass, "callBack", "(Ljava/lang/String;J[JJ)J" );
	}
	
	callback_struct* data = (callback_struct*)malloc(sizeof(callback_struct));
	data->env = env;
	data->cptr = cptr;
	
	const char *nativeName = (*env)->GetStringUTFChars(env, name, 0);
	jni_KnowledgeBase_define_Function_impl_PSPM(cptr, nativeName, data, c_callback);
	(*env)->ReleaseStringUTFChars(env, name, nativeName);	
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_defineFunction
 * Signature: (JLjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1defineFunction__JLjava_lang_String_2Ljava_lang_String_2 (JNIEnv *env, jobject obj, jlong cptr, jstring name, jstring expression)
{
	const char *nativeExpression = (*env)->GetStringUTFChars(env, expression, 0);
	const char *nativeName = (*env)->GetStringUTFChars(env, name, 0);
	
	jni_KnowledgeBase_define_function_impl_PSS(cptr, nativeName, nativeExpression);
	
	(*env)->ReleaseStringUTFChars(env, name, nativeName);	
	(*env)->ReleaseStringUTFChars(env, expression, nativeExpression);
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_defineFunction
 * Signature: (JLjava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1defineFunction__JLjava_lang_String_2J (JNIEnv *env, jobject obj, jlong cptr, jstring name, jlong expression)
{
	const char *nativeName = (*env)->GetStringUTFChars(env, name, 0);
	
	jni_KnowledgeBase_define_function_impl_PSP(cptr, nativeName, expression);
	
	(*env)->ReleaseStringUTFChars(env, name, nativeName);	
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_clear
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1clear (JNIEnv *env, jobject obj, jlong cptr)
{
	jni_KnowledgeBase_clear_impl_P(cptr);
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_get
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1get (JNIEnv *env, jobject obj, jlong cptr, jstring var)
{
	const char *nativeVar = (*env)->GetStringUTFChars(env, var, 0);
	void* ret;
	jni_KnowledgeBase_get_impl_RPS(&ret, cptr, nativeVar);
	(*env)->ReleaseStringUTFChars(env, var, nativeVar);
	return ret;
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_set
 * Signature: (JLjava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1set (JNIEnv *env, jobject obj, jlong cptr, jstring var, jlong record)
{
	const char *nativeVar = (*env)->GetStringUTFChars(env, var, 0);
	jni_KnowledgeBase_set_impl_PSP(cptr, nativeVar, record);
	(*env)->ReleaseStringUTFChars(env, var, nativeVar);
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_wait
 * Signature: (JLjava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1wait__JLjava_lang_String_2J (JNIEnv * env, jobject obj, jlong cptr, jstring expression, jlong waitSettings)
{
	const char *nativeExpression = (*env)->GetStringUTFChars(env, expression, 0);
	
	void* ret;
	jni_KnowledgeBase_wait_impl_RPSP(&ret, cptr, nativeExpression, waitSettings);
	
	(*env)->ReleaseStringUTFChars(env, expression, nativeExpression);
	
	return ret;
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_wait
 * Signature: (JJJ)J
 */
JNIEXPORT jlong JNICALL Java_com_madara_KnowledgeBase_jni_1wait__JJJ (JNIEnv * env, jobject obj, jlong cptr, jlong expression, jlong waitSettings)
{
	void* ret;
	jni_KnowledgeBase_wait_impl_RPPP(&ret, cptr, expression, waitSettings);
	return ret;
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_waitNoReturn
 * Signature: (JLjava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1waitNoReturn__JLjava_lang_String_2J (JNIEnv * env, jobject obj, jlong cptr, jstring expression, jlong waitSettings)
{
	const char *nativeExpression = (*env)->GetStringUTFChars(env, expression, 0);
	jni_KnowledgeBase_wait_no_return_PSP(cptr, nativeExpression, waitSettings);
	(*env)->ReleaseStringUTFChars(env, expression, nativeExpression);
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_waitNoReturn
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1waitNoReturn__JJJ (JNIEnv * env, jobject obj, jlong cptr, jlong expression, jlong waitSettings)
{
	jni_KnowledgeBase_wait_no_return_PPP(cptr, expression, waitSettings);
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_freeKnowledgeBase
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_jni_1freeKnowledgeBase (JNIEnv * env, jobject obj, jlong cptr)
{
	jni_KnowledgeBase_free_knowledge_base_P(cptr);
}
//===================================================================================

//===================================================================================
//CompiledExpression
//===================================================================================
/*
 * Class:     com_madara_KnowledgeBase_CompiledExpression
 * Method:    jni_freeCompiledExpression
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_madara_KnowledgeBase_00024CompiledExpression_jni_1freeCompiledExpression (JNIEnv * env, jobject obj, jlong cptr)
{
	jni_KnowledgeBase_free_compiled_expression_P(cptr);
}  
//===================================================================================


