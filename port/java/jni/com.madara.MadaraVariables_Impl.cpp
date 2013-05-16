/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
 
#include "com.madara.MadaraVariables_Impl.h"
#include "madara/knowledge_engine/Knowledge_Base.h"

//===================================================================================
//KnowledgeBase
//===================================================================================

void jni_MadaraVariables_evaluate_impl_RPPP(void** ret, void* cptr, void* expression, void* evalSettings)
{
	Madara::Knowledge_Engine::Variables vars = *(Madara::Knowledge_Engine::Variables*)cptr;
	Madara::Knowledge_Engine::Compiled_Expression compiled_expression = *(Madara::Knowledge_Engine::Compiled_Expression*)expression;
	Madara::Knowledge_Engine::Eval_Settings settings = *(Madara::Knowledge_Engine::Eval_Settings*)evalSettings;
	*ret =  new Madara::Knowledge_Record(vars.evaluate(compiled_expression, settings));
}

void jni_MadaraVariables_compile_impl_RPS(void** ret, void* cptr, const char* expression)
{
	Madara::Knowledge_Engine::Variables vars = *(Madara::Knowledge_Engine::Variables*)cptr;
	*ret = new Madara::Knowledge_Engine::Compiled_Expression(vars.compile(expression));
}

void jni_MadaraVariables_get_impl_RPS(void** ret, void* cptr, const char* var)
{
	Madara::Knowledge_Engine::Variables vars = *(Madara::Knowledge_Engine::Variables*)cptr;
	*ret = new Madara::Knowledge_Record(vars.get(std::string(var)));
}

void jni_MadaraVariables_set_impl_PSP(void* cptr, const char* var, void* kr)
{
	Madara::Knowledge_Engine::Variables vars = *(Madara::Knowledge_Engine::Variables*)cptr;
	Madara::Knowledge_Record record = *(Madara::Knowledge_Record*)kr;
	
	switch (record.type())
	{
		case Madara::Knowledge_Record::INTEGER:
			vars.set(std::string(var), record.to_integer()); break;
		case Madara::Knowledge_Record::DOUBLE:
			vars.set(std::string(var), record.to_double()); break;
		case Madara::Knowledge_Record::STRING:
			vars.set(std::string(var), record.to_string()); break;
			
		default:
			break;
	}
}

//===================================================================================
