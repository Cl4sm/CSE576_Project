void
usage(FILE *stream)
{
	fprintf(stream,
		"Usage: hostname [-b] {hostname|-F file}         set host name (from file)\n"
		"       hostname [-a|-A|-d|-f|-i|-I|-s|-y]       display formatted name\n"
		"       hostname                                 display host name\n"
		"\n"
		"       {yp,nis,}domainname {nisdomain|-F file}  set NIS domain name (from file)\n"
		"       {yp,nis,}domainname                      display NIS domain name\n"
		"\n"
		"       dnsdomainname                            display dns domain name\n"
		"\n"
		"       hostname -V|--version|-h|--help          print info and exit\n"
		"\n"
		"Program name:\n"
		"       {yp,nis,}domainname=hostname -y\n"
		"       dnsdomainname=hostname -d\n"
		"\n"
		"Program options:\n"
		"    -a, --alias            alias names\n"
		"    -A, --all-fqdns        all long host names (FQDNs)\n"
		"    -b, --boot             set default hostname if none available\n"
		"    -d, --domain           DNS domain name\n"
		"    -f, --fqdn, --long     long host name (FQDN)\n"
		"    -F, --file             read host name or NIS domain name from given file\n"
		"    -i, --ip-address       addresses for the host name\n"
		"    -I, --all-ip-addresses all addresses for the host\n"
		"    -s, --short            short host name\n"
		"    -y, --yp, --nis        NIS/YP domain name\n"
		"\n"
		"Description:\n"
		"   This command can get or set the host name or the NIS domain name. You can\n"
		"   also get the DNS domain or the FQDN (fully qualified domain name).\n"
		"   Unless you are using bind or NIS for host lookups you can change the\n"
		"   FQDN (Fully Qualified Domain Name) and the DNS domain name (which is\n"
		"   part of the FQDN) in the /etc/hosts file.\n");
	exit(-1);
}