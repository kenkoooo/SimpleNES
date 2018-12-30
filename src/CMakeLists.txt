include_directories(../include/)

add_library(Cartridge SHARED Cartridge.cpp)
target_link_libraries(Cartridge Log)

add_library(Controller SHARED Controller.cpp)
add_library(Log SHARED Log.cpp)
add_library(VirtualScreen SHARED VirtualScreen.cpp) 

add_library(MainBus SHARED MainBus.cpp)
target_link_libraries(MainBus PPU Mapper Log)

add_library(Mapper SHARED Mapper.cpp MapperCNROM.cpp MapperNROM.cpp MapperSxROM.cpp MapperUxROM.cpp)
target_link_libraries(Mapper Cartridge Log)

add_library(PictureBus SHARED PictureBus.cpp)
target_link_libraries(PictureBus Mapper Cartridge)

add_library(CPU SHARED CPU.cpp)
target_link_libraries(CPU MainBus Log)

add_library(Emulator SHARED Emulator.cpp)
target_link_libraries(Emulator CPU Controller MainBus PPU PictureBus Log)

add_library(PPU SHARED PPU.cpp)
target_link_libraries(PPU VirtualScreen PictureBus Log)
