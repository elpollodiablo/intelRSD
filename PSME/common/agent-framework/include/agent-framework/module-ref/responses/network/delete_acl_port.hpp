/*!
 * @copyright
 * Copyright (c) 2016 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * @file responses/network/delete_acl_port.hpp
 * @brief Definition of network deleteAclPort response
 * */

#pragma once

#include "agent-framework/module-ref/model/attributes/oem.hpp"

#include <string>

namespace Json {
    class Value;
}

namespace agent_framework {
namespace model {
namespace responses {

/*! DeleteAclPort response */
class DeleteAclPort {
public:
    using Oem = agent_framework::model::attribute::Oem;

    /*!
     * @brief explicit DeleteAclPort response constructor
     *
     * @param[in] oem OEM specific data
     */
    explicit DeleteAclPort(Oem oem = Oem{});

    /*!
     * @brief Transform request to Json
     *
     * @return created Json value
     */
    Json::Value to_json() const;

    /*!
     * @brief create DeleteAclPort from Json
     *
     * @param[in] json the input argument
     *
     * @return new DeleteAclPort
     */
    static DeleteAclPort from_json(const Json::Value& json);

private:
    Oem m_oem{};
};

}
}
}
