#include "Emulator.h"
#include "Log.h"
#include <sstream>
#include <string>

int main(int argc, char **argv) {
  std::ofstream logFile("simplenes.log"), cpuTraceFile;
  sn::TeeStream logTee(logFile, std::cout);

  if (logFile.is_open() && logFile.good())
    sn::Log::get().setLogStream(logTee);
  else
    sn::Log::get().setLogStream(std::cout);

  sn::Log::get().setLevel(sn::Info);

  // Default keybindings
  std::vector<sf::Keyboard::Key> p1{sf::Keyboard::J,      sf::Keyboard::K,
                                    sf::Keyboard::RShift, sf::Keyboard::Return,
                                    sf::Keyboard::W,      sf::Keyboard::S,
                                    sf::Keyboard::A,      sf::Keyboard::D},
      p2{sf::Keyboard::Numpad5, sf::Keyboard::Numpad6, sf::Keyboard::Numpad8,
         sf::Keyboard::Numpad9, sf::Keyboard::Up,      sf::Keyboard::Down,
         sf::Keyboard::Left,    sf::Keyboard::Right};
  sn::Emulator emulator;

  std::string path = argv[1];
  emulator.setKeys(p1, p2);
  emulator.run(path);
  return 0;
}
