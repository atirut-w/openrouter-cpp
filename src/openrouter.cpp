#include "openrouter/openrouter.hpp"
#include <cstddef>
#include <format>
#include <iostream>
#include <nlohmann/json.hpp>

namespace openrouter {

static size_t write_callback(char *ptr, size_t size, size_t nmemb,
                             std::string *data) {
  data->append(ptr, size * nmemb);
  return size * nmemb;
}

std::string OpenRouter::http_get(const std::string &url) {
  std::string response;
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    throw std::runtime_error(curl_easy_strerror(res));
  }

  return response;
}

std::string OpenRouter::http_post(const std::string &url,
                                  const std::string &data) {
  std::string response;
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    throw std::runtime_error(curl_easy_strerror(res));
  }

  return response;
}

OpenRouter::OpenRouter(const std::string &api_key) {
  curl = curl_easy_init();

  headers = curl_slist_append(
      headers, std::format("Authorization: Bearer {}", api_key).c_str());
  headers = curl_slist_append(headers, "Content-Type: application/json");

  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
}

OpenRouter::~OpenRouter() {
  curl_easy_cleanup(curl);
  curl_slist_free_all(headers);
}

Response OpenRouter::create_response(const Request &request) {
  std::string request_body = nlohmann::json(request).dump();
  nlohmann::json json = nlohmann::json::parse(
      http_post("https://openrouter.ai/api/v1/responses", request_body));

  if (!json["error"].is_null()) {
    throw std::runtime_error(
        std::format("OpenRouter API error: {}",
                    json["error"]["message"].get<std::string>()));
  }

  return json.get<Response>();
}

} // namespace openrouter
