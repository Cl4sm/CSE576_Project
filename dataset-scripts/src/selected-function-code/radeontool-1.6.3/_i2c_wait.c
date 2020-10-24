void _i2c_wait(void)
{
	int i, num_ready;
	unsigned int tmp;

	_i2c_set(AVIVO_I2C_STATUS, AVIVO_I2C_STATUS_CMD_WAIT);
	for (i = 0, num_ready = 0; num_ready < 3; i++) {
		tmp = _i2c_get(AVIVO_I2C_STATUS);
		if (tmp == AVIVO_I2C_STATUS_DONE) {
			num_ready++;
		} else if (tmp != AVIVO_I2C_STATUS_CMD_WAIT) {
			_i2c_stop();
		}
		/* Timeout. */
		if (i == 10) {
			fprintf(stderr, "i2c timeout\n");
			exit(1);
		}
		usleep(1000);
	}
	_i2c_set(AVIVO_I2C_STATUS, AVIVO_I2C_STATUS_DONE);	
}