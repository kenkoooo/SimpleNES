#ifndef MAPPERNROM_H
#define MAPPERNROM_H
#include "Mapper.h"

namespace sn {
class MapperNROM : public Mapper {
 public:
  MapperNROM(Cartridge &cart);
  void writePRG(Address addr, Byte value) override;
  Byte readPRG(Address addr) const override;

  Byte readCHR(Address addr) const override;
  void writeCHR(Address addr, Byte value) override;

 private:
  bool m_oneBank;
  bool m_usesCharacterRAM;

  std::vector<Byte> m_characterRAM;
};
}  // namespace sn
#endif  // MAPPERNROM_H
