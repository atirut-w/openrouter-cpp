#pragma once
#include <curl/curl.h>
#include <string>

namespace openrouter {

class OpenRouter {
  CURL *curl = nullptr;
  curl_slist *headers = nullptr;

  std::string http_get(const std::string &url);
  std::string http_post(const std::string &url, const std::string &data);

public:
  OpenRouter(const std::string &api_key);
  ~OpenRouter();

  OpenRouter(const OpenRouter &) = delete;
  OpenRouter &operator=(const OpenRouter &) = delete;
};

} // namespace openrouter
