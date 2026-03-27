import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@lipov3cz3k"]

DEPENDENCIES = ["uart"]

bs_pool_ns = cg.esphome_ns.namespace("bs_pool")
BSPoolComponent = bs_pool_ns.class_("BSPool", cg.PollingComponent, uart.UARTDevice)

CONF_BS_POOL_ID = "bs_pool_id"

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(BSPoolComponent),
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.polling_component_schema("60s"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
