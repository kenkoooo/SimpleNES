#ifndef MAPPERCNROM_H
#define MAPPERCNROM_H
#include "Mapper.h"

namespace sn {
class MapperCNROM : public Mapper {
public:
  MapperCNROM(Cartridge &cart);
  void writePRG(Address addr, Byte value);
  Byte readPRG(Address addr) const;
  const Byte *getPagePtr(Address addr);

  Byte readCHR(Address addr) const;
  void writeCHR(Address addr, Byte value);

private:
  bool m_oneBank;

  Address m_selectCHR;
};
} // namespace sn
#endif // MAPPERCNROM_H
