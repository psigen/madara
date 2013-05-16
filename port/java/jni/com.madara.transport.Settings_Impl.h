/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#ifndef _Included_com_madara_transport_Settings_Impl
#define _Included_com_madara_transport_Settings_Impl


#ifdef __cplusplus
extern "C" {
#endif

//===================================================================================
//Settings
//===================================================================================

void jni_Settings_impl_RV(void**);
void jni_Settings_impl_RP(void**, void*);

void jni_Settings_set_domains_impl_PS(void*, const char*);
const char* jni_Settings_get_domains_impl_P(void*);

void jni_Settings_set_queue_length_impl_PI(void*, int);
int jni_Settings_get_queue_length_impl_P(void*);

void jni_Settings_set_deadline_impl_PI(void*, int);
int jni_Settings_get_deadline_impl_P(void*);

void jni_Settings_set_type_impl_PI(void*, int);
int jni_Settings_get_type_impl_P(void*);

void jni_Settings_set_reliability_impl_PI(void*, int);
int jni_Settings_get_reliability_impl_P(void*);

void jni_Settings_set_id_impl_PI(void*, int);
int jni_Settings_get_id_impl_P(void*);

void jni_Settings_set_processes_impl_PI(void*, int);
int jni_Settings_get_processes_impl_P(void*);

void jni_Settings_set_on_data_received_logic_impl_PS(void*, const char*);
const char* jni_Settings_get_on_data_received_logic_impl_P(void*);

void jni_Settings_set_hosts_impl_PAI(void*, const char**, int);
const char** jni_Settings_get_hosts_PI(void*, int*);

//===================================================================================
#ifdef __cplusplus
}
#endif
#endif
