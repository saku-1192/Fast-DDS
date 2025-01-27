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

#ifndef FASTDDS_DDS_RPC__RPCENTITY_HPP
#define FASTDDS_DDS_RPC__RPCENTITY_HPP

#include <fastdds/dds/core/detail/DDSReturnCode.hpp>

namespace eprosima {
namespace fastdds {
namespace dds {
namespace rpc {

/**
 * @brief Abstract base class for all RPC Objects
 */
class RPCEntity
{
public:

    /**
     * @brief Enables the entity
     */
    virtual eprosima::fastdds::dds::ReturnCode_t enable()
    {
        enable_ = true;
        return eprosima::fastdds::dds::RETCODE_OK;
    }

    /**
     * @brief Disables the entity. This method should be called before the entity destruction
     */
    virtual eprosima::fastdds::dds::ReturnCode_t close()
    {
        enable_ = false;
        return eprosima::fastdds::dds::RETCODE_OK;
    }

    /**
     * @brief Checks if the entity is enabled
     */
    bool is_enabled() const
    {
        return enable_;
    }

    // TODO (Carlosespicur) Add here is_null() method? (based on RPC standard)

protected:
    //! Boolean that states if the Entity is enabled or disabled
    // TODO: Probably this should be an atomic<bool>
    bool enable_;
};

} // namespace rpc
} // namespace dds
} // namespace fastdds
} // namespace eprosima

#endif // FASTDDS_DDS_RPC__RPCENTITY_HPP
