void
xf86DestroyI2CDevRec(I2CDevPtr d, Bool unalloc) 
{
    if (d) {
	I2CDevPtr *p;

	/* Remove this from the list of active I2C devices. */

	for (p = &d->pI2CBus->FirstDev; *p != NULL; p = &(*p)->NextDev) 
	    if (*p == d) {
		*p = (*p)->NextDev;
		break;
	    }

	fprintf(stderr,
            "I2C device \"%s:%s\" removed.\n",
            d->pI2CBus->BusName, d->DevName);

	if (unalloc) free(d);
    }
}