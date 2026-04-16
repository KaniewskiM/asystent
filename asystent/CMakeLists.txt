cmake_minimum_required(VERSION 3.16)
project(asystent VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Szukamy niezbędnych modułów Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets Concurrent Charts Network)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        taxcalculator.cpp
        taxcalculator.h
        utils.cpp
        utils.h
        cryptoexchange.cpp
        cryptoexchange.h
        llmclient.cpp
        llmclient.h
        tabassistant.cpp
        tabassistant.h
        tabnews.cpp
        tabnews.h
        tabcharts.cpp
        tabcharts.h
        tabtaxes.cpp
        tabtaxes.h
        tabanalysis.cpp
        tabanalysis.h
)

qt_add_executable(asystent ${PROJECT_SOURCES})

# Skopiuj skrypt Pythona automatycznie do folderu budowania
configure_file(generate_chart.py generate_chart.py COPYONLY)

# Linkowanie bibliotek - Charts jest kluczowe dla wykresów
target_link_libraries(asystent PRIVATE
    Qt6::Widgets
    Qt6::Concurrent
    Qt6::Charts
    Qt6::Network
)

set_target_properties(asystent PROPERTIES
    WIN32_EXECUTABLE TRUE
)