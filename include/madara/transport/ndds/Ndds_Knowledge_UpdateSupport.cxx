
/*
  WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

  This file was generated from Ndds_Knowledge_Update.idl using "rtiddsgen".
  The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
  For more information, type 'rtiddsgen -help' at a command shell
  or consult the RTI Data Distribution Service manual.
*/

#include "Ndds_Knowledge_UpdateSupport.h"
#include "Ndds_Knowledge_UpdatePlugin.h"



#ifdef __cplusplus  
    #ifndef dds_c_log_impl_h              
         #include "dds_c/dds_c_log_impl.h"                                
    #endif        
#endif        




/* ========================================================================= */
/**
   <<IMPLEMENTATION>>

   Defines:   TData,
              TDataWriter,
              TDataReader,
              TTypeSupport

   Configure and implement 'NDDS_Knowledge_Update' support classes.

   Note: Only the #defined classes get defined
*/

/* ----------------------------------------------------------------- */
/* DDSDataWriter
*/

/**
  <<IMPLEMENTATION >>

   Defines:   TDataWriter, TData
*/

/* Requires */
#define TTYPENAME   NDDS_Knowledge_UpdateTYPENAME

/* Defines */
#define TDataWriter NDDS_Knowledge_UpdateDataWriter
#define TData       NDDS_Knowledge_Update


#ifdef __cplusplus
#include "dds_cpp/generic/dds_cpp_data_TDataWriter.gen"
#else
#include "dds_c/generic/dds_c_data_TDataWriter.gen"
#endif


#undef TDataWriter
#undef TData

#undef TTYPENAME

/* ----------------------------------------------------------------- */
/* DDSDataReader
*/

/**
  <<IMPLEMENTATION >>

   Defines:   TDataReader, TDataSeq, TData
*/

/* Requires */
#define TTYPENAME   NDDS_Knowledge_UpdateTYPENAME

/* Defines */
#define TDataReader NDDS_Knowledge_UpdateDataReader
#define TDataSeq    NDDS_Knowledge_UpdateSeq
#define TData       NDDS_Knowledge_Update


#ifdef __cplusplus
#include "dds_cpp/generic/dds_cpp_data_TDataReader.gen"
#else
#include "dds_c/generic/dds_c_data_TDataReader.gen"
#endif


#undef TDataReader
#undef TDataSeq
#undef TData

#undef TTYPENAME

/* ----------------------------------------------------------------- */
/* TypeSupport

  <<IMPLEMENTATION >>

   Requires:  TTYPENAME,
              TPlugin_new
              TPlugin_delete
   Defines:   TTypeSupport, TData, TDataReader, TDataWriter
*/

/* Requires */
#define TTYPENAME    NDDS_Knowledge_UpdateTYPENAME
#define TPlugin_new  NDDS_Knowledge_UpdatePlugin_new
#define TPlugin_delete  NDDS_Knowledge_UpdatePlugin_delete

/* Defines */
#define TTypeSupport NDDS_Knowledge_UpdateTypeSupport
#define TData        NDDS_Knowledge_Update
#define TDataReader  NDDS_Knowledge_UpdateDataReader
#define TDataWriter  NDDS_Knowledge_UpdateDataWriter
#ifdef __cplusplus



#include "dds_cpp/generic/dds_cpp_data_TTypeSupport.gen"



#else
#include "dds_c/generic/dds_c_data_TTypeSupport.gen"
#endif
#undef TTypeSupport
#undef TData
#undef TDataReader
#undef TDataWriter

#undef TTYPENAME
#undef TPlugin_new
#undef TPlugin_delete


