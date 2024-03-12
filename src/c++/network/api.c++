#include <constellation/network/api.h>

using std::make_unique;

namespace constellation::network
{
  NetworkAPI::NetworkAPI(QObject* parent)
    : m_power_switch(make_unique<modules::PowerSwitch>(
      "192.168.1.50",
      44000,
      std::chrono::seconds(30) // todo
    ))
  {}

  NetworkAPI::~NetworkAPI() = default;

  modules::PowerSwitch* NetworkAPI::powerSwitch() const { return this->m_power_switch.get(); }
}
