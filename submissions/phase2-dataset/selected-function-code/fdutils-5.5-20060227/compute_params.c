			   struct floppy_struct *medprm)
{
	int r, capacity, mysize;
	int header_size, fmt_gap;

	set_field(F_DENSITY, drvprm->type.max_density);
	
	r = dens_tab[DENSITY].dtr;
	if(r == 2 && drvprm->type.rpm == 360)
		r = 1;

	capacity = dens_tab[DENSITY].capacity[drvprm->type.ff];

	set_field(F_DTR, r);

	set_field(F_GAP, gap[DTR]);
	
	set_field(F_FM, dens_tab[DENSITY].fm);
	set_field(F_PERP, dens_tab[DENSITY].perp);

	set_field(F_HEAD, 2);

	/* 5 1/4 drives have two different cylinder densities */
	if(drvprm->type.ff == FF_525) {
		if(DENSITY == DENS_DD)
			/* double density disks are 48 TPI */
			set_field(F_TPI, 48);
		else
			set_field(F_TPI, 96);

		if(mask & (1 << FE_CYL)) {
			/* we know the number of cylinders, try to infer
			 * cylinder density from there */
			if(CYL < 50)
				set_field(F_TPI, 48);
			if(CYL > 70)
				set_field(F_TPI, 96);
		}

		if(drvprm->type.tpi == 96 && TPI == 48)
			set_field(F_STRETCH, 1);
		else
			set_field(F_STRETCH, 0);
		
		/* if, on the other hand, we know TPI, but not the number of
		 * cylinders, infer in the other direction */
		if(TPI == 48)
			set_field(F_CYL, 40);
		else
			set_field(F_CYL, 80);
	} else {
		set_field(F_STRETCH, 0);
		set_field(F_CYL, 80);
	}

	if(PERP)
		header_size = 81;
	else
		header_size = 62;

	switch(capacity) {
		case 25000:
			set_field(F_SECT, 36);
			break;
		case 12500:
			set_field(F_SECT, 18);
			break;
		case 10416:
			set_field(F_SECT, 15);
			break;
		case 6250:
			set_field(F_SECT, 9);
			break;
		default:
			set_field(F_SECT, capacity / (SSIZE + header_size + 1));
	}

	set_field(F_SSIZE, 512);
	set_field(F_VSECT, SECT * SSIZE);
	set_field(F_SIZE, HEAD * CYL * VSECT / 512);	
		
	if(mask & ( 1 << FE_SSIZE) ) {
		mysize = 128;
		for(ssize = 0; ssize < 8 ; ssize++) {
			if(mysize == SSIZE)
				break;
			mysize += mysize;
		}
		if(ssize == 8) {
			fprintf(stderr,"Bad sector size\n");
			exit(1);
		}
		ssize = (ssize + 6) % 8;
	} else
		ssize = 0;
	
	if(VSECT && VSECT >= SSIZE) {
		fmt_gap = (capacity * 199 / 200 / (VSECT / SSIZE)) - 
			SSIZE - header_size;
		if(fmt_gap < 1)
			fmt_gap = 1;
		if(fmt_gap > 0x6c)
			fmt_gap = 0x6c;
		set_field(F_FMT_GAP, fmt_gap);
	}
	set_field(F_2M,0);

	medprm->size = SIZE;
	medprm->sect = VSECT / 512;
	medprm->head = HEAD;
	medprm->track = CYL;
	medprm->stretch = STRETCH | (SWAPSIDES << 1) | (ZEROBASED << 2);
	medprm->gap = GAP;
	medprm->rate = (FM<<7) | (PERP<<6) | (ssize<<3) | (_2M<<2) | DTR;
	medprm->spec1 = SPEC1;
	medprm->fmt_gap = FMT_GAP;
}
