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

#ifndef FASTDDS_DDS_RPC__REQUESTERPARAMS_HPP
#define FASTDDS_DDS_RPC__REQUESTERPARAMS_HPP

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/qos/RequesterQos.hpp>

namespace eprosima {
namespace fastdds {
namespace dds {
namespace rpc {

/**
 * @brief Class that contains the parameters to create a requester
 * @note Service must validate these parameters before creating the requester
 */
class RequesterParams
{
public:
    
    /**
     * @brief Constructor
     * 
     */
    RequesterParams() = default;

    /**
     * @brief Destructor
     * 
     */
    ~RequesterParams() = default;

    // TODO (Carlosespicur) Define == and = operators?

    /**
     * @brief Getter for Requester QoS
     */
    const dds::RequesterQos& qos() const
    {
        return qos_;
    }

    /**
     * @brief Setter for Requester QoS
     */
    void qos(
            const dds::RequesterQos& qos)
    {
        qos_ = qos;
    }

private:
    dds::RequesterQos qos_;
    // TODO (Carlosespicur) Add here Requester Listeners when implemented
};
} // namespace rpc
} // namespace dds
} // namespace fastdds
} // namespace eprosima

#endif // FASTDDS_DDS_RPC__REQUESTERPARAMS_HPP