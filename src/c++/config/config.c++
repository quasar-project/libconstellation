#include <sstream>
#include <leafconf/serializers/toml.h>
#include <constellation/config/config.h>

using std::stringstream;
using std::bad_optional_access;

namespace constellation::config
{
  auto PowerSwitchConfigData::serialize(const leaf::serialization::Serializer serializer) const -> expected<string, string>
  {
    const auto out = toml::table{
          {"channel_names", toml::table{
            {"ch1", this->channel_names.ch1},
            {"ch2", this->channel_names.ch2},
            {"ch3", this->channel_names.ch3},
            {"ch4", this->channel_names.ch4},
            {"ch5", this->channel_names.ch5},
            {"ch6", this->channel_names.ch6},
            {"ch7", this->channel_names.ch7}
            }},
          {"address", toml::table{
            {"ip", this->address.ip},
            {"port", this->address.port}
          }},
          {"gauges", toml::table{
            {"cell_count", this->gauges.cell_count},
            {"cell_min_voltage_volts", this->gauges.cell_min_voltage_volts},
            {"cell_max_voltage_volts", this->gauges.cell_max_voltage_volts},
            {"max_current_amperes", this->gauges.max_current_amperes}
          }},
          {"channels", toml::table{
            {"ch1_min_voltage", this->channels.ch1_min_voltage},
            {"ch1_max_voltage", this->channels.ch1_max_voltage},
            {"ch2_min_voltage", this->channels.ch2_min_voltage},
            {"ch2_max_voltage", this->channels.ch2_max_voltage},
            {"ch3_min_voltage", this->channels.ch3_min_voltage},
            {"ch3_max_voltage", this->channels.ch3_max_voltage},
            {"ch4_min_voltage", this->channels.ch4_min_voltage},
            {"ch4_max_voltage", this->channels.ch4_max_voltage},
            {"ch5_min_voltage", this->channels.ch5_min_voltage},
            {"ch5_max_voltage", this->channels.ch5_max_voltage},
            {"ch6_min_voltage", this->channels.ch6_min_voltage},
            {"ch6_max_voltage", this->channels.ch6_max_voltage},
            {"ch7_min_voltage", this->channels.ch7_min_voltage},
            {"ch7_max_voltage", this->channels.ch7_max_voltage}
          }}
    };

    stringstream ss;
    switch(serializer)
    {
      case leaf::serialization::TOML: ss << out; break;
      case leaf::serialization::JSON: ss << toml::json_formatter(out); break;
      case leaf::serialization::YAML: ss << toml::yaml_formatter(out); break;
      default: leaf::unreachable();
    }
    return ss.str();
  }

  auto PowerSwitchConfigData::deserialize(const string_view data, const leaf::serialization::Serializer serializer) -> expected<void, string>
  {
    if(serializer != leaf::serialization::Serializer::TOML)
      return leaf::Err("unsupported deserialization format");
    toml::table in;
    try {
      in = toml::parse(data);
    } catch(const toml::parse_error& err) {
      return leaf::Err(err.what());
    }
    try {
      this->channel_names.ch1 = in["channel_names"]["ch1"].value<string>().value();
      this->channel_names.ch2 = in["channel_names"]["ch2"].value<string>().value();
      this->channel_names.ch3 = in["channel_names"]["ch3"].value<string>().value();
      this->channel_names.ch4 = in["channel_names"]["ch4"].value<string>().value();
      this->channel_names.ch5 = in["channel_names"]["ch5"].value<string>().value();
      this->channel_names.ch6 = in["channel_names"]["ch6"].value<string>().value();
      this->channel_names.ch7 = in["channel_names"]["ch7"].value<string>().value();
      this->address.ip = in["address"]["ip"].value<string>().value();
      this->address.port = in["address"]["port"].value<u16>().value();
      this->gauges.cell_count = in["gauges"]["cell_count"].value<i32>().value();
      this->gauges.cell_min_voltage_volts = in["gauges"]["cell_min_voltage_volts"].value<f32>().value();
      this->gauges.cell_max_voltage_volts = in["gauges"]["cell_max_voltage_volts"].value<f32>().value();
      this->gauges.max_current_amperes = in["gauges"]["max_current_amperes"].value<f32>().value();
      this->channels.ch1_min_voltage = in["channels"]["ch1_min_voltage"].value<f32>().value();
      this->channels.ch1_max_voltage = in["channels"]["ch1_max_voltage"].value<f32>().value();
      this->channels.ch2_min_voltage = in["channels"]["ch2_min_voltage"].value<f32>().value();
      this->channels.ch2_max_voltage = in["channels"]["ch2_max_voltage"].value<f32>().value();
      this->channels.ch3_min_voltage = in["channels"]["ch3_min_voltage"].value<f32>().value();
      this->channels.ch3_max_voltage = in["channels"]["ch3_max_voltage"].value<f32>().value();
      this->channels.ch4_min_voltage = in["channels"]["ch4_min_voltage"].value<f32>().value();
      this->channels.ch4_max_voltage = in["channels"]["ch4_max_voltage"].value<f32>().value();
      this->channels.ch5_min_voltage = in["channels"]["ch5_min_voltage"].value<f32>().value();
      this->channels.ch5_max_voltage = in["channels"]["ch5_max_voltage"].value<f32>().value();
      this->channels.ch6_min_voltage = in["channels"]["ch6_min_voltage"].value<f32>().value();
      this->channels.ch6_max_voltage = in["channels"]["ch6_max_voltage"].value<f32>().value();
      this->channels.ch7_min_voltage = in["channels"]["ch7_min_voltage"].value<f32>().value();
      this->channels.ch7_max_voltage = in["channels"]["ch7_max_voltage"].value<f32>().value();
    } catch(const bad_optional_access& err) {
      return leaf::Err(err.what());
    }
    return {};
  }
}
