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

#include <fastdds/dds/core/status/StatusMask.hpp>
#include <fastdds/dds/log/Log.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>
#include <fastdds/dds/rpc/Replier.hpp>
#include <fastdds/dds/rpc/ReplierParams.hpp>
#include <fastdds/dds/rpc/RequestInfo.hpp>
#include <fastdds/dds/rpc/Service.hpp>
#include <fastdds/dds/subscriber/qos/SubscriberQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>

#include "../domain/DomainParticipantImpl.hpp"

namespace eprosima {
namespace fastdds {
namespace dds {
namespace rpc {

Replier::Replier(
        Service* service,
        const ReplierParams& params)
        : qos_(params.qos()),
        service_(service)
{
    // Create required DDS entities. If any of them fails, the replier is not valid
    ReturnCode_t retcode = create_dds_entities(params);
    valid_ = (retcode == RETCODE_OK);
}

Replier::~Replier()
{
    delete replier_writer_;
    delete replier_reader_;

    if (replier_subscriber_)
    {
        replier_subscriber_->delete_contained_entities();
        delete replier_subscriber_;
    }

    if (replier_publisher_)
    {
        replier_publisher_->delete_contained_entities();
        delete replier_publisher_;
    }

    service_ = nullptr;
}

ReturnCode_t Replier::send_reply(
        void* data,
        RequestInfo& info)
{
    // TODO Implement matching algorithm
    return replier_writer_->write(data, info);
}

ReturnCode_t Replier::take_request(
        void* data,
        SampleInfo& info)
{
    // TODO Implement matching algorithm
    return replier_reader_->take_next_sample(data, &info);
}

ReturnCode_t Replier::take_request(
        LoanableCollection& data,
        LoanableSequence<SampleInfo>& info)
{
    // TODO Implement matching algorithm
    return replier_reader_->take(data, info);
}

ReturnCode_t Replier::create_dds_entities(const ReplierParams& params)
{
    if (!service_)
    {
        // Service is not configured properly, so the replier does not have access to a valid DomainParticipant
        EPROSIMA_LOG_ERROR(REPLIER, "Service is nullptr");
        return RETCODE_ERROR;
    }
    
    // Create publisher and writer for the Reply topic
    replier_publisher_ = service_->get_participant()->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr, StatusMask::none());

    if (!replier_publisher_)
    {
        EPROSIMA_LOG_ERROR(REPLIER, "Error creating replier publisher");
        return RETCODE_ERROR;
    }

    replier_writer_ = replier_publisher_->create_datawriter(service_->reply_topic_, params.qos().writer_qos, nullptr, StatusMask::none());

    if (!replier_writer_)
    {
        EPROSIMA_LOG_ERROR(REPLIER, "Error creating replier writer");
        return RETCODE_ERROR;
    }

    // Create subscriber and reader for the Reply topic
    replier_subscriber_ = service_->get_participant()->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr, StatusMask::none());

    if (!replier_subscriber_)
    {
        EPROSIMA_LOG_ERROR(REPLIER, "Error creating replier subscriber");
        return RETCODE_ERROR;
    }

    replier_reader_ = replier_subscriber_->create_datareader(service_->request_filtered_topic_, params.qos().reader_qos, nullptr, StatusMask::none());

    if (!replier_reader_)
    {
        EPROSIMA_LOG_ERROR(REPLIER, "Error creating reply reader");
        return RETCODE_ERROR;
    }

    return RETCODE_OK;
}

} // namespace rpc
} // namespace dds
} // namespace fastdds
} // namespace eprosima