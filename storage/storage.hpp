// Copyright (C) 2018 Bluzelle
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License, version 3,
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <include/bluzelle.hpp>
#include <storage/storage_base.hpp>
#include <node/node_base.hpp>
#include <unordered_map>
#include <shared_mutex>
#include <boost/serialization/unordered_map.hpp>


namespace bzn
{
    class storage : public bzn::storage_base
    {
    public:

        storage_base::result create(const bzn::uuid_t& uuid, const std::string& key, const std::string& value) override;

        std::shared_ptr<bzn::storage_base::record> read(const bzn::uuid_t& uuid, const std::string& key) override;

        storage_base::result update(const bzn::uuid_t& uuid, const std::string& key, const std::string& value) override;

        storage_base::result remove(const bzn::uuid_t& uuid, const std::string& key) override;

        storage_base::result save(const std::string& path) override;

        storage_base::result load(const std::string& path) override;

        std::vector<std::string> get_keys(const bzn::uuid_t& uuid) override;

        bool has(const bzn::uuid_t& uuid, const  std::string& key) override;

        std::size_t get_size(const bzn::uuid_t& uuid) override;

        std::size_t get_total_size() override;

    private:
        friend class boost::serialization::access;

        bzn::uuid_t generate_random_uuid();

        std::unordered_map<bzn::uuid_t, std::unordered_map<std::string, std::shared_ptr<bzn::storage_base::record>>> kv_store;

        std::shared_mutex lock; // for multi-reader and single writer access
    };

} // bzn
