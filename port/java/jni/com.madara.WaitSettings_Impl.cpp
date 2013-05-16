/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
 
#include "com.madara.WaitSettings_Impl.h"
#include "madara/knowledge_engine/Wait_Settings.h"


//===================================================================================
//WaitSettings
//===================================================================================
void jni_WaitSettings_get_default_wait_settings_RI(void** ret, int which)
{
	switch (which)
	{
		case 0:
		default: *ret = const_cast<Madara::Knowledge_Engine::Wait_Settings*>(&(Madara::Knowledge_Engine::DEFAULT_WAIT_SETTINGS));
	}
}

void jni_WaitSettings_RV(void** ret)
{
	*ret = new Madara::Knowledge_Engine::Wait_Settings();
}

void jni_WaitSettings_RP(void** ret, void* original)
{
	*ret = new Madara::Knowledge_Engine::Wait_Settings(*(Madara::Knowledge_Engine::Wait_Settings*)original);
}


void jni_WaitSettings_set_poll_frequency_PD(void* cptr, double freq)
{
	Madara::Knowledge_Engine::Wait_Settings* settings = (Madara::Knowledge_Engine::Wait_Settings*)cptr;
	settings->poll_frequency = freq;
}

double jni_WaitSettings_get_poll_frequency_P(void* cptr)
{
	Madara::Knowledge_Engine::Wait_Settings* settings = (Madara::Knowledge_Engine::Wait_Settings*)cptr;
	return settings->poll_frequency;
}

void jni_WaitSettings_set_max_wait_time_PD(void* cptr, double maxWaitTime)
{
	Madara::Knowledge_Engine::Wait_Settings* settings = (Madara::Knowledge_Engine::Wait_Settings*)cptr;
	settings->max_wait_time = maxWaitTime;
}

double jni_WaitSettings_get_max_wait_time(void* cptr)
{
	Madara::Knowledge_Engine::Wait_Settings* settings = (Madara::Knowledge_Engine::Wait_Settings*)cptr;
	return settings->max_wait_time;
}
//===================================================================================
