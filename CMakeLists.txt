cmake_minimum_required(VERSION 4.0.2)
project(crypto_games C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SODIUM REQUIRED libsodium)

include_directories(${SODIUM_INCLUDE_DIRS})
link_directories(${SODIUM_LIBRARY_DIRS})

add_executable(crypto_games
    main.c
    game1_xor.c
    game2_pow.c
    game3_merkletree.c
)

target_link_libraries(crypto_games ${SODIUM_LIBRARIES})
