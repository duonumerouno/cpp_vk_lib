#include "methods/groups.hpp"

#include "exception/error-inl.hpp"
#include "simdjson.h"

vk::method::groups::groups()
  : m_group_constructor()
  , m_parser(std::make_unique<simdjson::dom::parser>())
{}

vk::method::groups::~groups() = default;

std::int64_t vk::method::groups::get_by_id() const
{
    std::string raw_response = m_group_constructor
        .method("groups.getById")
        .execute();

    simdjson::dom::object response = m_parser->parse(raw_response);

    if (response.begin().key() == "error")
    {
        exception::dispatch_error_by_code(response["error"]["error_code"].get_int64(), exception::log_before_throw);
    }

    return response["response"].at(0)["id"];
}

simdjson::dom::object vk::method::groups::get_long_poll_server(std::int64_t group_id) const
{
    std::string raw_response = m_group_constructor
        .method("groups.getLongPollServer")
        .param("group_id", std::to_string(group_id))
        .param("random_id", "0")
        .execute();

    simdjson::dom::object response = m_parser->parse(raw_response);

    if (response.begin().key() == "error")
    {
        exception::dispatch_error_by_code(response["error"]["error_code"].get_int64(), exception::log_before_throw);
    }

    return response["response"];
}
