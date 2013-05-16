/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
 
#include "com.madara.EvalSettings_Impl.h"
#include "madara/knowledge_engine/Eval_Settings.h"

#include <stdio.h>
#include <string>

//==============================================================================
//EvalSettings
//==============================================================================
void jni_getDefaultEvalSettings_impl_RI(void** ret, int which)
{
	switch (which)
	{
		case 0: *ret = const_cast<Madara::Knowledge_Engine::Eval_Settings*>(&(Madara::Knowledge_Engine::DEFAULT_EVAL_SETTINGS));
		case 1: *ret = const_cast<Madara::Knowledge_Engine::Eval_Settings*>(&(Madara::Knowledge_Engine::DELAY_AND_TREAT_AS_LOCAL_EVAL_SETTINGS));
		case 2: *ret = const_cast<Madara::Knowledge_Engine::Eval_Settings*>(&(Madara::Knowledge_Engine::DELAY_ONLY_EVAL_SETTINGS));
		case 3: *ret = const_cast<Madara::Knowledge_Engine::Eval_Settings*>(&(Madara::Knowledge_Engine::TREAT_AS_LOCAL_EVAL_SETTINGS));
		default: *ret = const_cast<Madara::Knowledge_Engine::Eval_Settings*>(&(Madara::Knowledge_Engine::DEFAULT_EVAL_SETTINGS));
		
	}
}
void jni_EvalSettings_impl_RV(void** ret)
{
	*ret = new Madara::Knowledge_Engine::Eval_Settings();
}

void jni_EvalSettings_impl_RP(void** ret, void* original)
{
	*ret = new Madara::Knowledge_Engine::Eval_Settings(*(Madara::Knowledge_Engine::Eval_Settings*)original);
}


void jni_EvalSettings_set_delay_sending_modifieds_impl_PB(void* cptr, bool delay_sending_modifieds)
{
	Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
	evalSettings->delay_sending_modifieds = delay_sending_modifieds;
}

bool jni_EvalSettings_get_delay_sending_modifieds_impl_P(void* cptr)
{
	Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
	return evalSettings->delay_sending_modifieds;
}


void jni_EvalSettings_set_pre_print_statement_impl_PS(void* cptr, const char* pre_print_statement)
{
	Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
	evalSettings->pre_print_statement = std::string(pre_print_statement);
}

const char* jni_EvalSettings_get_pre_print_statement_impl_P(void* cptr)
{
	Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
	return evalSettings->pre_print_statement.c_str();
}

void jni_EvalSettings_set_post_print_statement_impl_PS(void* cptr, const char* post_print_statement)
{
	Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
	evalSettings->post_print_statement = std::string(post_print_statement);
}

const char* jni_EvalSettings_get_post_print_statement_impl_P(void* cptr)
{
	Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
	return evalSettings->post_print_statement.c_str();
}

void jni_EvalSettings_set_always_overwrite_impl_PB(void* cptr, bool always_overwrite)
{
	Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
	evalSettings->always_overwrite = always_overwrite;
}
bool jni_EvalSettings_get_always_overwrite_impl_P(void* cptr)
{
	Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
	return evalSettings->always_overwrite;
}

void jni_EvalSettings_set_treat_globals_as_locals_impl_PB(void* cptr, bool treat_globals_as_locals)
{
	Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
	evalSettings->treat_globals_as_locals = treat_globals_as_locals;
}

bool jni_EvalSettings_get_treat_globals_as_locals_impl_P(void* cptr)
{
	Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
	return evalSettings->treat_globals_as_locals;
}

void jni_EvalSettings_set_clock_increment_impl_PJ(void* cptr, long clock_increment)
{
	Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
	evalSettings->clock_increment = clock_increment;
}
long jni_EvalSettings_get_clock_increment(void* cptr)
{
	Madara::Knowledge_Engine::Eval_Settings* evalSettings = (Madara::Knowledge_Engine::Eval_Settings*)cptr;
	return evalSettings->clock_increment;
}
//==============================================================================
