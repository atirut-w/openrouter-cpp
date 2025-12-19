#include "openrouter/responses.hpp"
#include "nlohmann/json.hpp"
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
  // TODO
}

} // namespace openrouter
