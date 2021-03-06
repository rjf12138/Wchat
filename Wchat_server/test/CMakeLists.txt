project(server_test)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(server_test server_test.cc)
add_executable(sqlite3_test sqlite3_test.cc)
add_executable(user_group_manager_test user_group_manager_test.cc)
add_executable(msg_buffer_test msg_buffer_test.cc)
add_executable(msg_handle_test msg_handle_test.cc)
add_executable(print_database print_database.cc)
add_executable(msg_tansmission_test msg_transmission_test.cc)
add_executable(client_test client_test.cc)
add_executable(wchat_server wchat_server.cc)

target_link_libraries(server_test serverlog serverjson account_manager)
target_link_libraries(sqlite3_test sqlitewrapper serverlog serverjson)
target_link_libraries(user_group_manager_test user_manager group_manager sqlitewrapper serverlog serverjson)
target_link_libraries(msg_buffer_test msg_buffer queue user_manager sqlitewrapper serverlog serverjson)
target_link_libraries(msg_handle_test account_manager server_task msg_handler msg_buffer queue user_manager group_manager sqlitewrapper serverlog serverjson pthread connect_manager)
target_link_libraries(print_database sqlitewrapper serverlog serverjson)
target_link_libraries(msg_tansmission_test account_manager server_task msg_handler msg_buffer queue user_manager group_manager sqlitewrapper serverlog serverjson pthread msg_transmission server_signal connect_manager)
target_link_libraries(client_test serverlog serverjson pthread server_signal)
target_link_libraries(wchat_server account_manager server_task msg_handler msg_buffer queue user_manager group_manager sqlitewrapper serverlog serverjson pthread msg_transmission server_signal connect_manager thread_pool)