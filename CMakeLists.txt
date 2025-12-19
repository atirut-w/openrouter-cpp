project(openrouter-cpp)
add_subdirectory(3rd/json)

add_library(openrouter STATIC
    src/openrouter.cpp
    src/responses.cpp
)
target_include_directories(openrouter PUBLIC include)
target_link_libraries(openrouter PRIVATE curl nlohmann_json::nlohmann_json)
target_compile_features(openrouter PRIVATE cxx_std_23)
target_compile_options(openrouter PRIVATE -Wall -Wextra)
