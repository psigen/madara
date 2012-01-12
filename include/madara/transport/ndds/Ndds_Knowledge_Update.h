
/*
  WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

  This file was generated from Ndds_Knowledge_Update.idl using "rtiddsgen".
  The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
  For more information, type 'rtiddsgen -help' at a command shell
  or consult the RTI Data Distribution Service manual.
*/

#ifndef Ndds_Knowledge_Update_1088856923_h
#define Ndds_Knowledge_Update_1088856923_h

#ifndef NDDS_STANDALONE_TYPE
    #ifdef __cplusplus
        #ifndef ndds_cpp_h
            #include "ndds/ndds_cpp.h"
        #endif
    #else
        #ifndef ndds_c_h
            #include "ndds/ndds_c.h"
        #endif
    #endif
#else
    #include "ndds_standalone_type.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

        
extern const char *NDDS_Knowledge_UpdateTYPENAME;
        

#ifdef __cplusplus
}
#endif

typedef struct NDDS_Knowledge_Update
{
    char*  key; /* maximum length = (255) */
    DDS_LongLong  value;
    DDS_UnsignedLong  quality;
    char*  originator; /* maximum length = (255) */
    DDS_UnsignedLong  type;
    DDS_UnsignedLongLong  clock;

} NDDS_Knowledge_Update;
    
                            
#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, start exporting symbols.
   */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport __declspec(dllexport)
#endif

    
NDDSUSERDllExport DDS_TypeCode* NDDS_Knowledge_Update_get_typecode(void); /* Type code */
    

DDS_SEQUENCE(NDDS_Knowledge_UpdateSeq, NDDS_Knowledge_Update);
        
NDDSUSERDllExport
RTIBool NDDS_Knowledge_Update_initialize(
        NDDS_Knowledge_Update* self);
        
NDDSUSERDllExport
RTIBool NDDS_Knowledge_Update_initialize_ex(
        NDDS_Knowledge_Update* self,RTIBool allocatePointers);

NDDSUSERDllExport
void NDDS_Knowledge_Update_finalize(
        NDDS_Knowledge_Update* self);
                        
NDDSUSERDllExport
void NDDS_Knowledge_Update_finalize_ex(
        NDDS_Knowledge_Update* self,RTIBool deletePointers);
        
NDDSUSERDllExport
RTIBool NDDS_Knowledge_Update_copy(
        NDDS_Knowledge_Update* dst,
        const NDDS_Knowledge_Update* src);

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, stop exporting symbols.
   */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport
#endif



#endif /* Ndds_Knowledge_Update_1088856923_h */
