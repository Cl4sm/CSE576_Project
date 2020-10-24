Bool
xf86I2CDevInit(I2CDevPtr d) 
{
    I2CBusPtr b;

    if (d == NULL ||
	(b = d->pI2CBus) == NULL ||
        (d->SlaveAddr & 1) ||
        xf86I2CFindDev(b, d->SlaveAddr) != NULL)
	return FALSE;

    if (d->BitTimeout <= 0) d->BitTimeout = b->BitTimeout;
    if (d->ByteTimeout <= 0) d->ByteTimeout = b->ByteTimeout;
    if (d->AcknTimeout <= 0) d->AcknTimeout = b->AcknTimeout;
    if (d->StartTimeout <= 0) d->StartTimeout = b->StartTimeout;

    d->NextDev = b->FirstDev;
    b->FirstDev = d;

    fprintf(stderr,
            "I2C device \"%s:%s\" registered at address 0x%02X.\n",
            b->BusName, d->DevName, d->SlaveAddr);

    return TRUE;
}