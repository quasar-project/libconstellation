#pragma once

#include <memory>
#include <qobject.h>
#include <constellation/global.h>
#include <constellation/network/modules/powerswitch.h>

namespace constellation::network
{
  using std::unique_ptr;

  // ReSharper disable once CppClassCanBeFinal
  class NetworkAPI : public QObject
  {
    Q_OBJECT
    Q_PROPERTY(constellation::network::modules::PowerSwitch* powerSwitch READ powerSwitch CONSTANT FINAL)

    public:
      explicit NetworkAPI(QObject* parent = nullptr);
      virtual ~NetworkAPI() override;

      [[nodiscard]] modules::PowerSwitch* powerSwitch() const;

    private:
      unique_ptr<modules::PowerSwitch> m_power_switch;
  };
}