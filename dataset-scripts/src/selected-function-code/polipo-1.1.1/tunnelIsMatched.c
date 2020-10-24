int
tunnelIsMatched(char *url, int lurl, char *hostname, int lhost)
{
    DomainPtr *domain, *domains;
    
    domains=forbiddenTunnelsDomains;
    if (domains) {
	domain = domains;
	while(*domain) {
	    if (lhost == (*domain)->length && 
		memcmp(hostname, (*domain)->domain, lhost)==0)
		return 1;
	    domain++;
	}
    }

    if(forbiddenTunnelsRegex) {
	if(!regexec(forbiddenTunnelsRegex, url, 0, NULL, 0))
	    return 1;
    }
    return 0;
}