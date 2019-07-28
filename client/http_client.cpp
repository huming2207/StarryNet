#include <string>
#include <sstream>

#include "http_client.hpp"

using namespace snet::client;

std::string http_client::make_field(const std::map<std::string, std::string> &field)
{
    // Make it to a post field string
    std::stringstream post_field;
    for(auto& item : field) {
        post_field << item.first << "=" << item.second << "&";
    }

    // Make it a string
    auto field_str = post_field.str();

    // Remove the last '&'
    field_str.pop_back();

    return field_str;
}

std::string http_client::make_field(const std::vector<std::tuple<std::string, std::string>> &field)
{
    // Make it to a post field string
    std::stringstream post_field;
    for(auto& item : field) {
        post_field << std::get<0>(item) << "=" << std::get<1>(item) << "&";
    }

    // Make it a string
    auto field_str = post_field.str();

    // Remove the last '&'
    field_str.pop_back();

    return field_str;
}