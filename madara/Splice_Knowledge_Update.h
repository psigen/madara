//******************************************************************
// 
//  Generated by IDL to C++ Translator
//  
//  File name: Splice_Knowledge_Update.h
//  Source: Splice_Knowledge_Update.idl
//  Generated: Fri Aug 27 00:06:36 2010
//  OpenSplice V5.2.0
//  
//******************************************************************
#ifndef _SPLICE_KNOWLEDGE_UPDATE_H_
#define _SPLICE_KNOWLEDGE_UPDATE_H_


#include "sacpp_mapping.h"
#include "sacpp_DDS_DCPS.h"

namespace Knowledge
{
   struct Update;

   struct Update
   {
      DDS::String_mgr key;
      DDS::Long value;
   };

   typedef DDS_DCPSStruct_var < Update> Update_var;
   typedef DDS_DCPSStruct_out < Update> Update_out;
}




#endif 
