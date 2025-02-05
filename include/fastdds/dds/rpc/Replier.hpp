// Copyright 2025 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef FASTDDS_DDS_RPC__REPLIER_HPP
#define FASTDDS_DDS_RPC__REPLIER_HPP

#include <fastdds/dds/core/detail/DDSReturnCode.hpp>
#include <fastdds/dds/core/LoanableCollection.hpp>
#include <fastdds/dds/core/LoanableSequence.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/qos/ReplierQos.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>

#include "ReplierParams.hpp"
#include "RequestInfo.hpp"
#include "RPCEntity.hpp"
#include "Service.hpp"

namespace eprosima {
namespace fastdds {
namespace dds{
namespace rpc {

class Service;

/**
 * @brief Class that represents a replier entity in the RPC communication.
 */

// TODO: Due to we will not use callbacks in the replier base implementation, is it better to avoid inheritance from DataWriterListener and DataReaderListener?
// If we remove it, a different way to handle the DDS entities is needed, because they are created on Replier/Requester constructors using this as a pointer to the listener.
class Replier : public RPCEntity, public DataWriterListener, public DataReaderListener
{

friend class Service;

protected:
    /**
     * @brief Constructor
     * Don't instantiate this class directly. Use create_service_replier from DomainParticipant instead.
     */
    Replier(
            Service* service,
            const ReplierParams& params);
    
    /**
     * @brief Create required DDS entities to enable communication with the requester
     * 
     * @param params Replier parameters to configure the DDS entities
     * 
     * @return RETCODE_OK if all DDS entities were created successfully, RETCODE_ERROR otherwise
     */
    virtual ReturnCode_t create_dds_entities(const ReplierParams& params);

public:
    /**
     * @brief Destructor
     */
    virtual ~Replier();

    /**
     * @brief Send a reply message to a requester
     * 
     * @param data Data to send
     * @param info Information about the reply sample. This information is used to match the reply with the request through the SampleIdentity
     * @return RETCODE_OK if the reply was sent successfully or a ReturnCode related to the specific error otherwise
     */
    virtual ReturnCode_t send_reply(
            void* data,
            RequestInfo& info);
    
    /**
     * @brief Take a request message from the Replier DataReader's history.
     * 
     * @param data Data to receive the request
     * @param info Information about the request sample
     * @return RETCODE_OK if the request was taken successfully or a ReturnCode related to the specific error otherwise
     */
    virtual ReturnCode_t take_request(
            void* data,
            SampleInfo& info);

    /**
     * @brief Take all request messages stored in the Replier DataReader's history.
     * @note This method does not allow to take only the samples associated to a given request. User must implement a zero-copy solution to link request and reply samples.
     * 
     * @param data Data to receive the request
     * @param info Information about the request sample
     * @return RETCODE_OK if the request was taken successfully or a ReturnCode related to the specific error otherwise
     */
    virtual ReturnCode_t take_request(
            LoanableCollection& data,
            LoanableSequence<SampleInfo>& info);

    /**
     * @brief Check if the replier is valid (i.e: all DDS entities are correctly created)
     */
    inline bool is_valid() const
    {
        return valid_;
    }

    // Getters for all DDS entities
    inline void get_replier_writer(DataWriter*& writer) const
    {
        writer = replier_writer_;
    }

    inline void get_replier_reader(DataReader*& reader) const
    {
        reader = replier_reader_;
    }

    inline void get_replier_publisher(Publisher*& publisher) const
    {
        publisher = replier_publisher_;
    }

    inline void get_replier_subscriber(Subscriber*& subscriber) const
    {
        subscriber = replier_subscriber_;
    }

    // Getter for associated service
    inline void get_service(Service*& service) const
    {
        service = service_;
    }

    // DataWriter listener callbacks
    virtual void on_publication_matched(
            DataWriter* writer,
            const PublicationMatchedStatus& info) override
    {
        static_cast<void>(writer);
        static_cast<void>(info);
    }

    virtual void on_offered_deadline_missed(
            DataWriter* writer,
            const OfferedDeadlineMissedStatus& status) override
    {
        static_cast<void>(writer);
        static_cast<void>(status);
    }

    virtual void on_offered_incompatible_qos(
            DataWriter* writer,
            const OfferedIncompatibleQosStatus& status) override
    {
        static_cast<void>(writer);
        static_cast<void>(status);
    }

    virtual void on_liveliness_lost(
            DataWriter* writer,
            const LivelinessLostStatus& status) override
    {
        static_cast<void>(writer);
        static_cast<void>(status);
    }

    virtual void on_unacknowledged_sample_removed(
            DataWriter* writer,
            const InstanceHandle_t& instance) override
    {
        static_cast<void>(writer);
        static_cast<void>(instance);
    }

    // DataReader listener callbacks
    virtual void on_data_available(
            DataReader* reader) override
    {
        static_cast<void>(reader);
    }

    virtual void on_subscription_matched(
            DataReader* reader,
            const fastdds::dds::SubscriptionMatchedStatus& info) override
    {
        static_cast<void>(reader);
        static_cast<void>(info);
    }

    virtual void on_requested_deadline_missed(
            DataReader* reader,
            const RequestedDeadlineMissedStatus& status) override
    {
        static_cast<void>(reader);
        static_cast<void>(status);
    }

    virtual void on_liveliness_changed(
            DataReader* reader,
            const LivelinessChangedStatus& status) override
    {
        static_cast<void>(reader);
        static_cast<void>(status);
    }

    virtual void on_sample_rejected(
            DataReader* reader,
            const SampleRejectedStatus& status) override
    {
        static_cast<void>(reader);
        static_cast<void>(status);
    }

    virtual void on_requested_incompatible_qos(
            DataReader* reader,
            const RequestedIncompatibleQosStatus& status) override
    {
        static_cast<void>(reader);
        static_cast<void>(status);
    }

    virtual void on_sample_lost(
            DataReader* reader,
            const SampleLostStatus& status) override
    {
        static_cast<void>(reader);
        static_cast<void>(status);
    }

protected:
    DataReader* replier_reader_;
    DataWriter* replier_writer_;
    Publisher* replier_publisher_;
    Subscriber* replier_subscriber_;
    ReplierQos qos_;
    Service* service_;
    bool valid_;
};

} // namespace rpc
} // namespace dds
} // namespace fastdds
} // namespace eprosima

#endif // FASTDDS_DDS_RPC__REPLIER_HPP