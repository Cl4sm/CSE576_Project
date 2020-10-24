void tcpseg_report(void)
{
	int i,j;
	FILE *out = fopen("tcpseg.rpt", "w");
	if (!out) {
		perror("fopen");
		return;
	}
	fprintf(out, "%-16s\t%10s\t%16s %16s\n",
			"SEGMENT SIZE",
			"DIRECTION",
			"BYTES",
			"PACKETS");
	
	for(i=0;i<2048;++i) {
		bool indent_needed;
		if (tcpseg_stat[0][i].count==0 && 
			tcpseg_stat[1][i].count==0 && tcpseg_stat[2][i].count==0)
			continue;
		fprintf(out, "%16i:",i);
		indent_needed=false;
		for(j=0;j<3;j++){
			if (indent_needed) {
				fprintf(out, "%16s", " ");
			}
			if (suppress[j])
				continue;
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
				tcpseg_stat[j][i].bytes,
				tcpseg_stat[j][i].count);	
			indent_needed=true;
		}
	}
	fclose(out);
}