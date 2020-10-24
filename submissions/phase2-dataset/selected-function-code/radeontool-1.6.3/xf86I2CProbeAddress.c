Bool
xf86I2CProbeAddress(I2CBusPtr b, I2CSlaveAddr addr)
{
    int r;
    I2CDevRec d;

    d.DevName = "Probing";
    d.BitTimeout = b->BitTimeout;
    d.ByteTimeout = b->ByteTimeout;
    d.AcknTimeout = b->AcknTimeout;
    d.StartTimeout = b->StartTimeout;
    d.SlaveAddr = addr;
    d.pI2CBus = b;
    d.NextDev = NULL;

    r = b->I2CAddress(&d, addr);

    if (r) b->I2CStop(&d);

    return r;
}