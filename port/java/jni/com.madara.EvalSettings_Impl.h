/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#ifndef _Included_com_madara_EvalSettings_Impl
#define _Included_com_madara_EvalSettings_Impl

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

//===================================================================================
//EvalSettings
//===================================================================================
void jni_getDefaultEvalSettings_impl_RI(void**, int);
void jni_EvalSettings_impl_RV(void**);
void jni_EvalSettings_impl_RP(void**, void*);


void jni_EvalSettings_set_delay_sending_modifieds_impl_PB(void*, bool);
bool jni_EvalSettings_get_delay_sending_modifieds_impl_P(void*);

void jni_EvalSettings_set_pre_print_statement_impl_PS(void*, const char*);
const char* jni_EvalSettings_get_pre_print_statement_impl_P(void*);

void jni_EvalSettings_set_post_print_statement_impl_PS(void*, const char*);
const char* jni_EvalSettings_get_post_print_statement_impl_P(void*);

void jni_EvalSettings_set_always_overwrite_impl_PB(void*, bool);
bool jni_EvalSettings_get_always_overwrite_impl_P(void*);

void jni_EvalSettings_set_treat_globals_as_locals_impl_PB(void*, bool);
bool jni_EvalSettings_get_treat_globals_as_locals_impl_P(void*);

void jni_EvalSettings_set_clock_increment_impl_PJ(void*, long);
long jni_EvalSettings_get_clock_increment(void*);
//===================================================================================
#ifdef __cplusplus
}
#endif
#endif
