static Bool
I2CGetByte(I2CDevPtr d, I2CByte *data, Bool last)
{
    int i, sda;
    I2CBusPtr b = d->pI2CBus;

    b->I2CPutBits(b, 0, 1);
    b->I2CUDelay(b, b->RiseFallTime);

    if (!I2CReadBit(b, &sda, d->ByteTimeout))
	return FALSE;

    *data = (sda > 0) << 7;

    for (i = 6; i >= 0; i--)
	if (!I2CReadBit(b, &sda, d->BitTimeout))
	    return FALSE;
	else
	    *data |= (sda > 0) << i;

    if (!I2CWriteBit(b, last ? 1 : 0, d->BitTimeout))
	return FALSE;

    I2C_TRACE(ErrorF("R%02x%c ", (int) *data, last ? '+' : '-'));

    return TRUE;
}