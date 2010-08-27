#ifndef SPLICE_KNOWLEDGE_UPDATESPLTYPES_H
#define SPLICE_KNOWLEDGE_UPDATESPLTYPES_H

#include "ccpp_Splice_Knowledge_Update.h"

#include <c_base.h>
#include <c_misc.h>
#include <c_sync.h>
#include <c_collection.h>
#include <c_field.h>

extern c_metaObject __Splice_Knowledge_Update_Knowledge__load (c_base base);

extern c_metaObject __Knowledge_Update__load (c_base base);
extern char * __Knowledge_Update__keys (void);
extern char * __Knowledge_Update__name (void);
struct _Knowledge_Update ;
extern  c_bool __Knowledge_Update__copyIn(c_base base, struct Knowledge::Update *from, struct _Knowledge_Update *to);
extern  void __Knowledge_Update__copyOut(void *_from, void *_to);
struct _Knowledge_Update {
    c_string key;
    c_long value;
};

#endif
