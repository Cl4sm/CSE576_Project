{
	int sector_skew, byte_skew;

	if(cur_pos < f->min) {
		/* the current skew is smaller than the extend that this
		 * sequence can handle. Wait until its beginning */
		*lskew = 0;
		return f->min + f->length;
	}

	sector_skew = (cur_pos - f->min + f->chunksize - 1) / f->chunksize;
	byte_skew = f->min + sector_skew * f->chunksize;
	
	if(byte_skew >= f->max) {
		/* the current skew is larger than the extend that this
		 * sequence can handle. Wait until its next beginning */
		*lskew = 0;		
		return f->min + f->length + f->raw_capacity;
	}

	*lskew = sector_skew;
	return byte_skew + f->length;
}
