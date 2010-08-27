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
    return CORBA::string_dup("key");
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
    to->value = (c_long)from->value;
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
    to->value = (::DDS::Long)from->value;
}

