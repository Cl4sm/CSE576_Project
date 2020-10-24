void tcpopt_report(void)
{
	
	int i,j;
	
	FILE *out = fopen("tcpopt.rpt", "w");
	if (!out) {
		perror("fopen");
		return;
	}

	/* Put some headings up for human-readability */
	fprintf(out, "%-12s\t%10s\t%16s %16s\n",
			"OPTION",
			"DIRECTION",
			"BYTES",
			"PACKETS");
	
	for(i=0;i<256;++i) {
		if (tcpopt_stat[0][i].count==0 && 
			tcpopt_stat[1][i].count==0 && tcpopt_stat[2][i].count==0)
			continue;
		
		switch(i) {
			case 1:
				fprintf(out, "%12s", "NOP |");
				break;
			case 2:
				fprintf(out, "%12s", "MSS |");
				break;
			case 3:
				fprintf(out, "%12s", "Winscale |");
				break;
			case 4:
				fprintf(out, "%12s", "SACK Perm |");
				break;
			case 5:
				fprintf(out, "%12s", "SACK Info |");
				break;
			case 8:
				fprintf(out, "%12s", "Timestamp |");
				break;
			case 12:
				fprintf(out, "%12s", "CC.New |");
				break;
			case 19:
				fprintf(out, "%12s", "MD5 |");
				break;
			default:
				fprintf(out, "%10i |",i);
		}
		
		for(j=0;j<3;j++){
			if (j != 0) {
				fprintf(out, "%12s", " |");
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
				tcpopt_stat[j][i].bytes,
				tcpopt_stat[j][i].count);
		}
	}
	fclose(out);
}