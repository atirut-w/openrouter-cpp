#include "openrouter/responses.hpp"
#include "nlohmann/json.hpp"
#include <iostream>
#include <stdexcept>

namespace openrouter {

void to_json(nlohmann::json &j, const InputText &text) {
  j = nlohmann::json::object();
  j["type"] = "input_text";
  j["text"] = text.text;
}

void to_json(nlohmann::json &j, const InputImage &image) {
  j = nlohmann::json::object();
  j["type"] = "input_image";

  switch (image.detail) {
  case InputImage::Auto:
    j["detail"] = "auto";
    break;
  case InputImage::High:
    j["detail"] = "high";
    break;
  case InputImage::Low:
    j["detail"] = "low";
    break;
  default:
    throw std::runtime_error("Unknown InputImage detail enum value");
  }

  if (image.url) {
    j["image_url"] = *image.url;
  }
}

void to_json(nlohmann::json &j, const InputFile &file) {
  j = nlohmann::json::object();
  j["type"] = "input_file";

  if (file.id) {
    j["file_id"] = *file.id;
  }
  if (file.data) {
    j["file_data"] = *file.data;
  }
  if (file.filename) {
    j["filename"] = *file.filename;
  }
  if (file.url) {
    j["file_url"] = *file.url;
  }
}

void to_json(nlohmann::json &j, const InputAudio &audio) {
  j = nlohmann::json::object();
  j["type"] = "input_audio";

  j["input_audio"] = nlohmann::json::object();
  j["input_audio"]["data"] = audio.data;

  switch (audio.format) {
  case InputAudio::MP3:
    j["input_audio"]["format"] = "mp3";
    break;
  case InputAudio::WAV:
    j["input_audio"]["format"] = "wav";
    break;
  default:
    throw std::runtime_error("Unknown InputAudio format enum value");
  }
}

void to_json(nlohmann::json &j,
             const OpenResponsesEasyInputMessageContent &content) {
  std::visit([&j](auto &&arg) { j = arg; }, content);
}

void to_json(nlohmann::json &j, const OpenResponsesReasoning &reasoning) {
  j = nlohmann::json::object();
  j["id"] = reasoning.id;

  j["summary"] = nlohmann::json::array();
  for (const auto &item : reasoning.summary) {
    nlohmann::json part = nlohmann::json::object();
    part["type"] = "summary_text";
    part["text"] = item;
    j["summary"].push_back(part);
  }

  j["type"] = "reasoning";

  if (reasoning.content) {
    j["content"] = nlohmann::json::array();
    for (const auto &item : *reasoning.content) {
      nlohmann::json part;
      part["type"] = "reasoning_text";
      part["text"] = item;
      j["content"].push_back(part);
    }
  }

  if (reasoning.encrypted_content) {
    j["encrypted_content"] = *reasoning.encrypted_content;
  }

  if (reasoning.format) {
    if (*reasoning.format == OpenResponsesReasoning::Format::Unknown) {
      j["format"] = "unknown";
    } else if (*reasoning.format ==
               OpenResponsesReasoning::Format::OpenAIResponseV1) {
      j["format"] = "openai-response-v1";
    } else if (*reasoning.format ==
               OpenResponsesReasoning::Format::XAIResponseV1) {
      j["format"] = "xai-response-v1";
    } else if (*reasoning.format ==
               OpenResponsesReasoning::Format::AnthropicClaudeV1) {
      j["format"] = "anthropic-claude-v1";
    } else if (*reasoning.format ==
               OpenResponsesReasoning::Format::GoogleGeminiV1) {
      j["format"] = "google-gemini-v1";
    } else {
      throw std::runtime_error(
          "Unknown OpenResponsesReasoning format enum value");
    }
  }

  if (reasoning.signature) {
    j["signature"] = *reasoning.signature;
  }

  if (reasoning.status) {
    if (*reasoning.status == OpenResponsesReasoning::Status::Completed) {
      j["status"] = "completed";
    } else if (*reasoning.status ==
               OpenResponsesReasoning::Status::Incomplete) {
      j["status"] = "incomplete";
    } else if (*reasoning.status ==
               OpenResponsesReasoning::Status::InProgress) {
      j["status"] = "in_progress";
    } else {
      throw std::runtime_error(
          "Unknown OpenResponsesReasoning status enum value");
    }
  }
}

void to_json(nlohmann::json &j, const OpenResponsesEasyInputMessage &message) {
  j = nlohmann::json::object();

  switch (message.role) {
  case OpenResponsesEasyInputMessage::User:
    j["role"] = "user";
    break;
  case OpenResponsesEasyInputMessage::System:
    j["role"] = "system";
    break;
  case OpenResponsesEasyInputMessage::Assistant:
    j["role"] = "assistant";
    break;
  case OpenResponsesEasyInputMessage::Developer:
    j["role"] = "developer";
    break;
  default:
    throw std::runtime_error(
        "Unknown OpenResponsesEasyInputMessage role enum value");
  }

  j["content"] = nlohmann::json::array();
  for (const auto &item : message.content) {
    std::visit([&j](auto &&arg) { j["content"].push_back(arg); }, item);
  }

  j["type"] = "message";
}

void to_json(nlohmann::json &j,
             const OpenResponsesInputMessageItem &message_item) {
  j = nlohmann::json::object();

  switch (message_item.role) {
  case OpenResponsesInputMessageItem::User:
    j["role"] = "user";
    break;
  case OpenResponsesInputMessageItem::System:
    j["role"] = "system";
    break;
  case OpenResponsesInputMessageItem::Developer:
    j["role"] = "developer";
    break;
  default:
    throw std::runtime_error(
        "Unknown OpenResponsesInputMessageItem role enum value");
  }

  j["content"] = nlohmann::json::array();
  for (const auto &item : message_item.content) {
    nlohmann::json part;
    std::visit([&part](auto &&arg) { part = arg; }, item);
    j["content"].push_back(part);
  }

  if (message_item.id) {
    j["id"] = *message_item.id;
  }

  j["type"] = "message";
}

void to_json(nlohmann::json &j,
             const OpenResponsesFunctionToolCall &func_call) {
  j = nlohmann::json::object();
  j["type"] = "function_call";
  j["call_id"] = func_call.call_id;
  j["name"] = func_call.name;
  j["arguments"] = func_call.arguments;
  j["id"] = func_call.id;

  if (func_call.status) {
    if (*func_call.status == OpenResponsesFunctionToolCall::Status::Completed) {
      j["status"] = "completed";
    } else if (*func_call.status ==
               OpenResponsesFunctionToolCall::Status::Incomplete) {
      j["status"] = "incomplete";
    } else if (*func_call.status ==
               OpenResponsesFunctionToolCall::Status::InProgress) {
      j["status"] = "in_progress";
    } else {
      throw std::runtime_error(
          "Unknown OpenResponsesFunctionToolCall status enum value");
    }
  }
}

void to_json(nlohmann::json &j,
             const OpenResponsesFunctionCallOutput &func_call_output) {
  j = nlohmann::json::object();
  j["type"] = "function_call_output";
  j["call_id"] = func_call_output.call_id;
  j["output"] = func_call_output.output;

  if (func_call_output.id) {
    j["id"] = *func_call_output.id;
  }

  if (func_call_output.status) {
    if (*func_call_output.status ==
        OpenResponsesFunctionCallOutput::Status::Completed) {
      j["status"] = "completed";
    } else if (*func_call_output.status ==
               OpenResponsesFunctionCallOutput::Status::Incomplete) {
      j["status"] = "incomplete";
    } else if (*func_call_output.status ==
               OpenResponsesFunctionCallOutput::Status::InProgress) {
      j["status"] = "in_progress";
    } else {
      throw std::runtime_error(
          "Unknown OpenResponsesFunctionCallOutput status enum value");
    }
  }
}

void to_json(nlohmann::json &j, const ResponseOutputText &text) {
  j = nlohmann::json::object();
  j["type"] = "output_text";
  j["text"] = text.text;

  if (text.annotations) {
    j["annotations"] = nlohmann::json::array();
    for (const auto &ann : *text.annotations) {
      nlohmann::json ann_json;
      std::visit([&ann_json](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, ResponseOutputText::FileCitation>) {
          ann_json["type"] = "file_citation";
          ann_json["file_id"] = arg.file_id;
          ann_json["filename"] = arg.filename;
          ann_json["index"] = arg.index;
        } else if constexpr (std::is_same_v<T, ResponseOutputText::URLCitation>) {
          ann_json["type"] = "url_citation";
          ann_json["url"] = arg.url;
          ann_json["title"] = arg.title;
          ann_json["start_index"] = arg.start_index;
          ann_json["end_index"] = arg.end_index;
        } else if constexpr (std::is_same_v<T, ResponseOutputText::FilePath>) {
          ann_json["type"] = "file_path";
          ann_json["file_id"] = arg.file_id;
          ann_json["index"] = arg.index;
        } else {
          throw std::runtime_error("Unknown ResponseOutputText Annotation type");
        }
      }, ann);
      j["annotations"].push_back(ann_json);
    }
  }
}

void from_json(const nlohmann::json &j, ResponseOutputText &text) {
  text.text = j["text"].get<std::string>();

  for (const auto &ann : j["annotations"]) {
    std::string type = ann["type"].get<std::string>();
    if (type == "file_citation") {
      ResponseOutputText::FileCitation citation;
      citation.file_id = ann["file_id"].get<std::string>();
      citation.filename = ann["filename"].get<std::string>();
      citation.index = ann["index"].get<double>();
      text.annotations->push_back(citation);
    } else if (type == "url_citation") {
      ResponseOutputText::URLCitation citation;
      citation.url = ann["url"].get<std::string>();
      citation.title = ann["title"].get<std::string>();
      citation.start_index = ann["start_index"].get<double>();
      citation.end_index = ann["end_index"].get<double>();
      text.annotations->push_back(citation);
    } else if (type == "file_path") {
      ResponseOutputText::FilePath file_path;
      file_path.file_id = ann["file_id"].get<std::string>();
      file_path.index = ann["index"].get<double>();
      text.annotations->push_back(file_path);
    } else {
      throw std::runtime_error(
          std::format("Unknown ResponseOutputText Annotation type: {}", type));
    }
  }
}

void to_json(nlohmann::json &j, const OpenAIResponsesRefusalContent &refusal) {
  j = nlohmann::json::object();
  j["type"] = "refusal";
  j["refusal"] = refusal.refusal;
}

void from_json(const nlohmann::json &j,
               OpenAIResponsesRefusalContent &refusal) {
  refusal.refusal = j["refusal"].get<std::string>();
}

void to_json(nlohmann::json &j, const ResponsesOutputMessage &message) {
  j = nlohmann::json::object();

  for (const auto &item : message.content) {
    nlohmann::json part;
    std::visit([&part](auto &&arg) { part = arg; }, item);
    j["content"].push_back(part);
  }

  j["id"] = message.id;
  j["role"] = "assistant";
  j["type"] = "message";

  if (message.status) {
    if (*message.status == ResponsesOutputMessage::Status::Completed) {
      j["status"] = "completed";
    } else if (*message.status == ResponsesOutputMessage::Status::Incomplete) {
      j["status"] = "incomplete";
    } else if (*message.status == ResponsesOutputMessage::Status::InProgress) {
      j["status"] = "in_progress";
    } else {
      throw std::runtime_error(
          "Unknown ResponsesOutputMessage status enum value");
    }
  }
}

void from_json(const nlohmann::json &j, ResponsesOutputMessage &message) {
  for (const auto &item : j["content"]) {
    std::string type = item["type"].get<std::string>();
    if (type == "output_text") {
      message.content.push_back(item.get<ResponseOutputText>());
    } else if (type == "refusal") {
      message.content.push_back(item.get<OpenAIResponsesRefusalContent>());
    } else {
      throw std::runtime_error(
          std::format("Unknown OutputMessageContent type: {}", type));
    }
  }
}

void to_json(nlohmann::json &j, const ResponsesOutputItemReasoning &reasoning) {
  j = nlohmann::json::object();
  j["id"] = reasoning.id;

  j["summary"] = nlohmann::json::array();
  for (const auto &item : reasoning.summary) {
    nlohmann::json part = nlohmann::json::object();
    part["type"] = "summary_text";
    part["text"] = item;
    j["summary"].push_back(part);
  }

  j["type"] = "reasoning";

  if (reasoning.content) {
    j["content"] = nlohmann::json::array();
    for (const auto &item : *reasoning.content) {
      nlohmann::json part;
      part["type"] = "reasoning_text";
      part["text"] = item;
      j["content"].push_back(part);
    }
  }

  if (reasoning.encrypted_content) {
    j["encrypted_content"] = *reasoning.encrypted_content;
  }

  if (reasoning.status) {
    if (*reasoning.status == ResponsesOutputItemReasoning::Status::Completed) {
      j["status"] = "completed";
    } else if (*reasoning.status ==
               ResponsesOutputItemReasoning::Status::Incomplete) {
      j["status"] = "incomplete";
    } else if (*reasoning.status ==
               ResponsesOutputItemReasoning::Status::InProgress) {
      j["status"] = "in_progress";
    } else {
      throw std::runtime_error(
          "Unknown ResponsesOutputItemReasoning status enum value");
    }
  }
}

void from_json(const nlohmann::json &j,
               ResponsesOutputItemReasoning &reasoning) {
  reasoning.id = j["id"].get<std::string>();

  for (const auto &part : j["summary"]) {
    reasoning.summary.push_back(part["text"].get<std::string>());
  }

  if (!j["content"].is_null()) {
    reasoning.content = std::vector<std::string>();
    for (const auto &part : j["content"]) {
      reasoning.content->push_back(part["text"].get<std::string>());
    }
  }

  if (j.contains("encrypted_content") && !j["encrypted_content"].is_null()) {
    reasoning.encrypted_content = j["encrypted_content"].get<std::string>();
  }

  if (j.contains("status") && !j["status"].is_null()) {
    std::string status = j["status"].get<std::string>();
    if (status == "completed") {
      reasoning.status = ResponsesOutputItemReasoning::Status::Completed;
    } else if (status == "incomplete") {
      reasoning.status = ResponsesOutputItemReasoning::Status::Incomplete;
    } else if (status == "in_progress") {
      reasoning.status = ResponsesOutputItemReasoning::Status::InProgress;
    } else {
      throw std::runtime_error(std::format(
          "Unknown ResponsesOutputItemReasoning status: {}", status));
    }
  }
}

void to_json(nlohmann::json &j,
             const ResponsesOutputItemFunctionCall &function_call) {
  j = nlohmann::json::object();
  j["arguments"] = function_call.arguments;
  j["call_id"] = function_call.call_id;
  j["name"] = function_call.name;
  j["type"] = "function_call";

  if (function_call.id) {
    j["id"] = *function_call.id;
  }

  if (function_call.status) {
    if (*function_call.status == ResponsesOutputItemFunctionCall::Status::Completed) {
      j["status"] = "completed";
    } else if (*function_call.status ==
               ResponsesOutputItemFunctionCall::Status::Incomplete) {
      j["status"] = "incomplete";
    } else if (*function_call.status ==
               ResponsesOutputItemFunctionCall::Status::InProgress) {
      j["status"] = "in_progress";
    } else {
      throw std::runtime_error(
          "Unknown ResponsesOutputItemFunctionCall status enum value");
    }
  }
}

void from_json(const nlohmann::json &j,
               ResponsesOutputItemFunctionCall &function_call) {
  function_call.arguments = j["arguments"].get<std::string>();
  function_call.call_id = j["call_id"].get<std::string>();
  function_call.name = j["name"].get<std::string>();

  if (!j["id"].is_null()) {
    function_call.id = j["id"].get<std::string>();
  }

  if (!j["status"].is_null()) {
    std::string status = j["status"].get<std::string>();
    if (status == "completed") {
      function_call.status = ResponsesOutputItemFunctionCall::Status::Completed;
    } else if (status == "incomplete") {
      function_call.status =
          ResponsesOutputItemFunctionCall::Status::Incomplete;
    } else if (status == "in_progress") {
      function_call.status =
          ResponsesOutputItemFunctionCall::Status::InProgress;
    } else {
      throw std::runtime_error(std::format(
          "Unknown ResponsesOutputItemFunctionCall status: {}", status));
    }
  }
}

void to_json(nlohmann::json &j,
             const ResponsesWebSearchCallOutput &web_search_call) {
  j = nlohmann::json::object();
  j["id"] = web_search_call.id;

  switch (web_search_call.status) {
  case ResponsesWebSearchCallOutput::Status::Completed:
    j["status"] = "completed";
    break;
  case ResponsesWebSearchCallOutput::Status::Searching:
    j["status"] = "searching";
    break;
  case ResponsesWebSearchCallOutput::Status::InProgress:
    j["status"] = "in_progress";
    break;
  case ResponsesWebSearchCallOutput::Status::Failed:
    j["status"] = "failed";
    break;
  default:
    throw std::runtime_error(
        "Unknown ResponsesWebSearchCallOutput status enum value");
  }

  j["type"] = "web_search_call";
}

void from_json(const nlohmann::json &j,
               ResponsesWebSearchCallOutput &web_search_call) {
  web_search_call.id = j["id"].get<std::string>();

  std::string status = j["status"].get<std::string>();
  if (status == "completed") {
    web_search_call.status = ResponsesWebSearchCallOutput::Status::Completed;
  } else if (status == "searching") {
    web_search_call.status = ResponsesWebSearchCallOutput::Status::Searching;
  } else if (status == "in_progress") {
    web_search_call.status = ResponsesWebSearchCallOutput::Status::InProgress;
  } else if (status == "failed") {
    web_search_call.status = ResponsesWebSearchCallOutput::Status::Failed;
  } else {
    throw std::runtime_error(
        std::format("Unknown ResponsesWebSearchCallOutput status: {}", status));
  }
}

void to_json(nlohmann::json &j,
             const ResponsesOutputItemFileSearchCall &file_search_call) {
  j = nlohmann::json::object();
  j["id"] = file_search_call.id;

  j["queries"] = nlohmann::json::array();
  for (const auto &query : file_search_call.queries) {
    j["queries"].push_back(query);
  }

  switch (file_search_call.status) {
  case ResponsesOutputItemFileSearchCall::Status::Completed:
    j["status"] = "completed";
    break;
  case ResponsesOutputItemFileSearchCall::Status::Searching:
    j["status"] = "searching";
    break;
  case ResponsesOutputItemFileSearchCall::Status::InProgress:
    j["status"] = "in_progress";
    break;
  case ResponsesOutputItemFileSearchCall::Status::Failed:
    j["status"] = "failed";
    break;
  default:
    throw std::runtime_error(
        "Unknown ResponsesOutputItemFileSearchCall status enum value");
  }

  j["type"] = "file_search_call";
}

void from_json(const nlohmann::json &j,
               ResponsesOutputItemFileSearchCall &file_search_call) {
  file_search_call.id = j["id"].get<std::string>();

  for (const auto &query : j["queries"]) {
    file_search_call.queries.push_back(query.get<std::string>());
  }

  std::string status = j["status"].get<std::string>();
  if (status == "completed") {
    file_search_call.status =
        ResponsesOutputItemFileSearchCall::Status::Completed;
  } else if (status == "searching") {
    file_search_call.status =
        ResponsesOutputItemFileSearchCall::Status::Searching;
  } else if (status == "in_progress") {
    file_search_call.status =
        ResponsesOutputItemFileSearchCall::Status::InProgress;
  } else if (status == "failed") {
    file_search_call.status = ResponsesOutputItemFileSearchCall::Status::Failed;
  } else {
    throw std::runtime_error(std::format(
        "Unknown ResponsesOutputItemFileSearchCall status: {}", status));
  }
}

void to_json(nlohmann::json &j,
             const ResponsesImageGenerationCall &image_generation_call) {
  j = nlohmann::json::object();
  j["id"] = image_generation_call.id;

  switch (image_generation_call.status) {
  case ResponsesImageGenerationCall::Status::InProgress:
    j["status"] = "in_progress";
    break;
  case ResponsesImageGenerationCall::Status::Completed:
    j["status"] = "completed";
    break;
  case ResponsesImageGenerationCall::Status::Generating:
    j["status"] = "generating";
    break;
  case ResponsesImageGenerationCall::Status::Failed:
    j["status"] = "failed";
    break;
  default:
    throw std::runtime_error(
        "Unknown ResponsesImageGenerationCall status enum value");
  }

  if (image_generation_call.result) {
    j["result"] = *image_generation_call.result;
  }
}

void from_json(const nlohmann::json &j,
               ResponsesImageGenerationCall &image_generation_call) {
  image_generation_call.id = j["id"].get<std::string>();

  std::string status = j["status"].get<std::string>();
  if (status == "in_progress") {
    image_generation_call.status =
        ResponsesImageGenerationCall::Status::InProgress;
  } else if (status == "completed") {
    image_generation_call.status =
        ResponsesImageGenerationCall::Status::Completed;
  } else if (status == "generating") {
    image_generation_call.status =
        ResponsesImageGenerationCall::Status::Generating;
  } else if (status == "failed") {
    image_generation_call.status = ResponsesImageGenerationCall::Status::Failed;
  } else {
    throw std::runtime_error(
        std::format("Unknown ResponsesImageGenerationCall status: {}", status));
  }

  if (!j["result"].is_null()) {
    image_generation_call.result = j["result"].get<std::string>();
  }
}

void to_json(nlohmann::json &j, const OpenResponsesInput &input) {
  std::visit([&j](auto &&arg) { j = arg; }, input);
}

void to_json(nlohmann::json &j, const Request &req) {
  j = nlohmann::json::object();

  if (req.input) {
    std::visit([&j](auto &&arg) { j["input"] = arg; }, *req.input);
  }

  if (req.model) {
    j["model"] = *req.model;
  }
}

void from_json(const nlohmann::json &j, Response &resp) {
  if (!j["output"].is_null()) {
    resp.output = std::vector<std::variant<
        ResponsesOutputMessage, ResponsesOutputItemReasoning,
        ResponsesOutputItemFunctionCall, ResponsesWebSearchCallOutput,
        ResponsesOutputItemFileSearchCall, ResponsesImageGenerationCall>>();

    for (const auto &item : j["output"]) {
      std::string type = item["type"].get<std::string>();
      if (type == "message") {
        resp.output->push_back(item.get<ResponsesOutputMessage>());
      } else if (type == "reasoning") {
        resp.output->push_back(item.get<ResponsesOutputItemReasoning>());
      } else if (type == "function_call") {
        resp.output->push_back(item.get<ResponsesOutputItemFunctionCall>());
      } else if (type == "web_search_call") {
        resp.output->push_back(item.get<ResponsesWebSearchCallOutput>());
      } else if (type == "file_search_call") {
        resp.output->push_back(item.get<ResponsesOutputItemFileSearchCall>());
      } else if (type == "image_generation_call") {
        resp.output->push_back(item.get<ResponsesImageGenerationCall>());
      } else {
        throw std::runtime_error(
            std::format("Unknown OutputItem type: {}", type));
      }
    }
  }
}

} // namespace openrouter
