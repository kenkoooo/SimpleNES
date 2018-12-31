#include "MapperSxROM.h"
#include "Log.h"

namespace sn {
MapperSxROM::MapperSxROM(Cartridge &cart,
                         std::function<void(void)> mirroring_cb)
    : Mapper(cart, Mapper::SxROM),
      m_mirroringCallback(mirroring_cb),
      m_mirroring(Horizontal),
      m_modeCHR(0),
      m_modePRG(3),
      m_tempRegister(0),
      m_writeCounter(0),
      m_regPRG(0),
      m_regCHR0(0),
      m_regCHR1(0),
      m_firstBankPRG(0),
      m_secondBankPRG(0),
      m_firstBankCHR(0),
      m_secondBankCHR(0) {
  if (cart.getVROM().size() == 0) {
    m_usesCharacterRAM = true;
    m_characterRAM.resize(0x2000);
    LOG(Info) << "Uses character RAM" << std::endl;
  } else {
    LOG(Info) << "Using CHR-ROM" << std::endl;
    m_usesCharacterRAM = false;
    m_firstBankCHR = 0;
    m_secondBankCHR = 0x1000 * m_regCHR1;
  }

  m_firstBankPRG = 0;                               // first bank
  m_secondBankPRG = cart.getROM().size() - 0x4000;  // last bank
}

Byte MapperSxROM::readPRG(Address addr) const {
  if (addr < 0xc000) {
    return this->m_cartridge.getROM()[m_firstBankPRG + (addr & 0x3fff)];
  } else {
    return this->m_cartridge.getROM()[m_secondBankPRG + (addr & 0x3fff)];
  }
}

NameTableMirroring MapperSxROM::getNameTableMirroring() const {
  return m_mirroring;
}

void MapperSxROM::writePRG(Address addr, Byte value) {
  if (!(value & 0x80))  // if reset bit is NOT set
  {
    m_tempRegister = (m_tempRegister >> 1) | ((value & 1) << 4);
    ++m_writeCounter;

    if (m_writeCounter == 5) {
      if (addr <= 0x9fff) {
        switch (m_tempRegister & 0x3) {
          case 0:
            m_mirroring = OneScreenLower;
            break;
          case 1:
            m_mirroring = OneScreenHigher;
            break;
          case 2:
            m_mirroring = Vertical;
            break;
          case 3:
            m_mirroring = Horizontal;
            break;
        }
        m_mirroringCallback();

        m_modeCHR = (m_tempRegister & 0x10) >> 4;
        m_modePRG = (m_tempRegister & 0xc) >> 2;
        calculatePRGPointers();

        // Recalculate CHR pointers
        if (m_modeCHR == 0)  // one 8KB bank
        {
          m_firstBankCHR = 0x1000 * (m_regCHR0 | 1);  // ignore last bit
          m_secondBankCHR = m_firstBankCHR + 0x1000;
        } else  // two 4KB banks
        {
          m_firstBankCHR = 0x1000 * m_regCHR0;
          m_secondBankCHR = 0x1000 * m_regCHR1;
        }
      } else if (addr <= 0xbfff)  // CHR Reg 0
      {
        m_regCHR0 = m_tempRegister;
        m_firstBankCHR =
            0x1000 * (m_tempRegister | (1 - m_modeCHR));  // OR 1 if 8KB mode
        if (m_modeCHR == 0) m_secondBankCHR = m_firstBankCHR + 0x1000;
      } else if (addr <= 0xdfff) {
        m_regCHR1 = m_tempRegister;
        if (m_modeCHR == 1) m_secondBankCHR = 0x1000 * m_tempRegister;
      } else {
        // TODO PRG-RAM
        if ((m_tempRegister & 0x10) == 0x10) {
          LOG(Info) << "PRG-RAM activated" << std::endl;
        }

        m_tempRegister &= 0xf;
        m_regPRG = m_tempRegister;
        calculatePRGPointers();
      }

      m_tempRegister = 0;
      m_writeCounter = 0;
    }
  } else  // reset
  {
    m_tempRegister = 0;
    m_writeCounter = 0;
    m_modePRG = 3;
    calculatePRGPointers();
  }
}

void MapperSxROM::calculatePRGPointers() {
  if (m_modePRG <= 1)  // 32KB changeable
  {
    // equivalent to multiplying 0x8000 * (m_regPRG >> 1)
    m_firstBankPRG = 0x4000 * (m_regPRG & ~1);
    m_secondBankPRG = m_firstBankPRG + 0x4000;  // add 16KB
  } else if (m_modePRG == 2)                    // fix first switch second
  {
    m_firstBankPRG = 0;
    m_secondBankPRG = m_firstBankPRG + 0x4000 * m_regPRG;
  } else  // switch first fix second
  {
    m_firstBankPRG = 0x4000 * m_regPRG;
    m_secondBankPRG = m_cartridge.getROM().size() - 0x4000;
  }
}

Byte MapperSxROM::readCHR(Address addr) const {
  if (m_usesCharacterRAM) {
    return m_characterRAM[addr];
  } else if (addr < 0x1000) {
    return this->m_cartridge.getVROM()[m_firstBankCHR + addr];
  } else {
    return this->m_cartridge.getVROM()[m_secondBankCHR + (addr & 0xfff)];
  }
}

void MapperSxROM::writeCHR(Address addr, Byte value) {
  if (m_usesCharacterRAM) {
    m_characterRAM[addr] = value;
  } else {
    LOG(Info) << "Read-only CHR memory write attempt at " << std::hex << addr
              << std::endl;
  }
}
}  // namespace sn
