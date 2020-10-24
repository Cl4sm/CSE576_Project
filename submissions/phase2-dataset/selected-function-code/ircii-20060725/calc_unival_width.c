unsigned
calc_unival_width(unsigned unival)
{
	/* FIXME: Should we use some kind of database here?
	 * FIXME: Combining marks support is completely untested
	 */
	
	/* chinese, japanese, korean */
	if(unival >= 0x3000 && unival < 0xFF00)
		return 2;
	/* combining diacritical marks */
	if(unival >= 0x0300 && unival < 0x0400)
		return 0;
	/* combining diacritical marks for symbols */
	if(unival >= 0x20D0 && unival < 0x2100)
		return 0;
	/* combining half-marks */
	if(unival >= 0xFE20 && unival < 0xFE30)
		return 0;
	/* everything else */
	return 1;
}
