/**
 *    Copyright (c) 2024/12/18  DengWenjie@OurEDA， Dalian Univ of Tech
 */

#include "pinctrl.h"
#include "common_def.h"
#include "soc_osal.h"
#include "i2c.h"
#include "app_init.h"
#include "gpio.h"
#include "hal_gpio.h"
#include "watchdog.h"
#include "io_expander.h"

#define DELAY_500 500

int LED = 0;
int BUT = 0;
int time = 0;

uint8_t io_expander_i2c_lock = 1;

void gpio_callback_func(pin_t pin, uintptr_t param)
{

    UNUSED(pin);
    UNUSED(param);
    uint8_t tx3_buff[2] = {0x1C, 0x08}; // 寄存器
    uint8_t rx3_buff[2] = {0};
    i2c_data_t pca5 = {0};
    pca5.send_buf = tx3_buff;
    pca5.send_len = 2; // 发送2字节命令
    pca5.receive_buf = rx3_buff;
    pca5.receive_len = 2; // 接收2字节响应

    uapi_i2c_master_read(1, I2C_PCA_ADDR, &pca5); // 使用I2C总线1读取IO扩展器
    time = osal_msleep(2);                        // 延时2ms确保数据准备完毕
    osal_printk("sadsd %X %X \n", *rx3_buff, *(rx3_buff + 1));
    osal_printk("LED=%d", LED);
    osal_printk("BUT=%d", BUT);
    osal_printk("time=%d", time);
    if ((*rx3_buff & 0x80) && (*rx3_buff & 0x40)) { // 均为高电平
        LED++;
        if (LED >= 3) { // 3代表LED状态循环上限
            LED = 0;
        }
    }
    if ((*rx3_buff & 0x80) && !(*rx3_buff & 0x40)) { // 高低
        LED--;
        if (LED < 0) { // LED状态循环下限
            LED = 2;   // 2代表LED状态
        }
    }
    if (!(*(rx3_buff + 1) & 0x04)) { // 按键1
        BUT = 1;
    }
    if (!(*(rx3_buff + 1) & 0x08)) { // 按键2
        BUT = 2;
    }
    if (!(*(rx3_buff + 1) & 0x10)) { // 按键3
        BUT = 3;
    }
    switch (LED) {
        case 0: // LED状态0：绿灯亮
            uapi_gpio_set_val(LEDG, GPIO_LEVEL_HIGH);
            uapi_gpio_set_val(LEDR, GPIO_LEVEL_LOW);
            uapi_gpio_set_val(LEDB, GPIO_LEVEL_LOW);
            break;
        case 1: // LED状态1：黄灯亮
            uapi_gpio_set_val(LEDB, GPIO_LEVEL_HIGH);
            uapi_gpio_set_val(LEDG, GPIO_LEVEL_LOW);
            uapi_gpio_set_val(LEDR, GPIO_LEVEL_LOW);
            break;
        case 2: // LED状态2：红灯亮
            uapi_gpio_set_val(LEDR, GPIO_LEVEL_HIGH);
            uapi_gpio_set_val(LEDG, GPIO_LEVEL_LOW);
            uapi_gpio_set_val(LEDB, GPIO_LEVEL_LOW);
            break;
        default:
            break;
    }
    switch (BUT) {
        case 1: // 按键1触发红灯闪烁
            uapi_gpio_set_val(LEDR, GPIO_LEVEL_HIGH);
            uapi_gpio_set_val(LEDG, GPIO_LEVEL_LOW);
            uapi_gpio_set_val(LEDB, GPIO_LEVEL_LOW);
            osal_msleep(DELAY_500);
            uapi_gpio_toggle(LEDR);
            osal_msleep(DELAY_500);
            uapi_gpio_toggle(LEDR);
            osal_msleep(DELAY_500);
            uapi_gpio_toggle(LEDR);
            osal_msleep(DELAY_500);
            uapi_gpio_toggle(LEDR);
            break;

        case 2: // 按键2触发绿灯闪烁
            uapi_gpio_set_val(LEDR, GPIO_LEVEL_LOW);
            uapi_gpio_set_val(LEDG, GPIO_LEVEL_HIGH);
            uapi_gpio_set_val(LEDB, GPIO_LEVEL_LOW);
            osal_msleep(DELAY_500);
            uapi_gpio_toggle(LEDG);
            osal_msleep(DELAY_500);
            uapi_gpio_toggle(LEDG);
            osal_msleep(DELAY_500);
            uapi_gpio_toggle(LEDG);
            osal_msleep(DELAY_500);
            uapi_gpio_toggle(LEDG);
            break;
        case 3: // 按键3触发蓝灯闪烁
            uapi_gpio_set_val(LEDR, GPIO_LEVEL_LOW);
            uapi_gpio_set_val(LEDG, GPIO_LEVEL_LOW);
            uapi_gpio_set_val(LEDB, GPIO_LEVEL_HIGH);
            osal_msleep(DELAY_500);
            uapi_gpio_toggle(LEDB);
            osal_msleep(DELAY_500);
            uapi_gpio_toggle(LEDB);
            osal_msleep(DELAY_500);
            uapi_gpio_toggle(LEDB);
            osal_msleep(DELAY_500);
            uapi_gpio_toggle(LEDB);
            break;
        default:
            break;
    }
    BUT = 0;
    osal_printk("BUT=%d", BUT);
}

void io_expander_init(void)
{
    uapi_pin_set_mode(GPIO_12, PIN_MODE_0);
    uapi_pin_set_pull(GPIO_12, PIN_PULL_TYPE_DOWN);
    uapi_gpio_set_dir(GPIO_12, 0);

    uapi_pin_set_mode(LEDG, HAL_PIO_FUNC_GPIO);
    uapi_gpio_set_dir(LEDG, GPIO_DIRECTION_OUTPUT);
    uapi_gpio_set_val(LEDG, GPIO_LEVEL_LOW);

    uapi_pin_set_mode(LEDB, HAL_PIO_FUNC_GPIO);
    uapi_gpio_set_dir(LEDB, GPIO_DIRECTION_OUTPUT);
    uapi_gpio_set_val(LEDB, GPIO_LEVEL_LOW);

    uapi_pin_set_mode(LEDR, HAL_PIO_FUNC_GPIO);
    uapi_gpio_set_dir(LEDR, GPIO_DIRECTION_OUTPUT);
    uapi_gpio_set_val(LEDR, GPIO_LEVEL_LOW);

    osal_printk("io expander init start\r\n");

    // 发送命令初始化P0–P5和INT
    uint8_t tx_buff[2] = {0x10, 0x3B}; // 命令0x10（配置寄存器），掩码0x3B(00111011)启用P0–P5
    uint8_t rx_buff[2] = {0};
    i2c_data_t pca2 = {0};
    pca2.send_buf = tx_buff;
    pca2.send_len = 2; // 发送2字节
    pca2.receive_buf = rx_buff;
    pca2.receive_len = 2; // 接收2字节
    errcode_t ret;
    for (int i = 0; i <= 10; i++) { // 10代表重试11次写入
        ret = uapi_i2c_master_write(1, I2C_PCA_ADDR, &pca2);
    }
    if (ret != 0) {
        printf("io expander init failed, ret = %0x\r\n", ret);
    }

    uint8_t tx4_buff[2] = {0x18, 0x00}; // 命令0x18(配置寄存器)，掩码0x00禁用B0–B5
    uint8_t rx4_buff[2] = {0};
    i2c_data_t pca6 = {0};
    pca6.send_buf = tx4_buff;
    pca6.send_len = 2; // 长度为2
    pca6.receive_buf = rx4_buff;
    pca6.receive_len = 2;           // 长度为2
    for (int i = 0; i <= 10; i++) { // 10代表重试11次写入
        ret = uapi_i2c_master_write(1, I2C_PCA_ADDR, &pca6);
    }
    if (ret != 0) {
        printf("io expander init failed, ret = %0x\r\n", ret);
    }

    uint8_t tx1_buff[3] = {0x1B, 0xFF, 0x00}; // 命令0x1B(设置中断)，写入0xFF,清除状态0x00
    uint8_t rx1_buff[2] = {0};
    i2c_data_t pca3 = {0};
    pca3.send_buf = tx1_buff;
    pca3.send_len = 3; // 发送3字节
    pca3.receive_buf = rx1_buff;
    pca3.receive_len = 2;           // 接收2字节
    for (int i = 0; i <= 10; i++) { // 10代表重试11次写入
        ret = uapi_i2c_master_write(1, I2C_PCA_ADDR, &pca3);
    }
    if (ret != 0) {
        printf("io expander init failed, ret = %0x\r\n", ret);
    }

    errcode_t ASD = uapi_gpio_register_isr_func(GPIO_12, 0x00000008, gpio_callback_func); // 中断触发掩码0x08(第3位)
    if (ASD != 0) {
        uapi_gpio_unregister_isr_func(GPIO_12);
        osal_printk("io expander init FAILED:%0x\r\n", ASD);
    } else {
        osal_printk("io expander init SUCC!\r\n");
    }

    io_expander_i2c_lock = 1;
}
