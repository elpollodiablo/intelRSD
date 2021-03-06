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
 *
 * @file command/chassis/set_component_attributes.hpp
 * @brief Implementation of generic SetComponentAttributes command
 * */

#include "agent-framework/command/chassis/set_component_attributes.hpp"

using namespace agent_framework::command::chassis;

constexpr const char SetComponentAttributes::AGENT[];
constexpr const char SetComponentAttributes::TAG[];

SetComponentAttributes::~SetComponentAttributes() {}

SetComponentAttributes::Request::~Request() {}

SetComponentAttributes::Response::~Response() {}
