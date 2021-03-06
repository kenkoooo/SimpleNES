#include "MapperUxROM.h"
#include "Log.h"

namespace sn {
MapperUxROM::MapperUxROM(Cartridge &cart)
    : Mapper(cart, Mapper::UxROM), m_selectPRG(0) {
  this->m_usesCharacterRAM = cart.getVROM().size() == 0;
  if (this->m_usesCharacterRAM) {
    m_characterRAM.resize(0x2000);
    LOG(Info) << "Uses character RAM" << std::endl;
  }

  m_lastBankPtr = cart.getROM().size() - 0x4000;  // last - 16KB
}

Byte MapperUxROM::readPRG(Address addr) const {
  if (addr < 0xc000) {
    return m_cartridge
        .getROM()[((addr - 0x8000) & 0x3fff) | (m_selectPRG << 14)];
  } else {
    return this->m_cartridge.getROM()[m_lastBankPtr + (addr & 0x3fff)];
  }
}

void MapperUxROM::writePRG(Address addr, Byte value) { m_selectPRG = value; }

Byte MapperUxROM::readCHR(Address addr) const {
  if (m_usesCharacterRAM) {
    return m_characterRAM[addr];
  } else {
    return m_cartridge.getVROM()[addr];
  }
}

void MapperUxROM::writeCHR(Address addr, Byte value) {
  if (m_usesCharacterRAM) {
    m_characterRAM[addr] = value;
  } else {
    LOG(Info) << "Read-only CHR memory write attempt at " << std::hex << addr
              << std::endl;
  }
}
}  // namespace sn