static void print_usage(void) {
	sendip_module *mod;
	int i;
	printf("Usage: %s [-v] [-d data] [-h] [-f datafile] [-p module] [module options] hostname\n",progname);
	printf(" -d data\tadd this data as a string to the end of the packet\n");
	printf("\t\tData can be:\n");
	printf("\t\trN to generate N random(ish) data bytes;\n");
	printf("\t\t0x or 0X followed by hex digits;\n");
	printf("\t\t0 followed by octal digits;\n");
	printf("\t\tany other stream of bytes\n");
	printf(" -f datafile\tread packet data from file\n");
	printf(" -h\t\tprint this message\n");
	printf(" -p module\tload the specified module (see below)\n");
	printf(" -v\t\tbe verbose\n");

	printf("\n\nModules are loaded in the order the -p option appears.  The headers from\n");
	printf("each module are put immediately inside the headers from the previos model in\n");
	printf("the final packet.  For example, to embed bgp inside tcp inside ipv4, do\n");
	printf("sendip -p ipv4 -p tcp -p bgp ....\n");

	printf("\n\nModules available at compile time:\n");
	printf("\tipv4 ipv6 icmp tcp udp bgp rip ntp\n\n");
	for(mod=first;mod!=NULL;mod=mod->next) {
		printf("\n\nArguments for module %s:\n",mod->name);
		for(i=0;i<mod->num_opts;i++) {
			printf("   -%c%s %c\t%s\n",mod->optchar,
					  mod->opts[i].optname,mod->opts[i].arg?'x':' ',
					  mod->opts[i].description);
			if(mod->opts[i].def) printf("   \t\t  Default: %s\n", 
												 mod->opts[i].def);
		}
	}

}
