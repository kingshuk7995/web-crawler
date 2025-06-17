cmake_minimum_required(VERSION 3.5)
project(WebCrawler_test CXX)

add_executable(${PROJECT_NAME}
 test_main.cc
 test_parser.cc
)

target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

ParseAndAddDrogonTests(${PROJECT_NAME})
