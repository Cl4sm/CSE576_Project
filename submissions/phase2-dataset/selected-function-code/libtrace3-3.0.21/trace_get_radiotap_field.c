static void *trace_get_radiotap_field(void *link, libtrace_radiotap_field_t field)
{
	struct libtrace_radiotap_t *rtap = (struct libtrace_radiotap_t *)link;
	uint8_t *p;
	uint8_t *s;

	/* Check if the field exists in the radiotap header before proceeding
	*/
	if ((bswap_le_to_host32(rtap->it_present) & (1 << field)) == 0) return NULL;

	/* Skip over any extended bitmasks */
	p = (uint8_t *) &(rtap->it_present);

	while ( bswap_le_to_host32(*((uint32_t*)p)) & (1U << TRACE_RADIOTAP_EXT) ) {
		p += sizeof (uint32_t);
	}

	/* Point p at the first field of radiotap data and remember it for later
	 * when we're doing field alignment 
	 */
	p += sizeof(uint32_t);
	s = p;

	if (field == TRACE_RADIOTAP_TSFT) 
		/* Always aligned */
		return (void*)p;
	if (bswap_le_to_host32(rtap->it_present) & (1 << TRACE_RADIOTAP_TSFT))
		p += sizeof (uint64_t);

	if (field == TRACE_RADIOTAP_FLAGS)
		/* Always aligned */
		return (void*)p;
	if (bswap_le_to_host32(rtap->it_present) & (1 << TRACE_RADIOTAP_FLAGS))
		p += sizeof (uint8_t);

	if (field == TRACE_RADIOTAP_RATE)
		/* Always aligned */
		return (void*)p;
	if (bswap_le_to_host32(rtap->it_present) & (1 << TRACE_RADIOTAP_RATE))
		p+= sizeof (uint8_t);

	if (field == TRACE_RADIOTAP_CHANNEL)
	{
		ALIGN_NATURAL_16(p,s);
		return (void *)p;
	}
	if (bswap_le_to_host32(rtap->it_present) & (1 << TRACE_RADIOTAP_CHANNEL))
		p+= sizeof (uint32_t);

	if (field == TRACE_RADIOTAP_FHSS)
	{
		ALIGN_NATURAL_16(p,s);
		return (void *)p;
	}
	if (bswap_le_to_host32(rtap->it_present) & (1 << TRACE_RADIOTAP_FHSS))
		p+= sizeof (uint16_t);

	if (field == TRACE_RADIOTAP_DBM_ANTSIGNAL)
		return (void *)p;
	if (bswap_le_to_host32(rtap->it_present) & (1 << TRACE_RADIOTAP_DBM_ANTSIGNAL))
		p+= sizeof (uint8_t);

	if (field == TRACE_RADIOTAP_DBM_ANTNOISE)
		return (void *)p;
	if (bswap_le_to_host32(rtap->it_present) & (1 << TRACE_RADIOTAP_DBM_ANTNOISE))
		p+= sizeof (uint8_t);

	if (field == TRACE_RADIOTAP_LOCK_QUALITY)
	{
		ALIGN_NATURAL_16(p,s);
		return (void *)p;
	}
	if (bswap_le_to_host32(rtap->it_present) & (1 << TRACE_RADIOTAP_LOCK_QUALITY))
		p+= sizeof (uint16_t);

	if (field == TRACE_RADIOTAP_TX_ATTENUATION)
	{
		ALIGN_NATURAL_16(p,s);
		return (void *)p;
	}
	if (bswap_le_to_host32(rtap->it_present) & (1 << TRACE_RADIOTAP_TX_ATTENUATION))
		p+= sizeof (uint16_t);

	if (field == TRACE_RADIOTAP_DB_TX_ATTENUATION)
	{
		ALIGN_NATURAL_16(p,s);
		return (void *)p;
	}
	if (bswap_le_to_host32(rtap->it_present) & (1 << TRACE_RADIOTAP_DB_TX_ATTENUATION))
		p+= sizeof (uint16_t);

	if (field == TRACE_RADIOTAP_DBM_TX_POWER)
		return (void *)p;
	if (bswap_le_to_host32(rtap->it_present) & (1 << TRACE_RADIOTAP_DBM_TX_POWER))
		p+= sizeof (uint8_t);

	if (field == TRACE_RADIOTAP_ANTENNA)
		return (void *)p;
	if (bswap_le_to_host32(rtap->it_present) & (1 << TRACE_RADIOTAP_ANTENNA))
		p+= sizeof (uint8_t);

	if (field == TRACE_RADIOTAP_DB_ANTSIGNAL)
		return (void *)p;
	if (bswap_le_to_host32(rtap->it_present) & (1 << TRACE_RADIOTAP_DB_ANTSIGNAL))
		p+= sizeof (uint8_t);

	if (field == TRACE_RADIOTAP_DB_ANTNOISE)
		return (void *) p;
	/*
	if (bswap_le_to_host32(rtap->it_present) & (1 << TRACE_RADIOTAP_DB_ANTNOISE))
		p+= sizeof (uint8_t);
	*/

	/* Unknown field */
	return NULL;
} 