#pragma once
#include "nlohmann/json_fwd.hpp"
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace openrouter {

struct InputText {
  std::string text;
};

void to_json(nlohmann::json &j, const InputText &text);

struct InputImage {
  enum Detail {
    Auto,
    High,
    Low,
  };

  Detail detail;
  std::optional<std::string> url;
};

void to_json(nlohmann::json &j, const InputImage &image);

struct InputFile {
  std::optional<std::string> id;
  std::optional<std::string> data;
  std::optional<std::string> filename;
  std::optional<std::string> url;
};

void to_json(nlohmann::json &j, const InputFile &file);

struct InputAudio {
  enum Format {
    MP3,
    WAV,
  };

  Format format;
  std::string data;
};

void to_json(nlohmann::json &j, const InputAudio &audio);

using OpenResponsesEasyInputMessageContent =
    std::variant<InputText, InputImage, InputFile, InputAudio>;

void to_json(nlohmann::json &j,
             const OpenResponsesEasyInputMessageContent &content);

struct OpenResponsesReasoning {
  enum Format {
    Unknown,
    OpenAIResponseV1,
    XAIResponseV1,
    AnthropicClaudeV1,
    GoogleGeminiV1,
  };

  enum Status {
    Completed,
    Incomplete,
    InProgress,
  };

  std::string id;
  std::vector<std::string> summary;
  std::optional<std::vector<std::string>> content;
  std::optional<std::string> encrypted_content;
  std::optional<Format> format;
  std::optional<std::string> signature;
  std::optional<Status> status;
};

void to_json(nlohmann::json &j, const OpenResponsesReasoning &reasoning);

struct OpenResponsesEasyInputMessage {
  enum Role {
    User,
    System,
    Assistant,
    Developer,
  };

  Role role;
  std::vector<std::variant<OpenResponsesEasyInputMessageContent, std::string>>
      content;
};

void to_json(nlohmann::json &j, const OpenResponsesEasyInputMessage &message);

struct OpenResponsesInputMessageItem {
  enum Role {
    User,
    System,
    Developer,
  };

  Role role;
  std::vector<OpenResponsesEasyInputMessageContent> content;
  std::optional<std::string> id;
};

void to_json(nlohmann::json &j,
             const OpenResponsesInputMessageItem &message_item);

struct OpenResponsesFunctionToolCall {
  enum Status {
    InProgress,
    Completed,
    Incomplete,
  };

  std::string call_id;
  std::string name;
  std::string arguments;
  std::string id;
  std::optional<Status> status;
};

void to_json(nlohmann::json &j, const OpenResponsesFunctionToolCall &func_call);

struct OpenResponsesFunctionCallOutput {
  enum Status {
    InProgress,
    Completed,
    Incomplete,
  };

  std::string call_id;
  std::string output;
  std::optional<std::string> id;
  std::optional<Status> status;
};

void to_json(nlohmann::json &j,
             const OpenResponsesFunctionCallOutput &func_call_output);

struct ResponseOutputText {
  struct FileCitation {
    std::string file_id;
    std::string filename;
    double index;
  };

  struct URLCitation {
    std::string url;
    std::string title;
    double start_index;
    double end_index;
  };

  struct FilePath {
    std::string file_id;
    double index;
  };

  using Annotation = std::variant<FileCitation, URLCitation, FilePath>;

  std::string text;
  std::optional<std::vector<Annotation>> annotations;
};

void to_json(nlohmann::json &j, const ResponseOutputText &text);
void from_json(const nlohmann::json &j, ResponseOutputText &text);

struct OpenAIResponsesRefusalContent {
  std::string refusal;
};

void to_json(nlohmann::json &j, const OpenAIResponsesRefusalContent &refusal);
void from_json(const nlohmann::json &j, OpenAIResponsesRefusalContent &refusal);

struct ResponsesOutputMessage {
  enum Status {
    Completed,
    Incomplete,
    InProgress,
  };

  std::vector<std::variant<ResponseOutputText, OpenAIResponsesRefusalContent>>
      content;
  std::string id;
  std::optional<Status> status;
};

void to_json(nlohmann::json &j, const ResponsesOutputMessage &message);
void from_json(const nlohmann::json &j, ResponsesOutputMessage &message);

struct ResponsesOutputItemReasoning {
  enum Status {
    Completed,
    Incomplete,
    InProgress,
  };

  std::string id;
  std::vector<std::string> summary;
  std::optional<std::vector<std::string>> content;
  std::optional<std::string> encrypted_content;
  std::optional<Status> status;
};

void to_json(nlohmann::json &j, const ResponsesOutputItemReasoning &reasoning);
void from_json(const nlohmann::json &j,
               ResponsesOutputItemReasoning &reasoning);

struct ResponsesOutputItemFunctionCall {
  enum Status {
    InProgress,
    Completed,
    Incomplete,
  };

  std::string arguments;
  std::string call_id;
  std::string name;
  std::optional<std::string> id;
  std::optional<Status> status;
};

void to_json(nlohmann::json &j,
             const ResponsesOutputItemFunctionCall &function_call);
void from_json(const nlohmann::json &j,
               ResponsesOutputItemFunctionCall &function_call);

struct ResponsesWebSearchCallOutput {
  enum Status {
    Completed,
    Searching,
    InProgress,
    Failed,
  };

  std::string id;
  Status status;
};

void to_json(nlohmann::json &j,
             const ResponsesWebSearchCallOutput &web_search_call);
void from_json(const nlohmann::json &j,
               ResponsesWebSearchCallOutput &web_search_call);

struct ResponsesOutputItemFileSearchCall {
  enum Status {
    Completed,
    Searching,
    InProgress,
    Failed,
  };

  std::string id;
  std::vector<std::string> queries;
  Status status;
};

void to_json(nlohmann::json &j,
             const ResponsesOutputItemFileSearchCall &file_search_call);
void from_json(const nlohmann::json &j,
               ResponsesOutputItemFileSearchCall &file_search_call);

struct ResponsesImageGenerationCall {
  enum Status {
    InProgress,
    Completed,
    Generating,
    Failed,
  };

  std::string id;
  Status status;
  std::optional<std::string> result;
};

void to_json(nlohmann::json &j,
             const ResponsesImageGenerationCall &image_generation_call);
void from_json(const nlohmann::json &j,
               ResponsesImageGenerationCall &image_generation_call);

using OpenResponsesInput =
    std::variant<OpenResponsesReasoning, OpenResponsesEasyInputMessage,
                 OpenResponsesInputMessageItem, OpenResponsesFunctionToolCall,
                 OpenResponsesFunctionCallOutput, ResponsesOutputMessage,
                 ResponsesOutputItemReasoning, ResponsesOutputItemFunctionCall,
                 ResponsesWebSearchCallOutput,
                 ResponsesOutputItemFileSearchCall,
                 ResponsesImageGenerationCall>;

void to_json(nlohmann::json &j, const OpenResponsesInput &input);

struct Request {
  std::optional<std::variant<std::string, std::vector<OpenResponsesInput>>>
      input;
  std::optional<std::string> model;
};

void to_json(nlohmann::json &j, const Request &req);

struct Response {
  std::optional<std::vector<std::variant<
      ResponsesOutputMessage, ResponsesOutputItemReasoning,
      ResponsesOutputItemFunctionCall, ResponsesWebSearchCallOutput,
      ResponsesOutputItemFileSearchCall, ResponsesImageGenerationCall>>>
      output;
};

void from_json(const nlohmann::json &j, Response &resp);

} // namespace openrouter
