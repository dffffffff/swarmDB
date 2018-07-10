// Copyright (c) 2017-2018 Bluzelle Networks
//
// This file is part of Bluzelle.
//
// Bluzelle is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <include/bluzelle.hpp>
#include <options/options.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <gtest/gtest.h>


namespace
{
    const char* NO_ARGS[] = {"config_tests"};

    const std::string TEST_CONFIG_FILE = "bluzelle.json";

    const std::string DEFAULT_CONFIG_DATA = "{\n"
        "  \"listener_address\" : \"0.0.0.0\",\n"
        "  \"listener_port\" : 49152,\n"
        "  \"ethereum\" : \"0x006eae72077449caca91078ef78552c0cd9bce8f\",\n"
        "  \"ethereum_io_api_token\" : \"ASDFASDFASDFASDFSDF\",\n"
        "  \"bootstrap_file\" : \"peers.json\",\n"
        "  \"bootstrap_url\"  : \"example.org/peers.json\",\n"
        "  \"debug_logging\" : true,"
        "  \"log_to_stdout\" : true,"
        "  \"state_dir\" : \"./daemon_state/\""
        "}";

    const auto DEFAULT_LISTENER = boost::asio::ip::tcp::endpoint{boost::asio::ip::address::from_string("0.0.0.0"), 49152};
}

using namespace ::testing;


// create default options and remove when done...
class options_file_test : public Test
{
public:
    options_file_test()
    {
        this->save_options_file(DEFAULT_CONFIG_DATA);
    }

    ~options_file_test()
    {
        unlink(TEST_CONFIG_FILE.c_str());
    }

    void
    save_options_file(const std::string& content)
    {
        unlink(TEST_CONFIG_FILE.c_str());
        std::ofstream ofile(TEST_CONFIG_FILE);
        ofile.exceptions(std::ios::failbit);
        ofile << content;
    }
};


TEST_F(options_file_test, test_that_loading_of_default_config_file)
{
    bzn::options options;
    options.parse_command_line(1, NO_ARGS);

    EXPECT_EQ("0x006eae72077449caca91078ef78552c0cd9bce8f", options.get_ethererum_address());
    EXPECT_EQ(DEFAULT_LISTENER, options.get_listener());
    ASSERT_EQ(true, options.get_debug_logging());
    ASSERT_EQ(true, options.get_log_to_stdout());
    EXPECT_EQ("./daemon_state/", options.get_state_dir());
    EXPECT_EQ("peers.json", options.get_bootstrap_peers_file());
    EXPECT_EQ("example.org/peers.json", options.get_bootstrap_peers_url());
    EXPECT_EQ((size_t)(2147483648), options.get_max_storage());

    // defaults..
    {
        bzn::options options;
        EXPECT_EQ("./.state/", options.get_state_dir());
    }
}


TEST(options, test_that_missing_default_config_throws_exception)
{
    bzn::options options;

    EXPECT_THROW(options.parse_command_line(1, NO_ARGS), std::runtime_error);
}


TEST_F(options_file_test, test_max_storage_parsing)
{
    std::string config_data{DEFAULT_CONFIG_DATA};

    bzn::message json;
    std::string errors;

    Json::CharReaderBuilder builder;
    Json::CharReader* reader = builder.newCharReader();
    reader->parse(
            DEFAULT_CONFIG_DATA.c_str()
            , DEFAULT_CONFIG_DATA.c_str() + DEFAULT_CONFIG_DATA.size()
            , &json
            , &errors);

    std::for_each(bzn::BYTE_SUFFIXES.cbegin()
            , bzn::BYTE_SUFFIXES.cend()
            , [&](const auto& p)
                  {
                      const size_t expected = 3 * 1099511627776; // 3TB in B
                      {
                          json["max_storage"] = boost::lexical_cast<std::string>(expected / p.second) + p.first;

                          this->save_options_file(json.toStyledString());

                          bzn::options options;
                          options.parse_command_line(1, NO_ARGS);

                          EXPECT_EQ(expected, options.get_max_storage());
                      }
                      {
                          std::string max_storage{boost::lexical_cast<std::string>(expected / p.second)};
                          max_storage = max_storage + p.first;
                          if(p.first!='B')
                          {
                              max_storage = max_storage.append("B");
                          }
                          json["max_storage"] = max_storage;

                          this->save_options_file(json.toStyledString());

                          bzn::options options;
                          options.parse_command_line(1, NO_ARGS);

                          EXPECT_EQ(expected, options.get_max_storage());
                      }
                  });

}

