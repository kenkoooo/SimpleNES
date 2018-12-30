#ifndef MEMORY_H
#define MEMORY_H
#include "Controller.h"
#include "Mapper.h"
#include "PPU.h"
#include <functional>
#include <map>
#include <memory>
#include <vector>

namespace sn {
enum IORegisters {
  PPUCTRL = 0x2000,
  PPUMASK,
  PPUSTATUS,
  OAMADDR,
  OAMDATA,
  PPUSCROL,
  PPUADDR,
  PPUDATA,
  OAMDMA = 0x4014,
  JOY1 = 0x4016,
  JOY2 = 0x4017,
};

class MainBus {
public:
  MainBus();
  Byte read(Address addr) const;
  void write(Address addr, Byte value);
  bool setMapper(Mapper *mapper);
  void set_write_callback(std::function<void(IORegisters, Byte)>);
  const Byte *getPagePtr(Byte page);

  void set_ppu(PPU *ppu);
  void set_controller(Controller *controller1, Controller *controller2);
  PPU *ppu;

private:
  Byte read_callback(IORegisters reg) const;
  std::vector<Byte> m_RAM;
  std::vector<Byte> m_extRAM;
  Mapper *m_mapper;
  Controller *controller1;
  Controller *controller2;

  std::function<void(IORegisters, Byte)> write_callback;
  ;
};
}; // namespace sn

#endif // MEMORY_H
