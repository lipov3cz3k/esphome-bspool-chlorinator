import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_SWITCH,
    ENTITY_CATEGORY_CONFIG,
)

from .. import CONF_BS_POOL_ID, BSPoolComponent, bs_pool_ns

BSPoolSwitchComponent = bs_pool_ns.class_("BSPoolSwitch", cg.Component)

UserSettingsSwitch = bs_pool_ns.class_("UserSettingsSwitch", switch.Switch)

CONF_USER_IS_OUTDOOR = "user_is_outdoor"
CONF_USER_COVER_SWITCH_OFF = "user_cover_switch_off"
CONF_USER_FLOW_SWITCH_INSTALLED = "user_flow_switch_installed"
CONF_USER_ORP_DISPLAYED = "user_orp_displayed"
CONF_USER_PH_ALARM = "user_ph_alarm"
CONF_USER_PH_CORRECTOR_ALKALINE = "user_ph_corrector_alkaline"
CONF_USER_PH_CONTROL = "user_ph_control"
CONF_USER_COVER_INSTALLED = "user_cover_installed"

TYPES = [
    CONF_USER_IS_OUTDOOR,
    CONF_USER_COVER_SWITCH_OFF,
    CONF_USER_FLOW_SWITCH_INSTALLED,
    CONF_USER_ORP_DISPLAYED,
    CONF_USER_PH_ALARM,
    CONF_USER_PH_CORRECTOR_ALKALINE,
    CONF_USER_PH_CONTROL,
    CONF_USER_COVER_INSTALLED,
]


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(BSPoolSwitchComponent),
            cv.GenerateID(CONF_BS_POOL_ID): cv.use_id(BSPoolComponent),
            cv.Optional(CONF_USER_IS_OUTDOOR): switch.switch_schema(
                UserSettingsSwitch,
                device_class=DEVICE_CLASS_SWITCH,
                entity_category=ENTITY_CATEGORY_CONFIG,
            ),
            cv.Optional(CONF_USER_COVER_SWITCH_OFF): switch.switch_schema(
                UserSettingsSwitch,
                device_class=DEVICE_CLASS_SWITCH,
                entity_category=ENTITY_CATEGORY_CONFIG,
            ),
            cv.Optional(CONF_USER_FLOW_SWITCH_INSTALLED): switch.switch_schema(
                UserSettingsSwitch,
                device_class=DEVICE_CLASS_SWITCH,
                entity_category=ENTITY_CATEGORY_CONFIG,
            ),
            cv.Optional(CONF_USER_ORP_DISPLAYED): switch.switch_schema(
                UserSettingsSwitch,
                device_class=DEVICE_CLASS_SWITCH,
                entity_category=ENTITY_CATEGORY_CONFIG,
            ),
            cv.Optional(CONF_USER_PH_ALARM): switch.switch_schema(
                UserSettingsSwitch,
                device_class=DEVICE_CLASS_SWITCH,
                entity_category=ENTITY_CATEGORY_CONFIG,
            ),
            cv.Optional(CONF_USER_PH_CORRECTOR_ALKALINE): switch.switch_schema(
                UserSettingsSwitch,
                device_class=DEVICE_CLASS_SWITCH,
                entity_category=ENTITY_CATEGORY_CONFIG,
            ),
            cv.Optional(CONF_USER_PH_CONTROL): switch.switch_schema(
                UserSettingsSwitch,
                device_class=DEVICE_CLASS_SWITCH,
                entity_category=ENTITY_CATEGORY_CONFIG,
            ),
            cv.Optional(CONF_USER_COVER_INSTALLED): switch.switch_schema(
                UserSettingsSwitch,
                device_class=DEVICE_CLASS_SWITCH,
                entity_category=ENTITY_CATEGORY_CONFIG,
            ),
        }
    ).extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    bspool_switch = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(bspool_switch, config)

    await cg.register_parented(bspool_switch, config[CONF_BS_POOL_ID])

    bspool = await cg.get_variable(config[CONF_BS_POOL_ID])
    cg.add(bspool.register_listener(bspool_switch))

    for key in TYPES:
        if config_item := config.get(key):
            s = await switch.new_switch(config_item)
            await cg.register_parented(s, config[CONF_ID])
            cg.add(getattr(bspool_switch, f"set_{key}_switch")(s))
