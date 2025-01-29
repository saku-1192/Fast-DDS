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

#ifndef FASTDDS_DDS_RPC__SERVICE_HPP
#define FASTDDS_DDS_RPC__SERVICE_HPP

#include <fastdds/dds/topic/ContentFilteredTopic.hpp>
#include <fastdds/dds/topic/Topic.hpp>

#include <mutex>

#include "Replier.hpp"
#include "ReplierParams.hpp"
#include "Requester.hpp"
#include "RequesterParams.hpp"
#include "RPCEntity.hpp"

namespace eprosima {
namespace fastdds {
namespace dds {
class DomainParticipantImpl;
} // namespace dds
namespace dds {
namespace rpc {

class Requester;
class Replier;
class RequesterParams;

/**
 * @brief Class that represents a service in the RPC communication.
 */
class Service : public RPCEntity
{

friend class Requester;
friend class Replier;

protected:

    /**
     * @brief Template methods to validate parameters with a lambda
     */
    template <typename T>
    bool validate_params(
            const T& params,
            const std::function<bool(const T&)>& validator)
    {
        return validator(params);
    }

    /**
     * @brief Validate the requester/replier parameters. They should be consistent with the service configuration
     * 
     * @param params Requester/replier parameters to validate
     * @return True if the parameters are valid, false otherwise
     */
    template <typename T>
    bool validate_params(const T& params)
    {
        // TODO: This is so ugly. Maybe do a list of validators?
        std::function<bool(const T&)> validate_qos = [this](const T& params)
        {
            bool valid = true;
            const auto& qos = params.qos();
            
            if (qos.service_name != service_name_)
            {
                EPROSIMA_LOG_ERROR(SERVICE, "Service name in QoS does not match the service name");
                valid = false;
            }

            if (qos.request_type != request_topic_->get_type_name())
            {
                EPROSIMA_LOG_ERROR(SERVICE, "Request type in QoS does not match the service type name");
                valid = false;
            }

            if (qos.reply_type != reply_topic_->get_type_name())
            {
                EPROSIMA_LOG_ERROR(SERVICE, "Reply type in QoS does not match the service type name");
                valid = false;
            }
            
            if (qos.request_topic_name != request_topic_->get_name())
            {
                EPROSIMA_LOG_ERROR(SERVICE, "Request topic name in QoS does not match the request topic name");
                valid = false;
            }

            if (qos.reply_topic_name != reply_topic_->get_name())
            {
                EPROSIMA_LOG_ERROR(SERVICE, "Reply topic name in QoS does not match the reply topic name");
                valid = false;
            }

            if (qos.writer_qos.reliability().kind != RELIABLE_RELIABILITY_QOS)
            {
                EPROSIMA_LOG_ERROR(SERVICE, "Writer QoS reliability must be RELIABLE_RELIABILITY_QOS");
                valid = false;
            }

            if (qos.reader_qos.reliability().kind != RELIABLE_RELIABILITY_QOS)
            {
                EPROSIMA_LOG_ERROR(SERVICE, "Reader QoS reliability must be RELIABLE_RELIABILITY_QOS");
                valid = false;
            }

            return valid;
        };

        return validate_params(params, validate_qos);
    }

    /**
     * @brief Create a Requester instance with the given parameters. This method will be called internally by the public method create_requester
     * This method is intended to be overriden by the user in case of custom Requester implementations
     * @note This method ONLY instantiates the Requester object, there is no DDS entities/RequesterParams validation, which can lead to unexpected behavior if the created object is not valid
     * To avoid this, use ALWAYS the public method create_requester to create a Requester.
     * 
     * @param params Requester configuration parameters
     * @return reference to the created Requester object or nullptr if an error occurred
     */
    virtual Requester* create_requester_instance(
            const RequesterParams& params);

    /**
     * @brief Create a Replier instance with the given parameters. This method will be called internally by the public method create_replier
     * This method is intended to be overriden by the user in case of custom Replier implementations
     * @note This method ONLY instantiates the Replier object, there is no DDS entities/ReplierParams validation, which can lead to unexpected behavior if the created object is not valid
     * To avoid this, use ALWAYS the public method create_replier to create a Replier.
     * 
     * @param params Replier configuration parameters
     * @return reference to the created Replier object or nullptr if an error occurred
     */
    virtual Replier* create_replier_instance(
            const ReplierParams& params);

public:

    /**
     * @brief Constructor
     */
    Service(
            const std::string& service_name,
            const std::string& service_type_name,
            DomainParticipantImpl* participant);

    /**
     * @brief Destructor
     */
    virtual ~Service();

    /**
     * @brief Get the service name
     *
     * @return Service name
     */
    const std::string& get_service_name() const
    {
        return service_name_;
    }

    /**
     * @brief Get the service type name
     *
     * @return Service type name
     */
    const std::string& get_service_type_name() const
    {
        return service_type_name_;
    }

    /**
     * @brief Get the replier associated with the service
     *
     * @return A const reference to the Replier associated with the service
     */
    const Replier* get_replier() const
    {
        return replier_;
    }

    /**
     * @brief Remove a requester from the service
     *
     * @param requester Requester to remove
     */
    void remove_requester(
            Requester* requester)
    {
        std::lock_guard<std::mutex> lock(mtx_requesters_);
        auto it = std::find(requesters_.begin(), requesters_.end(), requester);
        if (it != requesters_.end())
        {
            requesters_.erase(it);
        }
    }

    /**
     * @brief Create a requester for the service
     * 
     * @param params Requester configuration parameters
     * @return A pointer to the created requester or nullptr if an error occurred
     */
    Requester* create_requester(
            const RequesterParams& params);

    /**
     * @brief Remove all requesters from the service
     * It destroys the underlying requester objects
     */
    void remove_all_requesters()
    {
        requesters_.clear();
    }

    /**
     * @brief Create a replier for the service
     * 
     * @param params Replier configuration parameters
     * @return A pointer to the created replier or nullptr if an error occurred
     */
    Replier* create_replier(
            const ReplierParams& params);

    /**
     * @brief Get the list of requesters attached to the service
     *
     * @return List of requesters attached to the service
     */
    const std::vector<Requester*>& get_requesters()
    {
        std::lock_guard<std::mutex> lock(mtx_requesters_);
        return requesters_;
    }

    /**
     * @brief Check if the service is active
     *
     * @return true if active, false if not
     */
    inline bool is_active() const
    {
        return enable_;
    }

    // TODO: Move this method to the base RPCEntity class?
    /**
     * @brief Check if the service is valid (i.e: all DDS entities are correctly created)
     */
    inline bool is_valid() const
    {
        return valid_;
    }

    /**
     * @brief Getter for participant_
     * 
     * @return A reference to the DomainParticipant implementation associated with the service
     */
    DomainParticipantImpl* get_participant()
    {
        return participant_;
    }

    bool service_type_in_use(
            const std::string& service_type_name) const
    {
        return service_type_name_ == service_type_name;
    }

    /**
     * @brief Create request and reply topics for the service
     * 
     * @return RETCODE_OK if request/reply topics were created successfully, RETCODE_ERROR otherwise
     */
    virtual ReturnCode_t create_request_reply_topics();

private:
    
    //! Service name
    std::string service_name_;

    //! Service type name
    std::string service_type_name_;

    //! DDS Participant associated with the service
    DomainParticipantImpl* participant_;

    //! Replier associated with the service
    Replier* replier_;

    //! List of requesters attached to the service
    std::mutex mtx_requesters_;
    std::vector<Requester*> requesters_;

    //! Request and Reply topics associated with the service
    // NOTE: These topics do not filter samples. They are used to create the content filtered topics
    // If we use these topics to publish/subscribe in a multiple requester - single replier service scenario,
    // The requesters will receive all the replies, not only the ones that match their requests
    Topic* request_topic_;
    Topic* reply_topic_;

    //! Request and Reply filtered topics associated with the service.
    // In a multiple requester - single replier service scenario, each requester will discard the received replies destinated to another requester
    ContentFilteredTopic* request_filtered_topic_;
    ContentFilteredTopic* reply_filtered_topic_;

    bool valid_;
};

} // namespace rpc
} // namespace dds
} // namespace fastdds
} // namespace eprosima

#endif // FASTDDS_DDS_RPC__SERVICE_HPP