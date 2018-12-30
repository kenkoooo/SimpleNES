#include "Controller.h"

namespace sn {
Controller::Controller(const std::vector<sf::Keyboard::Key> &keys)
    : m_keyStates(0), m_keyBindings(keys) {}

void Controller::strobe(Byte b) {
  m_strobe = (b & 1);
  if (!m_strobe) {
    m_keyStates = 0;
    int shift = 0;
    for (int button = A; button < TotalButtons; ++button) {
      m_keyStates |= (sf::Keyboard::isKeyPressed(
                          m_keyBindings[static_cast<Buttons>(button)])
                      << shift);
      ++shift;
    }
  }
}

Byte Controller::read() {
  Byte ret;
  if (m_strobe)
    ret = sf::Keyboard::isKeyPressed(m_keyBindings[A]);
  else {
    ret = (m_keyStates & 1);
    m_keyStates >>= 1;
  }
  return ret | 0x40;
}

} // namespace sn