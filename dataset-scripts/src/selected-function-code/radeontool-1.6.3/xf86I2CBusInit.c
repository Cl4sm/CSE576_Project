Bool
xf86I2CBusInit(I2CBusPtr b) 
{
    /* I2C buses must be identified by a unique scrnIndex 
     * and name. If scrnIndex is unspecified (a negative value), 
     * then the name must be unique throughout the server. 
     */

    if (b->BusName == NULL ||
	xf86I2CFindBus(b->scrnIndex, b->BusName) != NULL)
	return FALSE;

    /* If the high level functions are not
     * supplied, use the generic functions.
     * In this case we need the low-level
     * function.
     */
    if (b->I2CWriteRead == NULL) 
    {
        b->I2CWriteRead=I2CWriteRead;

        if (b->I2CPutBits == NULL ||
	    b->I2CGetBits == NULL) 
        {
	    if (b->I2CPutByte == NULL ||
	        b->I2CGetByte == NULL ||
	        b->I2CAddress == NULL ||
	        b->I2CStart   == NULL ||
	        b->I2CStop    == NULL)
	        return FALSE;
        } else { 
	    b->I2CPutByte = I2CPutByte;
	    b->I2CGetByte = I2CGetByte;
	    b->I2CAddress = I2CAddress;
	    b->I2CStop    = I2CStop;
	    b->I2CStart   = I2CStart;
        }
     }

    if (b->I2CUDelay == NULL)
	b->I2CUDelay = I2CUDelay;

    if (b->HoldTime < 2) b->HoldTime = 5;
    if (b->BitTimeout <= 0) b->BitTimeout = b->HoldTime;
    if (b->ByteTimeout <= 0) b->ByteTimeout = b->HoldTime;
    if (b->AcknTimeout <= 0) b->AcknTimeout = b->HoldTime;
    if (b->StartTimeout <= 0) b->StartTimeout = b->HoldTime;

    /* Put new bus on list. */

    b->NextBus = I2CBusList;
    I2CBusList = b;

    fprintf(stderr, "I2C bus \"%s\" initialized.\n",
	       b->BusName);

    return TRUE;
}