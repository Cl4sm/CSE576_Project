static void show_info(int what, struct xmp_module_info *mi)
{
	report("\r%78.78s\n", " ");
	switch (what) {
	case '?':
		info_help();
		break;
	case 'i':
		info_ins_smp(mi);
		break;
	case 'I':
		info_instruments(mi);
		break;
	case 'S':
		info_samples(mi);
		break;
	case 'c':
		info_comment(mi);
		break;
	case 'm':
		info_mod(mi);
		break;
	}
}