cmake_minimum_required(VERSION 3.14.0)
project(mailbox VERSION 1.2.0)

include(cmake/bin2h.cmake)
include(cmake/verstring.cmake)

if(NOT TARGET devicetree)
    add_subdirectory(devicetree.resource EXCLUDE_FROM_ALL)
endif()

include(cmake/sfdc.cmake)

get_verstring(VERSTRING)
sfdc(sfd/mailbox_lib.sfd)

add_executable(mailbox.resource
    src/main.c
    src/mbox.c
    src/init.c
    src/rawcommand.c
    src/stringcommand.c
    src/allocatebuffer.c
    src/allocatememory.c
    src/blankscreen.c
    src/getalphamode.c
    src/getarmmemory.c
    src/getboardmacaddress.c
    src/getboardmodel.c
    src/getboardrevision.c
    src/getboardserial.c
    src/getclockrate.c
    src/getclockratemax.c
    src/getclockratemeasured.c
    src/getclockratemin.c
    src/getclockstate.c
    src/getcommandline.c
    src/getdepth.c
    src/getdmachannels.c
    src/getedidblock.c
    src/getfirmwarerevision.c
    src/getgpiostate.c
    src/getoverscan.c
    src/getpalette.c
    src/getphysicalsize.c
    src/getpitch.c
    src/getpixelorder.c
    src/getpowerstate.c
    src/gettemperature.c
    src/gettemperaturemax.c
    src/getturbo.c
    src/getvcmemory.c
    src/getvirtualoffset.c
    src/getvirtualsize.c
    src/getvoltage.c
    src/getvoltagemax.c
    src/getvoltagemin.c
    src/lockmemory.c
    src/releasebuffer.c
    src/releasememory.c
    src/setalphamode.c
    src/setclockrate.c
    src/setclockstate.c
    src/setdepth.c
    src/setdisplaynum.c
    src/setgpiostate.c
    src/setoverscan.c
    src/setpalette.c
    src/setphysicalsize.c
    src/setpixelorder.c
    src/setpowerstate.c
    src/setturbo.c
    src/setvirtualoffset.c
    src/setvirtualsize.c
    src/setvoltage.c
    src/unlockmemory.c
)

bin_to_header(mailbox.resource)

target_compile_options(mailbox.resource PRIVATE -Os -m68040 -msmall-code -fomit-frame-pointer)
target_compile_definitions(mailbox.resource PRIVATE VERSION_STRING="${VERSTRING}")
target_include_directories(mailbox.resource PRIVATE include)
target_link_options(mailbox.resource PRIVATE -ffreestanding -nostdlib -nostartfiles -s -Wl,-e_code_start -Wl,-T${CMAKE_CURRENT_SOURCE_DIR}/ldscript.lds)
target_link_libraries(mailbox.resource devicetree)

set_target_properties(mailbox.resource PROPERTIES LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/ldscript.lds)

add_executable(mailbox_test
    test/startup.c
    test/test.c
)

target_compile_options(mailbox_test PRIVATE -Os -m68040 -fomit-frame-pointer)
target_include_directories(mailbox_test PRIVATE include)
target_link_options(mailbox_test PRIVATE -ffreestanding -m68040 -nostdlib -nostartfiles -Wl,-e,__start)
target_link_libraries(mailbox_test amiga mailbox)

add_custom_target(mailbox_autodoc ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/Autodocs
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Autodocs/mailbox.doc ${CMAKE_CURRENT_BINARY_DIR}/Autodocs/mailbox.doc
    COMMENT "Copying Autodocs/mailbox.doc into build/"
)

add_custom_target(mailbox_sfd ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/SFD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sfd/mailbox_lib.sfd ${CMAKE_CURRENT_BINARY_DIR}/SFD/mailbox_lib.sfd
    COMMENT "Copying sfd/mailbox_lib.sfd into build/"
)

add_custom_target(mailbox_resources_header ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/include/resources
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/include/resources/mailbox.h ${CMAKE_CURRENT_BINARY_DIR}/include/resources/mailbox.h
    COMMENT "Copying include/resources/mailbox.h into build/"
)

# no install for devicetree.resource itself, it goes to ROM
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/sfd DESTINATION Developer)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include DESTINATION Developer)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/resources DESTINATION Developer/include)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Autodocs DESTINATION Developer)
