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
 * @file ReqRepHelloWorldReplier.hpp
 *
 */

#ifndef _TEST_BLACKBOX_REQREPHELLOWORLDREPLIER_HPP_
#define _TEST_BLACKBOX_REQREPHELLOWORLDREPLIER_HPP_

#include <fastdds/dds/rpc/Replier.hpp>
#include <fastdds/rtps/common/SampleIdentity.hpp>

#include <condition_variable>

#include "../../types/HelloWorld.hpp"

class ReqRepHelloWorldReplier : public eprosima::fastdds::dds::rpc::Replier
{

protected:

    ReqRepHelloWorldReplier(
            eprosima::fastdds::dds::rpc::Service* service,
            const eprosima::fastdds::dds::rpc::ReplierParams& params)
        : eprosima::fastdds::dds::rpc::Replier(service, params),
        matched_(0)
    {
    }

public:

    virtual ~ReqRepHelloWorldReplier() = default;

    void new_sample(
            eprosima::fastdds::rtps::SampleIdentity sample_identity,
            uint16_t number);
    
    void wait_discovery();

    void matched();

    ///////////////////////////////////////////////
    // Request topic DataReader listener callbacks
    ///////////////////////////////////////////////

    void on_data_available(
            eprosima::fastdds::dds::DataReader* datareader) override;

    void on_subscription_matched(
            eprosima::fastdds::dds::DataReader* /*datareader*/,
            const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override
    {
        if (0 < info.current_count_change)
        {
            matched();
        }
    }

    /////////////////////////////////////////////
    // Reply topic DataWriter listener callbacks
    /////////////////////////////////////////////

    void on_publication_matched(
            eprosima::fastdds::dds::DataWriter* /*datawriter*/,
            const eprosima::fastdds::dds::PublicationMatchedStatus& info) override
    {
        if (0 < info.current_count_change)
        {
            matched();
        }
    }

private:

    ReqRepHelloWorldReplier& operator =(
            const ReqRepHelloWorldReplier&) = delete;

    unsigned int matched_;
    std::mutex mtx_discovery_;
    std::condition_variable cv_discovery_;
};

#endif // _TEST_BLACKBOX_REQREPHELLOWORLDREPLIER_HPP_
