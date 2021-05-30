#ifndef BMX055_H
#define BMX055_H

#include <Arduino.h>
#include <SPI.h>

#define BMX_SCK PB13
#define BMX_MISO PB14
#define BMX_MOSI PB15

#define ACC_CS PC7
#define GYR_CS PC4

#define CHP_ID_REG 0x00

const uint8_t READ = 0x80;  // BMX055 read command
const uint8_t WRITE = 0x00; // BMX055 write command

/**
     * BMA250E registers
     */
typedef enum : uint8_t
{
    REG_CHIP_ID = 0x00,
    // 0x01 reserved
    REG_ACCD_X_LSB = 0x02,
    REG_ACCD_X_MSB = 0x03,
    REG_ACCD_Y_LSB = 0x04,
    REG_ACCD_Y_MSB = 0x05,
    REG_ACCD_Z_LSB = 0x06,
    REG_ACCD_Z_MSB = 0x07,
    REG_TEMP = 0x08,
    REG_INT_STATUS_0 = 0x09,
    REG_INT_STATUS_1 = 0x0a,
    REG_INT_STATUS_2 = 0x0b,
    REG_INT_STATUS_3 = 0x0c,
    // 0x0d reserved
    REG_FIFO_STATUS = 0x0e,
    REG_PMU_RANGE = 0x0f,
    REG_PMU_BW = 0x10,
    REG_PMU_LPW = 0x11,
    REG_PMU_LOW_POWER = 0x12,
    REG_ACC_HBW = 0x13,
    REG_SOFTRESET = 0x14,
    // 0x15 reserved
    REG_INT_EN_0 = 0x16,
    REG_INT_EN_1 = 0x17,
    REG_INT_EN_2 = 0x18,
    REG_INT_MAP_0 = 0x19,
    REG_INT_MAP_1 = 0x1a,
    REG_INT_MAP_2 = 0x1b,
    // 0x1c-0x1d reserved
    REG_INT_SRC = 0x1e,
    // 0x1f reserved
    REG_INT_OUT_CTRL = 0x20,
    REG_INT_RST_LATCH = 0x21,
    REG_INT_0 = 0x22,
    REG_INT_1 = 0x23,
    REG_INT_2 = 0x24,
    REG_INT_3 = 0x25,
    REG_INT_4 = 0x26,
    REG_INT_5 = 0x27,
    REG_INT_6 = 0x28,
    REG_INT_7 = 0x29,
    REG_INT_8 = 0x2a,
    REG_INT_9 = 0x2b,
    REG_INT_A = 0x2c,
    REG_INT_B = 0x2d,
    REG_INT_C = 0x2e,
    REG_INT_D = 0x2f,
    REG_FIFO_CONFIG_0 = 0x30,
    // 0x31 reserved
    REG_PMU_SELFTEST = 0x32,
    REG_TRIM_NVM_CTRL = 0x33,
    REG_SPI3_WDT = 0x34,
    // 0x35 reserved
    REG_OFC_CTRL = 0x36,
    REG_OFC_SETTING = 0x37,
    REG_OFC_OFFSET_X = 0x38,
    REG_OFC_OFFSET_Y = 0x39,
    REG_OFC_OFFSET_Z = 0x3a,
    REG_TRIM_GP0 = 0x3b,
    REG_TRIM_GP1 = 0x3c,
    // 0x3d reserved
    REG_FIFO_CONFIG_1 = 0x3e,
    REG_FIFO_DATA = 0x3f
} BMA250E_REGS_T;

/**
* PMU_RANGE (accelerometer g-range) values
*/
typedef enum
{
    RANGE_2G = 3,
    RANGE_4G = 5,
    RANGE_8G = 8,
    RANGE_16G = 12
} RANGE_T;

/**
     * REG_ACC_HBW bits
     */
typedef enum
{
    _ACC_HBW_RESERVED_BITS = 0x0f | 0x10 | 0x20,
    // 0x01-0x20 reserved
    ACC_HBW_SHADOW_DIS = 0x40,
    ACC_HBW_DATA_HIGH_BW = 0x80
} ACC_HBW_BITS_T;

/**
* PMU_BW (accelerometer filter bandwidth) values
*/
typedef enum
{
    BW_7_81 = 8, // 7.81 Hz
    BW_15_63 = 9,
    BW_31_25 = 10,
    BW_62_5 = 11,
    BW_125 = 12,
    BW_250 = 13,
    BW_500 = 14,
    BW_1000 = 15
} BW_T;

/**
* POWER_MODE values
*/
typedef enum
{
    POWER_MODE_NORMAL = 0,
    POWER_MODE_DEEP_SUSPEND = 1,
    POWER_MODE_LOW_POWER = 2,
    POWER_MODE_SUSPEND = 4
} POWER_MODE_T;

/**
* REG_PMU_LOW_POWER bits
*/
typedef enum
{
    _LOW_POWER_RESERVED_BITS = 0x9F,
    // 0x01-0x10 reserved
    LOW_POWER_SLEEPTIMER_MODE = 0x20,
    LOW_POWER_LOWPOWER_MODE = 0x40 // LPM1 or LPM2 mode. see DS.
                                   // 0x80 reserved
} LOW_POWER_BITS_T;

/**
* REG_PMU_LPW bits
*/
typedef enum
{
    // 0x01 reserved
    _PMU_LPW_RESERVED_MASK = 0x01,
    PMU_LPW_SLEEP_DUR0 = 0x02, // sleep dur in low power mode
    PMU_LPW_SLEEP_DUR1 = 0x04,
    PMU_LPW_SLEEP_DUR2 = 0x08,
    PMU_LPW_SLEEP_DUR3 = 0x10,
    _PMU_LPW_SLEEP_MASK = 15,
    _PMU_LPW_SLEEP_SHIFT = 1,
    // These are separate bits, deep_suspend, lowpower_en and
    // suspend (and if all 0, normal).  Since only specific
    // combinations are allowed, we will treat this as a 3 bit
    // bitfield called POWER_MODE.
    PMU_LPW_POWER_MODE0 = 0x20, // deep_suspend
    PMU_LPW_POWER_MODE1 = 0x40, // lowpower_en
    PMU_LPW_POWER_MODE2 = 0x80, // suspend
    _PMU_LPW_POWER_MODE_MASK = 7,
    _PMU_LPW_POWER_MODE_SHIFT = 5
} PMU_LPW_BITS_T;

class BMX055
{
private:
    float xAccl, yAccl, zAccl;
    float xGyro, yGyro, zGyro;
    int xMag, yMag, zMag;
    SPISettings _spi_settings;
    SPIClass *_spi;

    uint32_t SPIReadAccel(uint8_t reg, uint8_t bytes_to_read);
    void SPIWriteAccel(uint8_t reg, uint8_t write_data);

public:
    BMX055();
    ~BMX055();

    void init(POWER_MODE_T power = POWER_MODE_NORMAL, RANGE_T range = RANGE_2G, BW_T bw = BW_7_81);
    void setPowerMode(POWER_MODE_T power_mode);
    void setLowPowerMode2();
    void setRange(RANGE_T range);
    void setBandwidth(BW_T bw);
    void enableOutputFiltering(bool enable = true);
    void enableRegisterShadowing(bool enable = true);
    uint8_t getChipID();
};

#endif