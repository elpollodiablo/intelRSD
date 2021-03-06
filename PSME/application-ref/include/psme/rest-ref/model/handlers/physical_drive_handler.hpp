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
#include "agent-framework/module-ref/storage_manager.hpp"
#include "agent-framework/module-ref/requests/storage/get_physical_drive_info.hpp"

#include "psme/rest-ref/model/handlers/handler_helpers.hpp"

namespace psme {
namespace rest {
namespace model {
namespace handler {

using PhysicalDriveHandlerBase = GenericHandler<
        agent_framework::model::requests::GetPhysicalDriveInfo,
        agent_framework::model::PhysicalDrive, LocalIdPolicy>;

/*!
 * @brief PhysicalDriveHandler template specialization
 *
 * Physical drives come from two sources
 * - elements of sub-collection of StorageServices
 * - elements of sub-collection of Physical drives (recursive definition)
 * Above is the reason why we need separate implementation.
 */
class PhysicalDriveHandler : public PhysicalDriveHandlerBase {
public:
    PhysicalDriveHandler() : PhysicalDriveHandlerBase() {}
    virtual ~PhysicalDriveHandler();
protected:
    /*!
     * brief Specialization of fetch siblings
     * @param ctx - keeps data that is required during processing and need to be passed down to sub-handlers
     * @param parent_uuid - uuid of parent for components we want to retrieve
     * @param collection_name name of the collection
     * Physical Drive can be either sub-component of StorageService or sub-component of LogicalDrive
     * This code handles both cases
     */
    void fetch_siblings(Context& ctx, const std::string &parent_uuid,
        const std::string &collection_name) override {
        if (Component::StorageServices == ctx.get_parent_component()) {
            return PhysicalDriveHandlerBase::fetch_siblings(ctx, parent_uuid,
                collection_name);
        }
        else if (Component::LogicalDrive == ctx.get_parent_component()) {
            log_debug(GET_LOGGER("rest"), ctx.indent << "[" << static_cast<char>(ctx.mode) << "] " << "Fetching PhysicalDrive for parent=" << parent_uuid);

            auto fetched = fetch_sibling_uuid_list(ctx, parent_uuid,
                collection_name);
            std::vector<std::string> to_add, to_remove;
            auto& manager = agent_framework::module::StorageManager::get_instance()->get_physical_drive_members_manager();
            helpers::what_to_add_what_to_remove(fetched,
                                                parent_uuid,
                                                manager,
                                                to_add,
                                                to_remove);


            for (const auto& uuid : to_add) {
                log_debug(GET_LOGGER("rest"), ctx.indent << "[" << static_cast<char>(ctx.mode) << "] " << "Adding physical drive " << uuid << " as sub-drive to drive " << parent_uuid);
                manager.add_entry(parent_uuid, uuid, ctx.agent->get_gami_id());
            }

            for (const auto& uuid : to_remove) {
                log_debug(GET_LOGGER("rest"), ctx.indent << "[" << static_cast<char>(ctx.mode) << "] " << "Removing physical drive " << uuid << " from drive " << parent_uuid);
                manager.remove_entry(parent_uuid, uuid);
            }
        }
    }

    /*!
     * @brief Overrides remove_agent_data() from GenericManager
     *
     * This override is necessary to properly clean the contents of
     * LogicalDrivesMembersManager
     * */
    void remove_agent_data(const std::string& gami_id) override {
        PhysicalDriveHandlerBase::remove_agent_data(gami_id);

        agent_framework::module::StorageManager::get_instance()->
            get_physical_drive_members_manager().clean_resources_for_agent(gami_id);
    }

    /*!
     * @brief removes a component and all its descendants from the model
     *
     * @param uuid component's uuid
     * */
    void remove(const std::string &uuid) override {
        // remove links where physical to remove is a child of other drive
        auto& physical_drive_members_manager = agent_framework::module::StorageManager::get_instance()->get_physical_drive_members_manager();
        physical_drive_members_manager.remove_if([&uuid](const std::string&, const std::string& child, const std::string&) {
            return child == uuid;
        });

        PhysicalDriveHandlerBase::remove(uuid);
    }
};

PhysicalDriveHandler::~PhysicalDriveHandler() {}

}
}
}
}

