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

#include "TCPReqRepHelloWorldService.hpp"
#include "TCPReqRepHelloWorldReplier.hpp"
#include "TCPReqRepHelloWorldRequester.hpp"
#include "../../common/BlackboxTests.hpp"

using namespace eprosima::fastdds::dds::rpc;
using namespace eprosima::fastdds::dds;

Requester* ReqRepHelloWorldService::create_requester_instance(
        const RequesterParams& params)
{
    try
    {
        return new TCPReqRepHelloWorldRequester(this, params);
    }
    catch (const std::exception& e)
    {
        EPROSIMA_LOG_ERROR(TCPREQREPHELLOWORLDSERVICE, "Error creating Requester instance: " << e.what());
        return nullptr;
    }
}

Replier* ReqRepHelloWorldService::create_replier_instance(
        const ReplierParams& params)
{
    try
    {
        return new TCPReqRepHelloWorldReplier(this, params);
    }
    catch (const std::exception& e)
    {
        EPROSIMA_LOG_ERROR(TCPREQREPHELLOWORLDSERVICE, "Error creating Replier instance: " << e.what());
        return nullptr;
    }
}

ReplierParams ReqRepHelloWorldService::create_replier_params()
{
    ReplierQos replier_qos;
    DataWriterQos writer_qos;
    DataReaderQos reader_qos;
    ReplierParams replier_params;

    reader_qos.endpoint().history_memory_policy = PREALLOCATED_WITH_REALLOC_MEMORY_MODE;
    writer_qos.endpoint().history_memory_policy = PREALLOCATED_WITH_REALLOC_MEMORY_MODE;
    reader_qos.reliability().kind = RELIABLE_RELIABILITY_QOS;
    // Increase default max_blocking_time to 1s in case the CPU is overhead
    reader_qos.reliability().max_blocking_time = Duration_t(1, 0);
    writer_qos.reliability().kind = RELIABLE_RELIABILITY_QOS;
    writer_qos.reliability().max_blocking_time = Duration_t(1, 0);

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

    reader_qos.endpoint().history_memory_policy = PREALLOCATED_WITH_REALLOC_MEMORY_MODE;
    writer_qos.endpoint().history_memory_policy = PREALLOCATED_WITH_REALLOC_MEMORY_MODE;
    reader_qos.reliability().kind = RELIABLE_RELIABILITY_QOS;
    reader_qos.reliability().max_blocking_time = Duration_t(1, 0);
    writer_qos.reliability().kind = RELIABLE_RELIABILITY_QOS;
    writer_qos.reliability().max_blocking_time = Duration_t(1, 0);

    return requester_params;
}