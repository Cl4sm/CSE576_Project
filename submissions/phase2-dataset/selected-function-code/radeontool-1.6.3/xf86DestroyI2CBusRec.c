void
xf86DestroyI2CBusRec(I2CBusPtr b, Bool unalloc, Bool devs_too) 
{
    if (b) {
	I2CBusPtr *p;

	/* Remove this from the list of active I2C buses */

	for (p = &I2CBusList; *p != NULL; p = &(*p)->NextBus) 
	    if (*p == b) {
		*p = (*p)->NextBus;
		break;
	    }

	if (b->FirstDev != NULL) {
	    if (devs_too) {
		I2CDevPtr d;

		while ((d = b->FirstDev) != NULL) {
		    b->FirstDev = d->NextDev;
		    xf86DestroyI2CDevRec(d, unalloc);
		}
	    } else {
		if (unalloc) {
		    fprintf(stderr, "i2c bug: Attempt to remove I2C bus \"%s\", "
                    "but device list is not empty.\n",
                    b->BusName);
		    return;
		}
	    }
	}

	fprintf(stderr, "I2C bus \"%s\" removed.\n",
		   b->BusName);

	if (unalloc) free(b);
    }
}