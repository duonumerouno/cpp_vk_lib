#ifndef VK_MESSAGE_CONSTRUCTOR_H
#define VK_MESSAGE_CONSTRUCTOR_H

#include <string>
#include <map>
#include <numeric>

#include "attachment/attachment.hpp"


namespace vk {
struct message_constructor {
public:
  message_constructor(const message_constructor&) = default;
  message_constructor(message_constructor&&) = default;
  message_constructor& operator=(const message_constructor&) = default;
  message_constructor& operator=(message_constructor&&) = default;
 ~message_constructor() = default;
  using parameter_t = std::map <std::string, std::string>;

  explicit message_constructor() {
    params.emplace("random_id",        "0");
    params.emplace("disable_mentions", "1");
  }
  void append(std::pair<std::string, std::string>&& pair) {
    params.emplace(std::move(pair));
  }
  void append_map(parameter_t&& additional_params) {
    params.merge(std::move(additional_params));
  }
  void append_attachments(vk::attachment::attachments_t&& attachments) {
    params.emplace("attachment", append_attachments_impl(std::move(attachments)).data());
  }
  parameter_t&& consume_map() noexcept {
    return std::move(params);
  }
  parameter_t map() const noexcept {
    return params;
  }
private:
  std::string append_attachments_impl(vk::attachment::attachments_t&& attachments) const {
    return std::accumulate(
      attachments.begin(), attachments.end(), std::string(),
        [](std::string& res, std::shared_ptr<vk::attachment::base> att) mutable {
            return res += att->value() + ',';
        }
    );
  }
  parameter_t params;
};
} // namespace vk

#endif // VK_MESSAGE_CONSTRUCTOR_H