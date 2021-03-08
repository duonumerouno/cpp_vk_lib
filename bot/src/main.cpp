#include <unordered_map>

#include "commands/docs_search.hpp"
#include "commands/keyboard.hpp"
#include "commands/hide_keyboard.hpp"
#include "events/on_message_pin.hpp"
#include "events/on_chat_invite_user.hpp"
#include "long_poller/long_poller.hpp"


namespace bot {
// Example bot class.
// You can add handling other types of events.
class bot_object {
public:
  void setup() {
    setup_commands();
    setup_event_reactions();
  }
  int run() {
    return poller.run();
  }
private:
  void setup_commands() {
    poller.get_message_handler().on_command("doc", std::make_unique<command::docs_search_command>());
    poller.get_message_handler().on_command("show", std::make_unique<command::keyboard_command>());
    poller.get_message_handler().on_command("hide", std::make_unique<command::hide_keyboard_command>());
    // Other commands...
  }
  void setup_event_reactions() {
    poller.get_message_handler().on_message_pin(std::make_unique<event::on_message_pin_event>());
    poller.get_message_handler().on_chat_invite_user(std::make_unique<event::on_chat_invite_user_event>());
    // Other event reactions...
  }
  long_poller poller;
};
} // namespace bot

int main() {
  vk::config::load("/path/to/config.json");
  bot::bot_object example;
  example.setup();
  return example.run();
}