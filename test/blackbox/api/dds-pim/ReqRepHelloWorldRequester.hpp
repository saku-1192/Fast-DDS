// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

/**
 * @file ReqRepHelloWorldRequester.hpp
 *
 */

#ifndef _TEST_BLACKBOX_REQREPHELLOWORLDREQUESTER_HPP_
#define _TEST_BLACKBOX_REQREPHELLOWORLDREQUESTER_HPP_

#include <fastdds/dds/rpc/Requester.hpp>
#include <fastdds/rtps/common/SampleIdentity.hpp>

#include <condition_variable>

#include "../../types/HelloWorld.hpp"

class ReqRepHelloWorldRequester : public eprosima::fastdds::dds::rpc::Requester
{

protected:

    ReqRepHelloWorldRequester(
            eprosima::fastdds::dds::rpc::Service* service,
            const eprosima::fastdds::dds::rpc::RequesterParams& params)
        : eprosima::fastdds::dds::rpc::Requester(service, params),
        matched_(0),
        current_number_(std::numeric_limits<uint16_t>::max()),
        number_received_(std::numeric_limits<uint16_t>::max())
    {
    }

public:

    virtual ~ReqRepHelloWorldRequester() = default;

    void newNumber(
            eprosima::fastdds::rtps::SampleIdentity related_sample_identity,
            uint16_t number);
    void wait_discovery();
    void matched();
    void send(
            const uint16_t number);
    void block(
            const std::chrono::seconds& seconds);
    
    ///////////////////////////////////////////////
    // Reply topic DataReader listener callbacks
    ///////////////////////////////////////////////

    void on_data_available(
            eprosima::fastdds::dds::DataReader* datareader) override;

    void on_subscription_matched(
                eprosima::fastdds::dds::DataReader*,
                const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override
    {
        if (0 < info.current_count_change)
        {
            matched();
        }
    }

    ///////////////////////////////////////////////
    // Request topic DataWriter listener callbacks
    ///////////////////////////////////////////////

    void on_publication_matched(
                eprosima::fastdds::dds::DataWriter*,
                const eprosima::fastdds::dds::PublicationMatchedStatus& info) override
    {
        if (0 < info.current_count_change)
        {
            matched();
        }
    }
    
private:

    unsigned int matched_;
    uint16_t current_number_;
    uint16_t number_received_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::mutex mtx_discovery_;
    std::condition_variable cv_discovery_;
    eprosima::fastdds::rtps::SampleIdentity related_sample_identity_;
    eprosima::fastdds::rtps::SampleIdentity received_sample_identity_;

};

#endif // _TEST_BLACKBOX_REQREPHELLOWORLDREQUESTER_HPP_
