Bool
xf86I2CWriteBytes(I2CDevPtr d, I2CByte subaddr, 
		  I2CByte *WriteBuffer, int nWrite)
{
    I2CBusPtr b = d->pI2CBus;
    Bool r = TRUE;

    if (nWrite > 0) {
	r = b->I2CAddress(d, d->SlaveAddr & ~1);
	if (r){
	    if ((r = b->I2CPutByte(d, subaddr)))
		for (; nWrite > 0; WriteBuffer++, nWrite--)
		    if (!(r = b->I2CPutByte(d, *WriteBuffer))) 
			break;

	    b->I2CStop(d);
	}
    }

    return r;
}