#include "MapperCNROM.h"
#include "Log.h"

namespace sn {
MapperCNROM::MapperCNROM(Cartridge &cart)
    : Mapper(cart, Mapper::CNROM), m_selectCHR(0) {
  m_oneBank = cart.getROM().size() == 0x4000;
}

Byte MapperCNROM::readPRG(Address addr) const {
  if (!m_oneBank)
    return m_cartridge.getROM()[addr - 0x8000];
  else  // mirrored
    return m_cartridge.getROM()[(addr - 0x8000) & 0x3fff];
}

void MapperCNROM::writePRG(Address addr, Byte value) {
  m_selectCHR = value & 0x3;
}

Byte MapperCNROM::readCHR(Address addr) const {
  return m_cartridge.getVROM()[addr | (m_selectCHR << 13)];
}

void MapperCNROM::writeCHR(Address addr, Byte value) {
  LOG(Info) << "Read-only CHR memory write attempt at " << std::hex << addr
            << std::endl;
}
}  // namespace sn