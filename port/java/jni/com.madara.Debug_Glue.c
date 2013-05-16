/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#include "com.madara.Debug_Glue.h"
#include "com.madara.Debug_Impl.h"

/*
 * Class:     com_madara_Debug
 * Method:    jni_setDebugLevel
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_madara_Debug_jni_1setDebugLevel (JNIEnv *env, jclass clazz, jint debugLevel)
{
	jni_Debug_set_debug_level_I(debugLevel);
}
