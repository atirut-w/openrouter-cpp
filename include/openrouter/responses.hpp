#pragma once
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace openrouter {

struct InputText {
  std::string text;
};

struct InputImage {
  enum Detail {
    Auto,
    High,
    Low,
  };

  Detail detail;
  std::optional<std::string> url;
};

struct InputFile {
  std::optional<std::string> id;
  std::optional<std::string> data;
  std::optional<std::string> filename;
  std::optional<std::string> url;
};

struct InputAudio {
  enum Format {
    MP3,
    WAV,
  };

  Format format;
  std::string data;
};

using OpenResponsesEasyInputMessageContent =
    std::variant<InputText, InputImage, InputFile, InputAudio>;

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

struct OpenAIResponsesRefusalContent {
  std::string refusal;
};

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

using OpenResponsesInput =
    std::variant<OpenResponsesReasoning, OpenResponsesEasyInputMessage,
                 OpenResponsesInputMessageItem, OpenResponsesFunctionToolCall,
                 OpenResponsesFunctionCallOutput, ResponsesOutputMessage,
                 ResponsesOutputItemReasoning, ResponsesOutputItemFunctionCall,
                 ResponsesWebSearchCallOutput,
                 ResponsesOutputItemFileSearchCall,
                 ResponsesImageGenerationCall>;

struct Request {
  std::optional<std::variant<std::string, std::vector<OpenResponsesInput>>>
      input;
};

struct Response {
  std::optional<std::vector<std::variant<
      ResponsesOutputMessage, ResponsesOutputItemReasoning,
      ResponsesOutputItemFunctionCall, ResponsesWebSearchCallOutput,
      ResponsesOutputItemFileSearchCall, ResponsesImageGenerationCall>>>
      output;
};

} // namespace openrouter
