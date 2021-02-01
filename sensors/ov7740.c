/*
 * This file is part of the OpenMV project.
 * Copyright (c) 2013/2014 Ibrahim Abdelkader <i.abdalkader@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * OV7725 driver.
 *
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sccb.h"
#include "ov7740.h"
#include "ov7740_regs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_ARDUHAL_ESP_LOG)
#include "esp32-hal-log.h"
#else
#include "esp_log.h"
static const char* TAG = "ov7740";
#endif


static const uint8_t default_regs[][2] = {
    /* QVGA RGB888 */
    // {0x0e, 0x00},

    // {0x12, 0x80},
    // {0x13, 0x00},

    // {0x11, 0x05},
    // {0x12, 0x00},
    // {0xd5, 0x10},
    // {0x0c, 0x12},
    // {0x0d, 0x34},
    // {0x17, 0x25},
    // {0x18, 0xa0},
    // {0x19, 0x03},
    // {0x1a, 0xf0},
    // {0x1b, 0x89},
    // {0x22, 0x03},
    // {0x29, 0x18},
    // {0x2b, 0xf8},
    // {0x2c, 0x01},
    // {0x31, 0xa0},
    // {0x32, 0xf0},
    // {0x33, 0xc4},
    // {0x3a, 0xb4},
    // {0x36, 0x3f},

    // {0x04, 0x60},
    // {0x27, 0x80},
    // {0x3d, 0x0f},
    // {0x3e, 0x80},
    // {0x3f, 0x40},
    // {0x40, 0x7f},
    // {0x41, 0x6a},
    // {0x42, 0x29},
    // {0x44, 0xe5},
    // {0x45, 0x41},
    // {0x47, 0x02},
    // {0x49, 0x64},
    // {0x4a, 0xa1},
    // {0x4b, 0x70},
    // {0x4c, 0x1a},
    // {0x4d, 0x50},
    // {0x4e, 0x13},
    // {0x64, 0x00},
    // {0x67, 0x88},
    // {0x68, 0x1a},

    // {0x14, 0x38},
    // {0x24, 0x3c},
    // {0x25, 0x30},
    // {0x26, 0x72},
    // {0x50, 0x97},
    // {0x51, 0x7e},
    // {0x52, 0x00},
    // {0x53, 0x00},
    // {0x20, 0x00},
    // {0x21, 0x23},
    // {0x38, 0x14},
    // {0xe9, 0x00},
    // {0x56, 0x55},
    // {0x57, 0xff},
    // {0x58, 0xff},
    // {0x59, 0xff},
    // {0x5f, 0x04},
    // {0xec, 0x00},
    // {0x13, 0xff},

    // {0x80, 0x7f},
    // {0x81, 0x3f},
    // {0x82, 0x32},
    // {0x83, 0x01},
    // {0x38, 0x11},
    // {0x84, 0x70},
    // {0x85, 0x00},
    // {0x86, 0x03},
    // {0x87, 0x01},
    // {0x88, 0x05},
    // {0x89, 0x30},
    // {0x8d, 0x30},
    // {0x8f, 0x85},
    // {0x93, 0x30},
    // {0x95, 0x85},
    // {0x99, 0x30},
    // {0x9b, 0x85},

    // {0x9c, 0x08},
    // {0x9d, 0x12},
    // {0x9e, 0x23},
    // {0x9f, 0x45},
    // {0xa0, 0x55},
    // {0xa1, 0x64},
    // {0xa2, 0x72},
    // {0xa3, 0x7f},
    // {0xa4, 0x8b},
    // {0xa5, 0x95},
    // {0xa6, 0xa7},
    // {0xa7, 0xb5},
    // {0xa8, 0xcb},
    // {0xa9, 0xdd},
    // {0xaa, 0xec},
    // {0xab, 0x1a},

    // {0xce, 0x78},
    // {0xcf, 0x6e},
    // {0xd0, 0x0a},
    // {0xd1, 0x0c},
    // {0xd2, 0x84},
    // {0xd3, 0x90},
    // {0xd4, 0x1e},

    // {0x5a, 0x24},
    // {0x5b, 0x1f},
    // {0x5c, 0x88},
    // {0x5d, 0x60},

    // {0xac, 0x6e},
    // {0xbe, 0xff},
    // {0xbf, 0x00},

    // /* 320x240 */
    // {0x31, 0x50},
    // {0x32, 0x78},
    // {0x82, 0x3f},

    // /* VGA,RGBRAW_8 */
    // {0x12, 0x01},
    // {0x36, 0x2f},
    // {0x83, 0x04},
    // {0x53, 0x00},

    // {0x33, 0xf4},
    // {0x1b, 0x8a},
    // {0x22, 0x03},

    // /*  */
    // {0x84, 0x00},
    // {0x84, 0x00},

    // {0x28, 0x02},


    /* YUV422 */
	// {0x0e, 0x00},

    {0x12, 0x80},
    /* flag for soft reset delay */
    {0xFE, 0x05},
    {0x13, 0x00},

    /**************************************************************/
    /*  30fps  11 01 ;clock_divider ;sysclk=24MHz at XCLK=24MHz   */
    /*  20fps  11 02 ;clock_divider ;sysclk=16MHz at XCLK=24MHz   */
    /*  15fps  11 03 ;clock_divider ;sysclk=12MHz at XCLK=24MHz   */
    /*  10fps  11 05 ;sysclk=8MHz at XCLK=24MHz                   */
    /*  7p5fps 11 07 ;sysclk=6MHz at XCLK=24MHz                   */
    /**************************************************************/
    {0x11, 0x05},
    /**************************************************************/

    {0x12, 0x00},
    {0xd5, 0x10},
    //{0x00,0x10}, //GAIN
    {0x01,0x40},
    {0x02,0x40},
    {0x03,0x40},
    //{0x0c,0x02}, //Trying to RGB output working
    //{0x0c, ( OV7740_REG0C_MAX_EXPOSURE(3) | OV7740_REG0C_YUV_SWAP_ENABLE)},
    {0x0c, (OV7740_REG0C_MIRROR_ENABLE | OV7740_REG0C_FLIP_ENABLE | OV7740_REG0C_MAX_EXPOSURE(3) | OV7740_REG0C_YUV_SWAP_ENABLE)},
    {0x10,0xf0},
    //{0x13, (OV7740_REG13_EXPOSURE_AUTO | OV7740_REG13_WBAL_AUTO | OV7740_REG13_AGC_AUTO | )},
    {0x15,0x90}, //NIGHT Mode?
    {0x0d, 0x34},
    {0x16, 0x01},
    /* Frame  sizing clipping stuff */
    {0x17, 0x25}, //AHSTART Sensor horizontal Output start point
    {0x18, 0xa0}, //AHSIZE
    {0x19, 0x03}, //AVSTART 
    {0x1a, 0xf0}, //AVSize
    {0x1b, 0x89}, //PShift pixel shift
    
    {0x22, 0x03},
    {0x29, 0x18},
    {0x2b, 0xf8},
    {0x2c, 0x01},
    {0x31, 0xa0},
    {0x32, 0xf0},
    {0x33, 0xc4},
    {0x3a, 0xb4},
    {0x36, 0x3f},

    {0x04, 0x60},
    {0x27, 0x80},
    {0x3d, 0x0f},
    {0x3e, 0x80},
    {0x3f, 0x40},
    {0x40, 0x7f},
    {0x41, 0x6a},
    {0x42, 0x29},
    {0x44, 0xe5},
    {0x45, 0x41},
    {0x47, 0x02},
    {0x49, 0x64},
    {0x4a, 0xa1},
    {0x4b, 0x70},
    {0x4c, 0x1a},
    {0x4d, 0x50},
    {0x4e, 0x13},
    {0x64, 0x00},
    {0x67, 0x88},
    {0x68, 0x1a},

    {0x14, 0x40}, //gain ceiling 2x = 0x00, 4x = 0x10, 8x = 0x20, 16x = 0x30, 32x = 0x40, 
    
    /* working values */
    {0x24, 0x3c},
    {0x25, 0x30},
    {0x26, 0x72},
    
    // {0x24, 0x40}, //WPT default
    // {0x25, 0x30}, //BPT  default
    // {0x26, 0x72}, //VPT  default

    {0x50, 0x97},
    {0x51, 0x7e},
    {0x52, 0x00},
    {0x53, 0x00},
    {0x20, 0x00},
    {0x21, 0x23},
    /*********************************/
    /* To enable Static Test Pattern */
    /*********************************/
    /*  {0x38, 0x07}, */
    /*  {0x84, 0x02}, */

    /*********************************/
    /* Normal Mode / No test pattern */
    {0x38, 0x14},
    /*********************************/
    {0xe9, 0x00},
    {0x56, 0x55},
    {0x57, 0xff},
    {0x58, 0xff},
    {0x59, 0xff},
    {0x5f, 0x04},
    {0xec, 0x00},
    {0x13, 0xcf}, //REG13 controls AEC stuff

    {0x80, 0x7f},
    {0x81, 0x3f},
    {0x82, 0x32},
    {0x83, 0x01}, //ISP CTRL03 worth looking into more
    {0x38, 0x11},
    {0x85, 0x00},
    {0x86, 0x03},
    {0x87, 0x01},
    {0x88, 0x05},
    {0x89, 0x30},
    {0x8d, 0x30},
    {0x8f, 0x85},
    {0x93, 0x30},
    {0x95, 0x85},
    {0x99, 0x30},
    {0x9b, 0x85},

    {0x9c, 0x08},
    {0x9d, 0x12},
    {0x9e, 0x23},
    {0x9f, 0x45},
    {0xa0, 0x55},
    {0xa1, 0x64},
    {0xa2, 0x72},
    {0xa3, 0x7f},
    {0xa4, 0x8b},
    {0xa5, 0x95},
    {0xa6, 0xa7},
    {0xa7, 0xb5},
    {0xa8, 0xcb},
    {0xa9, 0xdd},
    {0xaa, 0xec},
    {0xab, 0x1a},

    {0xce, 0x78},
    {0xcf, 0x6e},
    {0xd0, 0x0a},
    {0xd1, 0x0c},
    {0xd2, 0x84},
    {0xd3, 0x90},
    {0xd4, 0x1e},

    {0x5a, 0x24},
    {0x5b, 0x1f},
    {0x5c, 0x88},
    {0x5d, 0x60},

    {0xac, 0x6e},
    {0xbe, 0xff},
    {0xbf, 0x00},


    /* 640x480 */

    {0x31, 0xA0},
    {0x32, 0xF0},
    {0x82, 0x32},

    /* 320x240 */

    // {0x31, 0x50},
    // {0x32, 0x78},
    // {0x82, 0x3f},

    // /* YUV */
    {0x12, 0x00},
    {0x36, 0x3f},
    {0x53, 0x00},

    //{0x33, 0xc4},
    //{0x1b, 0x89},
    //{0x22, 0x03},

    // /* VGA,RGBRAW_8 */
    // {0x12, 0x01},
    // {0x36, 0x2f},
    // //{0x83, 0x04},
    // {0x53, 0x00},

    // {0x33, 0xc4},
    // {0x1b, 0x89},
    // {0x22, 0x03},

    /* VSYNC, inverse */
    {0x28, 0x2},
    {0xcc, 0x20}, //CIP Control set to auto
    {0xcb, 0x00}, //CIP Control set to auto
    {0xcd, 0x00}, //CIP Control set to auto
    {0xFF, 0xFF},
	{0,0},
};

#define NUM_BRIGHTNESS_LEVELS (9)
static const uint8_t brightness_regs[NUM_BRIGHTNESS_LEVELS][2] = {
    {0x06, 0x40}, /* -4 */
    {0x06, 0x30}, /* -3 */
    {0x06, 0x20}, /* -2 */
    {0x06, 0x10}, /* -1 */
    {0x0E, 0x00}, /*  0 */
    {0x0E, 0x10}, /* +1 */
    {0x0E, 0x20}, /* +2 */
    {0x0E, 0x30}, /* +3 */
    {0x0E, 0x40}, /* +4 */
};

#define NUM_CONTRAST_LEVELS (9)
static const uint8_t contrast_regs[NUM_CONTRAST_LEVELS][3] = {
    {0x20, 0x10, 0xD0}, /* -4 */
    {0x20, 0x14, 0x80}, /* -3 */
    {0x20, 0x18, 0x48}, /* -2 */
    {0x20, 0x1C, 0x20}, /* -1 */
    {0x20, 0x20, 0x00}, /*  0 */
    {0x20, 0x24, 0x00}, /* +1 */
    {0x20, 0x28, 0x00}, /* +2 */
    {0x20, 0x2C, 0x00}, /* +3 */
    {0x20, 0x30, 0x00}, /* +4 */
};

#define NUM_SATURATION_LEVELS (9)
static const uint8_t saturation_regs[NUM_SATURATION_LEVELS][2] = {
    {0x00, 0x00}, /* -4 */
    {0x10, 0x10}, /* -3 */
    {0x20, 0x20}, /* -2 */
    {0x30, 0x30}, /* -1 */
    {0x40, 0x40}, /*  0 */
    {0x50, 0x50}, /* +1 */
    {0x60, 0x60}, /* +2 */
    {0x70, 0x70}, /* +3 */
    {0x80, 0x80}, /* +4 */
};



static int ov7740_sleep(sensor_t *sensor, int enable)
{
    if(enable)
    {
        //DCMI_PWDN_HIGH();
    }
    else
    {
        //DCMI_PWDN_LOW();
    }
    
    return 0;
}

static int get_reg(sensor_t *sensor, int reg, int mask)
{
    int ret = SCCB_Read(sensor->slv_addr, reg & 0xFF);
     ESP_LOGE(TAG, "SCCB Read %x value %x", reg,ret);
    if(ret > 0){
        ret &= mask;
    }
    return ret;
}

static int set_reg(sensor_t *sensor, int reg, int mask, int value)
{
    int ret = 0;
    ret = SCCB_Read(sensor->slv_addr, reg & 0xFF);
    if(ret < 0){
        return ret;
    }
    value = (ret & ~mask) | (value & mask);
    ret = SCCB_Write(sensor->slv_addr, reg & 0xFF, value);
    return ret;
}

static int set_reg_bits(sensor_t *sensor, uint8_t reg, uint8_t offset, uint8_t length, uint8_t value)
{
    int ret = 0;
    ret = SCCB_Read(sensor->slv_addr, reg);
    if(ret < 0){
        return ret;
    }
    uint8_t mask = ((1 << length) - 1) << offset;
    value = (ret & ~mask) | ((value << offset) & mask);
    ret = SCCB_Write(sensor->slv_addr, reg & 0xFF, value);
    return ret;
}

static int get_reg_bits(sensor_t *sensor, uint8_t reg, uint8_t offset, uint8_t length)
{
    int ret = 0;
    ret = SCCB_Read(sensor->slv_addr, reg);
    if(ret < 0){
        return ret;
    }
    uint8_t mask = ((1 << length) - 1) << offset;
    return (ret & mask) >> offset;
}
static int reset(sensor_t *sensor)
{
    // // Reset all registers
    int ret;
    ret = SCCB_Write(sensor->slv_addr, 0x12, 0x80);

    // Delay 10 ms
    vTaskDelay(10 / portTICK_PERIOD_MS);
    ESP_LOGE(TAG, "OV7740 Reset");
    // Write default regsiters
    for (int i = 0; default_regs[i][0]; i++) {
        ret |= SCCB_Write(sensor->slv_addr, default_regs[i][0], default_regs[i][1]);
    }

    // Delay
    // vTaskDelay(30 / portTICK_PERIOD_MS);
    // for (int i = 0; default_regs[i][0]; i++) {
    //     get_reg(sensor,default_regs[i][0],0);
    
    // }
    return ret;
}

static int set_pixformat(sensor_t *sensor, pixformat_t pixformat)
{
    // int ret=0;
    // // Write back register COM7
    // ret = SCCB_Write(sensor->slv_addr, 0x0C, 0x02);

    // // Delay
    // vTaskDelay(30 / portTICK_PERIOD_MS);
    // ret = SCCB_Write(sensor->slv_addr, 0x12, 0x00);
    // ret = SCCB_Write(sensor->slv_addr, 0x81, 0x08);
    // return ret;
    return 0;

}

static int set_framesize(sensor_t *sensor, framesize_t framesize)
{
    int ret=0;
    // uint16_t w = resolution[framesize].width;
    // uint16_t h = resolution[framesize].height;
    sensor->status.framesize = framesize;

     ESP_LOGE(TAG, "Setting Frame Size: %d",framesize);
    // // VGA
    if (framesize == 8)
    {
        ESP_LOGE(TAG, "OV7740 set_framesize 640x480");
        ret |= SCCB_Write(sensor->slv_addr, 0x31, 0xA0);
        ret |= SCCB_Write(sensor->slv_addr, 0x32, 0xF0);
        ret |= SCCB_Write(sensor->slv_addr, 0x82, 0x32);
    }

    /*
    
     640x480

    {0x31, 0xA0},
    {0x32, 0xF0},
    {0x82, 0x32},

    320x240
    {0x31, 0x50},
    {0x32, 0x78},
    {0x82, 0x3f},

    */


    // // QVGA
    // else if( ((w <= 320) && (h <= 240)) && ((w > 160) || (h > 120)) )
    // {
    //     ret |= SCCB_Write(sensor->slv_addr, 0x31, 0x50);
    //     ret |= SCCB_Write(sensor->slv_addr, 0x32, 0x78);
    //     ret |= SCCB_Write(sensor->slv_addr, 0x82, 0x3F);
    // }
    // QVGA
    else
    {
        ESP_LOGE(TAG, "OV7740 set_framesize 320x240");
        ret |= SCCB_Write(sensor->slv_addr, 0x31, 0x50);
        ret |= SCCB_Write(sensor->slv_addr, 0x32, 0x78);
        ret |= SCCB_Write(sensor->slv_addr, 0x82, 0x3F);
    }    
    // Delay
    vTaskDelay(30 / portTICK_PERIOD_MS);
    return ret;
}

static int set_framerate(sensor_t *sensor, framerate_t framerate)
{
    // int ret = 0;
    // switch(framerate)
    // {
    //     case FRAMERATE_60FPS:
    //         ret |= SCCB_Write(sensor->slv_addr, 0x11, 0x00);
    //         ret |= SCCB_Write(sensor->slv_addr, 0x55, 0x40);
    //         ret |= SCCB_Write(sensor->slv_addr, 0x2b, 0xF0);
    //         ret |= SCCB_Write(sensor->slv_addr, 0x2c, 0x01);
    //         break;
    //     case FRAMERATE_30FPS:
    //         ret |= SCCB_Write(sensor->slv_addr, 0x11, 0x01);
    //         ret |= SCCB_Write(sensor->slv_addr, 0x55, 0x40);
    //         ret |= SCCB_Write(sensor->slv_addr, 0x2b, 0xF0);
    //         ret |= SCCB_Write(sensor->slv_addr, 0x2c, 0x01);
    //         break;
    //     case FRAMERATE_25FPS:
    //         ret |= SCCB_Write(sensor->slv_addr, 0x11, 0x01);
    //         ret |= SCCB_Write(sensor->slv_addr, 0x55, 0x40);
    //         ret |= SCCB_Write(sensor->slv_addr, 0x2b, 0x5E);
    //         ret |= SCCB_Write(sensor->slv_addr, 0x2c, 0x02);
    //         break;
    //     case FRAMERATE_15FPS:
    //         ret |= SCCB_Write(sensor->slv_addr, 0x11, 0x03);
    //         ret |= SCCB_Write(sensor->slv_addr, 0x55, 0x40);
    //         ret |= SCCB_Write(sensor->slv_addr, 0x2b, 0xF0);
    //         ret |= SCCB_Write(sensor->slv_addr, 0x2c, 0x01);
    //         break;
    //     default:
    //         return -1;
    // }
    // // Delay
    // vTaskDelay(30 / portTICK_PERIOD_MS);
    // return ret;
    return 0;
}
esp_err_t xclk_timer_conf(int ledc_timer, int xclk_freq_hz);
static int set_xclk(sensor_t *sensor, int timer, int xclk)
{
    int ret = 0;
    // sensor->xclk_freq_hz = xclk * 1000000U;
    // ret = xclk_timer_conf(timer, sensor->xclk_freq_hz);
    return ret;
}

static int set_contrast(sensor_t *sensor, int level)
{
    int ret=0;
    // uint8_t tmp = 0;

    // level += (NUM_CONTRAST_LEVELS / 2);
    // if (level < 0 || level >= NUM_CONTRAST_LEVELS) {
    //     return -1;
    // }
    // tmp = SCCB_Read(sensor->slv_addr, 0x81);
    // ret |= tmp;
    // tmp |= 0x20;
    // ret |= SCCB_Write(sensor->slv_addr, 0x81, tmp);
    // tmp = SCCB_Read(sensor->slv_addr, 0xDA);
    // ret |= tmp;
    // tmp |= 0x04;
    // ret |= SCCB_Write(sensor->slv_addr, 0xDA, tmp);
    // ret |= SCCB_Write(sensor->slv_addr, 0xE1, contrast_regs[level][0]);
    // ret |= SCCB_Write(sensor->slv_addr, 0xE2, contrast_regs[level][1]);
    // ret |= SCCB_Write(sensor->slv_addr, 0xE3, contrast_regs[level][2]);
    // tmp = SCCB_Read(sensor->slv_addr, 0xE4);
    // ret |= tmp;
    // tmp &= 0xFB;
    // ret |= SCCB_Write(sensor->slv_addr, 0xE4, tmp);

    return ret;
}

static int set_brightness(sensor_t *sensor, int level)
{
    int ret=0;
    // uint8_t tmp = 0;

    // level += (NUM_BRIGHTNESS_LEVELS / 2);
    // if (level < 0 || level >= NUM_BRIGHTNESS_LEVELS) {
    //     return -1;
    // }
    // //ret |= SCCB_Read(sensor->slv_addr, 0x81,&tmp);
    // tmp = SCCB_Read(sensor->slv_addr, 0x81);
    // ret |= tmp;
    // tmp |= 0x20;
    // ret |= SCCB_Write(sensor->slv_addr, 0x81, tmp);
    // //ret |= SCCB_Read(sensor->slv_addr, 0xDA,&tmp);
    // tmp = SCCB_Read(sensor->slv_addr, 0xDA);
    // ret |= tmp;
    // tmp |= 0x04;
    // ret |= SCCB_Write(sensor->slv_addr, 0xDA, tmp);
    // ret |= SCCB_Write(sensor->slv_addr, 0xE4, brightness_regs[level][0]);
    // ret |= SCCB_Write(sensor->slv_addr, 0xE3, brightness_regs[level][1]);
    return ret;
}

static int set_saturation(sensor_t *sensor, int level)
{
    int ret=0;
    // uint8_t tmp = 0;

    // level += (NUM_SATURATION_LEVELS / 2 );
    // if (level < 0 || level >= NUM_SATURATION_LEVELS) {
    //     return -1;
    // }
    // //ret |= SCCB_Read(sensor->slv_addr, 0x81,&tmp);
    // tmp = SCCB_Read(sensor->slv_addr, 0x81);
    // ret |= tmp;
    // tmp |= 0x20;
    // ret |= SCCB_Write(sensor->slv_addr, 0x81, tmp);
    // //ret |= SCCB_Read(sensor->slv_addr, 0xDA,&tmp);
    // tmp = SCCB_Read(sensor->slv_addr, 0xDA);
    // ret |= tmp;
    // tmp |= 0x02;
    // ret |= SCCB_Write(sensor->slv_addr, 0xDA, tmp);
    // ret |= SCCB_Write(sensor->slv_addr, 0xDD, saturation_regs[level][0]);
    // ret |= SCCB_Write(sensor->slv_addr, 0xDE, saturation_regs[level][1]);
    return ret;
}

static int set_gainceiling(sensor_t *sensor, gainceiling_t gainceiling)
{
    return -1;
}

static int set_colorbar(sensor_t *sensor, int enable)
{
    sensor->status.colorbar = enable;
   /*********************************/
    /* To enable Static Test Pattern */
    /*********************************/
    /*  {0x38, 0x07}, */
    /*  {0x84, 0x02}, */

    /*********************************/
    /* Normal Mode / No test pattern */
    //{0x38, 0x14},

    int ret=0;
    uint8_t reg;

    if(enable){
        ESP_LOGE(TAG, "enable colorbar");
        ret |= SCCB_Write(sensor->slv_addr, 0x38, 0x07);
        ret |= SCCB_Write(sensor->slv_addr, 0x84, 0x02);
    }else{
        ESP_LOGE(TAG, "disable colorbar");
        ret |= SCCB_Write(sensor->slv_addr, 0x38, 0x07);
        ret |= SCCB_Write(sensor->slv_addr, 0x84, 0x00);
        ret |= SCCB_Write(sensor->slv_addr, 0x38, 0x14);
    }

     return ret;
}


static int set_auto_gain(sensor_t *sensor, int enable, float gain_db, float gain_db_ceiling)
{
    return -1;
}

// static int get_gain_db(sensor_t *sensor, float *gain_db)
// {
//     return -1;
// }

static int set_auto_exposure(sensor_t *sensor, int enable, int exposure_us)
{
    return -1;
}

// static int get_exposure_us(sensor_t *sensor, int *exposure_us)
// {
//     return -1;
// }

static int set_auto_whitebal(sensor_t *sensor, int enable, float r_gain_db, float g_gain_db, float b_gain_db)
{
    return -1;
}

static int get_rgb_gain_db(sensor_t *sensor, float *r_gain_db, float *g_gain_db, float *b_gain_db)
{
    return -1;
}



static int set_vflip(sensor_t *sensor, int enable)
{
    sensor->status.vflip = enable;
    int ret=0;
    
    ret = set_reg_bits(sensor,OV7740_REG0C,7,1,enable);
     ESP_LOGE(TAG, "flip: %d",enable);
   //ret |= SCCB_Write(sensor->slv_addr, 0x0c, ret);
    return ret;
}

static int set_hmirror(sensor_t *sensor, int enable)
{
    int ret=0;
    sensor->status.hmirror = enable;
     ret = set_reg_bits(sensor,OV7740_REG0C,6,1,enable);
     ESP_LOGE(TAG, "hmirror: %d",enable);
    // int ret=0;
    // ret = SCCB_Read(sensor->slv_addr, 0x0c);
    // ret |= OV7740_REG0C_FLIP_ENABLE;
    // if(enable){
    //     ret |= OV7740_REG0C_MIRROR_ENABLE;
    // }else{
    //     ret |= OV7740_REG0C_MIRROR_DISABLE;
    // }
    //ESP_LOGE(TAG, "mirror: %d",enable);
    //ret = SCCB_Write(sensor->slv_addr, 0x0c, ret);
    return ret;
}

static int set_special_effect(sensor_t *sensor, int effect)
{
    return -1;
}

static int set_lens_correction(sensor_t *sensor, int enable, int radi, int coef)
{
    return -1;
}

static int init_status(sensor_t *sensor)
{
//     sensor->status.brightness = SCCB_Read(sensor->slv_addr, 0x9B);
//     sensor->status.contrast = SCCB_Read(sensor->slv_addr, 0x9C);
//     sensor->status.saturation = 0;
//     sensor->status.ae_level = 0;
//     sensor->status.special_effect = get_reg_bits(sensor, 0x64, 5, 1);
//     sensor->status.wb_mode = get_reg_bits(sensor, 0x6B, 7, 1);
//     sensor->status.agc_gain = get_reg_bits(sensor, COM9, 4, 3);
//     sensor->status.aec_value = SCCB_Read(sensor->slv_addr, AEC) | (SCCB_Read(sensor->slv_addr, AECH) << 8);
//     sensor->status.gainceiling = SCCB_Read(sensor->slv_addr, 0x00);
//     sensor->status.awb = get_reg_bits(sensor, COM8, 1, 1);
//     sensor->status.awb_gain = get_reg_bits(sensor, 0x63, 7, 1);
//     sensor->status.aec = get_reg_bits(sensor, COM8, 0, 1);
//     sensor->status.aec2 = get_reg_bits(sensor, COM8, 7, 1);
//     sensor->status.agc = get_reg_bits(sensor, COM8, 2, 1);
//     sensor->status.bpc = get_reg_bits(sensor, 0x64, 1, 1);
//     sensor->status.wpc = get_reg_bits(sensor, 0x64, 0, 1);
//     sensor->status.raw_gma = get_reg_bits(sensor, 0x64, 2, 1);
//     sensor->status.lenc = get_reg_bits(sensor, LC_CTR, 0, 1);
        sensor->status.hmirror = get_reg_bits(sensor, OV7740_REG0C, 6, 1);
        ESP_LOGE(TAG,"HMIRROR STATUS: %d", get_reg_bits(sensor, OV7740_REG0C, 6, 1));
        sensor->status.vflip = get_reg_bits(sensor, OV7740_REG0C, 7, 1);
//     sensor->status.dcw = get_reg_bits(sensor, 0x65, 2, 1);
//     sensor->status.colorbar = get_reg_bits(sensor, COM3, 0, 1);
//     sensor->status.sharpness = get_reg_bits(sensor, EDGE0, 0, 5);
//     sensor->status.denoise = SCCB_Read(sensor->slv_addr, 0x8E);
    return 0;
}
static int _set_pll(sensor_t *sensor, int bypass, int multiplier, int sys_div, int root_2x, int pre_div, int seld5, int pclk_manual, int pclk_div){return -1;}

int ov7740_init(sensor_t *sensor)
{
    // Initialize sensor structure.
    //sensor->gs_bpp              = 2;
    sensor->reset               = reset;

    sensor->init_status = init_status;
    //sensor->sleep               = ov7740_sleep;
    sensor->get_reg             = get_reg;
    sensor->set_reg             = set_reg;
    sensor->set_pixformat       = set_pixformat;
    sensor->set_framesize       = set_framesize;
    sensor->set_framerate       = set_framerate;
    sensor->set_contrast        = set_contrast;
    sensor->set_brightness      = set_brightness;
    sensor->set_saturation      = set_saturation;
    sensor->set_gainceiling     = set_gainceiling;
    sensor->set_colorbar        = set_colorbar;
    sensor->set_gain_ctrl       = set_auto_gain;
    //sensor->get_gain_db         = get_gain_db;
    sensor->set_exposure_ctrl   = set_auto_exposure;
    //sensor->get_exposure_us     = get_exposure_us;
    sensor->set_whitebal   = set_auto_whitebal;
    //sensor->get_rgb_gain_db     = get_rgb_gain_db;
    sensor->set_hmirror         = set_hmirror;
    sensor->set_vflip           = set_vflip;
    sensor->set_special_effect  = set_special_effect;
    sensor->set_lenc = set_lens_correction;
    //sensor->set_pll = _set_pll;
    sensor->set_xclk = set_xclk;

    // // Set sensor flags
    // SENSOR_HW_FLAGS_SET(sensor, SENSOR_HW_FLAGS_VSYNC, 1);
    // SENSOR_HW_FLAGS_SET(sensor, SENSOR_HW_FLAGS_HSYNC, 0);
    // SENSOR_HW_FLAGS_SET(sensor, SENSOR_HW_FLAGS_PIXCK, 1);
    // SENSOR_HW_FLAGS_SET(sensor, SENSOR_HW_FLAGS_FSYNC, 1);
    // SENSOR_HW_FLAGS_SET(sensor, SENSOR_HW_FLAGS_JPEGE, 0);

     // Retrieve sensor's signature
    sensor->id.MIDH = SCCB_Read(sensor->slv_addr, REG_MIDH);
    sensor->id.MIDL = SCCB_Read(sensor->slv_addr, REG_MIDL);
    sensor->id.PID = SCCB_Read(sensor->slv_addr, REG_PID);
    sensor->id.VER = SCCB_Read(sensor->slv_addr, REG_VER);
    
    ESP_LOGE(TAG, "OV7740 Attached");
    //  ESP_LOGE(TAG, "default 0x36 reg");
    // get_reg(sensor, 0x36, 0);
    return 0;
}


