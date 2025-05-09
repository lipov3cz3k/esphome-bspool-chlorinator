import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_PH,
    CONF_TEMPERATURE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_VOLTAGE,
    ICON_CURRENT_AC,
    ICON_PERCENT,
    ICON_THERMOMETER,
    STATE_CLASS_MEASUREMENT,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_PERCENT,
    UNIT_PH,
    UNIT_VOLT,
)

from .. import CONF_BS_POOL_ID, BSPoolComponent, bs_pool_ns

BSPoolSensorComponent = bs_pool_ns.class_("BSPoolSensor", cg.Component)

CONF_CELL_CURRENT = "cell_current"
CONF_CELL_INTENSITY = "cell_intensity"
CONF_CELL_VOLTAGE = "cell_voltage"
CONF_SALT_CONCENTRATION = "salt_concentration"
CONF_RADOX = "radox"


TYPES = [
    CONF_CELL_CURRENT,
    CONF_CELL_INTENSITY,
    CONF_CELL_VOLTAGE,
    CONF_PH,
    CONF_SALT_CONCENTRATION,
    CONF_RADOX,
    CONF_TEMPERATURE,
]

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(BSPoolSensorComponent),
            cv.GenerateID(CONF_BS_POOL_ID): cv.use_id(BSPoolComponent),
            cv.Optional(CONF_CELL_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE,
                icon=ICON_CURRENT_AC,
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
                device_class=DEVICE_CLASS_CURRENT,
            ),
            cv.Optional(CONF_CELL_INTENSITY): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                icon=ICON_PERCENT,
                accuracy_decimals=0,
                state_class=STATE_CLASS_MEASUREMENT,
                device_class=DEVICE_CLASS_EMPTY,
            ),
            cv.Optional(CONF_CELL_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                icon="mdi:sine-wave",
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
                device_class=DEVICE_CLASS_VOLTAGE,
            ),
            cv.Optional(CONF_PH): sensor.sensor_schema(
                unit_of_measurement=UNIT_PH,
                icon="mdi:ph",
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
                device_class=DEVICE_CLASS_EMPTY,
            ),
            cv.Optional(CONF_SALT_CONCENTRATION): sensor.sensor_schema(
                unit_of_measurement="g/l",
                icon="mdi:shaker-outline",
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
                device_class=DEVICE_CLASS_EMPTY,
            ),
            cv.Optional(CONF_RADOX): sensor.sensor_schema(
                unit_of_measurement="mV",
                icon="mdi:water",
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT,
                device_class=DEVICE_CLASS_EMPTY,
            ),
            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_THERMOMETER,
                accuracy_decimals=0,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    ).extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    bspool_sensor = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(bspool_sensor, config)

    for key in TYPES:
        if key in config:
            sensor_ = await sensor.new_sensor(config[key])
            cg.add(getattr(bspool_sensor, f"set_{key}_sensor")(sensor_))

    bspool = await cg.get_variable(config[CONF_BS_POOL_ID])
    cg.add(bspool.register_listener(bspool_sensor))
