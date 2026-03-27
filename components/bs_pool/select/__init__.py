import esphome.codegen as cg
from esphome.components import select
import esphome.config_validation as cv
from esphome.const import (
    ENTITY_CATEGORY_CONFIG,
    CONF_ID,
)
from .. import CONF_BS_POOL_ID, BSPoolComponent, bs_pool_ns

BSPoolSelectComponent = bs_pool_ns.class_("BSPoolSelect", cg.Component)

ControlModeSelect = bs_pool_ns.class_("ControlModeSelect", select.Select)

CONF_CONTROL_MODE = "control_mode"

OPTIONS = {
    CONF_CONTROL_MODE: ["manual", "auto", "semi_auto"],
}


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(BSPoolSelectComponent),
            cv.GenerateID(CONF_BS_POOL_ID): cv.use_id(BSPoolComponent),
            cv.Optional(CONF_CONTROL_MODE): select.select_schema(
                ControlModeSelect,
                entity_category=ENTITY_CATEGORY_CONFIG,
            ),
        }
    ).extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    bspool_select = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(bspool_select, config)

    await cg.register_parented(bspool_select, config[CONF_BS_POOL_ID])

    bspool = await cg.get_variable(config[CONF_BS_POOL_ID])
    cg.add(bspool.register_listener(bspool_select))

    for key, options in OPTIONS.items():
        if config_item := config.get(key):
            s = await select.new_select(config_item, options=options)
            await cg.register_parented(s, config[CONF_ID])
            cg.add(getattr(bspool_select, f"set_{key}_select")(s))
