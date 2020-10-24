int write_mailbody_from_map(int sockfd, char *mapstart, size_t size,
			    const char *tohdr)
{
	char *cur, *next;
	char newlinebuf[3];
	size_t len;
	int i = 1;

	for(next = cur = mapstart; next < mapstart + size; next++) {
		if(*next == '\n') {
			if(writen(sockfd, cur, next - cur) < 0) {
				log_error(LOG_ARGS, "Could not write mail");
				return -1;
			}
			newlinebuf[0] = '\r';
			newlinebuf[1] = '\n';
			len = 2;
			if(next < (mapstart + size - 1) && *(next+1) == '.') {
				newlinebuf[2] = '.';
				len = 3;
			}
			if(writen(sockfd, newlinebuf, len) < 0) {
				log_error(LOG_ARGS, "Could not write mail");
				return -1;
			}
			if(i && tohdr && *(next+1) == '\n') { /* add To: header */
				if(writen(sockfd, tohdr, strlen(tohdr)) < 0) {
					log_error(LOG_ARGS, "Could not write"
							    " To: header");
					return -1;
				}
				i = 0; /* Make sure we don't write it again */
			}
			cur = next + 1;
		}
	}

	return 0;
}