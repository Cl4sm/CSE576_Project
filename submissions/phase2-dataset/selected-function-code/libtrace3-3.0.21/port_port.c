static void port_port(int i,char *prot, int j, FILE *out)
{
	struct servent *ent = getservbyport(htons(j),prot);
	int k;
	
	if(ent){
		fprintf(out,"%16s:",ent->s_name);
	}
	else{
		fprintf(out,"%16i:",j);
	}

	for (k = 0; k < 3; k++) {
		if (!ports[k][i])
			continue;
		if (k != 0) {
			fprintf(out, "%16s", " ");
		}
		switch (k) {
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
			ports[k][i][j].bytes,
			ports[k][i][j].count);
	}
}