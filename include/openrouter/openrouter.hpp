#pragma once
#include "openrouter/responses.hpp"
#include <curl/curl.h>
#include <optional>
#include <string>
#include <string_view>

namespace openrouter {

class OpenRouter {
  CURL *curl = nullptr;
  curl_slist *headers = nullptr;

  std::string http_get(const std::string &url);
  std::string http_post(const std::string &url, const std::string &data);

public:
  explicit OpenRouter(std::optional<std::string_view> api_key = std::nullopt);
  ~OpenRouter();

  OpenRouter(const OpenRouter &) = delete;
  OpenRouter &operator=(const OpenRouter &) = delete;

  Response create_response(const Request &request);
};

} // namespace openrouter
