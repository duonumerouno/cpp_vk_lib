#include "simdjson.h"

#include "processing/exception.hpp"
#include "events/message_new.hpp"


vk::event::message_new::message_new(std::int64_t peer_id, std::int64_t from_id, std::string_view text, std::string_view raw_json, const simdjson::dom::array& attachments) {
    this->_peer_id  = peer_id;
    this->_from_id  = from_id;
    this->_text     = text;
    this->_raw_json = raw_json;

    _attachments = att_handler.try_get(attachments);
}

vk::event::message_new::message_new(std::string_view raw_json) {
    simdjson::dom::parser parser;
    simdjson::dom::object message = parser.parse(raw_json)["object"]["message"];

    _raw_json  = raw_json;
    _from_id   = message["from_id"].get_int64();
    _peer_id   = message["peer_id"].get_int64();
    _text      = message["text"].get_c_str();

    if (message["reply_message"].is_object())
        try_get_reply(message["reply_message"].get_object());

    if (message["attachments"].is_array())
        _attachments = att_handler.try_get(message["attachments"].get_array());

    if (message["fwd_messages"].get_array().size() != 0)
        try_get_fwd_messages(message["fwd_messages"].get_array());

    if (message["action"].is_object())
        try_get_actions(message["action"].get_object());
}

void vk::event::message_new::try_get_actions(const simdjson::dom::object& action) {
    std::string action_name = action["type"].get_string().take_value().data();
    if (action_name == "chat_invite_user") {
        _action = std::make_shared<action::action_chat_invite_user>(
            action["member_id"].get_int64()
        );
    }
    if (action_name == "chat_kick_user") {
        _action = std::make_shared<action::action_chat_kick_user>(
            action["member_id"].get_int64()
        );
    }
    if (action_name == "chat_pin_message") {
        _action = std::make_shared<action::action_chat_pin_message>(
            action["member_id"].get_int64(),
            action["conversation_message_id"].get_int64(),
            action["message"].get_string()
        );
    }
    if (action_name == "chat_unpin_message") {
        _action = std::make_shared<action::action_chat_unpin_message>(
            action["member_id"].get_int64(),
            action["conversation_message_id"].get_int64()
        );
    }
    if (action_name == "chat_photo_update") {
        _action = std::make_shared<action::action_chat_photo_update>(
            // empty
        );
    }
    if (action_name == "chat_title_update") {
        _action = std::make_shared<action::action_chat_title_update>(
            action["text"].get_string()
        );
    }
    _has_action = true;
}

void vk::event::message_new::try_get_fwd_messages(const simdjson::dom::array& messages) {
    for (const simdjson::dom::element& fwd_message : messages) {
        _fwd_messages.push_back(
            std::make_shared<message_new>(
                fwd_message["peer_id"].get_int64(),
                fwd_message["from_id"].get_int64(),
                fwd_message["text"].get_string(),
                simdjson::to_string(fwd_message),
                fwd_message["attachments"].get_array()
            )
        );
    }
    _has_fwd_messages = true;
}

void vk::event::message_new::try_get_reply(const simdjson::dom::object& object) {
    _reply = std::make_shared<message_new>(
        object["peer_id"].get_int64(),
        object["from_id"].get_int64(),
        object["text"].get_string(),
        simdjson::to_string(object),
        object["attachments"].get_array()
    );
    _has_reply = true;
}

vk::attachment::attachments_t vk::event::message_new::attachments() const {
    return _attachments;
}
std::string vk::event::message_new::text()      const noexcept { return _text; }
std::int64_t vk::event::message_new::from_id()  const noexcept { return _from_id; }
std::int64_t vk::event::message_new::peer_id()  const noexcept { return _peer_id; }
bool vk::event::message_new::has_action()       const noexcept { return _has_action; }
bool vk::event::message_new::has_reply()        const noexcept { return _has_reply; }
bool vk::event::message_new::has_fwd_messages() const noexcept { return _has_fwd_messages; }
std::string vk::event::message_new::dump()      const noexcept { return _raw_json; }

vk::event::message_new::any_action vk::event::message_new::action() const {
    if (vk_likely(_has_action)) return _action;
    else vk_throw(exception::access_error, -1, "Attempting accessing empty action.");
}
std::vector<std::shared_ptr<vk::event::message_new>> vk::event::message_new::fwd_messages() const {
    if (vk_likely(_has_fwd_messages)) return _fwd_messages;
    else vk_throw(exception::access_error, -1, "Attempting accessing empty forward message list.");
}
vk::event::message_new vk::event::message_new::reply() const {
    if (vk_likely(_has_reply)) return *_reply;
    else vk_throw(exception::access_error, -1, "Attempting accessing empty reply.");
}
