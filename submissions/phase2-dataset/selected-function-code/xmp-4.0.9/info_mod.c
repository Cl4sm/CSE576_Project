void info_mod(struct xmp_module_info *mi)
{
	int i;
	int num_seq;
	int total_time;

	report("Module name  : %s\n", mi->mod->name);
	report("Module type  : %s\n", mi->mod->type);
	report("Module length: %d patterns\n", mi->mod->len);
	report("Patterns     : %d\n", mi->mod->pat);
	report("Instruments  : %d\n", mi->mod->ins);
	report("Samples      : %d\n", mi->mod->smp);
	report("Channels     : %d [ ", mi->mod->chn);

	for (i = 0; i < mi->mod->chn; i++) {
		if (mi->mod->xxc[i].flg & XMP_CHANNEL_SYNTH) {
			report("S ");
		} else if (mi->mod->xxc[i].flg & XMP_CHANNEL_MUTE) {
			report("- ");
		} else {
			report("%x ", mi->mod->xxc[i].pan >> 4);
		}
	}
	report("]\n");

	total_time = mi->seq_data[0].duration;

	report("Duration     : %dmin%02ds", (total_time + 500) / 60000,
					((total_time + 500) / 1000) % 60);

	/* Check non-zero-length sequences */
	num_seq = 0;
	for (i = 0; i <  mi->num_sequences; i++) {
		if (mi->seq_data[i].duration > 0)
			num_seq++;
	}

	if (num_seq > 1) {
		report(" (main sequence)\n");
		for (i = 1; i < mi->num_sequences; i++) {
			int dur = mi->seq_data[i].duration;

			if (dur == 0) {
				continue;
			}

			report("               %dmin%02ds "
				"(sequence %d at position %02X)\n",
				(dur + 500) / 60000, ((dur + 500) / 1000) % 60,
				i, mi->seq_data[i].entry_point);
		}
	} else {
		report("\n");
	}
}