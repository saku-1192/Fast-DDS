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

#ifndef _TEST_BLACKBOX_REQREPHELLOWORLDSERVICE_HPP_
#define _TEST_BLACKBOX_REQREPHELLOWORLDSERVICE_HPP_

#include <fastdds/dds/rpc/ReplierParams.hpp>
#include <fastdds/dds/rpc/RequesterParams.hpp>
#include <fastdds/dds/rpc/Service.hpp>

class ReqRepHelloWorldService : public eprosima::fastdds::dds::rpc::Service
{
        
public:

    ReqRepHelloWorldService(
            const std::string& service_name,
            const std::string& service_type_name,
            eprosima::fastdds::dds::DomainParticipantImpl* participant)
            : eprosima::fastdds::dds::rpc::Service(service_name, service_type_name, participant)
    {
    }

    virtual ~ReqRepHelloWorldService() = default;

    eprosima::fastdds::dds::rpc::ReplierParams create_replier_params();
    eprosima::fastdds::dds::rpc::RequesterParams create_requester_params();
    
protected:

    virtual eprosima::fastdds::dds::rpc::Requester* create_requester_instance(
            const eprosima::fastdds::dds::rpc::RequesterParams& params) override;
    
    virtual eprosima::fastdds::dds::rpc::Replier* create_replier_instance(
            const eprosima::fastdds::dds::rpc::ReplierParams& params) override;

    
};


#endif // _TEST_BLACKBOX_REQREPHELLOWORLDSERVICE_HPP_