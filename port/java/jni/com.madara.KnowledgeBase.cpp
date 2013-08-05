/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#include "com.madara.KnowledgeBase.h"
#include "madara/knowledge_engine/Knowledge_Base.h"

#include <string>
#include <map>


static jclass knowledgeBaseClass = 0;
static jmethodID callbackMethod = 0;

//===================================================================================
//KnowledgeBase
//===================================================================================
MADARA_Export jlong JNICALL Java_com_madara_KnowledgeBase_jni_1KnowledgeBase__ (JNIEnv * env, jobject obj)
{
    return (jlong) (new Madara::Knowledge_Engine::Knowledge_Base());
}

MADARA_Export jlong JNICALL Java_com_madara_KnowledgeBase_jni_1KnowledgeBase__Ljava_lang_String_2ILjava_lang_String_2 (JNIEnv * env, jobject obj, jstring host, jint transport, jstring domain)
{

	const char *nativeHost = env->GetStringUTFChars(host, 0);
	const char *nativeDomain = env->GetStringUTFChars(domain, 0);

    Madara::Knowledge_Engine::Knowledge_Base* knowledge = new Madara::Knowledge_Engine::Knowledge_Base(std::string(nativeHost), transport, std::string(nativeDomain));
	
	env->ReleaseStringUTFChars(host, nativeHost);
	env->ReleaseStringUTFChars(domain, nativeDomain);

	return (jlong) knowledge;
}

MADARA_Export jlong JNICALL Java_com_madara_KnowledgeBase_jni_1KnowledgeBase__Ljava_lang_String_2J (JNIEnv *env, jobject obj, jstring host, jlong settings)
{
	const char *nativeHost = env->GetStringUTFChars(host, 0);
	
    Madara::Knowledge_Engine::Knowledge_Base* knowledge = new Madara::Knowledge_Engine::Knowledge_Base(std::string(nativeHost), *(Madara::Transport::Settings*) settings);
	
	env->ReleaseStringUTFChars(host, nativeHost);

	return (jlong) knowledge;
}

MADARA_Export jlong JNICALL Java_com_madara_KnowledgeBase_jni_1KnowledgeBase__J (JNIEnv * env, jobject obj, jlong original)
{
    return (jlong) new Madara::Knowledge_Engine::Knowledge_Base(*(Madara::Knowledge_Engine::Knowledge_Base*) original);
}


/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_evaluate
 * Signature: (JLjava/lang/String;J)J
 */
MADARA_Export jlong JNICALL Java_com_madara_KnowledgeBase_jni_1evaluate__JLjava_lang_String_2J (JNIEnv *env, jobject obj, jlong cptr, jstring expression, jlong evalSettings)
{
	const char *nativeExpression = env->GetStringUTFChars(expression, 0);
	
    Madara::Knowledge_Engine::Knowledge_Base* knowledge = (Madara::Knowledge_Engine::Knowledge_Base*) cptr;
    Madara::Knowledge_Engine::Eval_Settings settings = *(Madara::Knowledge_Engine::Eval_Settings*) evalSettings;

    Madara::Knowledge_Record* ret = new Madara::Knowledge_Record(knowledge->evaluate(std::string(nativeExpression), settings));
	
	env->ReleaseStringUTFChars(expression, nativeExpression);
	
	return (jlong) ret;
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_evaluate
 * Signature: (JJJ)J
 */
MADARA_Export jlong JNICALL Java_com_madara_KnowledgeBase_jni_1evaluate__JJJ (JNIEnv *env, jobject obj, jlong cptr, jlong expression, jlong evalSettings)
{
    Madara::Knowledge_Engine::Knowledge_Base* knowledge = (Madara::Knowledge_Engine::Knowledge_Base*) cptr;
    Madara::Knowledge_Engine::Compiled_Expression compiled_expression = *(Madara::Knowledge_Engine::Compiled_Expression*)expression;
    Madara::Knowledge_Engine::Eval_Settings settings = *(Madara::Knowledge_Engine::Eval_Settings*) evalSettings;

    return (jlong) new Madara::Knowledge_Record(knowledge->evaluate(compiled_expression, settings));
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_evaluateNoReturn
 * Signature: (JLjava/lang/String;J)V
 */
MADARA_Export void JNICALL Java_com_madara_KnowledgeBase_jni_1evaluateNoReturn__JLjava_lang_String_2J (JNIEnv * env, jobject obj, jlong cptr, jstring expression, jlong evalSettings)
{
	const char *nativeExpression = env->GetStringUTFChars(expression, 0);
	
    Madara::Knowledge_Engine::Knowledge_Base* knowledge = (Madara::Knowledge_Engine::Knowledge_Base*) cptr;
    Madara::Knowledge_Engine::Eval_Settings settings = *(Madara::Knowledge_Engine::Eval_Settings*) evalSettings;
    
    knowledge->evaluate(std::string(nativeExpression), settings);
	
	env->ReleaseStringUTFChars(expression, nativeExpression);
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_evaluateNoReturn
 * Signature: (JJJ)V
 */
MADARA_Export void JNICALL Java_com_madara_KnowledgeBase_jni_1evaluateNoReturn__JJJ (JNIEnv *env, jobject obj, jlong cptr, jlong expression, jlong evalSettings)
{
	Madara::Knowledge_Engine::Knowledge_Base* knowledge = (Madara::Knowledge_Engine::Knowledge_Base*) cptr;
    Madara::Knowledge_Engine::Compiled_Expression compiled_expression = *(Madara::Knowledge_Engine::Compiled_Expression*)expression;
    Madara::Knowledge_Engine::Eval_Settings settings = *(Madara::Knowledge_Engine::Eval_Settings*) evalSettings;
    
    knowledge->evaluate(compiled_expression, settings);
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_compile
 * Signature: (JLjava/lang/String;)J
 */
MADARA_Export jlong JNICALL Java_com_madara_KnowledgeBase_jni_1compile (JNIEnv *env, jobject obj, jlong cptr, jstring expression)
{
	const char *nativeExpression = env->GetStringUTFChars(expression, 0);

    Madara::Knowledge_Engine::Knowledge_Base* knowledge = (Madara::Knowledge_Engine::Knowledge_Base*) cptr;
    
    Madara::Knowledge_Engine::Compiled_Expression* ret = new Madara::Knowledge_Engine::Compiled_Expression(knowledge->compile(std::string(nativeExpression)));
	
	env->ReleaseStringUTFChars(expression, nativeExpression);
	
	return (jlong) ret;
}

/*
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
*/


/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_defineFunction
 * Signature: (JLjava/lang/String;)V
 */
/*
MADARA_Export void JNICALL Java_com_madara_KnowledgeBase_jni_1defineFunction__JLjava_lang_String_2 (JNIEnv *env, jobject obj, jlong cptr, jstring name)
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
 */

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_defineFunction
 * Signature: (JLjava/lang/String;Ljava/lang/String;)V
 */
/*
MADARA_Export void JNICALL Java_com_madara_KnowledgeBase_jni_1defineFunction__JLjava_lang_String_2Ljava_lang_String_2 (JNIEnv *env, jobject obj, jlong cptr, jstring name, jstring expression)
{
	const char *nativeExpression = (*env)->GetStringUTFChars(env, expression, 0);
	const char *nativeName = (*env)->GetStringUTFChars(env, name, 0);
	
	jni_KnowledgeBase_define_function_impl_PSS(cptr, nativeName, nativeExpression);
	
	(*env)->ReleaseStringUTFChars(env, name, nativeName);	
	(*env)->ReleaseStringUTFChars(env, expression, nativeExpression);
}
*/

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_defineFunction
 * Signature: (JLjava/lang/String;J)V
 */
/*
MADARA_Export void JNICALL Java_com_madara_KnowledgeBase_jni_1defineFunction__JLjava_lang_String_2J (JNIEnv *env, jobject obj, jlong cptr, jstring name, jlong expression)
{
	const char *nativeName = (*env)->GetStringUTFChars(env, name, 0);
	
	jni_KnowledgeBase_define_function_impl_PSP(cptr, nativeName, expression);
	
	(*env)->ReleaseStringUTFChars(env, name, nativeName);	
}
*/

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_clear
 * Signature: (J)V
 */
MADARA_Export void JNICALL Java_com_madara_KnowledgeBase_jni_1clear (JNIEnv *env, jobject obj, jlong cptr)
{
    Madara::Knowledge_Engine::Knowledge_Base* knowledge = (Madara::Knowledge_Engine::Knowledge_Base*) cptr;
    knowledge->clear();
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_get
 * Signature: (JLjava/lang/String;)J
 */
MADARA_Export jlong JNICALL Java_com_madara_KnowledgeBase_jni_1get (JNIEnv *env, jobject obj, jlong cptr, jstring var)
{
	const char *nativeVar = env->GetStringUTFChars(var, 0);
    
    Madara::Knowledge_Engine::Knowledge_Base* knowledge = (Madara::Knowledge_Engine::Knowledge_Base*) cptr;
    Madara::Knowledge_Record* ret = new Madara::Knowledge_Record(knowledge->get(std::string(nativeVar)));
    
	env->ReleaseStringUTFChars(var, nativeVar);
    
	return (jlong) ret;
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_set
 * Signature: (JLjava/lang/String;J)V
 */
MADARA_Export void JNICALL Java_com_madara_KnowledgeBase_jni_1set (JNIEnv *env, jobject obj, jlong cptr, jstring var, jlong recordPtr)
{
	const char *nativeVar = env->GetStringUTFChars(var, 0);

    Madara::Knowledge_Engine::Knowledge_Base* knowledge = (Madara::Knowledge_Engine::Knowledge_Base*) cptr;
    Madara::Knowledge_Record record = *(Madara::Knowledge_Record*)recordPtr;
    
    switch (record.type())
	{
		case Madara::Knowledge_Record::INTEGER:
			knowledge->set(std::string(nativeVar), record.to_integer()); break;
		case Madara::Knowledge_Record::DOUBLE:
			knowledge->set(std::string(nativeVar), record.to_double()); break;
		case Madara::Knowledge_Record::STRING:
			knowledge->set(std::string(nativeVar), record.to_string()); break;
			
		default:
			break;
	}
    
	env->ReleaseStringUTFChars(var, nativeVar);
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_wait
 * Signature: (JLjava/lang/String;J)J
 */
MADARA_Export jlong JNICALL Java_com_madara_KnowledgeBase_jni_1wait__JLjava_lang_String_2J (JNIEnv * env, jobject obj, jlong cptr, jstring expression, jlong waitSettings)
{
	const char *nativeExpression = env->GetStringUTFChars(expression, 0);
	
    Madara::Knowledge_Engine::Knowledge_Base* knowledge = (Madara::Knowledge_Engine::Knowledge_Base*) cptr;
	Madara::Knowledge_Engine::Wait_Settings settings = *(Madara::Knowledge_Engine::Wait_Settings*)waitSettings;
    Madara::Knowledge_Record* ret = new Madara::Knowledge_Record(knowledge->wait(std::string(nativeExpression), settings));
    
	env->ReleaseStringUTFChars(expression, nativeExpression);
	
	return (jlong) ret;
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_wait
 * Signature: (JJJ)J
 */
MADARA_Export jlong JNICALL Java_com_madara_KnowledgeBase_jni_1wait__JJJ (JNIEnv * env, jobject obj, jlong cptr, jlong expression, jlong waitSettings)
{
    Madara::Knowledge_Engine::Knowledge_Base* knowledge = (Madara::Knowledge_Engine::Knowledge_Base*) cptr;
    Madara::Knowledge_Engine::Compiled_Expression compiled_expression = *(Madara::Knowledge_Engine::Compiled_Expression*)expression;
	Madara::Knowledge_Engine::Wait_Settings settings = *(Madara::Knowledge_Engine::Wait_Settings*)waitSettings;
    Madara::Knowledge_Record* ret = new Madara::Knowledge_Record(knowledge->wait(compiled_expression, settings));
    
    return (jlong) ret;
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_waitNoReturn
 * Signature: (JLjava/lang/String;J)V
 */
MADARA_Export void JNICALL Java_com_madara_KnowledgeBase_jni_1waitNoReturn__JLjava_lang_String_2J (JNIEnv * env, jobject obj, jlong cptr, jstring expression, jlong waitSettings)
{
    const char *nativeExpression = env->GetStringUTFChars(expression, 0);
	
    Madara::Knowledge_Engine::Knowledge_Base* knowledge = (Madara::Knowledge_Engine::Knowledge_Base*) cptr;
	Madara::Knowledge_Engine::Wait_Settings settings = *(Madara::Knowledge_Engine::Wait_Settings*)waitSettings;
    knowledge->wait(std::string(nativeExpression), settings);
    
	env->ReleaseStringUTFChars(expression, nativeExpression);
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_waitNoReturn
 * Signature: (JJJ)V
 */
MADARA_Export void JNICALL Java_com_madara_KnowledgeBase_jni_1waitNoReturn__JJJ (JNIEnv * env, jobject obj, jlong cptr, jlong expression, jlong waitSettings)
{
    Madara::Knowledge_Engine::Knowledge_Base* knowledge = (Madara::Knowledge_Engine::Knowledge_Base*) cptr;
    Madara::Knowledge_Engine::Compiled_Expression compiled_expression = *(Madara::Knowledge_Engine::Compiled_Expression*)expression;
	Madara::Knowledge_Engine::Wait_Settings settings = *(Madara::Knowledge_Engine::Wait_Settings*)waitSettings;
    knowledge->wait(compiled_expression, settings);
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_freeKnowledgeBase
 * Signature: (J)V
 */
MADARA_Export void JNICALL Java_com_madara_KnowledgeBase_jni_1freeKnowledgeBase (JNIEnv * env, jobject obj, jlong cptr)
{
	Madara::Knowledge_Engine::Knowledge_Base* knowledge = (Madara::Knowledge_Engine::Knowledge_Base*) cptr;
    if (knowledge)
        delete knowledge;
}

/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_toKnowledgeList
 * Signature: (JLjava/lang/String;II)[J
 */

MADARA_Export jlongArray JNICALL Java_com_madara_KnowledgeBase_jni_1toKnowledgeList (JNIEnv * env, jobject obj, jlong cptr, jstring subject, jint start, jint end)
{
	const char *nativeSubject = env->GetStringUTFChars(subject, 0);
    
    Madara::Knowledge_Engine::Knowledge_Base* knowledge = (Madara::Knowledge_Engine::Knowledge_Base*) cptr;
	
    std::vector<Madara::Knowledge_Record> returnVector;
    
    knowledge->to_vector(std::string(nativeSubject), start, end, returnVector);
    
    jlongArray ret = env->NewLongArray(returnVector.size());
    jlong tmp[returnVector.size()];
    
    for (int x = 0; x < returnVector.size(); x++)
    {
        tmp[x] = (jlong) new Madara::Knowledge_Record(returnVector[x]);
    }
    
	env->SetLongArrayRegion(ret, 0, returnVector.size(), tmp);
	
	return ret;
}


/*
 * Class:     com_madara_KnowledgeBase
 * Method:    jni_toKnowledgeMap
 * Signature: (JLjava/lang/String;Lcom/madara/KnowledgeBase/MapReturn;)V
 */

MADARA_Export void JNICALL Java_com_madara_KnowledgeBase_jni_1toKnowledgeMap (JNIEnv * env, jobject obj, jlong cptr, jstring expression, jobject jniRet)
{
	jclass jniRetClass = env->GetObjectClass(jniRet);
    jclass classStrArray = env->FindClass("java/lang/String");
	jfieldID valsID = env->GetFieldID(jniRetClass, "vals", "[J");
	jfieldID keysID = env->GetFieldID(jniRetClass, "keys", "[Ljava/lang/String;");
    
    const char *nativeExpression = env->GetStringUTFChars(expression, 0);
    
    Madara::Knowledge_Engine::Knowledge_Base* knowledge = (Madara::Knowledge_Engine::Knowledge_Base*) cptr;
	
    std::map<std::string, Madara::Knowledge_Record> recordsMap;
    
    knowledge->to_map(std::string(nativeExpression), recordsMap);
    
    env->ReleaseStringUTFChars(expression, nativeExpression);
    
    
    jlongArray recordsArray = env->NewLongArray(recordsMap.size());
    jlong records[recordsMap.size()];
    
    jobjectArray keysArray = env->NewObjectArray(recordsMap.size(), classStrArray, NULL);
    
    std::map<std::string, Madara::Knowledge_Record>::iterator iter;
    int counter = 0;
    for (iter = recordsMap.begin(); iter != recordsMap.end(); ++iter)
	{
        env->SetObjectArrayElement(keysArray, counter, env->NewStringUTF(iter->first.c_str()));
		records[counter++] = (jlong) new Madara::Knowledge_Record(iter->second);
	}
	
    env->SetLongArrayRegion(recordsArray, 0, recordsMap.size(), records);
    
    env->SetObjectField(jniRet, valsID, recordsArray);
    env->SetObjectField(jniRet, keysID, keysArray);
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
MADARA_Export void JNICALL Java_com_madara_KnowledgeBase_00024CompiledExpression_jni_1freeCompiledExpression (JNIEnv * env, jobject obj, jlong cptr)
{
	Madara::Knowledge_Engine::Compiled_Expression* expression = (Madara::Knowledge_Engine::Compiled_Expression*)cptr;
    if (expression)
        delete expression;
}  
//===================================================================================


