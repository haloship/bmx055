#include "bmx055.h"

BMX055::BMX055() : _spi()
{
    pinMode(ACC_CS, OUTPUT);
    this->_spi_settings = SPISettings(SPI_SPEED_CLOCK_DEFAULT, MSBFIRST, SPI_MODE_0);
    this->_spi = new SPIClass(BMX_MOSI, BMX_MISO, BMX_SCK);
}

BMX055::~BMX055() {}

void BMX055::init(POWER_MODE_T power, RANGE_T range, BW_T bw)
{
    // In case we use low power mode, we want to make sure it works in low power mode 2
    this->setLowPowerMode2();

    // Set the default power mode to normal
    this->setPowerMode(power);

    // Set range to 2G
    this->setRange(range);

    // Set bandwidth
    this->setBandwidth(bw);

    // enable output filtering, register shadowing, FIFO
    this->enableOutputFiltering();
    this->enableRegisterShadowing();
    this->FIFOConfig();
    this->enableFIFO();

    return;
}

void BMX055::update()
{
    int buffer_length = FIFO_BUF_LEN;
    uint16_t mask = 0xFFFF;
    uint8_t shift = 0;
    float divisor = 16.0;
    uint8_t reg = REG_FIFO_DATA;

    uint64_t result = this->SPIReadAccel(reg, FIFO_BUF_LEN);

    int16_t value;
    // z accel
    shift = 0;
    value = (result >> shift) & mask;
    this->accZ = float(value / divisor);
    Serial.println((accZ * this->accScale) / 1000.0);

    // z accel
    shift = 0;
    value = (result >> shift) & mask;
    this->accZ = float(value / divisor);
    Serial.println((accZ * this->accScale) / 1000.0);

    // z accel
    shift = 0;
    value = (result >> shift) & mask;
    this->accZ = float(value / divisor);
    Serial.println((accZ * this->accScale) / 1000.0);
}

uint8_t BMX055::getChipID()
{
    uint8_t result = this->SPIReadAccel(REG_CHIP_ID, 1);
    return result;
}

void BMX055::setRange(RANGE_T range)
{
    switch (range)
    {
    case RANGE_2G:
        this->accScale = 0.91;
        break;
    case RANGE_4G:
        this->accScale = 1.95;
        break;
    case RANGE_8G:
        this->accScale = 3.91;
        break;
    case RANGE_16G:
        this->accScale = 7.81;
        break;
    }
    this->SPIWriteAccel(REG_PMU_RANGE, range);
}

void BMX055::setBandwidth(BW_T bw)
{
    this->SPIWriteAccel(REG_PMU_BW, bw);
}

void BMX055::FIFOConfig(FIFO_MODE_T mode, FIFO_DATA_SEL_T data_select)
{
    uint8_t config = ((mode << _FIFO_CONFIG_1_FIFO_MODE_SHIFT) | (data_select << _FIFO_CONFIG_1_FIFO_DATA_SHIFT));

    this->SPIWriteAccel(REG_FIFO_CONFIG_1, config);
}

void BMX055::enableFIFO(bool enable)
{
    this->use_fifo = enable;
}

void BMX055::enableOutputFiltering(bool enable)
{
    uint8_t filtering = (uint8_t)this->SPIReadAccel(REG_ACC_HBW, 1) & ~_ACC_HBW_RESERVED_BITS;

    if (enable)
    {
        filtering &= ~ACC_HBW_DATA_HIGH_BW;
    }
    else
    {
        filtering |= ACC_HBW_DATA_HIGH_BW;
    }

    this->SPIWriteAccel(REG_ACC_HBW, filtering);
}

void BMX055::enableRegisterShadowing(bool enable)
{
    uint8_t shadowing = (uint8_t)this->SPIReadAccel(REG_ACC_HBW, 1) & ~_ACC_HBW_RESERVED_BITS;

    if (enable)
    {
        shadowing &= ~ACC_HBW_SHADOW_DIS;
    }
    else
    {
        shadowing |= ACC_HBW_SHADOW_DIS;
    }

    this->SPIWriteAccel(REG_ACC_HBW, shadowing);
}

void BMX055::setPowerMode(POWER_MODE_T power)
{
    uint8_t power_mode = (uint8_t)this->SPIReadAccel(REG_PMU_LPW, 1) & ~_PMU_LPW_RESERVED_MASK;
    power_mode &= ~(_PMU_LPW_POWER_MODE_MASK << _PMU_LPW_POWER_MODE_SHIFT);
    power_mode |= (power << _PMU_LPW_POWER_MODE_SHIFT);
    this->SPIWriteAccel(REG_PMU_LPW, power_mode);
}

void BMX055::setLowPowerMode2()
{
    uint8_t low_power_config = (uint8_t)this->SPIReadAccel(REG_PMU_LOW_POWER, 1) & ~_LOW_POWER_RESERVED_BITS;
    low_power_config |= LOW_POWER_LOWPOWER_MODE;
    this->SPIWriteAccel(REG_PMU_LOW_POWER, low_power_config);
}

uint64_t BMX055::SPIReadAccel(uint8_t reg, uint8_t bytes_to_read)
{
    uint8_t in_byte = 0;
    uint64_t result = 0;
    uint8_t command = reg | READ;
    this->_spi->beginTransaction(this->_spi_settings);
    digitalWrite(ACC_CS, LOW);
    this->_spi->transfer(command);
    result = this->_spi->transfer(0x00);
    bytes_to_read--;
    while (bytes_to_read > 0)
    {
        result = result << 8;
        in_byte = this->_spi->transfer(0x00);
        result = result | in_byte;
        bytes_to_read--;
    }
    digitalWrite(ACC_CS, HIGH);
    this->_spi->endTransaction();
    return result;
}

void BMX055::SPIWriteAccel(uint8_t reg, uint8_t write_data)
{
    uint8_t command = reg | WRITE;
    this->_spi->beginTransaction(this->_spi_settings);
    digitalWrite(ACC_CS, LOW);
    this->_spi->transfer(command);
    this->_spi->transfer(write_data);
    digitalWrite(ACC_CS, HIGH);
    this->_spi->endTransaction();
    return;
}