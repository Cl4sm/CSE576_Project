void radeon_i2c_monitor(int gpio_in, int gpio_out)
{
    I2CBusPtr i2cbus;
	I2CByte wbuf[128];
	I2CByte rbuf[128];
    int i, j;
    I2CDevPtr dev;

    GPIO_IN = gpio_in;
    GPIO_OUT = gpio_out;

    printf("GPIO_IN = 0x%X, GPIO_OUT = 0x%X\n", GPIO_IN, GPIO_OUT);
    if (GPIO_IN != 0x7E3C) {
    //    i2c_initialize();
    } else {
        i2c_initialize_7e30();
    }
    i2cbus = xf86CreateI2CBusRec();
    if (!i2cbus) {
        return;
    }

    i2cbus->BusName    = "DDC";
    i2cbus->I2CPutBits = AVIVOI2CPutBits;
    i2cbus->I2CGetBits = AVIVOI2CGetBits;
    i2cbus->AcknTimeout = 5;

    if (!xf86I2CBusInit(i2cbus))
        return;

    if (!(dev = xf86I2CFindDev(i2cbus, 0x00A0))) {
        dev = xf86CreateI2CDevRec();
        dev->DevName = "ddc2";
        dev->SlaveAddr = 0xA0;
        dev->ByteTimeout = 2200; /* VESA DDC spec 3 p. 43 (+10 %) */
        dev->StartTimeout = 550;
        dev->BitTimeout = 40;
        dev->AcknTimeout = 40;

        dev->pI2CBus = i2cbus;
        if (!xf86I2CDevInit(dev)) {
            fprintf(stderr, "No DDC2 device\n");
            return;
        }
    } else {
        fprintf(stderr, "No device at 0xA0\n");
    }

    wbuf[0] = 0x0;
    xf86I2CWriteRead(dev, wbuf, 1, rbuf, 128);
    for (j=0; j < 8; j++) {
        for (i=0; i < 16; i++) {
            printf("%02X", rbuf[i + j * 16]);
        }
        printf("\n");
    }
    printf("\n");
}