#ifndef BMX055_H
#define BMX055_H

#include <Arduino.h>
#include <SPI.h>
#include "register_map.h"

#define BMX_SCK PB13
#define BMX_MISO PB14
#define BMX_MOSI PB15

#define ACC_CS PC7
#define GYR_CS PC4

#define FIFO_BUF_LEN 6

const uint8_t READ = 0x80;  // BMX055 read command
const uint8_t WRITE = 0x00; // BMX055 write command

class BMX055
{
private:
    SPISettings _spi_settings;
    SPIClass *_spi;
    bool use_fifo;

    float accScale;
    float accX, accY, accZ;

    uint64_t SPIReadAccel(uint8_t reg, uint8_t bytes_to_read);
    void SPIWriteAccel(uint8_t reg, uint8_t write_data);

public:
    BMX055();
    ~BMX055();

    void init(POWER_MODE_T power = POWER_MODE_NORMAL, RANGE_T range = RANGE_2G, BW_T bw = BW_7_81);
    void update();
    void setPowerMode(POWER_MODE_T power_mode);
    void setLowPowerMode2();
    void setRange(RANGE_T range);
    void setBandwidth(BW_T bw);
    void enableOutputFiltering(bool enable = true);
    void enableRegisterShadowing(bool enable = true);
    void enableFIFO(bool enable = true);
    void FIFOConfig(FIFO_MODE_T mode = FIFO_MODE_BYPASS, FIFO_DATA_SEL_T data_select = FIFO_DATA_SEL_XYZ);
    void getAccelerometer(float *x, float *y, float *z);
    uint8_t getChipID();
};

#endif