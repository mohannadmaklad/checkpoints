add_library(checkpointsplugin MODULE checkpointsPlugin.cpp addSequenceDialog.cpp form.cpp sequences.cpp resources.qrc)

find_package(${QT_PREFIX} COMPONENTS Concurrent REQUIRED)

target_link_libraries(checkpointsplugin qdlt ${QT_PREFIX}::Widgets  ${QT_PREFIX}::Concurrent)

add_plugin(checkpointsplugin)
