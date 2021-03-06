/*!
 * @copyright
 * Copyright (c) 2015-2016 Intel Corporation
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
 * */

#pragma once
#include "agent-framework/module-ref/requests/network/get_acl_info.hpp"
#include "agent-framework/module-ref/network_manager.hpp"
#include "agent-framework/module-ref/model/acl.hpp"

#include "psme/rest-ref/model/handlers/handler_helpers.hpp"

namespace psme {
namespace rest {
namespace model {
namespace handler {

using AclHandlerBase = GenericHandler<
        agent_framework::model::requests::GetAclInfo,
        agent_framework::model::Acl, LocalIdPolicy>;

/*!
 * @brief AclHandler template specialization
 *
 * Ports have a collection of ACLs, which are not their children in the
 * meaning of PSME core logic. This is the reason why we need a separate
 * implementation.
 */
class AclHandler : public AclHandlerBase {
public:
    AclHandler() : AclHandlerBase() {}
    virtual ~AclHandler();

protected:
    using NetworkManager = agent_framework::module::NetworkManager;

    /*!
     * @brief Specialization of fetch_siblings()
     *
     * An ACL can be either a subcomponent of a Switch or a Switch Port.
     * This code handles both cases.
     *
     * @param[in] ctx - keeps data that is required during processing and
     * needs to be passed down to sub-handlers
     * @param[in] parent_uuid - uuid of parent whose subcomponents we want to
     * retrieve
     * @param[in] collection_name name of the collection
     */
    void fetch_siblings(Context& ctx, const std::string& parent_uuid,
        const std::string& collection_name) override {
        if (Component::Switch == ctx.get_parent_component()) {
            return AclHandlerBase::fetch_siblings(ctx, parent_uuid,
                collection_name);
        }
        else if (Component::SwitchPort == ctx.get_parent_component()) {
            log_debug(GET_LOGGER("rest"), ctx.indent
                << "[" << static_cast<char>(ctx.mode) << "] "
                << "Binding port " << parent_uuid << " to ACLs");

            auto fetched = fetch_sibling_uuid_list(ctx, parent_uuid,
                collection_name);
            std::vector<std::string> to_add, to_remove;
            auto& manager = NetworkManager::get_instance()->
                get_acl_port_manager();

            helpers::what_to_add_what_to_remove(fetched,
                                                parent_uuid,
                                                manager,
                                                to_add,
                                                to_remove);

            for (const auto& uuid : to_add) {
                log_debug(GET_LOGGER("rest"), ctx.indent
                    << "[" << static_cast<char>(ctx.mode) << "] "
                    << "Binding port " << parent_uuid << " to ACL "
                    << uuid);
                manager.add_entry(uuid, parent_uuid, ctx.agent->get_gami_id());
            }

            for (const auto& uuid : to_remove) {
                log_debug(GET_LOGGER("rest"), ctx.indent
                << "[" << static_cast<char>(ctx.mode) << "] "
                << "Unbinding port " << parent_uuid << " from ACL "
                << uuid);
                manager.remove_entry(uuid, parent_uuid);
            }
        }
    }

    /*!
     * @brief Specialization of remove_agent_data()
     *
     * This override is necessary to properly clean the ACL <-> port
     * bindings for all the ports
     *
     * @param[in] gami_id uuid of the agent whose data is to be removed
     * */
    void remove_agent_data(const std::string& gami_id) override {
        AclHandlerBase::remove_agent_data(gami_id);
        NetworkManager::get_instance()->
            get_acl_port_manager().clean_resources_for_agent(gami_id);
    }

    /*!
     * @brief Specialization of remove()
     *
     * This override is necessary for clearing the ACL <-> port bindings
     * for given ACL
     *
     * @param[in] uuid uuid of the ACL to be removed
     * */
    void remove(const std::string& uuid) override {
        // ACL is the parent in ACL <-> port relation
        NetworkManager::get_instance()->get_acl_port_manager().
            remove_parent(uuid);
        AclHandlerBase::remove(uuid);
    }
};

AclHandler::~AclHandler() {}

}
}
}
}

