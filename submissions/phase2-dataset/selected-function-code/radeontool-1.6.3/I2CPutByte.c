static Bool
I2CPutByte(I2CDevPtr d, I2CByte data)
{
    Bool r;
    int i, scl, sda;
    I2CBusPtr b = d->pI2CBus;

    if (!I2CWriteBit(b, (data >> 7) & 1, d->ByteTimeout))
	return FALSE;

    for (i = 6; i >= 0; i--)
	if (!I2CWriteBit(b, (data >> i) & 1, d->BitTimeout))
	    return FALSE;

    b->I2CPutBits(b, 0, 1);
    b->I2CUDelay(b, b->RiseFallTime);

    r = I2CRaiseSCL(b, 1, b->HoldTime);

    if (r) {
    	for (i = d->AcknTimeout; i > 0; i -= b->HoldTime) {
	    b->I2CUDelay(b, b->HoldTime);
	    b->I2CGetBits(b, &scl, &sda);
	    if (sda == 0) break;
	}

	if (i <= 0) {
	    I2C_TIMEOUT(ErrorF("[I2CPutByte(<%s>, 0x%02x, %d, %d, %d) timeout]", 
				       b->BusName, data, d->BitTimeout, 
				       d->ByteTimeout, d->AcknTimeout));
	    r = FALSE;
	}

	I2C_TRACE(ErrorF("W%02x%c ", (int) data, sda ? '-' : '+'));
    }

    b->I2CPutBits(b, 0, 1);
    b->I2CUDelay(b, b->HoldTime);

    return r;
}