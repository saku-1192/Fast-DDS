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

#include <fastdds/dds/domain/qos/ReplierQos.hpp>
#include <fastdds/dds/domain/qos/RequesterQos.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>

#include "ReqRepHelloWorldService.hpp"
#include "ReqRepHelloWorldReplier.hpp"
#include "ReqRepHelloWorldRequester.hpp"
#include "../../common/BlackboxTests.hpp"

using namespace eprosima::fastdds::dds::rpc;
using namespace eprosima::fastdds::dds;

Requester* ReqRepHelloWorldService::create_requester_instance(
        const RequesterParams& params)
{
    try
    {
        return new ReqRepHelloWorldRequester(this, params);
    }
    catch (const std::exception& e)
    {
        EPROSIMA_LOG_ERROR(REQREPHELLOWORLDSERVICE, "Error creating Requester instance: " << e.what());
        return nullptr;
    }
}

Replier* ReqRepHelloWorldService::create_replier_instance(
        const ReplierParams& params)
{
    try
    {
        return new ReqRepHelloWorldReplier(this, params);
    }
    catch (const std::exception& e)
    {
        EPROSIMA_LOG_ERROR(REQREPHELLOWORLDSERVICE, "Error creating Replier instance: " << e.what());
        return nullptr;
    }
}

ReplierParams ReqRepHelloWorldService::create_replier_params()
{
    ReplierQos replier_qos;
    DataWriterQos writer_qos;
    DataReaderQos reader_qos;
    ReplierParams replier_params;

    reader_qos.endpoint().history_memory_policy = eprosima::fastdds::rtps::PREALLOCATED_WITH_REALLOC_MEMORY_MODE;
    writer_qos.endpoint().history_memory_policy = eprosima::fastdds::rtps::PREALLOCATED_WITH_REALLOC_MEMORY_MODE;

    writer_qos.reliable_writer_qos().times.heartbeat_period.seconds = 1;
    writer_qos.reliable_writer_qos().times.heartbeat_period.nanosec = 0;

    if (enable_datasharing)
    {
        reader_qos.data_sharing().automatic();
        writer_qos.data_sharing().automatic();
    }
    else
    {
        reader_qos.data_sharing().off();
        writer_qos.data_sharing().off();
    }

    if (use_pull_mode)
    {
        writer_qos.properties().properties().emplace_back("fastdds.push_mode", "false");
    }

    replier_qos.service_name = service_name_;
    replier_qos.request_type = request_topic_->get_type_name();
    replier_qos.reply_type = reply_topic_->get_type_name();
    replier_qos.request_topic_name = request_topic_->get_name();
    replier_qos.reply_topic_name = reply_topic_->get_name();
    replier_qos.writer_qos = writer_qos;
    replier_qos.reader_qos = reader_qos;
    replier_params.qos(replier_qos);

    return replier_params;
}

RequesterParams ReqRepHelloWorldService::create_requester_params()
{
    RequesterQos requester_qos;
    DataWriterQos writer_qos;
    DataReaderQos reader_qos;
    RequesterParams requester_params;

    // Requester/Replier DataWriter QoS configuration
    reader_qos.endpoint().history_memory_policy = eprosima::fastdds::rtps::PREALLOCATED_WITH_REALLOC_MEMORY_MODE;
    writer_qos.endpoint().history_memory_policy = eprosima::fastdds::rtps::PREALLOCATED_WITH_REALLOC_MEMORY_MODE;

    writer_qos.reliable_writer_qos().times.heartbeat_period.seconds = 1;
    writer_qos.reliable_writer_qos().times.heartbeat_period.nanosec = 0;

    if (enable_datasharing)
    {
        reader_qos.data_sharing().automatic();
        writer_qos.data_sharing().automatic();
    }
    else
    {
        reader_qos.data_sharing().off();
        writer_qos.data_sharing().off();
    }

    if (use_pull_mode)
    {
        writer_qos.properties().properties().emplace_back("fastdds.push_mode", "false");
    }

    requester_qos.service_name = service_name_;
    requester_qos.request_type = request_topic_->get_type_name();
    requester_qos.reply_type = reply_topic_->get_type_name();
    requester_qos.request_topic_name = request_topic_->get_name();
    requester_qos.reply_topic_name = reply_topic_->get_name();
    requester_qos.writer_qos = writer_qos;
    requester_qos.reader_qos = reader_qos;
    requester_params.qos(requester_qos);

    return requester_params;
}
