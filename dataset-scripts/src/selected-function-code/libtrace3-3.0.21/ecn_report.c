void ecn_report(void)
{
	int i,j;
	int total = 0;
	
	FILE *out = fopen("ecn.rpt", "w");
	if (!out) {
		perror("fopen");
		return;
	}

	/* Put some headings up for human-readability */
	fprintf(out, "%-12s\t%10s\t%16s %16s\n",
			"ECN",
			"DIRECTION",
			"BYTES",
			"PACKETS");
	
	for(i=0;i<4;++i) {
		if (ecn_stat[0][i].count==0 && 
			ecn_stat[1][i].count==0 && ecn_stat[2][i].count==0)
			continue;
		switch(i){
			case 1:
				fprintf(out, "%12s", "ECN CAPE |");
				break;
			case 2:
				fprintf(out, "%12s", "ECN CAPE |");
				break;
			case 3:
				fprintf(out, "%12s", "CONG EXP |");
				break;
			default:
				fprintf(out, "%12s", "NO ECN |");	
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
				ecn_stat[j][i].bytes,
				ecn_stat[j][i].count);
		}
	}
	
	for(i=0;i<3;i++){
		for(j=1;j<4;j++)
			total += ecn_stat[i][j].count;
	}
	fprintf(out, "%s: %i\n", "Total ECN", total);
	fclose(out);
}