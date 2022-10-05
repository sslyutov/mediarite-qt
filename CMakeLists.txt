cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

# set the project name
project(igcryptor VERSION 1.0.0.1)

configure_file(igcryptorConfig.h.in igcryptorConfig.h)

add_executable( igcryptor 
    source/main.cpp
    source/igcryptor.cpp 
    #source/igcryptoengine.cpp
    )

set(HEADERS
    include/igcryptor.h
    #include/igcryptoengine.h 
    )

target_include_directories(igcryptor
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries( igcryptor LINK_PUBLIC 
    ../../../Libraries/igcrypto/binary/Release/igcrypto
)

target_include_directories( igcryptor PUBLIC
    "../../Libraries/igcrypto/include"
)

#SET_TARGET_PROPERTIES(igcryptor PROPERTIES LINK_FLAGS "/versionflags")


