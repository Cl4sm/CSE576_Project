static void calc_sequence(struct params *fd, int tailsect)
{
	int sec_id, cur_sector, i;

	fd->sequence = SafeNewArray(fd->dsect,struct fparm2);
	cur_sector = fd->dsect-1;

	/* construct the sequence while working backwards.  cur_sector
	 * points to the place where the next sector will be placed.
	 * We place it, then move circularily backwards placing more
	 * and more sectors */
	sec_id = tailsect;
	fd->rotations = 0;
	for(i=0; i < fd->dsect; 
	    i++, cur_sector -= fd->actual_interleave, sec_id--) {
		if (sec_id == 0)
			sec_id = fd->dsect;

		if ( cur_sector < 0) {
			cur_sector += fd->dsect;
			if(sec_id != fd->dsect)
				fd->rotations++;
		}
			
		/* slot occupied, look elsewhere */
		while(fd->sequence[cur_sector].sect ){
			cur_sector--;
			if ( cur_sector < 0 ) {
				cur_sector += fd->dsect;
				if(sec_id != fd->dsect)
					fd->rotations++;
			}
		}

		/* place the sector */
		fd->sequence[cur_sector].sect = sec_id;
		fd->sequence[cur_sector].size = sizeOfSector(fd, sec_id);
	}

	/* handle wrap-around between tailsect and tailsect+1 */
	if(tailsect != fd->dsect) {
		/* always add one rotation, because tailsect+1 cannot be
		 * at the last position, thus is necessarily earlyer */
		fd->rotations++;
		
		if(fd->actual_interleave == 2 && 
		   cur_sector + fd->actual_interleave == 1)
			/* if we use interleave, and the last sector was
			 * placed at the first last position, add one
			 * extra rotation for tailsect+1 following tailsect
			 * too closely */
			fd->rotations++;
	}
}
