#include "Mapper.h"
#include "MapperCNROM.h"
#include "MapperNROM.h"
#include "MapperSxROM.h"
#include "MapperUxROM.h"

namespace sn {
NameTableMirroring Mapper::getNameTableMirroring() const {
  return static_cast<NameTableMirroring>(m_cartridge.getNameTableMirroring());
}

std::unique_ptr<Mapper> Mapper::createMapper(
    sn::Cartridge &cart, std::function<void(void)> mirroring_cb) {
  Mapper::Type mapper_t = static_cast<Mapper::Type>(cart.getMapper());
  std::unique_ptr<Mapper> ret(nullptr);
  switch (mapper_t) {
    case NROM:
      ret.reset(new MapperNROM(cart));
      break;
    case SxROM:
      ret.reset(new MapperSxROM(cart, mirroring_cb));
      break;
    case UxROM:
      ret.reset(new MapperUxROM(cart));
      break;
    case CNROM:
      ret.reset(new MapperCNROM(cart));
      break;
    default:
      break;
  }
  return ret;
}
}  // namespace sn