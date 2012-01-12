
/*
  WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

  This file was generated from Ndds_Knowledge_Update.idl using "rtiddsgen".
  The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
  For more information, type 'rtiddsgen -help' at a command shell
  or consult the RTI Data Distribution Service manual.
*/

#ifndef Ndds_Knowledge_UpdateSupport_1088856923_h
#define Ndds_Knowledge_UpdateSupport_1088856923_h

/* Uses */
#include "Ndds_Knowledge_Update.h"



#ifdef __cplusplus
#ifndef ndds_cpp_h
  #include "ndds/ndds_cpp.h"
#endif
#else
#ifndef ndds_c_h
  #include "ndds/ndds_c.h"
#endif
#endif

        

/* ========================================================================= */
/**
   Uses:     T

   Defines:  TTypeSupport, TDataWriter, TDataReader

   Organized using the well-documented "Generics Pattern" for
   implementing generics in C and C++.
*/

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, start exporting symbols.
   */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport __declspec(dllexport)

#ifdef __cplusplus
  /* If we're building on Windows, explicitly import the superclasses of
   * the types declared below.
   */        
  class __declspec(dllimport) DDSTypeSupport;
  class __declspec(dllimport) DDSDataWriter;
  class __declspec(dllimport) DDSDataReader;
#endif

#endif

#ifdef __cplusplus

DDS_TYPESUPPORT_CPP(NDDS_Knowledge_UpdateTypeSupport, NDDS_Knowledge_Update);

DDS_DATAWRITER_CPP(NDDS_Knowledge_UpdateDataWriter, NDDS_Knowledge_Update);
DDS_DATAREADER_CPP(NDDS_Knowledge_UpdateDataReader, NDDS_Knowledge_UpdateSeq, NDDS_Knowledge_Update);


#else

DDS_TYPESUPPORT_C(NDDS_Knowledge_UpdateTypeSupport, NDDS_Knowledge_Update);
DDS_DATAWRITER_C(NDDS_Knowledge_UpdateDataWriter, NDDS_Knowledge_Update);
DDS_DATAREADER_C(NDDS_Knowledge_UpdateDataReader, NDDS_Knowledge_UpdateSeq, NDDS_Knowledge_Update);

#endif

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, stop exporting symbols.
   */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport
#endif



#endif  /* Ndds_Knowledge_UpdateSupport_1088856923_h */
