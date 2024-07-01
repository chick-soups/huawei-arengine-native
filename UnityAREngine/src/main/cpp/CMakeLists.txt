# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)


# Declares and names the project.

project("UnityAREngine")

add_library(huawei_arengine_ndk SHARED IMPORTED)
set_target_properties(huawei_arengine_ndk PROPERTIES IMPORTED_LOCATION
        "${CMAKE_CURRENT_SOURCE_DIR}/huawei_arengine/libs/${ANDROID_ABI}/libhuawei_arengine_ndk.so")
 target_include_directories(huawei_arengine_ndk INTERFACE
        huawei_arengine/include)


add_library( # Sets the UnityAREngine
        UnityAREngine

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        unityarengine.cpp
        InputProvider.cpp
        InputLifecycleProivider.cpp
        UnityInterface/XRMath.cpp)

 target_include_directories(UnityAREngine PRIVATE
        huawei_arengine/include
         UnityInterface
         include)

target_link_libraries( # Specifies the target library.
        UnityAREngine
        huawei_arengine_ndk
        GLESv3
        android
        log
        mediandk
        )