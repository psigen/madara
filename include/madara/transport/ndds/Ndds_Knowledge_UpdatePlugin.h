
/*
  WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

  This file was generated from Ndds_Knowledge_Update.idl using "rtiddsgen".
  The rtiddsgen tool is part of the RTI Data Distribution Service distribution.
  For more information, type 'rtiddsgen -help' at a command shell
  or consult the RTI Data Distribution Service manual.
*/

#ifndef Ndds_Knowledge_UpdatePlugin_1088856923_h
#define Ndds_Knowledge_UpdatePlugin_1088856923_h

#include "Ndds_Knowledge_Update.h"




struct RTICdrStream;

#ifndef pres_typePlugin_h
#include "pres/pres_typePlugin.h"
#endif


#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* The type used to store keys for instances of type struct
 * NDDS_Knowledge_Update.
 *
 * By default, this type is struct NDDS_Knowledge_Update
 * itself. However, if for some reason this choice is not practical for your
 * system (e.g. if sizeof(struct NDDS_Knowledge_Update)
 * is very large), you may redefine this typedef in terms of another type of
 * your choosing. HOWEVER, if you define the KeyHolder type to be something
 * other than struct NDDS_Knowledge_Update, the
 * following restriction applies: the key of struct
 * NDDS_Knowledge_Update must consist of a
 * single field of your redefined KeyHolder type and that field must be the
 * first field in struct NDDS_Knowledge_Update.
*/
typedef  struct NDDS_Knowledge_Update NDDS_Knowledge_UpdateKeyHolder;


#define NDDS_Knowledge_UpdatePlugin_get_sample PRESTypePluginDefaultEndpointData_getSample 
#define NDDS_Knowledge_UpdatePlugin_return_sample PRESTypePluginDefaultEndpointData_returnSample 
#define NDDS_Knowledge_UpdatePlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define NDDS_Knowledge_UpdatePlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer 

#define NDDS_Knowledge_UpdatePlugin_get_key PRESTypePluginDefaultEndpointData_getKey 
#define NDDS_Knowledge_UpdatePlugin_return_key PRESTypePluginDefaultEndpointData_returnKey
 

#define NDDS_Knowledge_UpdatePlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define NDDS_Knowledge_UpdatePlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
    Support functions:
 * -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern NDDS_Knowledge_Update*
NDDS_Knowledge_UpdatePluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern NDDS_Knowledge_Update*
NDDS_Knowledge_UpdatePluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
NDDS_Knowledge_UpdatePluginSupport_copy_data(
    NDDS_Knowledge_Update *out,
    const NDDS_Knowledge_Update *in);

NDDSUSERDllExport extern void 
NDDS_Knowledge_UpdatePluginSupport_destroy_data_ex(
    NDDS_Knowledge_Update *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
NDDS_Knowledge_UpdatePluginSupport_destroy_data(
    NDDS_Knowledge_Update *sample);

NDDSUSERDllExport extern void 
NDDS_Knowledge_UpdatePluginSupport_print_data(
    const NDDS_Knowledge_Update *sample,
    const char *desc,
    unsigned int indent);


NDDSUSERDllExport extern NDDS_Knowledge_Update*
NDDS_Knowledge_UpdatePluginSupport_create_key_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern NDDS_Knowledge_Update*
NDDS_Knowledge_UpdatePluginSupport_create_key(void);

NDDSUSERDllExport extern void 
NDDS_Knowledge_UpdatePluginSupport_destroy_key_ex(
    NDDS_Knowledge_UpdateKeyHolder *key,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
NDDS_Knowledge_UpdatePluginSupport_destroy_key(
    NDDS_Knowledge_UpdateKeyHolder *key);
 

/* ----------------------------------------------------------------------------
    Callback functions:
 * ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
NDDS_Knowledge_UpdatePlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
NDDS_Knowledge_UpdatePlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);
    
NDDSUSERDllExport extern PRESTypePluginEndpointData 
NDDS_Knowledge_UpdatePlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
NDDS_Knowledge_UpdatePlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern RTIBool 
NDDS_Knowledge_UpdatePlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    NDDS_Knowledge_Update *out,
    const NDDS_Knowledge_Update *in);

/* --------------------------------------------------------------------------------------
    (De)Serialize functions:
 * -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool 
NDDS_Knowledge_UpdatePlugin_serialize(
    PRESTypePluginEndpointData endpoint_data,
    const NDDS_Knowledge_Update *sample,
    struct RTICdrStream *stream, 
    RTIBool serialize_encapsulation,
    RTIEncapsulationId encapsulation_id,
    RTIBool serialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool 
NDDS_Knowledge_UpdatePlugin_deserialize_sample(
    PRESTypePluginEndpointData endpoint_data,
    NDDS_Knowledge_Update *sample, 
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

 
NDDSUSERDllExport extern RTIBool 
NDDS_Knowledge_UpdatePlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    NDDS_Knowledge_Update **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);



NDDSUSERDllExport extern RTIBool
NDDS_Knowledge_UpdatePlugin_skip(
    PRESTypePluginEndpointData endpoint_data,
    struct RTICdrStream *stream, 
    RTIBool skip_encapsulation,  
    RTIBool skip_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern unsigned int 
NDDS_Knowledge_UpdatePlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int size);

NDDSUSERDllExport extern unsigned int 
NDDS_Knowledge_UpdatePlugin_get_serialized_sample_min_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int size);

NDDSUSERDllExport extern unsigned int
NDDS_Knowledge_UpdatePlugin_get_serialized_sample_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment,
    const NDDS_Knowledge_Update * sample);


/* --------------------------------------------------------------------------------------
    Key Management functions:
 * -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginKeyKind 
NDDS_Knowledge_UpdatePlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
NDDS_Knowledge_UpdatePlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
NDDS_Knowledge_UpdatePlugin_serialize_key(
    PRESTypePluginEndpointData endpoint_data,
    const NDDS_Knowledge_Update *sample,
    struct RTICdrStream *stream,
    RTIBool serialize_encapsulation,
    RTIEncapsulationId encapsulation_id,
    RTIBool serialize_key,
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool 
NDDS_Knowledge_UpdatePlugin_deserialize_key_sample(
    PRESTypePluginEndpointData endpoint_data,
    NDDS_Knowledge_Update * sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

 
NDDSUSERDllExport extern RTIBool 
NDDS_Knowledge_UpdatePlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    NDDS_Knowledge_Update ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);


NDDSUSERDllExport extern RTIBool
NDDS_Knowledge_UpdatePlugin_serialized_sample_to_key(
    PRESTypePluginEndpointData endpoint_data,
    NDDS_Knowledge_Update *sample,
    struct RTICdrStream *stream, 
    RTIBool deserialize_encapsulation,  
    RTIBool deserialize_key, 
    void *endpoint_plugin_qos);

 
NDDSUSERDllExport extern RTIBool 
NDDS_Knowledge_UpdatePlugin_instance_to_key(
    PRESTypePluginEndpointData endpoint_data,
    NDDS_Knowledge_UpdateKeyHolder *key, 
    const NDDS_Knowledge_Update *instance);

NDDSUSERDllExport extern RTIBool 
NDDS_Knowledge_UpdatePlugin_key_to_instance(
    PRESTypePluginEndpointData endpoint_data,
    NDDS_Knowledge_Update *instance, 
    const NDDS_Knowledge_UpdateKeyHolder *key);

NDDSUSERDllExport extern RTIBool 
NDDS_Knowledge_UpdatePlugin_instance_to_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    DDS_KeyHash_t *keyhash,
    const NDDS_Knowledge_Update *instance);

NDDSUSERDllExport extern RTIBool 
NDDS_Knowledge_UpdatePlugin_serialized_sample_to_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    struct RTICdrStream *stream, 
    DDS_KeyHash_t *keyhash,
    RTIBool deserialize_encapsulation,
    void *endpoint_plugin_qos); 
     
/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
NDDS_Knowledge_UpdatePlugin_new(void);

NDDSUSERDllExport extern void
NDDS_Knowledge_UpdatePlugin_delete(struct PRESTypePlugin *);
 

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif


#ifdef __cplusplus
}
#endif
        

#endif /* Ndds_Knowledge_UpdatePlugin_1088856923_h */
