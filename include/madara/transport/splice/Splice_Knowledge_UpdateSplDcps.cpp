#include "Splice_Knowledge_UpdateSplDcps.h"
#include "ccpp_Splice_Knowledge_Update.h"

char *
__Knowledge_Update__name(void)
{
    return CORBA::string_dup("Knowledge::Update");
}

char *
__Knowledge_Update__keys(void)
{
    return CORBA::string_dup("originator");
}

#include <v_kernel.h>
#include <v_topic.h>
#include <string.h>

c_bool
__Knowledge_Update__copyIn(
    c_base base,
    struct ::Knowledge::Update *from,
    struct _Knowledge_Update *to)
{
    c_bool result = TRUE;

#ifdef OSPL_BOUNDS_CHECK
    if(from->key){
        to->key = c_stringNew(base, from->key);
    } else {
        result = FALSE;
    }
#else
    to->key = c_stringNew(base, from->key);
#endif
    to->value = (c_longlong)from->value;
    to->quality = (c_ulong)from->quality;
#ifdef OSPL_BOUNDS_CHECK
    if(from->originator){
        to->originator = c_stringNew(base, from->originator);
    } else {
        result = FALSE;
    }
#else
    to->originator = c_stringNew(base, from->originator);
#endif
    to->type = (c_ulong)from->type;
    to->clock = (c_ulonglong)from->clock;
    return result;
}

void
__Knowledge_Update__copyOut(
    void *_from,
    void *_to)
{
    struct _Knowledge_Update *from = (struct _Knowledge_Update *)_from;
    struct ::Knowledge::Update *to = (struct ::Knowledge::Update *)_to;
    to->key = CORBA::string_dup(from->key);
    to->value = (::DDS::LongLong)from->value;
    to->quality = (::DDS::ULong)from->quality;
    to->originator = CORBA::string_dup(from->originator);
    to->type = (::DDS::ULong)from->type;
    to->clock = (::DDS::ULongLong)from->clock;
}

