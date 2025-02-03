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
 * @file TCPReqRepHelloWorldRequester.cpp
 *
 */

#include "TCPReqRepHelloWorldRequester.hpp"

#include <gtest/gtest.h>

using namespace eprosima::fastdds::rtps;
using namespace eprosima::fastdds::dds;
using namespace eprosima::fastdds::dds::rpc;

void TCPReqRepHelloWorldRequester::newNumber(
        SampleIdentity related_sample_identity,
        uint16_t number)
{
    std::unique_lock<std::mutex> lock(mutex_);
    received_sample_identity_ = related_sample_identity;
    number_received_ = number;
    ASSERT_EQ(current_number_, number_received_);
    if (current_number_ == number_received_)
    {
        cv_.notify_one();
    }
}

void TCPReqRepHelloWorldRequester::ReplyListener::on_data_available(
        eprosima::fastdds::dds::DataReader* datareader)
{
    ASSERT_NE(datareader, nullptr);

    HelloWorld hello;
    SampleInfo info;

    if (RETCODE_OK == this->take_reply((void*)&hello, info))
    {
        if (info.valid_data)
        {
            ASSERT_EQ(hello.message().compare("GoodBye"), 0);
            newNumber(info.related_sample_identity, hello.index());
        }
    }
}

void TCPReqRepHelloWorldRequester::wait_discovery(
        std::chrono::seconds timeout)
{
    std::unique_lock<std::mutex> lock(mtx_discovery_);

    std::cout << "Requester waiting for discovery..." << std::endl;

    if (timeout == std::chrono::seconds::zero())
    {
        cv_discovery_.wait(lock, [&]()
                {
                    return is_matched();
                });
    }
    else
    {
        cv_discovery_.wait_for(lock, timeout, [&]()
                {
                    return is_matched();
                });
    }

    std::cout << "Requester discovery phase finished" << std::endl;
}

void TCPReqRepHelloWorldRequester::block()
{
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait(lock, [this]() -> bool
            {
                return current_number_ == number_received_;
            });

    ASSERT_EQ(related_sample_identity_, received_sample_identity_);
}


void TCPReqRepHelloWorldRequester::matched()
{
    std::unique_lock<std::mutex> lock(mtx_discovery_);
    ++matched_;
    if (is_matched())
    {
        cv_discovery_.notify_one();
    }
}

void TCPReqRepHelloWorldRequester::unmatched()
{
    std::unique_lock<std::mutex> lock(mtx_discovery_);
    --matched_;
}

bool TCPReqRepHelloWorldRequester::is_matched()
{
    return matched_ > 1;
}

void TCPReqRepHelloWorldRequester::send(
        const uint16_t number)
{
    RequestInfo info;
    HelloWorld hello;
    hello.index(number);
    hello.message("HelloWorld");

    {
        std::unique_lock<std::mutex> lock(mutex_);
        current_number_ = number;
    }

    ASSERT_EQ(this->send_request((void*)&hello, wparams), RETCODE_OK);
    related_sample_identity_ = info.sample_identity();
    ASSERT_NE(related_sample_identity_.sequence_number(), SequenceNumber_t());
}
