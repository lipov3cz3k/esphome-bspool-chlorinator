import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import (
    CONF_ID,
    CONF_TIME,
    CONF_VERSION,
    ENTITY_CATEGORY_DIAGNOSTIC,
)

from .. import CONF_BS_POOL_ID, BSPoolComponent, bs_pool_ns

BSPoolTextSensorComponent = bs_pool_ns.class_("BSPoolTextSensor", cg.Component)

TYPES = [
    CONF_TIME,
    CONF_VERSION,
]

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(BSPoolTextSensorComponent),
            cv.GenerateID(CONF_BS_POOL_ID): cv.use_id(BSPoolComponent),
            cv.Optional(CONF_TIME): text_sensor.text_sensor_schema(
                icon="mdi:clock-outline",
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_VERSION): text_sensor.text_sensor_schema(
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
        }
    ).extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    bspool_text_sensor = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(bspool_text_sensor, config)

    for key in TYPES:
        if key in config:
            sensor_ = await text_sensor.new_text_sensor(config[key])
            cg.add(getattr(bspool_text_sensor, f"set_{key}_text_sensor")(sensor_))

    bspool = await cg.get_variable(config[CONF_BS_POOL_ID])
    cg.add(bspool.register_listener(bspool_text_sensor))
