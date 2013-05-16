/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/
 
#include "com.madara.KnowledgeRecord_Impl.h"
#include "madara/knowledge_engine/Knowledge_Record.h"

void jni_KnowledgeRecord_impl_RS(void** ret, const char * data)
{
	*ret = new Madara::Knowledge_Record(data);
}

void jni_KnowledgeRecord_impl_RD(void** ret, double data)
{
	*ret = new Madara::Knowledge_Record(data);
}

void jni_KnowledgeRecord_impl_RJ(void** ret, long data)
{
	*ret = new Madara::Knowledge_Record(data);
}

long jni_KnowledgeRecord_to_long_value_impl_P(void* cptr)
{
	Madara::Knowledge_Record record = *(Madara::Knowledge_Record*)cptr;
	return record.to_integer();
}

const char* jni_KnowledgeRecord_to_string_value_impl_P(void* cptr)
{
	Madara::Knowledge_Record record = *(Madara::Knowledge_Record*)cptr;
	return record.to_string().c_str();
}

double jni_KnowledgeRecord_to_double_value_impl_P(void* cptr)
{
	Madara::Knowledge_Record record = *(Madara::Knowledge_Record*)cptr;
	return record.to_double();
}

int jni_KnowledgeRecord_get_type_P(void* cptr)
{
	Madara::Knowledge_Record record = *(Madara::Knowledge_Record*)cptr;
	return record.type();
}

void jni_KnowledgeRecord_free_P(void* cptr)
{
	Madara::Knowledge_Record* record = (Madara::Knowledge_Record*)cptr;
	if (record)
		delete record;
}
