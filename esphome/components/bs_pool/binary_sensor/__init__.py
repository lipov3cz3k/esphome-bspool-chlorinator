import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_CONNECTIVITY,
    DEVICE_CLASS_PROBLEM,
    DEVICE_CLASS_RUNNING,
    ENTITY_CATEGORY_DIAGNOSTIC,
)

from .. import CONF_BS_POOL_ID, BSPoolComponent, bs_pool_ns

BSPoolBinarySensorComponent = bs_pool_ns.class_("BSPoolBinarySensor", cg.Component)

CONF_ALARM_REDOX_SATURATED = "alarm_redox_saturated"
CONF_ALARM_OVERTEMPERATURE = "alarm_overtemperature"
CONF_ALARM_OPEN_CIRCUIT = "alarm_open_circuit"
CONF_ALARM_WATER_FLOW = "alarm_water_flow"
CONF_ALARM_SHORT_CIRCUIT = "alarm_short_circuit"
CONF_ALARM_UNIT_STOPPED = "alarm_unit_stopped"
CONF_ALARM_COMMUNICATION_ERROR = "alarm_communication_error"

CONF_WARNING_LOW_SALT = "warning_low_salt"
CONF_WARNING_HIGH_SALT = "warning_high_salt"
CONF_WARNING_PH_PUMP_OVERRUN = "warning_ph_pump_overrun"
CONF_WARNING_ACID = "warning_acid"
CONF_WARNING_CLOCK_SET = "warning_clock_set"
CONF_WARNING_PROBE_DISCONNECTED = "warning_probe_disconnected"

CONF_STATUS_OUTPUT_VOLTAGE_POLARITY = "status_output_voltage_polarity"
CONF_STATUS_FILTER_PUMP_RUNNING = "status_filter_pump_running"
CONF_STATUS_CHLORINATION_PROCESS = "status_chlorination_process"
CONF_STATUS_STOPPED_BY_REMOTE = "status_stopped_by_remote"
CONF_STATUS_ORP_SENSOR_STABLE = "status_orp_sensor_stable"
CONF_STATUS_SALT_SENSOR_CONNECTED = "status_salt_sensor_connected"
CONF_STATUS_ELECTRODE_CLEANING = "status_electrode_cleaning"
CONF_STATUS_SUPER_CHLORINATION = "status_super_chlorination"

TYPES = [
    CONF_ALARM_REDOX_SATURATED,
    CONF_ALARM_OVERTEMPERATURE,
    CONF_ALARM_OPEN_CIRCUIT,
    CONF_ALARM_WATER_FLOW,
    CONF_ALARM_SHORT_CIRCUIT,
    CONF_ALARM_UNIT_STOPPED,
    CONF_ALARM_COMMUNICATION_ERROR,
    CONF_WARNING_LOW_SALT,
    CONF_WARNING_HIGH_SALT,
    CONF_WARNING_PH_PUMP_OVERRUN,
    CONF_WARNING_ACID,
    CONF_WARNING_CLOCK_SET,
    CONF_WARNING_PROBE_DISCONNECTED,
    CONF_STATUS_OUTPUT_VOLTAGE_POLARITY,
    CONF_STATUS_FILTER_PUMP_RUNNING,
    CONF_STATUS_CHLORINATION_PROCESS,
    CONF_STATUS_STOPPED_BY_REMOTE,
    CONF_STATUS_ORP_SENSOR_STABLE,
    CONF_STATUS_SALT_SENSOR_CONNECTED,
    CONF_STATUS_ELECTRODE_CLEANING,
    CONF_STATUS_SUPER_CHLORINATION,
]


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(BSPoolBinarySensorComponent),
            cv.GenerateID(CONF_BS_POOL_ID): cv.use_id(BSPoolComponent),
            cv.Optional(CONF_ALARM_REDOX_SATURATED): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_PROBLEM,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_ALARM_OVERTEMPERATURE): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_PROBLEM,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_ALARM_OPEN_CIRCUIT): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_PROBLEM,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_ALARM_WATER_FLOW): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_PROBLEM,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_ALARM_SHORT_CIRCUIT): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_PROBLEM,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_ALARM_UNIT_STOPPED): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_PROBLEM,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(
                CONF_ALARM_COMMUNICATION_ERROR
            ): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_PROBLEM,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_WARNING_LOW_SALT): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_PROBLEM,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_WARNING_HIGH_SALT): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_PROBLEM,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(
                CONF_WARNING_PH_PUMP_OVERRUN
            ): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_PROBLEM,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_WARNING_ACID): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_PROBLEM,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_WARNING_CLOCK_SET): binary_sensor.binary_sensor_schema(
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(
                CONF_WARNING_PROBE_DISCONNECTED
            ): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_CONNECTIVITY,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(
                CONF_STATUS_OUTPUT_VOLTAGE_POLARITY
            ): binary_sensor.binary_sensor_schema(
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(
                CONF_STATUS_FILTER_PUMP_RUNNING
            ): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_RUNNING,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(
                CONF_STATUS_CHLORINATION_PROCESS
            ): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_RUNNING,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(
                CONF_STATUS_STOPPED_BY_REMOTE
            ): binary_sensor.binary_sensor_schema(
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(
                CONF_STATUS_ORP_SENSOR_STABLE
            ): binary_sensor.binary_sensor_schema(
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(
                CONF_STATUS_SALT_SENSOR_CONNECTED
            ): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_CONNECTIVITY,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(
                CONF_STATUS_ELECTRODE_CLEANING
            ): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_RUNNING,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(
                CONF_STATUS_SUPER_CHLORINATION
            ): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_RUNNING,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
        }
    ).extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    bspool_binary_sensor = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(bspool_binary_sensor, config)

    for key in TYPES:
        if key in config:
            sensor_ = await binary_sensor.new_binary_sensor(config[key])
            cg.add(getattr(bspool_binary_sensor, f"set_{key}_binary_sensor")(sensor_))

    bspool = await cg.get_variable(config[CONF_BS_POOL_ID])
    cg.add(bspool.register_listener(bspool_binary_sensor))
