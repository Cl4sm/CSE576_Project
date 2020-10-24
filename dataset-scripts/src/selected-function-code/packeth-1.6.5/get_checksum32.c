guint32 get_checksum32(int cks_start, int cks_stop) 

{
	guint32 value;
	long sum = 0;

	for (; cks_start<cks_stop; ) {
		/* we take 16 bit word's -> 2 bytes */
		value = (packet[cks_start]<<8) + packet[cks_start+1];
		sum = sum + value;
		/* for every cicle, this means where the sum exceeds 
		 * the 16 bit unsigned max value (65536), you have to add 1
		 * to the rest */
		//sum = (sum % 0x10000) + (sum / 0x10000);
		cks_start +=2; 
	}
	/* we don't do extract the sum from 0xFFFF (or -1), so you have to do
	 * this later */
	//return (sum % 0x10000);
	return sum;
}