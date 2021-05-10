#include "commands/docs_search.hpp"
#include "commands/download_voice_message.hpp"
#include "commands/hide_keyboard.hpp"
#include "commands/keyboard.hpp"
#include "commands/set_chat_photo.hpp"
#include "commands/upload_voice_message.hpp"
#include "cpp_vk_lib/config/loader.hpp"
#include "events/on_chat_invite_user.hpp"
#include "events/on_message_pin.hpp"
#include "long_poller/long_poller.hpp"


namespace bot {
// Example bot class.
// You can add handling other types of events.
class bot_object
{
public:
    void setup()
    {
        setup_commands();
        setup_event_reactions();
    }
    int run()
    {
        return poller.run();
    }

private:
    void setup_commands()
    {
        // DAZDADZDADZDADZDDADZDADDZDADDZ закрой тот пр
    }
    void setup_event_reactions()
    {
        poller.get_message_handler().on_message_pin(std::make_unique<event::on_message_pin_event>());
        poller.get_message_handler().on_chat_invite_user(std::make_unique<event::on_chat_invite_user_event>());
        // Other event reactions...
    }
    long_poller poller{};
};
}// namespace bot

int main()
{
    vk::config::load("/home/machen/TextData/configs/config.json");
    spdlog::info("Maximum num of workers: {}", vk::config::num_workers());
    bot::bot_object example;
    example.setup();
    return example.run();
}
