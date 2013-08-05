/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#include "com.madara.Debug.h"
#include "madara/utility/Log_Macros.h"

/*
 * Class:     com_madara_Debug
 * Method:    jni_setDebugLevel
 * Signature: (I)V
 */
MADARA_Export void JNICALL Java_com_madara_Debug_jni_1setDebugLevel (JNIEnv *env, jclass clazz, jint debugLevel)
{
	MADARA_debug_level = debugLevel;
}
