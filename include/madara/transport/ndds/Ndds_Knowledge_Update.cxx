
/*
  WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

  This file was generated from Ndds_Knowledge_Update.idl using "rtiddsgen".
  The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
  For more information, type 'rtiddsgen -help' at a command shell
  or consult the RTI Data Distribution Service manual.
*/


#ifndef NDDS_STANDALONE_TYPE
    #ifdef __cplusplus
        #ifndef ndds_cpp_h
            #include "ndds/ndds_cpp.h"
        #endif
        #ifndef dds_c_log_impl_h              
            #include "dds_c/dds_c_log_impl.h"                                
        #endif        
    #else
        #ifndef ndds_c_h
            #include "ndds/ndds_c.h"
        #endif
    #endif
    
    #ifndef cdr_type_h
        #include "cdr/cdr_type.h"
    #endif    

    #ifndef osapi_heap_h
        #include "osapi/osapi_heap.h" 
    #endif
#else
    #include "ndds_standalone_type.h"
#endif



#include "Ndds_Knowledge_Update.h"

/* ========================================================================= */
const char *NDDS_Knowledge_UpdateTYPENAME = "NDDS::Knowledge::Update";

DDS_TypeCode* NDDS_Knowledge_Update_get_typecode()
{
    static RTIBool is_initialized = RTI_FALSE;

    static DDS_TypeCode NDDS_Knowledge_Update_g_tc_key_string = DDS_INITIALIZE_STRING_TYPECODE(255);
    static DDS_TypeCode NDDS_Knowledge_Update_g_tc_originator_string = DDS_INITIALIZE_STRING_TYPECODE(255);

    static DDS_TypeCode_Member NDDS_Knowledge_Update_g_tc_members[6]=
    {
        {
            (char *)"key",/* Member name */
            {
                0,/* Representation ID */
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            DDS_BOOLEAN_FALSE, /* Is a key? */
            DDS_PRIVATE_MEMBER,/* Ignored */
            0,/* Ignored */
            NULL/* Ignored */
        },
        {
            (char *)"value",/* Member name */
            {
                0,/* Representation ID */
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            DDS_BOOLEAN_FALSE, /* Is a key? */
            DDS_PRIVATE_MEMBER,/* Ignored */
            0,/* Ignored */
            NULL/* Ignored */
        },
        {
            (char *)"quality",/* Member name */
            {
                0,/* Representation ID */
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            DDS_BOOLEAN_FALSE, /* Is a key? */
            DDS_PRIVATE_MEMBER,/* Ignored */
            0,/* Ignored */
            NULL/* Ignored */
        },
        {
            (char *)"originator",/* Member name */
            {
                0,/* Representation ID */
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            DDS_BOOLEAN_TRUE, /* Is a key? */
            DDS_PRIVATE_MEMBER,/* Ignored */
            0,/* Ignored */
            NULL/* Ignored */
        },
        {
            (char *)"type",/* Member name */
            {
                0,/* Representation ID */
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            DDS_BOOLEAN_FALSE, /* Is a key? */
            DDS_PRIVATE_MEMBER,/* Ignored */
            0,/* Ignored */
            NULL/* Ignored */
        },
        {
            (char *)"clock",/* Member name */
            {
                0,/* Representation ID */
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            DDS_BOOLEAN_FALSE, /* Is a key? */
            DDS_PRIVATE_MEMBER,/* Ignored */
            0,/* Ignored */
            NULL/* Ignored */
        }
    };

    static DDS_TypeCode NDDS_Knowledge_Update_g_tc =
    {{
        DDS_TK_STRUCT,/* Kind */
        DDS_BOOLEAN_FALSE, /* Ignored */
        -1,/* Ignored */
        (char *)"NDDS::Knowledge::Update", /* Name */
        NULL, /* Ignored */
        0, /* Ignored */
        0, /* Ignored */
        NULL, /* Ignored */
        6, /* Number of members */
        NDDS_Knowledge_Update_g_tc_members, /* Members */
        DDS_VM_NONE /* Ignored */
    }}; /* Type code for NDDS_Knowledge_Update*/

    if (is_initialized) {
        return &NDDS_Knowledge_Update_g_tc;
    }


    NDDS_Knowledge_Update_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&NDDS_Knowledge_Update_g_tc_key_string;
    NDDS_Knowledge_Update_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_longlong;
    NDDS_Knowledge_Update_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ulong;
    NDDS_Knowledge_Update_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&NDDS_Knowledge_Update_g_tc_originator_string;
    NDDS_Knowledge_Update_g_tc_members[4]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ulong;
    NDDS_Knowledge_Update_g_tc_members[5]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_ulonglong;

    is_initialized = RTI_TRUE;

    return &NDDS_Knowledge_Update_g_tc;
}


RTIBool NDDS_Knowledge_Update_initialize(
    NDDS_Knowledge_Update* sample) {
  return NDDS_Knowledge_Update_initialize_ex(sample,RTI_TRUE);
}
        
RTIBool NDDS_Knowledge_Update_initialize_ex(
    NDDS_Knowledge_Update* sample,RTIBool allocatePointers)
{

    sample->key = DDS_String_alloc((255));
    if (sample->key == NULL) {
        return RTI_FALSE;
    }
            
    if (!RTICdrType_initLongLong(&sample->value)) {
        return RTI_FALSE;
    }                
            
    if (!RTICdrType_initUnsignedLong(&sample->quality)) {
        return RTI_FALSE;
    }                
            
    sample->originator = DDS_String_alloc((255));
    if (sample->originator == NULL) {
        return RTI_FALSE;
    }
            
    if (!RTICdrType_initUnsignedLong(&sample->type)) {
        return RTI_FALSE;
    }                
            
    if (!RTICdrType_initUnsignedLongLong(&sample->clock)) {
        return RTI_FALSE;
    }                
            

    return RTI_TRUE;
}

void NDDS_Knowledge_Update_finalize(
    NDDS_Knowledge_Update* sample)
{
    NDDS_Knowledge_Update_finalize_ex(sample,RTI_TRUE);
}
        
void NDDS_Knowledge_Update_finalize_ex(
    NDDS_Knowledge_Update* sample,RTIBool deletePointers)
{        

    DDS_String_free(sample->key);                
            
    DDS_String_free(sample->originator);                
            
}

RTIBool NDDS_Knowledge_Update_copy(
    NDDS_Knowledge_Update* dst,
    const NDDS_Knowledge_Update* src)
{        

    if (!RTICdrType_copyString(
        dst->key, src->key, (255) + 1)) {
        return RTI_FALSE;
    }
            
    if (!RTICdrType_copyLongLong(
        &dst->value, &src->value)) {
        return RTI_FALSE;
    }
            
    if (!RTICdrType_copyUnsignedLong(
        &dst->quality, &src->quality)) {
        return RTI_FALSE;
    }
            
    if (!RTICdrType_copyString(
        dst->originator, src->originator, (255) + 1)) {
        return RTI_FALSE;
    }
            
    if (!RTICdrType_copyUnsignedLong(
        &dst->type, &src->type)) {
        return RTI_FALSE;
    }
            
    if (!RTICdrType_copyUnsignedLongLong(
        &dst->clock, &src->clock)) {
        return RTI_FALSE;
    }
            

    return RTI_TRUE;
}


/**
 * <<IMPLEMENTATION>>
 *
 * Defines:  TSeq, T
 *
 * Configure and implement 'NDDS_Knowledge_Update' sequence class.
 */
#define T NDDS_Knowledge_Update
#define TSeq NDDS_Knowledge_UpdateSeq
#define T_initialize_ex NDDS_Knowledge_Update_initialize_ex
#define T_finalize_ex   NDDS_Knowledge_Update_finalize_ex
#define T_copy       NDDS_Knowledge_Update_copy

#ifndef NDDS_STANDALONE_TYPE
#include "dds_c/generic/dds_c_sequence_TSeq.gen"
#ifdef __cplusplus
#include "dds_cpp/generic/dds_cpp_sequence_TSeq.gen"
#endif
#else
#include "dds_c_sequence_TSeq.gen"
#ifdef __cplusplus
#include "dds_cpp_sequence_TSeq.gen"
#endif
#endif

#undef T_copy
#undef T_finalize_ex
#undef T_initialize_ex
#undef TSeq
#undef T

