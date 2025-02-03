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
 * @file ReqRepHelloWorldReplier.cpp
 *
 */

#include "ReqRepHelloWorldReplier.hpp"

#include <gtest/gtest.h>

using namespace eprosima::fastdds::dds::rpc;
using namespace eprosima::fastdds::dds;

void ReqRepHelloWorldReplier::new_sample(
        eprosima::fastdds::rtps::SampleIdentity sample_identity,
        uint16_t number)
{
    RequestInfo info;
    HelloWorld hello;
    hello.index(number);
    hello.message("GoodBye");
    info.related_sample_identity(sample_identity);
    ASSERT_EQ(this->send_reply((void*)&hello, info), RETCODE_OK);
}

void ReqRepHelloWorldReplier::wait_discovery()
{
    std::unique_lock<std::mutex> lock(mtx_discovery_);

    std::cout << "Replier is waiting discovery..." << std::endl;

    cv_discovery_.wait(lock, [&]()
            {
                return matched_ > 1;
            });

    std::cout << "Replier discovery finished..." << std::endl;
}

void ReqRepHelloWorldReplier::matched()
{
    std::unique_lock<std::mutex> lock(mtx_discovery_);
    ++matched_;
    if (matched_ > 1)
    {
        cv_discovery_.notify_one();
    }
}

void ReqRepHelloWorldReplier::on_data_available(
        DataReader* datareader)
{
    ASSERT_NE(datareader, nullptr);

    HelloWorld hello;
    SampleInfo info;

    if (RETCODE_OK == this->take_reply((void*)&hello, &info))
    {
        if (info.valid_data)
        {
            ASSERT_EQ(hello.message().compare("HelloWorld"), 0);
            new_sample(info.sample_identity, hello.index());
        }
    }
}
