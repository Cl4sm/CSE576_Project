int
xf86I2CGetScreenBuses(int scrnIndex, I2CBusPtr **pppI2CBus)
{
    I2CBusPtr pI2CBus;
    int n = 0;

    if (pppI2CBus)
	*pppI2CBus = NULL;

    for (pI2CBus = I2CBusList;  pI2CBus;  pI2CBus = pI2CBus->NextBus) {
	if ((pI2CBus->scrnIndex >= 0) && (pI2CBus->scrnIndex != scrnIndex))
	    continue;

	n++;

	if (!pppI2CBus)
	    continue;

        *pppI2CBus = realloc(*pppI2CBus, n * sizeof(I2CBusPtr));
	*pppI2CBus[n - 1] = pI2CBus;
    }

    return n;
}