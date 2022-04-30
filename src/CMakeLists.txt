project(main)

add_executable(main main.cpp)
# target_link_libraries(main thostmduserapi_se)
target_link_libraries(main PRIVATE thostmduserapi_se thosttraderapi_se spdlog nanomsg boost_locale)

add_executable(market_pub market_pub.cpp)
target_link_libraries(market_pub PRIVATE thostmduserapi_se thosttraderapi_se spdlog nanomsg boost_locale)

add_executable(trader trader.cpp)
target_link_libraries(trader PRIVATE thostmduserapi_se thosttraderapi_se spdlog nanomsg boost_locale)

