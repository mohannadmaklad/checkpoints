add_library(checkpointsplugin MODULE checkpointsPlugin.cpp addSequenceDialog.cpp form.cpp sequences.cpp resources.qrc)

target_link_libraries(checkpointsplugin qdlt ${QT_PREFIX}::Widgets)

add_plugin(checkpointsplugin)
