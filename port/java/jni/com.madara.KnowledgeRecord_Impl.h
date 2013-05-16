/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#ifndef _Included_com_madara_KnowledgeRecord_Impl
#define _Included_com_madara_KnowledgeRecord_Impl

#ifdef __cplusplus
extern "C" {
#endif

//===================================================================================
//KnowledgeRecord
//===================================================================================
void jni_KnowledgeRecord_impl_RS(void**, const char *);
void jni_KnowledgeRecord_impl_RD(void**, double);
void jni_KnowledgeRecord_impl_RJ(void**, long);

long jni_KnowledgeRecord_to_long_value_impl_P(void*);
const char* jni_KnowledgeRecord_to_string_value_impl_P(void*);
double jni_KnowledgeRecord_to_double_value_impl_P(void*);

int jni_KnowledgeRecord_get_type_P(void*);

void jni_KnowledgeRecord_free_P(void*);
//===================================================================================

#ifdef __cplusplus
}
#endif
#endif
