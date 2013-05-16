/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
 
#include "com.madara.Debug_Impl.h"
#include "madara/utility/Log_Macros.h"


void jni_Debug_set_debug_level_I(int debugLevel)
{
	MADARA_debug_level = debugLevel;
}
