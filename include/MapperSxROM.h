#ifndef MAPPERSXROM_H
#define MAPPERSXROM_H
#include "Mapper.h"

namespace sn {
class MapperSxROM : public Mapper {
public:
  MapperSxROM(Cartridge &cart, std::function<void(void)> mirroring_cb);
  void writePRG(Address addr, Byte value) override;
  Byte readPRG(Address addr) const override;
  const Byte *getPagePtr(Address addr) const override;

  Byte readCHR(Address addr) const override;
  void writeCHR(Address addr, Byte value) override;

  NameTableMirroring getNameTableMirroring() override;

private:
  void calculatePRGPointers();

  std::function<void(void)> m_mirroringCallback;
  NameTableMirroring m_mirroring;

  bool m_usesCharacterRAM;
  int m_modeCHR;
  int m_modePRG;

  Byte m_tempRegister;
  int m_writeCounter;

  Byte m_regPRG;
  Byte m_regCHR0;
  Byte m_regCHR1;

  const Byte *m_firstBankPRG;
  const Byte *m_secondBankPRG;

  const Byte *m_firstBankCHR;
  const Byte *m_secondBankCHR;

  std::vector<Byte> m_characterRAM;
};
} // namespace sn
#endif // MAPPERSXROM_H
