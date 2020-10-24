		  drivedesc_t *drvprm,
		  struct floppy_struct *medprm)
{
	int found;

	if(argc != 1)
		return 2; /* more than one parameter */
	mediaprmin = fopen(MEDIAPRMFILE, "r");
	if(!mediaprmin)
		return 2; /* no file */

	zero_all(ids, &mask);

	found = 0;
	mediaprmlex(argv[0], ids, sizeof(ids)/sizeof(ids[0]), &mask, &found);
	fclose(mediaprmin);
	if(!found)
		return 1;
	compute_params(drvprm, medprm);
	return 0;
}
