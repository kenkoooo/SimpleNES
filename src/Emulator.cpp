#include "Emulator.h"
#include "Log.h"

#include <chrono>
#include <memory>
#include <thread>

namespace sn {
const float ScreenScale = 2.f;
const std::chrono::nanoseconds CpuCycleDuration = std::chrono::nanoseconds(559);
Emulator::Emulator(Controller controller1, Controller controller2,
                   Cartridge cartridge)
    : m_cpu(m_bus),
      m_controller1(controller1),
      m_controller2(controller2),
      m_cartridge(cartridge) {
  m_bus.set_cpu_callback([&]() { m_cpu.skipDMACycles(); });
  m_ppu.setInterruptCallback([&]() { m_cpu.interrupt(CPU::NMI); });
}

void Emulator::run() {
  this->m_bus.set_ppu(&this->m_ppu);
  this->m_bus.set_controller(&this->m_controller1, &this->m_controller2);

  m_mapper = Mapper::createMapper(
      m_cartridge, [&]() { this->m_ppu.m_bus.updateMirroring(); });
  if (!m_mapper) {
    LOG(Error) << "Creating Mapper failed. Probably unsupported." << std::endl;
    return;
  }

  if (!m_bus.setMapper(m_mapper.get()) ||
      !this->m_ppu.m_bus.setMapper(m_mapper.get()))
    return;

  m_cpu.reset();

  m_window.create(
      sf::VideoMode(NESVideoWidth * ScreenScale, NESVideoHeight * ScreenScale),
      "SimpleNES", sf::Style::Titlebar | sf::Style::Close);
  m_window.setVerticalSyncEnabled(true);
  m_ppu.m_screen.create(NESVideoWidth, NESVideoHeight, ScreenScale,
                        sf::Color::White);

  TimePoint cycleTimer = std::chrono::high_resolution_clock::now();

  std::chrono::high_resolution_clock::duration elapsedTime =
      cycleTimer - cycleTimer;

  sf::Event event;
  bool focus = true, pause = false;
  while (m_window.isOpen()) {
    while (m_window.pollEvent(event)) {
      if (event.type == sf::Event::Closed ||
          (event.type == sf::Event::KeyPressed &&
           event.key.code == sf::Keyboard::Escape)) {
        m_window.close();
        return;
      } else if (event.type == sf::Event::GainedFocus) {
        focus = true;
        cycleTimer = std::chrono::high_resolution_clock::now();
      } else if (event.type == sf::Event::LostFocus)
        focus = false;
      else if (event.type == sf::Event::KeyPressed &&
               event.key.code == sf::Keyboard::F2) {
        pause = !pause;
        if (!pause) cycleTimer = std::chrono::high_resolution_clock::now();
      } else if (pause && event.type == sf::Event::KeyReleased &&
                 event.key.code == sf::Keyboard::F3) {
        for (int i = 0; i < 29781; ++i)  // Around one frame
        {
          // PPU
          m_ppu.step();
          m_ppu.step();
          m_ppu.step();
          // CPU
          m_cpu.step();
        }
      } else if (focus && event.type == sf::Event::KeyReleased &&
                 event.key.code == sf::Keyboard::F4) {
        Log::get().setLevel(Info);
      } else if (focus && event.type == sf::Event::KeyReleased &&
                 event.key.code == sf::Keyboard::F5) {
        Log::get().setLevel(InfoVerbose);
      } else if (focus && event.type == sf::Event::KeyReleased &&
                 event.key.code == sf::Keyboard::F6) {
        Log::get().setLevel(CpuTrace);
      }
    }

    if (focus && !pause) {
      elapsedTime += std::chrono::high_resolution_clock::now() - cycleTimer;
      cycleTimer = std::chrono::high_resolution_clock::now();

      while (elapsedTime > CpuCycleDuration) {
        // PPU
        m_ppu.step();
        m_ppu.step();
        m_ppu.step();
        // CPU
        m_cpu.step();

        elapsedTime -= CpuCycleDuration;
      }

      m_window.draw(m_ppu.m_screen);
      m_window.display();
    } else {
      sf::sleep(sf::milliseconds(1000 / 60));
      // std::this_thread::sleep_for(std::chrono::milliseconds(1000/60)); //1/60
      // second
    }
  }
}

}  // namespace sn