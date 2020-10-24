static Bool
I2CWriteRead(I2CDevPtr d,
		 I2CByte *WriteBuffer, int nWrite,
		 I2CByte *ReadBuffer,  int nRead) 
{
    Bool r = TRUE;
    I2CBusPtr b = d->pI2CBus;
    int s = 0;

    if (r && nWrite > 0) {
	r = b->I2CAddress(d, d->SlaveAddr & ~1);
	if (r) {
	    for (; nWrite > 0; WriteBuffer++, nWrite--)
		if (!(r = b->I2CPutByte(d, *WriteBuffer))) 
		    break;
	    s++;
	}
    }

    if (r && nRead > 0) {
	r = b->I2CAddress(d, d->SlaveAddr | 1);
	if (r) {
	    for (; nRead > 0; ReadBuffer++, nRead--)
		if (!(r = b->I2CGetByte(d, ReadBuffer, nRead == 1))) 
		    break;
	    s++;
	}
    }

    if (s) b->I2CStop(d);

    return r;
}