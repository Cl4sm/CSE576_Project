void info_instruments(struct xmp_module_info *mi)
{
	int i, j;
	struct xmp_module *mod = mi->mod;

	report("Instruments:\n");
	report("   Instrument name                  Vl Fade Env Ns Sub  Gv Vl Fine Xpo Pan Sm\n");
	for (i = 0; i < mod->ins; i++) {
		struct xmp_instrument *ins = &mod->xxi[i];

		if (strlen(ins->name) == 0 && ins->nsm == 0) {
			continue;
		}

		report("%02x %-32.32s %02x %04x %c%c%c %02x ",
			i + 1, ins->name, ins->vol, ins->rls,
			ins->aei.flg & XMP_ENVELOPE_ON ? 'A' : '-',
			ins->fei.flg & XMP_ENVELOPE_ON ? 'F' : '-',
			ins->pei.flg & XMP_ENVELOPE_ON ? 'P' : '-',
			ins->nsm
		);

		for (j = 0; j < ins->nsm; j++) {
			struct xmp_subinstrument *sub = &ins->sub[j];
			struct xmp_sample *smp = &mod->xxs[sub->sid];

			if (j > 0) {
				if (smp->len == 0) {
					continue;
				}
				report("%51.51s", " ");
			}

			report("[%02x] %02x %02x %+04d %+03d P%02x %02x\n",
				j + 1,
				sub->gvl,
				sub->vol,
				sub->fin,
				sub->xpo,
				sub->pan,
				sub->sid);
		}

		if (j == 0) {
			report("[  ] -- -- ---- --- --- --\n");
		}

	}
}