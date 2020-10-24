void port_report(void)
{
	int i;
	FILE *out = fopen("ports.rpt", "w");
	if (!out) {
		perror("fopen");
		return;
	}
	fprintf(out, "%-16s\t%10s\t%16s %16s\n",
                        "PORT",
                        "DIRECTION",
                        "BYTES",
                        "PACKETS");	

	setservent(1);
	setprotoent(1);
	for(i=0;i<256;++i) {
		if (protn[i]) {
			port_protocol(i, out);
			free(ports[0][i]);
			free(ports[1][i]);
			free(ports[2][i]);
		}
	}
	endprotoent();
	endservent();
	fclose(out);
}