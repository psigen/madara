/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
 
#include "com.madara.KnowledgeBase_Impl.h"
#include "madara/knowledge_engine/Knowledge_Base.h"

#include <string>
#include <map>

static std::map<std::string, void*> definedFunctionsMap;
static void (*cbFunction)(void**, const char*, void**, unsigned int, void*, void*);

//===================================================================================
//KnowledgeBase
//===================================================================================
void jni_KnowledgeBase_impl_RV(void** ret)
{
	*ret = new Madara::Knowledge_Engine::Knowledge_Base();
}

void jni_KnowledgeBase_impl_RSIS(void** ret, const char* host, int transport, const char* domain)
{
	std::string hostStr(host);
	std::string domainStr(domain);
	
	*ret = new Madara::Knowledge_Engine::Knowledge_Base(hostStr, transport, domain);
}

void jni_KnowledgeBase_impl_RSP(void** ret, const char* host, void* config)
{
	std::string hostStr(host);
	Madara::Transport::Settings* settings = (Madara::Transport::Settings*)config;
	*ret = new Madara::Knowledge_Engine::Knowledge_Base(hostStr, *settings);
}

void jni_KnowledgeBase_impl_RP(void** ret, void* _original)
{
	*ret = new Madara::Knowledge_Engine::Knowledge_Base(*(Madara::Knowledge_Engine::Knowledge_Base*)_original);
}

void jni_KnowledgeBase_evaluate_impl_RPSP(void** ret, void* cptr, const char* expression, void* evalSettings)
{
	std::string expressionStr(expression);
	Madara::Knowledge_Engine::Knowledge_Base knowledge = *(Madara::Knowledge_Engine::Knowledge_Base*)cptr;
	Madara::Knowledge_Engine::Eval_Settings settings = *(Madara::Knowledge_Engine::Eval_Settings*)evalSettings;
	*ret = new Madara::Knowledge_Record(knowledge.evaluate(expressionStr, settings));
}
void jni_KnowledgeBase_evaluate_impl_RPPP(void** ret, void* cptr, void* expression, void* evalSettings)
{
	Madara::Knowledge_Engine::Knowledge_Base knowledge = *(Madara::Knowledge_Engine::Knowledge_Base*)cptr;
	Madara::Knowledge_Engine::Compiled_Expression compiled_expression = *(Madara::Knowledge_Engine::Compiled_Expression*)expression;
	Madara::Knowledge_Engine::Eval_Settings settings = *(Madara::Knowledge_Engine::Eval_Settings*)evalSettings;
	*ret =  new Madara::Knowledge_Record(knowledge.evaluate(compiled_expression, settings));
}

void jni_KnowledgeBase_evaluate_no_return_impl_PSP(void* cptr, const char* expression, void* evalSettings)
{
	std::string expressionStr(expression);
	Madara::Knowledge_Engine::Knowledge_Base knowledge = *(Madara::Knowledge_Engine::Knowledge_Base*)cptr;
	Madara::Knowledge_Engine::Eval_Settings settings = *(Madara::Knowledge_Engine::Eval_Settings*)evalSettings;
	knowledge.evaluate(expressionStr, settings);
}

void jni_KnowledgeBase_evaluate_no_return_impl_PPP(void* cptr, void* expression, void* evalSettings)
{
	Madara::Knowledge_Engine::Knowledge_Base knowledge = *(Madara::Knowledge_Engine::Knowledge_Base*)cptr;
	Madara::Knowledge_Engine::Compiled_Expression compiled_expression = *(Madara::Knowledge_Engine::Compiled_Expression*)expression;
	Madara::Knowledge_Engine::Eval_Settings settings = *(Madara::Knowledge_Engine::Eval_Settings*)evalSettings;
	knowledge.evaluate(compiled_expression, settings);
}

void jni_KnowledgeBase_compile_impl_RPS(void** ret, void* cptr, const char* expression)
{
	Madara::Knowledge_Engine::Knowledge_Base knowledge = *(Madara::Knowledge_Engine::Knowledge_Base*)cptr;
	*ret = new Madara::Knowledge_Engine::Compiled_Expression(knowledge.compile(expression));
}


//Function_Arguements = std::vector<Knowledge_Record>
Madara::Knowledge_Record default_madara_return_function (const char * name, Madara::Knowledge_Engine::Function_Arguments & args, Madara::Knowledge_Engine::Variables & variables)
{
	void* ret;
	void* argsArray[args.size()];
	for (int x = 0; x < args.size(); x++)
	{
		argsArray[x] = &(args[x]);
	}

	cbFunction(&ret, name, argsArray /*args*/, args.size(), &variables, definedFunctionsMap[std::string(name)]);
	return *(Madara::Knowledge_Record*)ret;
}

void jni_KnowledgeBase_define_Function_impl_PSPM(void* cptr, const char* name, void* data, void (*callback)(void**, const char*, void**, unsigned int, void*, void*))
{
	std::string strname(name);
	definedFunctionsMap[strname] = data;
	cbFunction = callback;
	
	Madara::Knowledge_Engine::Knowledge_Base knowledge = *(Madara::Knowledge_Engine::Knowledge_Base*)cptr;

	knowledge.define_function(strname, default_madara_return_function);
}

void jni_KnowledgeBase_define_function_impl_PSS(void* cptr, const char* name, const char* expression)
{
	Madara::Knowledge_Engine::Knowledge_Base knowledge = *(Madara::Knowledge_Engine::Knowledge_Base*)cptr;
	knowledge.define_function(std::string(name), std::string(expression));	
}

void jni_KnowledgeBase_define_function_impl_PSP(void* cptr, const char* name, void* expression)
{
	Madara::Knowledge_Engine::Knowledge_Base knowledge = *(Madara::Knowledge_Engine::Knowledge_Base*)cptr;
	Madara::Knowledge_Engine::Compiled_Expression compiled_expression = *(Madara::Knowledge_Engine::Compiled_Expression*)expression;
	
	knowledge.define_function(std::string(name), compiled_expression);
}

void jni_KnowledgeBase_clear_impl_P(void* cptr)
{
	Madara::Knowledge_Engine::Knowledge_Base knowledge = *(Madara::Knowledge_Engine::Knowledge_Base*)cptr;
	knowledge.clear();
}

void jni_KnowledgeBase_get_impl_RPS(void** ret, void* cptr, const char* var)
{
	Madara::Knowledge_Engine::Knowledge_Base knowledge = *(Madara::Knowledge_Engine::Knowledge_Base*)cptr;
	*ret = new Madara::Knowledge_Record(knowledge.get(std::string(var)));
}

void jni_KnowledgeBase_set_impl_PSP(void* cptr, const char* var, void* kr)
{
	Madara::Knowledge_Engine::Knowledge_Base knowledge = *(Madara::Knowledge_Engine::Knowledge_Base*)cptr;
	Madara::Knowledge_Record record = *(Madara::Knowledge_Record*)kr;
	
	switch (record.type())
	{
		case Madara::Knowledge_Record::INTEGER:
			knowledge.set(std::string(var), record.to_integer()); break;
		case Madara::Knowledge_Record::DOUBLE:
			knowledge.set(std::string(var), record.to_double()); break;
		case Madara::Knowledge_Record::STRING:
			knowledge.set(std::string(var), record.to_string()); break;
			
		default:
			break;
	}
}

void jni_KnowledgeBase_wait_impl_RPSP(void** ret, void* cptr, const char* expression, void* waitSettings)
{
	Madara::Knowledge_Engine::Knowledge_Base knowledge = *(Madara::Knowledge_Engine::Knowledge_Base*)cptr;
	Madara::Knowledge_Engine::Wait_Settings settings = *(Madara::Knowledge_Engine::Wait_Settings*)waitSettings;
	*ret = new Madara::Knowledge_Record(knowledge.wait(std::string(expression), settings));
}

void jni_KnowledgeBase_wait_impl_RPPP(void** ret, void* cptr, void* expression, void* waitSettings)
{
	Madara::Knowledge_Engine::Knowledge_Base knowledge = *(Madara::Knowledge_Engine::Knowledge_Base*)cptr;
	Madara::Knowledge_Engine::Compiled_Expression compiled_expression = *(Madara::Knowledge_Engine::Compiled_Expression*)expression;
	Madara::Knowledge_Engine::Wait_Settings settings = *(Madara::Knowledge_Engine::Wait_Settings*)waitSettings;
	*ret = new Madara::Knowledge_Record(knowledge.wait(compiled_expression, settings));
}

void jni_KnowledgeBase_wait_no_return_PSP(void* cptr, const char* expression, void* waitSettings)
{
	Madara::Knowledge_Engine::Knowledge_Base knowledge = *(Madara::Knowledge_Engine::Knowledge_Base*)cptr;
	Madara::Knowledge_Engine::Wait_Settings settings = *(Madara::Knowledge_Engine::Wait_Settings*)waitSettings;
	knowledge.wait(std::string(expression), settings);
}

void jni_KnowledgeBase_wait_no_return_PPP(void* cptr, void* expression, void* waitSettings)
{
	Madara::Knowledge_Engine::Knowledge_Base knowledge = *(Madara::Knowledge_Engine::Knowledge_Base*)cptr;
	Madara::Knowledge_Engine::Compiled_Expression compiled_expression = *(Madara::Knowledge_Engine::Compiled_Expression*)expression;
	Madara::Knowledge_Engine::Wait_Settings settings = *(Madara::Knowledge_Engine::Wait_Settings*)waitSettings;
	knowledge.wait(compiled_expression, settings);
}

void jni_KnowledgeBase_free_knowledge_base_P(void* cptr)
{
	Madara::Knowledge_Engine::Knowledge_Base* knowledge = (Madara::Knowledge_Engine::Knowledge_Base*)cptr;
	if (knowledge)
		delete knowledge;
}

//===================================================================================

//===================================================================================
//CompiledExpression
//===================================================================================
void jni_KnowledgeBase_free_compiled_expression_P(void* cptr)
{
	Madara::Knowledge_Engine::Compiled_Expression* expression = (Madara::Knowledge_Engine::Compiled_Expression*)cptr;
	if (expression)
		delete expression;
}
//===================================================================================



