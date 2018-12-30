include_directories(../include/)

add_library(Cartridge Cartridge.cpp)
target_link_libraries(Cartridge Log)

add_library(Controller Controller.cpp)

add_library(CPU CPU.cpp)
target_link_libraries(CPU MainBus Log)

add_library(Emulator Emulator.cpp)
target_link_libraries(Emulator CPU Controller MainBus PPU PictureBus Log)

add_library(Log Log.cpp)

add_library(MainBus MainBus.cpp)
target_link_libraries(MainBus Cartridge Mapper Log)

add_library(Mapper Mapper.cpp)
target_link_libraries(Mapper Cartridge MapperCNROM MapperNROM MapperSxROM MapperUxROM Log)

add_library(MapperCNROM MapperCNROM.cpp)
target_link_libraries(MapperCNROM Mapper Log)

add_library(MapperNROM MapperNROM.cpp)
target_link_libraries(MapperNROM Mapper Log)

add_library(MapperSxROM MapperSxROM.cpp)
target_link_libraries(MapperSxROM Mapper Log)

add_library(MapperUxROM MapperUxROM.cpp)
target_link_libraries(MapperUxROM Mapper Log)

add_library(PictureBus PictureBus.cpp)
target_link_libraries(PictureBus Mapper Cartridge)

add_library(PPU PPU.cpp)
target_link_libraries(PPU VirtualScreen PictureBus Log)

add_library(VirtualScreen VirtualScreen.cpp) 