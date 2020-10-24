void protocol_report(void)
{
	int i,j;
	FILE *out = fopen("protocol.rpt", "w");
	if (!out) {
		perror("fopen");
		return;
	}
	fprintf(out, "%-16s\t%10s\t%16s %16s\n",
			"PROTOCOL",
			"DIRECTION",
			"BYTES",
			"PACKETS");
	
	setprotoent(1);
	for(i=0;i<256;++i) {
		struct protoent *prot;
		if (prot_stat[0][i].count==0 && 
			prot_stat[1][i].count==0 && prot_stat[2][i].count==0)
			continue;
		prot = getprotobynumber(i);
		if (prot) {
			fprintf(out, "%16s",prot->p_name);
		}
		else {
			fprintf(out, "%16i:",i);
		}
		for (j=0; j < 3; j++) {
			if (j != 0) {
				fprintf(out, "%16s", " ");
			}
			switch (j) {
				case 0:
                                        fprintf(out, "\t%10s", "Outbound");
                                        break;
                                case 1:
                                        fprintf(out, "\t%10s", "Inbound");
                                        break;
                                case 2:
                                        fprintf(out, "\t%10s", "Unknown");
                                        break;
                        }

                        fprintf(out, "\t%16" PRIu64 " %16" PRIu64 "\n",
                                        prot_stat[j][i].bytes,
                                        prot_stat[j][i].count);
                }
	}

	setprotoent(0);
	fclose(out);
}