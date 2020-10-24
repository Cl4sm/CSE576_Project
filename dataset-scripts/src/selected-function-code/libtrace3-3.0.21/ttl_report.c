void ttl_report(void)
{
	int i,j;
	FILE *out = fopen("ttl.rpt", "w");
	if (!out) {
		perror("fopen");
		return;
	}
	fprintf(out, "%-12s\t%10s\t%16s %16s\n",
			"TTL",
			"DIRECTION",
			"BYTES",
			"PACKETS");
	for(i=0;i<256;++i) {
		if (ttl_stat[0][i].count==0 && 
			ttl_stat[1][i].count==0 && ttl_stat[2][i].count==0)
			continue;
		fprintf(out, "%12i:",i);
		for(j=0;j<3;j++){
			if (j != 0) {
				fprintf(out, "%12s", " ");
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
					ttl_stat[j][i].bytes,
					ttl_stat[j][i].count);
		}
	}
	fclose(out);
}