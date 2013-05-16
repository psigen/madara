/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#ifndef _Included_com_madara_MadaraVariables_Impl
#define _Included_com_madara_MadaraVariables_Impl

#ifdef __cplusplus
extern "C" {
#endif

//===================================================================================
//MadaraVariables
//===================================================================================
void jni_MadaraVariables_evaluate_impl_RPPP(void**, void*, void*, void*);
void jni_MadaraVariables_compile_impl_RPS(void**, void*, const char*);

void jni_MadaraVariables_get_impl_RPS(void**, void*, const char*);
void jni_MadaraVariables_set_impl_PSP(void*, const char*, void*);
//===================================================================================

#ifdef __cplusplus
}
#endif
#endif
