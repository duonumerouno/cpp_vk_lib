#ifndef VK_ERROR_HANDLER_H
#define VK_ERROR_HANDLER_H

#include "exception/exception.hpp"


namespace simdjson {
namespace dom {
class object;
} // namespace dom
} // namespace simdjson

namespace vk {
namespace processing {

std::pair<std::int64_t, std::string> extract_from_json(const simdjson::dom::object& error_object);
void rethrow_and_log(std::string_view label, std::exception_ptr exception_ptr);

template <typename _Error_t>
std::exception_ptr try_construct_exception_ptr(const std::pair<std::int64_t, std::string>& error) {
  std::exception_ptr exception_ptr =
    std::make_exception_ptr(_Error_t(error.first, error.second.c_str()));
  return exception_ptr;
}

template <typename _Error_t>
void log_and_throw(std::string_view label, const simdjson::dom::object& error_object) {
  std::exception_ptr exception_ptr = try_construct_exception_ptr<_Error_t>(
    extract_from_json(error_object));
  rethrow_and_log(label, exception_ptr);
  throw std::current_exception();
}
template <typename _Error_t>
void log_and_throw(std::string_view label, std::string_view error_message) {
  std::exception_ptr exception_ptr = try_construct_exception_ptr<_Error_t>(
    {-1, error_message.data()});
  rethrow_and_log(label, exception_ptr);
  throw std::current_exception();
}
template <typename _Error_t>
void log_only(std::string_view label, const simdjson::dom::object& error_object) {
  std::exception_ptr exception_ptr = try_construct_exception_ptr<_Error_t>(
    extract_from_json(error_object));
  rethrow_and_log(label, exception_ptr);
}
template <typename _Error_t>
void log_only(std::string_view label, std::string_view error_message) {
  std::exception_ptr exception_ptr = try_construct_exception_ptr<_Error_t>(
    {-1, error_message});
  rethrow_and_log(label, exception_ptr);
}

} // namespace processing
} // namespace vk

#endif // VK_ERROR_HANDLER_H