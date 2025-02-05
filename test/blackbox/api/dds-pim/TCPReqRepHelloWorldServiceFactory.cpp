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
#include <fastdds/dds/domain/qos/DomainParticipantQos.hpp>
#include <fastdds/rtps/common/LocatorList.hpp>
#include <fastdds/rtps/transport/TCPv4TransportDescriptor.hpp>
#include <fastdds/rtps/transport/TCPv6TransportDescriptor.hpp>

#include <fastdds/utils/IPFinder.hpp>
#include <fastdds/utils/IPLocator.hpp>

#include <gtest/gtest.h>

#include "TCPReqRepHelloWorldServiceFactory.hpp"
#include "../../common/BlackboxTests.hpp"

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastdds::dds::rpc;
using namespace eprosima::fastdds::rtps;

DomainParticipant* TCPReqRepHelloWorldServiceFactory::create_service_participant_for_requester(
        int participantId,
        int domainId,
        uint16_t listeningPort,
        uint32_t maxInitialPeer,
        const char* certs_folder,
        bool force_localhost)
{
    DomainParticipantQos participant_qos;

    int32_t kind;
    LocatorList_t loc;

    if (use_ipv6)
    {
        kind = LOCATOR_KIND_TCPv6;
        IPFinder::getIP6Address(&loc);

    }
    else
    {
        kind = LOCATOR_KIND_TCPv4;
        IPFinder::getIP4Address(&loc);

    }


    Locator_t initial_peer_locator;
    initial_peer_locator.kind = kind;
    if (!force_localhost && loc.size() > 0)
    {
        if (use_ipv6)
        {
            IPLocator::setIPv6(initial_peer_locator, *(loc.begin()));
        }
        else
        {
            IPLocator::setIPv4(initial_peer_locator, *(loc.begin()));
        }
    }
    else
    {
        if (use_ipv6)
        {
            IPLocator::setIPv6(initial_peer_locator, "::1");
        }
        else
        {
            IPLocator::setIPv4(initial_peer_locator, "127.0.0.1");
        }
    }

    initial_peer_locator.port = listeningPort;
    participant_qos.wire_protocol().builtin.initialPeersList.push_back(initial_peer_locator);
    participant_qos.transport().use_builtin_transports = false;

    std::shared_ptr<TCPTransportDescriptor> descriptor;
    if (use_ipv6)
    {
        descriptor = std::make_shared<eprosima::fastdds::rtps::TCPv6TransportDescriptor>();
    }
    else
    {
        descriptor = std::make_shared<eprosima::fastdds::rtps::TCPv4TransportDescriptor>();
    }

    if (maxInitialPeer > 0)
    {
        descriptor->maxInitialPeersRange = maxInitialPeer;
    }

    if (certs_folder != nullptr)
    {
        using TLSOptions = eprosima::fastdds::rtps::TCPTransportDescriptor::TLSConfig::TLSOptions;
        using TLSVerifyMode = eprosima::fastdds::rtps::TCPTransportDescriptor::TLSConfig::TLSVerifyMode;
        descriptor->apply_security = true;
        //descriptor->tls_config.password = "testkey";
        descriptor->tls_config.verify_file = std::string(certs_folder) + "/maincacert.pem";
        descriptor->tls_config.verify_mode = TLSVerifyMode::VERIFY_PEER;
        descriptor->tls_config.add_option(TLSOptions::DEFAULT_WORKAROUNDS);
        descriptor->tls_config.add_option(TLSOptions::SINGLE_DH_USE);
        descriptor->tls_config.add_option(TLSOptions::NO_COMPRESSION);
        descriptor->tls_config.add_option(TLSOptions::NO_SSLV2);
    }

    participant_qos.transport().user_transports.push_back(descriptor);

    participant_qos.wire_protocol().participant_id = participantId;
    participant_qos.wire_protocol().builtin.discovery_config.leaseDuration = eprosima::fastdds::dds::c_TimeInfinite;
    participant_qos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod = Duration_t(1, 0);

    DomainParticipant* participant = eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->create_participant(
        domainId, participant_qos);
    assert(participant != nullptr);
    assert(participant->is_enabled());

    // Register service type
    assert(participant->register_service_type(service_type_, service_type_name_) == RETCODE_OK);

    return participant;
}

DomainParticipant* TCPReqRepHelloWorldServiceFactory::create_service_participant_for_replier(
        int participantId,
        int domainId,
        uint16_t listeningPort,
        uint32_t maxInitialPeer,
        const char* certs_folder)
{
    DomainParticipantQos participant_qos;
    participant_qos.wire_protocol().participant_id = participantId;
    participant_qos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod = Duration_t(1, 0);
    participant_qos.wire_protocol().builtin.discovery_config.leaseDuration = eprosima::fastdds::dds::c_TimeInfinite;

    participant_qos.transport().use_builtin_transports = false;

    std::shared_ptr<TCPTransportDescriptor> descriptor;
    if (use_ipv6)
    {
        descriptor = std::make_shared<eprosima::fastdds::rtps::TCPv6TransportDescriptor>();
    }
    else
    {
        descriptor = std::make_shared<eprosima::fastdds::rtps::TCPv4TransportDescriptor>();
    }

    descriptor->sendBufferSize = 0;
    descriptor->receiveBufferSize = 0;
    if (maxInitialPeer > 0)
    {
        descriptor->maxInitialPeersRange = maxInitialPeer;
    }
    descriptor->add_listener_port(listeningPort);

    if (certs_folder != nullptr)
    {
        using TLSOptions = eprosima::fastdds::rtps::TCPTransportDescriptor::TLSConfig::TLSOptions;
        using TLSVerifyMode = eprosima::fastdds::rtps::TCPTransportDescriptor::TLSConfig::TLSVerifyMode;
        descriptor->apply_security = true;
        descriptor->tls_config.password = "testkey";
        descriptor->tls_config.cert_chain_file = std::string(certs_folder) + "/mainsubcert.pem";
        descriptor->tls_config.private_key_file = std::string(certs_folder) + "/mainsubkey.pem";
        descriptor->tls_config.verify_mode = TLSVerifyMode::VERIFY_PEER;
        descriptor->tls_config.add_option(TLSOptions::DEFAULT_WORKAROUNDS);
        descriptor->tls_config.add_option(TLSOptions::SINGLE_DH_USE);
        descriptor->tls_config.add_option(TLSOptions::NO_COMPRESSION);
        descriptor->tls_config.add_option(TLSOptions::NO_SSLV2);
    }

    participant_qos.transport().user_transports.push_back(descriptor);

    DomainParticipant* participant = DomainParticipantFactory::get_instance()->create_participant(
        domainId, participant_qos);
    assert(participant != nullptr);
    assert(participant->is_enabled());

    // Register service type
    assert(participant->register_service_type(service_type_, service_type_name_) == RETCODE_OK);

    return participant;
}
