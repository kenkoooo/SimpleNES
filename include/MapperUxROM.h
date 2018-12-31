#ifndef MAPPERUXROM_H
#define MAPPERUXROM_H
#include "Mapper.h"

namespace sn {
class MapperUxROM : public Mapper {
 public:
  MapperUxROM(Cartridge &cart);
  void writePRG(Address addr, Byte value) override;
  Byte readPRG(Address addr) const override;

  Byte readCHR(Address addr) const override;
  void writeCHR(Address addr, Byte value) override;

 private:
  bool m_usesCharacterRAM;

  const Byte *m_lastBankPtr;
  Address m_selectPRG;

  std::vector<Byte> m_characterRAM;
};
}  // namespace sn
#endif  // MAPPERUXROM_H
