/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#include "com.madara.EvalSettings.h"
#include "madara/knowledge_engine/Eval_Settings.h"

#include <stdio.h>
#include <string>

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getDefaultEvalSettings
 * Signature: (I)J
 */
MADARA_Export jlong JNICALL Java_com_madara_EvalSettings_jni_1getDefaultEvalSettings(JNIEnv *env, jclass clazz, jint which)
{
	//void* ret;
	//jni_getDefaultEvalSettings_impl_RI(&ret, which);
	return 0;
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_evalSettings
 * Signature: ()J
 */
MADARA_Export jlong JNICALL Java_com_madara_EvalSettings_jni_1evalSettings__(JNIEnv *env, jobject obj)
{
	return (jlong)(new Madara::Knowledge_Engine::Eval_Settings());
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_evalSettings
 * Signature: (J)J
 */
MADARA_Export jlong JNICALL Java_com_madara_EvalSettings_jni_1evalSettings__J(JNIEnv *env, jobject obj, jlong original)
{
	return (jlong) (new Madara::Knowledge_Engine::Eval_Settings(*(Madara::Knowledge_Engine::Eval_Settings*)original));
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_setDelaySendingModifieds
 * Signature: (JZ)V
 */
MADARA_Export void JNICALL Java_com_madara_EvalSettings_jni_1setDelaySendingModifieds (JNIEnv * env, jobject obj, jlong cptr, jboolean delaySendingModifieds)
{
	Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
    evalSettings->delay_sending_modifieds = delaySendingModifieds;
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getDelaySendingModifieds
 * Signature: (J)Z
 */
MADARA_Export jboolean JNICALL Java_com_madara_EvalSettings_jni_1getDelaySendingModifieds (JNIEnv * env, jobject obj, jlong cptr)
{
    Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
	return (jboolean) evalSettings->delay_sending_modifieds;
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_setPrePrintStatement
 * Signature: (JLjava/lang/String;)V
 */
MADARA_Export void JNICALL Java_com_madara_EvalSettings_jni_1setPrePrintStatement (JNIEnv * env, jobject obj, jlong cptr, jstring prePrintStatement)
{
	const char *nativePrePrint = env->GetStringUTFChars(prePrintStatement, 0);
	
    Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
    evalSettings->pre_print_statement = std::string(nativePrePrint);
	
	env->ReleaseStringUTFChars(prePrintStatement, nativePrePrint);
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getPrePrintStatement
 * Signature: (J)Ljava/lang/String;
 */
MADARA_Export jstring JNICALL Java_com_madara_EvalSettings_jni_1getPrePrintStatement (JNIEnv * env, jobject obj, jlong cptr)
{
    
    Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
	return env->NewStringUTF(evalSettings->pre_print_statement.c_str());
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_setPostPrintStatement
 * Signature: (JLjava/lang/String;)V
 */
MADARA_Export void JNICALL Java_com_madara_EvalSettings_jni_1setPostPrintStatement (JNIEnv *env, jobject obj, jlong cptr, jstring postPrintStatement)
{
	const char *nativePostPrint = env->GetStringUTFChars(postPrintStatement, 0);
	
    Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
    evalSettings->post_print_statement = std::string(nativePostPrint);
	
	env->ReleaseStringUTFChars(postPrintStatement, nativePostPrint);
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getPostPrintStatement
 * Signature: (J)Ljava/lang/String;
 */
MADARA_Export jstring JNICALL Java_com_madara_EvalSettings_jni_1getPostPrintStatement (JNIEnv * env, jobject obj, jlong cptr)
{
	Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
	return env->NewStringUTF(evalSettings->post_print_statement.c_str());
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_setAlwaysOverwrite
 * Signature: (JZ)V
 */
MADARA_Export void JNICALL Java_com_madara_EvalSettings_jni_1setAlwaysOverwrite (JNIEnv * env, jobject obj, jlong cptr, jboolean alwaysOverwrite)
{
	Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
    evalSettings->always_overwrite = alwaysOverwrite;
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getAlwaysOverwrite
 * Signature: (J)Z
 */
MADARA_Export jboolean JNICALL Java_com_madara_EvalSettings_jni_1getAlwaysOverwrite (JNIEnv * env, jobject obj, jlong cptr)
{
    Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
	return (jboolean) evalSettings->always_overwrite;
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_setTreatGlobalsAsLocals
 * Signature: (JZ)V
 */
MADARA_Export void JNICALL Java_com_madara_EvalSettings_jni_1setTreatGlobalsAsLocals (JNIEnv *env, jobject obj, jlong cptr, jboolean treatGlobalsAsLocals)
{
    Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
    evalSettings->treat_globals_as_locals = treatGlobalsAsLocals;
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getTreatGlobalsAsLocals
 * Signature: (J)Z
 */
MADARA_Export jboolean JNICALL Java_com_madara_EvalSettings_jni_1getTreatGlobalsAsLocals (JNIEnv * env, jobject obj, jlong cptr)
{
    Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
	return (jboolean) evalSettings->treat_globals_as_locals;
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_setDefaultClockIncrement
 * Signature: (JJ)V
 */
MADARA_Export void JNICALL Java_com_madara_EvalSettings_jni_1setClockIncrement (JNIEnv *env, jobject obj, jlong cptr, jlong defaultClockIncrement)
{
    Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
    evalSettings->clock_increment = defaultClockIncrement;
}

/*
 * Class:     com_madara_EvalSettings
 * Method:    jni_getDefaultClockIncrement
 * Signature: (J)J
 */
MADARA_Export jlong JNICALL Java_com_madara_EvalSettings_jni_1getClockIncrement (JNIEnv *env, jobject obj, jlong cptr)
{
    Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
	return (jlong) evalSettings->clock_increment;
}
  
  
  
