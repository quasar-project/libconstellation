#pragma once

#include <leafconf/static.h>
#include <constellation/global.h>

namespace constellation::config
{
  struct PowerSwitchConfigData final : public leaf::conf::AbstractConfigData
  {
    virtual ~PowerSwitchConfigData() override = default;

    [[nodiscard]] virtual auto serialize(leaf::serialization::Serializer) const -> expected<string, string> override;
    [[nodiscard]] virtual auto deserialize(string_view, leaf::serialization::Serializer) -> expected<void, string> override;

    struct ChannelNames
    {
      string ch1 = "АПД";
      string ch2 = "РЛМ";
      string ch3 = "ЭВМ";
      string ch4 = "УМ";
      string ch5 = "К5";
      string ch6 = "КОМ";
      string ch7 = "НАВ";
    } channel_names;

    struct Address
    {
      string ip = "192.168.1.50";
      u16 port  = 44000;
    } address;

    struct Gauges
    {
      i32 cell_count               = 5;
      f32 cell_min_voltage_volts   = 3.2f;
      f32 cell_max_voltage_volts   = 4.3f;
      f32 max_current_amperes      = 7.0f;
    } gauges;

    struct Channels
    {
      f32 ch1_min_voltage          = 10.0f;
      f32 ch1_max_voltage          = 20.0f;
      f32 ch2_min_voltage          = 10.0f;
      f32 ch2_max_voltage          = 20.0f;
      f32 ch3_min_voltage          = 10.0f;
      f32 ch3_max_voltage          = 20.0f;
      f32 ch4_min_voltage          = 10.0f;
      f32 ch4_max_voltage          = 20.0f;
      f32 ch5_min_voltage          = 10.0f;
      f32 ch5_max_voltage          = 20.0f;
      f32 ch6_min_voltage          = 0.0f;
      f32 ch6_max_voltage          = 10.0f;
      f32 ch7_min_voltage          = 0.0f;
      f32 ch7_max_voltage          = 10.0f;
    } channels;
  };

  [[nodiscard]] inline auto build_powerswitch_config() -> leaf::conf::Config<PowerSwitchConfigData>
  {
    using namespace leaf::conf;
    using enum leaf::conf::Config<PowerSwitchConfigData>::SavingPolicy;

    return leaf::conf::Config<PowerSwitchConfigData>(
      "powerswitch.toml",
      fs::current_path() / "config",
      SaveOnDestruction,
      PowerSwitchConfigData()
    );
  }
}