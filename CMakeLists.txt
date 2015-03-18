cmake_minimum_required(VERSION 2.6)
project(ascii-paint)

include_directories(include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(CMAKE_BUILD_TYPE DEBUG)

ADD_EXECUTABLE(ascii-paint
    app.cpp
    app_data.cpp
    app_gui.cpp
    app_user.cpp
    asc_file.cpp
    cell_operation.cpp
    color_box.cpp
    export_operation.cpp
    fill_operation.cpp
    fov_operation.cpp
    get_string_message_box.cpp
    import_operation.cpp
    line_operation.cpp
    main.cpp
    message_box.cpp
    misc.cpp
    new_operation.cpp
    open_operation.cpp
    operation.cpp
    oval_operation.cpp
    path_operation.cpp
    pick_operation.cpp
    quit_operation.cpp
    rect_operation.cpp
    redo_operation.cpp
    save_operation.cpp
    text_operation.cpp
    undo_operation.cpp
)

#target_link_libraries(ascii-paint tcod tcodxx tcodgui)
target_link_libraries(ascii-paint tcod)

INSTALL(TARGETS ascii-paint
    DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}
)
