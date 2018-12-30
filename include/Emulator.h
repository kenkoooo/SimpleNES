#ifndef EMULATOR_H
#define EMULATOR_H
#include <SFML/Graphics.hpp>
#include <chrono>

#include "CPU.h"
#include "Controller.h"
#include "MainBus.h"
#include "PPU.h"
#include "PictureBus.h"

namespace sn {
using TimePoint = std::chrono::high_resolution_clock::time_point;

const int NESVideoWidth = ScanlineVisibleDots;
const int NESVideoHeight = VisibleScanlines;

class Emulator {
 public:
  Emulator() = delete;
  Emulator(Controller controller1, Controller controller2, Cartridge cartridge);
  void run();

 private:
  MainBus m_bus;
  PictureBus m_pictureBus;
  CPU m_cpu;
  PPU m_ppu;
  Cartridge m_cartridge;
  std::unique_ptr<Mapper> m_mapper;

  Controller m_controller1, m_controller2;

  sf::RenderWindow m_window;
};
}  // namespace sn
#endif  // EMULATOR_H
