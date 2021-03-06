/*
 * Copyright (c) 2015 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.intel.podm.business.dto.redfish;

import com.intel.podm.common.types.Id;
import com.intel.podm.common.types.Status;

import java.util.List;

public final class RemoteTargetDto {
    private final Id id;
    private final String name;
    private final String description;
    private final Status status;
    private final String type;
    private final List<RemoteTargetIscsiAddressDto> iscsiAddresses;
    private final String iscsiInitiatorIqn;

    private RemoteTargetDto(Builder builder) {
        id = builder.id;
        name = builder.name;
        description = builder.description;
        status = builder.status;
        type = builder.type;
        iscsiAddresses = builder.iscsiAddresses;
        iscsiInitiatorIqn = builder.iscsiInitiatorIqn;
    }

    public static Builder newBuilder() {
        return new Builder();
    }

    public Id getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public String getDescription() {
        return description;
    }

    public Status getStatus() {
        return status;
    }

    public String getType() {
        return type;
    }

    public List<RemoteTargetIscsiAddressDto> getIscsiAddresses() {
        return iscsiAddresses;
    }

    public String getIscsiInitiatorIqn() {
        return iscsiInitiatorIqn;
    }

    public static final class Builder {
        private Id id;
        private String name;
        private String description;
        private Status status;
        private String type;
        private List<RemoteTargetIscsiAddressDto> iscsiAddresses;
        private String iscsiInitiatorIqn;

        private Builder() {
        }

        public Builder id(Id val) {
            id = val;
            return this;
        }

        public Builder name(String val) {
            name = val;
            return this;
        }

        public Builder description(String val) {
            description = val;
            return this;
        }

        public Builder status(Status val) {
            status = val;
            return this;
        }

        public Builder type(String val) {
            type = val;
            return this;
        }

        public Builder iscsiAddresses(List<RemoteTargetIscsiAddressDto> val) {
            iscsiAddresses = val;
            return this;
        }

        public Builder iscsiInitiatorIqn(String val) {
            iscsiInitiatorIqn = val;
            return this;
        }

        public RemoteTargetDto build() {
            return new RemoteTargetDto(this);
        }
    }
}
