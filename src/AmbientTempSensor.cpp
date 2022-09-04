#include "../inc/AmbientTempSensor.h"


AmbientTempSensor::AmbientTempSensor(char *name, struct bme280_dev *dev, struct identifier *id) {
    this->rslt = BME280_OK;
    id->fd = -1;
    id->fd = open(name, O_RDWR);
    if (id->fd == -1) raise(SIGINT);

    id->dev_addr = BME280_I2C_ADDR_PRIM;

    if (ioctl(id->fd, I2C_SLAVE, id->dev_addr) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        exit(1);
    }

    dev->intf = BME280_I2C_INTF;
    dev->read = user_i2c_read;
    dev->write = user_i2c_write;
    dev->delay_us = user_delay_us;

    dev->intf_ptr = id;

    rslt = bme280_init(dev);
    if (rslt != BME280_OK)
    {
        printf("Failed to initialize the device (code %+d).\n", rslt);
        raise(SIGINT);
    }
}

int8_t AmbientTempSensor::user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr) {
    struct identifier id;

    id = *((struct identifier *)intf_ptr);

    write(id.fd, &reg_addr, 1);
    read(id.fd, data, len);

    return 0;
}

int8_t AmbientTempSensor::user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr) {
    uint8_t *buf;
    struct identifier id;

    id = *((struct identifier *)intf_ptr);

    buf = (uint8_t *)malloc(len + 1);
    buf[0] = reg_addr;
    memcpy(buf + 1, data, len);
    if (write(id.fd, buf, len + 1) < (uint16_t)len)
    {
        return BME280_E_COMM_FAIL;
    }

    free(buf);

    return BME280_OK;
}

void AmbientTempSensor::user_delay_us(uint32_t period, void *intf_ptr) {
    usleep(period*1000);
}


double AmbientTempSensor::getAmbientTemp(struct bme280_dev *dev){
	uint8_t settings_sel;
	uint32_t reqTime;
	struct bme280_data comp_data;

	dev->settings.osr_h = BME280_OVERSAMPLING_1X;
	dev->settings.osr_p = BME280_OVERSAMPLING_16X;
	dev->settings.osr_t = BME280_OVERSAMPLING_2X;
	dev->settings.filter = BME280_FILTER_COEFF_16;
	
    reqTime = bme280_cal_meas_delay(&dev->settings);	
	settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;
	
    rslt = bme280_set_sensor_settings(settings_sel, dev);
	rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);
	
	dev->delay_us(reqTime, dev->intf_ptr);
	rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
	
	return comp_data.temperature;
}