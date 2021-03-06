#ifndef VK_COMMON_DOCUMENT_H
#define VK_COMMON_DOCUMENT_H

#include "attachment/attachment.hpp"
#include "methods/utility/constructor.hpp"

namespace simdjson {
namespace dom {
class parser;
class object;
}// namespace dom
}// namespace simdjson

namespace vk {
namespace document {
/*!
 * @brief The base class for @ref vk::docs, @ref vk::photos and @ref vk::video.
 */
class common
{
public:
    explicit common();
    explicit common(std::string_view user_token);

    ~common();
    /*!
     * @brief Upload file to server.
     * @return parsed JSON response.
     */
    simdjson::dom::object upload(std::string_view filename, std::string_view server, std::string_view field_name) const;
    /*!
     * @brief Search attachments of requested type.
     * @param method    - method name (photos.search, video.search or docs.search)
     * @param query     - search query
     * @param count     - maximum count of documents to search
     * @return vector of attachments.
     */
    vk::attachment::attachments_t search(std::string_view method, std::string_view query, std::int64_t count) const;

private:
    std::shared_ptr<simdjson::dom::parser> m_parser;
    mutable method::group_constructor_proxy m_user_method;
    network::request_manager m_request_manager;
};
}// namespace document
}// namespace vk

#endif// VK_COMMON_DOCUMENT_H
