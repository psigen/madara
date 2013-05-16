/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

#ifndef _Included_com_madara_KnowledgeBase_Impl
#define _Included_com_madara_KnowledgeBase_Impl

#ifdef __cplusplus
extern "C" {
#endif

//===================================================================================
//KnowledgeBase
//===================================================================================
void jni_KnowledgeBase_impl_RV(void**);
void jni_KnowledgeBase_impl_RSIS(void**, const char*, int, const char*);
void jni_KnowledgeBase_impl_RSP(void**, const char*, void*);
void jni_KnowledgeBase_impl_RP(void**, void*);

void jni_KnowledgeBase_evaluate_impl_RPSP(void**, void*, const char*, void*);
void jni_KnowledgeBase_evaluate_impl_RPPP(void**, void*, void*, void*);
void jni_KnowledgeBase_evaluate_no_return_impl_PSP(void*, const char*, void*);
void jni_KnowledgeBase_evaluate_no_return_impl_PPP(void*, void*, void*);

void jni_KnowledgeBase_compile_impl_RPS(void**, void*, const char*);

void jni_KnowledgeBase_define_Function_impl_PSPM(void*, const char*, void*, void (*callback)(void**, const char*, void**, unsigned int, void*, void*));
void jni_KnowledgeBase_define_function_impl_PSS(void*, const char*, const char*);
void jni_KnowledgeBase_define_function_impl_PSP(void*, const char*, void*);

void jni_KnowledgeBase_clear_impl_P(void*);

void jni_KnowledgeBase_get_impl_RPS(void**, void*, const char*);
void jni_KnowledgeBase_set_impl_PSP(void*, const char*, void*);

void jni_KnowledgeBase_wait_impl_RPSP(void**, void*, const char*, void*);
void jni_KnowledgeBase_wait_impl_RPPP(void**, void*, void*, void*);
void jni_KnowledgeBase_wait_no_return_PSP(void*, const char*, void*);
void jni_KnowledgeBase_wait_no_return_PPP(void*, void*, void*);

void jni_KnowledgeBase_free_knowledge_base_P(void*);
//===================================================================================

//===================================================================================
//CompiledExpression
//===================================================================================
void jni_KnowledgeBase_free_compiled_expression_P(void*);
//===================================================================================

#ifdef __cplusplus
}
#endif
#endif
