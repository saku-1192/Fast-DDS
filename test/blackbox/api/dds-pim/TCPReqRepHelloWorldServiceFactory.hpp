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

#ifndef _TEST_BLACKBOX_TCPREQREPHELLOWORLDSERVICEFACTORY_HPP_
#define _TEST_BLACKBOX_TCPREQREPHELLOWORLDSERVICEFACTORY_HPP_

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/rpc/IServiceFactory.hpp>
#include <fastdds/dds/rpc/ServiceTypeSupport.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>

#include <asio.hpp>

#include "TCPReqRepHelloWorldService.hpp"
#include "../../types/HelloWorldPubSubTypes.hpp"

#if defined(_WIN32)
#define GET_PID _getpid
#include <process.h>
#else
#define GET_PID getpid
#endif // if defined(_WIN32)

class TCPReqRepHelloWorldServiceFactory : public eprosima::fastdds::dds::rpc::IServiceFactory
{

public:

    TCPReqRepHelloWorldServiceFactory()
        : service_type_name_("TCPReqRepHelloWorldServiceType"),
        service_type_(eprosima::fastdds::dds::TypeSupport(new HelloWorldPubSubType()), eprosima::fastdds::dds::TypeSupport(new HelloWorldPubSubType()))
    {
        std::ostringstream service_name;
        service_name << "TCPReqRepHelloWorldService_" << asio::ip::host_name() << "_" << GET_PID();
        service_name_ = service_name.str();
    }

    virtual ~TCPReqRepHelloWorldServiceFactory() = default;

    // Create participants with specific configurations for this service
    // The user is responsible for deleting the participant manually
    // Custom participant configuration for TCPReqRepHelloWorldRequester
    eprosima::fastdds::dds::DomainParticipant* create_service_participant_for_requester(
            int participantId,
            int domainId,
            uint16_t listeningPort,
            uint32_t maxInitialPeer = 0,
            const char* certs_folder = nullptr,
            bool force_localhost = false);

    // Custom participant configuration for TCPReqRepHelloWorldReplier
    eprosima::fastdds::dds::DomainParticipant* create_service_participant_for_replier(
            int participantId,
            int domainId,
            uint16_t listeningPort,
            uint32_t maxInitialPeer = 0,
            const char* certs_folder = nullptr);

    const std::string& get_service_name() const
    {
        return service_name_;
    }

    const std::string& get_service_type_name() const
    {
        return service_type_name_;
    }

protected:

    virtual eprosima::fastdds::dds::rpc::Service* create_service_instance(
                const std::string& service_name,
                const std::string& service_type_name,
                eprosima::fastdds::dds::DomainParticipantImpl* participant) override
    {   
        try
        {
            return new TCPReqRepHelloWorldService(service_name, service_type_name, participant);
        }
        catch (const std::exception& e)
        {
            EPROSIMA_LOG_ERROR(TCPREQREPHELLOWORLDSERVICEFACTORY, "Error creating TCPReqRepHelloWorldService instance: " << e.what());
            return nullptr;
        }
    }

private:

    std::string service_name_;
    std::string service_type_name_;
    eprosima::fastdds::dds::rpc::ServiceTypeSupport service_type_;

};

#endif // _TEST_BLACKBOX_TCPREQREPHELLOWORLDSERVICEFACTORY_HPP_