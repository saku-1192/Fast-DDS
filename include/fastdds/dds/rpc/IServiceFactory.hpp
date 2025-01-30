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

#ifndef FASTDDS_DDS_RPC__ISERVICEFACTORY_HPP
#define FASTDDS_DDS_RPC__ISERVICEFACTORY_HPP

namespace eprosima {
namespace fastdds {
namespace dds {
class DomainParticipantImpl;
} // namespace dds
namespace dds {
namespace rpc {

class Service;

/**
 * @brief Interface for a service factory. Can be used to create custom services (i.e: services that inherit from rpc::Service)
 */
class IServiceFactory
{

public:

    virtual ~IServiceFactory() = default;

    /**
     * @brief Return a new instance of a service
     * 
     * @param service_name Name of the service
     * @param service_type_name Name of the service type
     * @param participant DomainParticipant
     * 
     * @return A pointer to the service instance or nullptr if the service could not be created
     */
    virtual Service* create_service_instance(
            const std::string& service_name,
            const std::string& service_type_name,
            DomainParticipantImpl* participant) = 0;
}; 

} // namespace rpc
} // namespace dds
} // namespace fastdds
} // namespace eprosima

#endif // FASTDDS_DDS_RPC__ISERVICEFACTORY_HPP