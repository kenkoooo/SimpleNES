#ifndef MAPPERCNROM_H
#define MAPPERCNROM_H
#include "Mapper.h"

namespace sn {
class MapperCNROM : public Mapper {
public:
  MapperCNROM(Cartridge &cart);
  void writePRG(Address addr, Byte value) override;
  Byte readPRG(Address addr) const override;
  const Byte *getPagePtr(Address addr) const override;

  Byte readCHR(Address addr) const override;
  void writeCHR(Address addr, Byte value) override;

private:
  bool m_oneBank;

  Address m_selectCHR;
};
} // namespace sn
#endif // MAPPERCNROM_H
