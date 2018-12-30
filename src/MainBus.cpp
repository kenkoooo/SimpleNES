#include "MainBus.h"
#include "Log.h"
#include <cstring>

namespace sn {
MainBus::MainBus() : m_RAM(0x800, 0), m_mapper(nullptr) {}
void MainBus::set_cpu_callback(std::function<void()> f) {
  this->cpu_callback = f;
}
void MainBus::set_ppu(PPU *ppu) { this->ppu = ppu; }

Byte MainBus::read_callback(IORegisters reg) const {
  switch (reg) {
  case PPUSTATUS:
    return this->ppu->getStatus();
  case PPUDATA:
    return this->ppu->getData();
  case JOY1:
    return this->controller1->read();
  case JOY2:
    return this->controller2->read();
  case OAMDATA:
    return this->ppu->getOAMData();
  default:
    LOG(InfoVerbose) << "No read callback registered for I/O register at: "
                     << std::hex << +reg << std::endl;
    return 0;
  }
}

Byte MainBus::read(Address addr) const {
  if (addr < 0x2000) {
    return m_RAM[addr & 0x7ff];
  } else if (addr < 0x4020) {
    if (addr < 0x4000) { // PPU registers, mirrored
      IORegisters reg = static_cast<IORegisters>(addr & 0x2007);
      return this->read_callback(reg);
    } else if (addr < 0x4018 && addr >= 0x4014) { // Only *some* IO registers
      IORegisters reg = static_cast<IORegisters>(addr);
      return this->read_callback(reg);
    } else
      LOG(InfoVerbose) << "Read access attempt at: " << std::hex << +addr
                       << std::endl;
  } else if (addr < 0x6000) {
    LOG(InfoVerbose)
        << "Expansion ROM read attempted. This is currently unsupported"
        << std::endl;
  } else if (addr < 0x8000) {
    if (m_mapper->hasExtendedRAM()) {
      return m_extRAM[addr - 0x6000];
    }
  } else {
    return m_mapper->readPRG(addr);
  }
  return 0;
}

void MainBus::write_callback(IORegisters reg, Byte b) {
  switch (reg) {
  case PPUCTRL:
    this->ppu->control(b);
    break;
  case PPUMASK:
    this->ppu->setMask(b);
    break;
  case OAMADDR:
    this->ppu->setOAMAddress(b);
    break;
  case PPUADDR:
    this->ppu->setDataAddress(b);
    break;
  case PPUSCROL:
    this->ppu->setScroll(b);
    break;
  case PPUDATA:
    this->ppu->setData(b);
    break;
  case OAMDMA: {
    this->cpu_callback();
    const auto *page_ptr = this->getPagePtr(b);
    this->ppu->doDMA(page_ptr);
  } break;
  case JOY1:
    this->controller1->strobe(b);
    this->controller2->strobe(b);
    break;
  case OAMDATA:
    this->ppu->setOAMData(b);
    break;
  default:
    LOG(InfoVerbose) << "No write callback registered for I/O register at: "
                     << std::hex << +reg << std::endl;
    break;
  }
}

void MainBus::write(Address addr, Byte value) {
  if (addr < 0x2000) {
    m_RAM[addr & 0x7ff] = value;
  } else if (addr < 0x4020) {
    if (addr < 0x4000) { // PPU registers, mirrored
      IORegisters reg = static_cast<IORegisters>(addr & 0x2007);
      this->write_callback(reg, value);
    } else if (addr < 0x4017 && addr >= 0x4014) { // only some registers
      IORegisters reg = static_cast<IORegisters>(addr);
      this->write_callback(reg, value);
    } else {
      LOG(InfoVerbose) << "Write access attmept at: " << std::hex << +addr
                       << std::endl;
    }
  } else if (addr < 0x6000) {
    LOG(InfoVerbose)
        << "Expansion ROM access attempted. This is currently unsupported"
        << std::endl;
  } else if (addr < 0x8000) {
    if (m_mapper->hasExtendedRAM()) {
      m_extRAM[addr - 0x6000] = value;
    }
  } else {
    m_mapper->writePRG(addr, value);
  }
}

const Byte *MainBus::getPagePtr(Byte page) {
  Address addr = page << 8;
  if (addr < 0x2000)
    return &m_RAM[addr & 0x7ff];
  else if (addr < 0x4020) {
    LOG(Error) << "Register address memory pointer access attempt" << std::endl;
  } else if (addr < 0x6000) {
    LOG(Error) << "Expansion ROM access attempted, which is unsupported"
               << std::endl;
  } else if (addr < 0x8000) {
    if (m_mapper->hasExtendedRAM()) {
      return &m_extRAM[addr - 0x6000];
    }
  } else {
  }
  return nullptr;
}

bool MainBus::setMapper(Mapper *mapper) {
  m_mapper = mapper;

  if (!mapper) {
    LOG(Error) << "Mapper pointer is nullptr" << std::endl;
    return false;
  }

  if (mapper->hasExtendedRAM())
    m_extRAM.resize(0x2000);

  return true;
}

void MainBus::set_controller(Controller *controller1, Controller *controller2) {
  this->controller1 = controller1;
  this->controller2 = controller2;
}

}; // namespace sn
