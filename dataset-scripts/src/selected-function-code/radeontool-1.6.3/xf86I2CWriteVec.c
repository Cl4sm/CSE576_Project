Bool
xf86I2CWriteVec(I2CDevPtr d, I2CByte *vec, int nValues)
{
    I2CBusPtr b = d->pI2CBus;
    Bool r = TRUE;
    int s = 0;

    if (nValues > 0) {
	for (; nValues > 0; nValues--, vec += 2) {
	    if (!(r = b->I2CAddress(d, d->SlaveAddr & ~1)))
	    	break;

	    s++;

	    if (!(r = b->I2CPutByte(d, vec[0])))
		break;

	    if (!(r = b->I2CPutByte(d, vec[1]))) 
		break;
	}

	if (s > 0) b->I2CStop(d);	
    }

    return r;
}