#include "utility/exception.hpp"
#include "net/network_client.hpp"
#include "api/long_poll_api.hpp"

#include "../../dependencies/logger/logger.hpp"


static void token_assertation(const simdjson::dom::object& obj)
{
    if (obj.at_key("error").is_object() && static_cast<std::int64_t>(obj["error"]["error_code"]) == 5)
    {
        VK_THROW(vk::exception::access_error, 5, "Failed to retrieve group id. Maybe, wrong access token was passed.");
    }
}

vk::long_poll_api::long_poll_api()
{
    std::string response = network_client.request(append_url("groups.getById"), group_params({ }));
    simdjson::dom::object obj = parser.parse(response);

    token_assertation(obj);
    _group_id = std::to_string(static_cast<long>(obj["response"].at(0)["id"]));

    logger(logflag::info) << "long polling started";
}

vk::long_poll_data vk::long_poll_api::server()
{
    std::string response =
    call("groups.getLongPollServer", group_params({
        {"group_id",   _group_id},
        {"random_id",  "0"}
    }));

    simdjson::dom::object server = parser.parse(response)["response"];

    return {
        static_cast<std::string>(server["key"]),
        static_cast<std::string>(server["server"]),
        static_cast<std::string>(server["ts"])
    };
}

static simdjson::dom::object get_updates(const vk::long_poll_data& data, std::size_t timeout)
{
    static vk::lib::network_client network_client;
    static simdjson::dom::parser parser;

    return
    parser.parse(
        network_client.request(data.server + '?', {
            {"act",     "a_check"},
            {"key",     data.key},
            {"ts",      data.ts},
            {"wait",    std::to_string(timeout)}
        })
    );
}

vk::long_poll_api::events_t vk::long_poll_api::listen(const long_poll_data& data, std::size_t timeout) const
{
    simdjson::dom::object raw_updates = get_updates(data, timeout);
    std::vector<std::unique_ptr<vk::event::common>> updates_list;

    for (simdjson::dom::element&& update : raw_updates["updates"].get_array())
    {
        updates_list.push_back(std::make_unique<vk::event::common>(
            static_cast<std::string>(raw_updates["ts"]),
            static_cast<std::string>(update["type"]),
            simdjson::to_string(update)
        ));
    }
    return updates_list;
}

void vk::long_poll_api::run(const std::size_t num_threads)
{
    thread_pool.start(num_threads);
}
