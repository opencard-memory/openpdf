cmake_minimum_required(VERSION 3.24)
project(OpenPDFOffice VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 6.6 REQUIRED COMPONENTS Widgets Pdf PdfWidgets PrintSupport Network)
qt_standard_project_setup()
qt_add_executable(OpenPDFOffice WIN32
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/Editor/IPdfEditorEngine.h
    src/Editor/SdkPluginEngine.h
    src/Editor/SdkPluginEngine.cpp
    src/Editor/OpenSourceEngine.h
    src/Editor/OpenSourceEngine.cpp
    src/Editor/AdvancedEditOperations.h
    src/Fonts/FontResolver.h
    src/Fonts/FontResolver.cpp
    src/Fonts/GoogleFontsApiConfig.h
    src/Fonts/GoogleFontsApiConfig.cpp
    src/Fonts/GoogleFontsApiClient.h
    src/Fonts/GoogleFontsApiClient.cpp
    src/Localization/LocaleManager.h
    src/Localization/LocaleManager.cpp
)
target_link_libraries(OpenPDFOffice PRIVATE
    Qt6::Widgets Qt6::Pdf Qt6::PdfWidgets Qt6::PrintSupport Qt6::Network
)
set_target_properties(OpenPDFOffice PROPERTIES
    WIN32_EXECUTABLE TRUE
    OUTPUT_NAME "OpenPDFOffice"
)
install(TARGETS OpenPDFOffice RUNTIME DESTINATION bin)
