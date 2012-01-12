#ifndef SPLICE_KNOWLEDGE_UPDATEDCPS_IMPL_H_
#define SPLICE_KNOWLEDGE_UPDATEDCPS_IMPL_H_

#include "ccpp.h"
#include "ccpp_Splice_Knowledge_Update.h"
#include "ccpp_TypeSupport_impl.h"
#include "ccpp_DataWriter_impl.h"
#include "ccpp_DataReader_impl.h"
#include "ccpp_DataReaderView_impl.h"


namespace Knowledge {

    class  UpdateTypeSupportFactory : public ::DDS::TypeSupportFactory_impl
    {
    public:
        UpdateTypeSupportFactory() {}
        virtual ~UpdateTypeSupportFactory() {}
    private:
        ::DDS::DataWriter_ptr 
        create_datawriter (gapi_dataWriter handle);
    
        ::DDS::DataReader_ptr 
        create_datareader (gapi_dataReader handle);
    
        ::DDS::DataReaderView_ptr 
        create_view (gapi_dataReaderView handle);
    };
    
    class  UpdateTypeSupport : public virtual UpdateTypeSupportInterface,
                                   public ::DDS::TypeSupport_impl
    {
    public:
        virtual ::DDS::ReturnCode_t register_type(
            ::DDS::DomainParticipant_ptr participant,
            const char * type_name) THROW_ORB_EXCEPTIONS;
    
        virtual char * get_type_name() THROW_ORB_EXCEPTIONS;    
    
        UpdateTypeSupport (void);
        virtual ~UpdateTypeSupport (void);
    
    private:
        UpdateTypeSupport (const UpdateTypeSupport &);
        void operator= (const UpdateTypeSupport &);
    
        static const char *metaDescriptor;
    };
    
    typedef UpdateTypeSupportInterface_var UpdateTypeSupport_var;
    typedef UpdateTypeSupportInterface_ptr UpdateTypeSupport_ptr;
    
    class  UpdateDataWriter_impl : public virtual UpdateDataWriter,
                                        public ::DDS::DataWriter_impl
    {
    public:
    
        virtual ::DDS::InstanceHandle_t register_instance(
            const Update & instance_data) THROW_ORB_EXCEPTIONS;
            
        virtual ::DDS::InstanceHandle_t register_instance_w_timestamp(
            const Update & instance_data,
            const ::DDS::Time_t & source_timestamp) THROW_ORB_EXCEPTIONS;
            
        virtual ::DDS::ReturnCode_t unregister_instance(
            const Update & instance_data,
            ::DDS::InstanceHandle_t handle) THROW_ORB_EXCEPTIONS;
    
        virtual ::DDS::ReturnCode_t unregister_instance_w_timestamp(
            const Update & instance_data,
            ::DDS::InstanceHandle_t handle,
            const ::DDS::Time_t & source_timestamp) THROW_ORB_EXCEPTIONS;
    
        virtual ::DDS::ReturnCode_t write(
            const Update & instance_data,
            ::DDS::InstanceHandle_t handle) THROW_ORB_EXCEPTIONS;
    
        virtual ::DDS::ReturnCode_t write_w_timestamp(
            const Update & instance_data,
            ::DDS::InstanceHandle_t handle,
            const ::DDS::Time_t & source_timestamp) THROW_ORB_EXCEPTIONS;
    
        virtual ::DDS::ReturnCode_t dispose(
            const Update & instance_data,
            ::DDS::InstanceHandle_t handle) THROW_ORB_EXCEPTIONS;
    
        virtual ::DDS::ReturnCode_t dispose_w_timestamp(
            const Update & instance_data,
            ::DDS::InstanceHandle_t handle,
            const ::DDS::Time_t & source_timestamp) THROW_ORB_EXCEPTIONS;
    
        virtual ::DDS::ReturnCode_t writedispose(
            const Update & instance_data,
            ::DDS::InstanceHandle_t handle) THROW_ORB_EXCEPTIONS;
    
        virtual ::DDS::ReturnCode_t writedispose_w_timestamp(
            const Update & instance_data,
            ::DDS::InstanceHandle_t handle,
            const ::DDS::Time_t & source_timestamp) THROW_ORB_EXCEPTIONS;
    
        virtual ::DDS::ReturnCode_t get_key_value(
            Update & key_holder,
            ::DDS::InstanceHandle_t handle) THROW_ORB_EXCEPTIONS;
    
        virtual ::DDS::InstanceHandle_t lookup_instance(
            const Update & instance_data) THROW_ORB_EXCEPTIONS;
    
    
        UpdateDataWriter_impl (
            gapi_dataWriter handle
        );
    
        virtual ~UpdateDataWriter_impl (void);
    
    private:
        UpdateDataWriter_impl(const UpdateDataWriter_impl &);
        void operator= (const UpdateDataWriter &);
    };
    
    class  UpdateDataReader_impl : public virtual UpdateDataReader,
                                        public ::DDS::DataReader_impl
    {
        friend class UpdateDataReaderView_impl;
    public:
        virtual ::DDS::ReturnCode_t read(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::SampleStateMask sample_states,
            ::DDS::ViewStateMask view_states,
            ::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::ReturnCode_t take(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::SampleStateMask sample_states,
            ::DDS::ViewStateMask view_states,
            ::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::ReturnCode_t read_w_condition(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::ReadCondition_ptr a_condition) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::ReturnCode_t take_w_condition(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::ReadCondition_ptr a_condition) THROW_ORB_EXCEPTIONS;
    
        virtual ::DDS::ReturnCode_t read_next_sample(
            Update & received_data,
            ::DDS::SampleInfo & sample_info) THROW_ORB_EXCEPTIONS;
    
        virtual ::DDS::ReturnCode_t take_next_sample(
            Update & received_data,
            ::DDS::SampleInfo & sample_info) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::ReturnCode_t read_instance(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::InstanceHandle_t a_handle,
            ::DDS::SampleStateMask sample_states,
            ::DDS::ViewStateMask view_states,
            ::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::ReturnCode_t take_instance(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::InstanceHandle_t a_handle,
            ::DDS::SampleStateMask sample_states,
            ::DDS::ViewStateMask view_states,
            ::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::ReturnCode_t read_next_instance(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::InstanceHandle_t a_handle,
            ::DDS::SampleStateMask sample_states,
            ::DDS::ViewStateMask view_states,
            ::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::ReturnCode_t take_next_instance(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::InstanceHandle_t a_handle,
            ::DDS::SampleStateMask sample_states,
            ::DDS::ViewStateMask view_states,
            ::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::ReturnCode_t read_next_instance_w_condition(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::InstanceHandle_t a_handle,
            ::DDS::ReadCondition_ptr a_condition) THROW_ORB_EXCEPTIONS;
    
        virtual ::DDS::ReturnCode_t take_next_instance_w_condition(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::InstanceHandle_t a_handle,
            ::DDS::ReadCondition_ptr a_condition) THROW_ORB_EXCEPTIONS;
    
        virtual ::DDS::ReturnCode_t return_loan(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::ReturnCode_t get_key_value(
            Update & key_holder,
            ::DDS::InstanceHandle_t handle) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::InstanceHandle_t lookup_instance(
            const Update & instance) THROW_ORB_EXCEPTIONS;
    
        UpdateDataReader_impl (
            gapi_dataReader handle
        );
    
        virtual ~UpdateDataReader_impl(void);
    
    private:
        UpdateDataReader_impl(const UpdateDataReader &);
        void operator= (const UpdateDataReader &);
    
        static ::DDS::ReturnCode_t check_preconditions(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples
        );
    };
    
    class  UpdateDataReaderView_impl : public virtual UpdateDataReaderView,
                                        public ::DDS::DataReaderView_impl
    {
    public:
        virtual ::DDS::ReturnCode_t read(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::SampleStateMask sample_states,
            ::DDS::ViewStateMask view_states,
            ::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::ReturnCode_t take(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::SampleStateMask sample_states,
            ::DDS::ViewStateMask view_states,
            ::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::ReturnCode_t read_w_condition(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::ReadCondition_ptr a_condition) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::ReturnCode_t take_w_condition(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::ReadCondition_ptr a_condition) THROW_ORB_EXCEPTIONS;
    
        virtual ::DDS::ReturnCode_t read_next_sample(
            Update & received_data,
            ::DDS::SampleInfo & sample_info) THROW_ORB_EXCEPTIONS;
    
        virtual ::DDS::ReturnCode_t take_next_sample(
            Update & received_data,
            ::DDS::SampleInfo & sample_info) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::ReturnCode_t read_instance(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::InstanceHandle_t a_handle,
            ::DDS::SampleStateMask sample_states,
            ::DDS::ViewStateMask view_states,
            ::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::ReturnCode_t take_instance(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::InstanceHandle_t a_handle,
            ::DDS::SampleStateMask sample_states,
            ::DDS::ViewStateMask view_states,
            ::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::ReturnCode_t read_next_instance(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::InstanceHandle_t a_handle,
            ::DDS::SampleStateMask sample_states,
            ::DDS::ViewStateMask view_states,
            ::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::ReturnCode_t take_next_instance(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::InstanceHandle_t a_handle,
            ::DDS::SampleStateMask sample_states,
            ::DDS::ViewStateMask view_states,
            ::DDS::InstanceStateMask instance_states) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::ReturnCode_t read_next_instance_w_condition(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::InstanceHandle_t a_handle,
            ::DDS::ReadCondition_ptr a_condition) THROW_ORB_EXCEPTIONS;
    
        virtual ::DDS::ReturnCode_t take_next_instance_w_condition(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq,
            CORBA::Long max_samples,
            ::DDS::InstanceHandle_t a_handle,
            ::DDS::ReadCondition_ptr a_condition) THROW_ORB_EXCEPTIONS;
    
        virtual ::DDS::ReturnCode_t return_loan(
            UpdateSeq & received_data,
            ::DDS::SampleInfoSeq & info_seq) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::ReturnCode_t get_key_value(
            Update & key_holder,
            ::DDS::InstanceHandle_t handle) THROW_ORB_EXCEPTIONS;
        
        virtual ::DDS::InstanceHandle_t lookup_instance(
            const Update & instance) THROW_ORB_EXCEPTIONS;
    
        UpdateDataReaderView_impl (
            gapi_dataReader handle
        );
    
        virtual ~UpdateDataReaderView_impl(void);
    
    private:
        UpdateDataReaderView_impl(const UpdateDataReaderView &);
        void operator= (const UpdateDataReaderView &);
    };
    
}

#endif
