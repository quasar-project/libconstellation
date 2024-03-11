add_executable(${PROJECT_NAME}-bench)

target_sources(${PROJECT_NAME}-bench
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c++
)

if(UNIT_TESTS)
  target_link_libraries(${PROJECT_NAME}-bench
    PRIVATE
      ${PROJECT_NAME}
      GTest::gtest
      GTest::gtest_main
  )
else()
  target_link_libraries(${PROJECT_NAME}-bench
    PRIVATE
      ${PROJECT_NAME}
  )
endif()
