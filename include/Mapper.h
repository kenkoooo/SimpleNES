#ifndef MAPPER_H
#define MAPPER_H
#include <functional>
#include <memory>
#include "Cartridge.h"

namespace sn {
enum NameTableMirroring {
  Horizontal = 0,
  Vertical = 1,
  FourScreen = 8,
  OneScreenLower,
  OneScreenHigher,
};

class Mapper {
 public:
  enum Type {
    NROM = 0,
    SxROM = 1,
    UxROM = 2,
    CNROM = 3,
  };

  Mapper(Cartridge &cart, Type t) : m_cartridge(cart), m_type(t){};
  virtual void writePRG(Address addr, Byte value) = 0;
  virtual Byte readPRG(Address addr) const = 0;

  virtual Byte readCHR(Address addr) const = 0;
  virtual void writeCHR(Address addr, Byte value) = 0;

  virtual NameTableMirroring getNameTableMirroring() const;

  bool inline hasExtendedRAM() { return m_cartridge.hasExtendedRAM(); }

  static std::unique_ptr<Mapper> createMapper(
      Cartridge &cart, std::function<void(void)> mirroring_cb);

  Type m_type;

 protected:
  Cartridge &m_cartridge;
};
}  // namespace sn

#endif  // MAPPER_H
