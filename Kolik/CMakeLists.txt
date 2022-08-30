add_executable(kolik
    ${KOLIK_INC}/main.cpp

    # --- COMMANDS --- #
    ${KOLIK_INC}/command/test.cpp
    ${KOLIK_INC}/command/help.cpp

    ${KOLIK_INC}/process/process.cpp

    ${KOLIK_INC}/save/save.cpp
)

target_include_directories(kolik PRIVATE ${KOLIK_INC})
target_link_libraries(kolik PRIVATE reproc++ 
                            PRIVATE nlohmann_json::nlohmann_json)