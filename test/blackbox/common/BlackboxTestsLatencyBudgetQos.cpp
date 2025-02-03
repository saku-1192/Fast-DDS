// Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

#include "BlackboxTests.hpp"
#include "../api/dds-pim/ReqRepHelloWorldServiceFactory.hpp"

#include <gtest/gtest.h>

TEST(LatencyBudgetQos, DurationCheck)
{
    std::unique_ptr<ReqRepHelloWorldServiceFactory> factory = std::make_unique<ReqRepHelloWorldServiceFactory>();

    DomainParticipant* participant = factory->create_service_participant();
    ASSERT_NE(participant, nullptr);
    Service* service = participant->create_service_from_factory(
            factory, factory->get_service_name(), factory->get_service_type_name());
    ASSERT_NE(service, nullptr);

    eprosima::fastdds::dds::Duration_t latency_budget_pub(10);
    eprosima::fastdds::dds::Duration_t latency_budget_sub(20);

    RequesterParams requester_params = service->create_requester_params();
    requester_params.qos().writer_qos.latency_budget_duration(latency_budget_pub);
    requester_params.qos().reader_qos.latency_budget_duration(latency_budget_sub);

    Requester* requester = participant->create_service_requester(service, requester_params.qos());
    ASSERT_NE(requester, nullptr);

    eprosima::fastdds::dds::DataWriter* datawriter(nullptr);
    eprosima::fastdds::dds::DataReader* datareader(nullptr);

    requester->get_requester_writer(datawriter);
    requester->get_requester_reader(datareader);
    ASSERT_NE(datawriter, nullptr);
    ASSERT_NE(datareader, nullptr);

    EXPECT_EQ(datawriter->get_qos().latency_budget().duration, latency_budget_pub);
    EXPECT_EQ(datareader->get_qos().latency_budget().duration, latency_budget_sub);

    // Clean entities
    eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->delete_participant(participant);
}
