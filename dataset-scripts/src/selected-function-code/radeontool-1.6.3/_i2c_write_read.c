void _i2c_write_read(unsigned char *write_buf, int num_write,
                     unsigned char *read_buf, int num_read,
                     unsigned int connector)
{
	unsigned int tmp;

	if (num_write) {
		_i2c_start(connector);
		tmp = _i2c_get(AVIVO_I2C_CONTROL2) & (~AVIVO_I2C_7D3C_SIZE_MASK);
		tmp |= num_write << AVIVO_I2C_7D3C_SIZE_SHIFT; 
		_i2c_set(AVIVO_I2C_CONTROL2, tmp);
		tmp = _i2c_get(AVIVO_I2C_CONTROL3);
		_i2c_set(AVIVO_I2C_CONTROL3, tmp);

		_i2c_set(AVIVO_I2C_DATA, 0xA0);

		_i2c_write(write_buf, num_write);
		tmp = _i2c_get(AVIVO_I2C_START_CNTL) & (~AVIVO_I2C_STATUS_MASK);
		_i2c_set(AVIVO_I2C_START_CNTL,
                 tmp
                 | AVIVO_I2C_STATUS_DONE
                 | AVIVO_I2C_STATUS_NACK);
		_i2c_wait();
	}

	if (num_read) {
		_i2c_set(AVIVO_I2C_DATA, 0xA0 | 1);
		tmp = _i2c_get(AVIVO_I2C_CONTROL2) & (~AVIVO_I2C_7D3C_SIZE_MASK);
		tmp |= num_read << AVIVO_I2C_7D3C_SIZE_SHIFT; 
		_i2c_set(AVIVO_I2C_CONTROL2, tmp);
		tmp = _i2c_get(AVIVO_I2C_START_CNTL) & (~AVIVO_I2C_STATUS_MASK);
		_i2c_set(AVIVO_I2C_START_CNTL,
                 tmp
                 | AVIVO_I2C_STATUS_DONE
                 | AVIVO_I2C_STATUS_NACK
                 | AVIVO_I2C_STATUS_HALT);
		_i2c_wait();
		_i2c_read(read_buf, num_read);
		_i2c_stop();
	}	
}