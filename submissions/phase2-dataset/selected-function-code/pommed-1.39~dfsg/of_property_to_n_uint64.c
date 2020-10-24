int of_property_to_n_uint64(uint64_t *val, void *prop, uint32_t len, int n)
{
	*val = 0;
	int i=0;
	uint8_t *data = prop;
	
	if((n*8 > len) || (n == 0))
		return 0;
	
	for(i=0; i < (n*8)-1; i++);
	
	*val = 		    ((uint64_t) data[i - 7] << 56) +
			    ((uint64_t) data[i - 6] << 48) +
			    ((uint64_t) data[i - 5] << 40)
			    + ((uint64_t) data[i - 4] << 32) +
			    ((uint64_t) data[i - 3] << 24) +
			    ((uint64_t) data[i - 2] << 16)
			    + ((uint64_t) data[i - 1] << 8) + (uint64_t) data[i];
	
	return 1;
}