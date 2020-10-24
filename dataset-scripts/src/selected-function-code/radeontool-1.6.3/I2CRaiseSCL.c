static Bool
I2CRaiseSCL(I2CBusPtr b, int sda, int timeout)
{
    int i, scl;

    b->I2CPutBits(b, 1, sda);
    b->I2CUDelay(b, b->RiseFallTime);

    for (i = timeout; i > 0; i -= b->RiseFallTime) {
	b->I2CGetBits(b, &scl, &sda);
	if (scl) break;
	b->I2CUDelay(b, b->RiseFallTime);
    }

    if (i <= 0) {
	I2C_TIMEOUT(ErrorF("[I2CRaiseSCL(<%s>, %d, %d) timeout]", b->BusName, sda, timeout));    
	return FALSE;
    }

    return TRUE;
}