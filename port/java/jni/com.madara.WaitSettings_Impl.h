/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#ifndef _Included_com_madara_WaitSettings_Impl
#define _Included_com_madara_WaitSettings_Impl

#ifdef __cplusplus
extern "C" {
#endif

//===================================================================================
//WaitSettings
//===================================================================================
void jni_WaitSettings_get_default_wait_settings_RI(void**, int);
void jni_WaitSettings_RV(void**);
void jni_WaitSettings_RP(void**, void*);


void jni_WaitSettings_set_poll_frequency_PD(void*, double);
double jni_WaitSettings_get_poll_frequency_P(void*);

void jni_WaitSettings_set_max_wait_time_PD(void*, double);
double jni_WaitSettings_get_max_wait_time(void*);
//===================================================================================
#ifdef __cplusplus
}
#endif
#endif
