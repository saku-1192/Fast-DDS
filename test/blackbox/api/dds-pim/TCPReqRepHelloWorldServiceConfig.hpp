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

#ifndef _TEST_BLACKBOX_TCPREQREPHELLOWORLDSERVICECONFIG_HPP_
#define _TEST_BLACKBOX_TCPREQREPHELLOWORLDSERVICECONFIG_HPP_

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/rpc/ReplierParams.hpp>
#include <fastdds/dds/rpc/RequesterParams.hpp>
#include <fastdds/dds/rpc/ServiceTypeSupport.hpp>

#include "ReqRepHelloWorldService.hpp"

/**
 * @brief Class that creates a Service with specific participant configuration. It also provides the required parameters to create a requester/replier 
 */
class ReqRepHelloWorldServiceConfig
{

private:

    void configure_participant();
    void configure_params();

public:

    ReqRepHelloWorldServiceConfig();
    virtual ~ReqRepHelloWorldServiceConfig();

    /**
     * @brief Get the required params to configure a ReqRepHelloWorldRequester
     */
    const eprosima::fastdds::dds::rpc::RequesterParams& get_requester_params() const
    {
        return requester_params_;
    }

    /**
     * @brief Get the required params to configure a ReqRepHelloWorldReplier
     */
    const eprosima::fastdds::dds::rpc::ReplierParams& get_replier_params() const
    {
        return replier_params_;
    }

private:

    eprosima::fastdds::dds::DomainParticipant* participant_;
    eprosima::fastdds::dds::rpc::RequesterParams requester_params_;
    eprosima::fastdds::dds::rpc::ReplierParams replier_params_;
    eprosima::fastdds::dds::rpc::ServiceTypeSupport service_type_;

}

#endif // _TEST_BLACKBOX_TCPREQREPHELLOWORLDSERVICECONFIG_HPP_