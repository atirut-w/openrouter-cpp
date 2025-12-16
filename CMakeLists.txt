project(openrouter-cpp)

add_library(openrouter STATIC
    src/openrouter.cpp
)
target_include_directories(openrouter PUBLIC include)
target_link_libraries(openrouter PRIVATE curl)
target_compile_features(openrouter PRIVATE cxx_std_23)
target_compile_options(openrouter PRIVATE -Wall -Wextra)
