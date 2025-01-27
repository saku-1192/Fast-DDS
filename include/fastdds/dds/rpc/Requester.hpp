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

#ifndef FASTDDS_DDS_RPC__REQUESTER_HPP
#define FASTDDS_DDS_RPC__REQUESTER_HPP

#include <fastdds/dds/core/LoanableCollection.hpp>
#include <fastdds/dds/core/LoanableSequence.hpp>
#include <fastdds/dds/domain/qos/RequesterQos.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/rtps/common/SampleIdentity.hpp>

#include "RequesterParams.hpp"
#include "RequestInfo.hpp"
#include "RPCEntity.hpp"
#include "Service.hpp"

namespace eprosima {
namespace fastdds {
namespace dds {
namespace rpc {

class Service;

/**
 * @brief Class that represents a requester entity in the RPC communication.
 */
class Requester : public RPCEntity
{

friend class Service;

protected:
    /**
     * @brief Constructor
     * Don't instantiate this class directly. Use create_service_requester from DomainParticipant instead.
     */
    Requester(
            Service* service,
            const RequesterParams& params);
    
    /**
     * @brief Create required DDS entities to enable communication with the replier
     * 
     * @param params Requester parameters to configure the DDS entities
     * 
     * @return RETCODE_OK if all DDS entities were created successfully, RETCODE_ERROR otherwise
     */
    virtual ReturnCode_t create_dds_entities(const RequesterParams& params);

public:
    /**
     * @brief Destructor
     */
    virtual ~Requester();

    /**
     * @brief Send a request message to a replier
     * 
     * @param data Data to send
     * @param info Information about the request sample. This information is used to match the request with the reply through the SampleIdentity
     * @return RETCODE_OK if the reply was sent successfully or a ReturnCode related to the specific error otherwise
     */
    virtual ReturnCode_t send_request(
            void* data,
            RequestInfo& info);

    /**
     * @brief Take a reply message from the Requester DataReader's history.
     * 
     * @param data Data to receive the reply
     * @param info Information about the reply sample
     * @return RETCODE_OK if the reply was taken successfully or a ReturnCode related to the specific error otherwise
     */
    virtual ReturnCode_t take_reply(
            void* data,
            SampleInfo& info);

    /**
     * @brief Take all reply messages stored in the Requester DataReader's history.
     * @note This method does not allow to take only the samples associated to a given request. User must implement a zero-copy solution to link request and reply samples.
     * 
     * @param data Data to receive the replies
     * @param info Information about the reply samples
     * @return RETCODE_OK if the replies were taken successfully or a ReturnCode related to the specific error otherwise
     */
    virtual ReturnCode_t take_reply(
            LoanableCollection& data,
            LoanableSequence<SampleInfo>& info);

    /**
     * @brief Check if the requester is valid (i.e: all DDS entities are correctly created)
     */
    inline bool is_valid() const
    {
        return valid_;
    }

protected:
    DataReader* requester_reader_;
    DataWriter* requester_writer_;
    Publisher* requester_publisher_;
    Subscriber* requester_subscriber_;
    RequesterQos qos_;
    Service* service_;
    bool valid_;
};

} // namespace rpc
} // namespace dds
} // namespace fastdds
} // namespace eprosima

#endif // FASTDDS_DDS_RPC__REQUESTER_HPP