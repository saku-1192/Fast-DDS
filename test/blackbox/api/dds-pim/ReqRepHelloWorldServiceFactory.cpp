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

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

#include <gtest/gtest.h>

#include "ReqRepHelloWorldServiceFactory.hpp"

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastdds::dds::rpc;

DomainParticipant* ReqRepHelloWorldServiceFactory::create_service_participant()
{
    DomainParticipant* participant = DomainParticipantFactory::get_instance()->create_participant(
        (uint32_t)GET_PID() % 230,
        PARTICIPANT_QOS_DEFAULT);
    ASSERT_NE(participant, nullptr);
    ASSERT_TRUE(participant->is_enabled());

    // Register service type
    ASSERT_EQ(participant->register_service_type(service_type_, service_type_name_), RETCODE_OK);

    return participant;
}