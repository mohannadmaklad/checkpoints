include(ExternalProject)
ExternalProject_Add( dlt-viewer
GIT_REPOSITORY    https://github.com/COVESA/dlt-viewer.git
GIT_TAG           master
PATCH_COMMAND     cp -r ${CMAKE_CURRENT_SOURCE_DIR}/checkpointsplugin <SOURCE_DIR>/plugin && git apply  ${CMAKE_CURRENT_SOURCE_DIR}/patches/add_checkpoints_subdir.patch
INSTALL_COMMAND sudo make install
)