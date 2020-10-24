int main(int argc, char **argv)
{
    enum { BINARY, DEFAULT, HEX } outputmode = DEFAULT;
    char *inhex = NULL;
    unsigned char *data;
    int datalen;
    int decode = -1;
    int doing_opts = TRUE;

    while (--argc > 0) {
	char *p = *++argv;

	if (doing_opts && *p == '-') {
	    if (!strcmp(p, "--")) {
		doing_opts = 0;
		continue;
	    }
	    p++;
	    while (*p) {
		switch (*p) {
		  case 'e':
		    decode = 0;
		    break;
		  case 'd':
		    decode = 1;
		    break;
		  case 'b':
		    outputmode = BINARY;
		    break;
		  case 'h':
		    outputmode = HEX;
		    break;
		  default:
		    fprintf(stderr, "obfusc: unrecognised option '-%c'\n",
			    *p);
		    return 1;
		}
		p++;
	    }
	} else {
	    if (!inhex) {
		inhex = p;
	    } else {
		fprintf(stderr, "obfusc: expected at most one argument\n");
		return 1;
	    }
	}
    }

    if (decode < 0) {
	fprintf(stderr, "usage: obfusc < -e | -d > [ -b | -h ] [hex data]\n");
	return 0;
    }

    if (outputmode == DEFAULT)
	outputmode = (decode ? BINARY : HEX);

    if (inhex) {
	datalen = strlen(inhex) / 2;
	data = hex2bin(inhex, datalen);
    } else {
	int datasize = 4096;
	datalen = 0;
	data = snewn(datasize, unsigned char);
	while (1) {
	    int ret = fread(data + datalen, 1, datasize - datalen, stdin);
	    if (ret < 0) {
		fprintf(stderr, "obfusc: read: %s\n", strerror(errno));
		return 1;
	    } else if (ret == 0) {
		break;
	    } else {
		datalen += ret;
		if (datasize - datalen < 4096) {
		    datasize = datalen * 5 / 4 + 4096;
		    data = sresize(data, datasize, unsigned char);
		}
	    }
	}
    }

    obfuscate_bitmap(data, datalen * 8, decode);

    if (outputmode == BINARY) {
	int ret = fwrite(data, 1, datalen, stdout);
        if (ret < 0) {
            fprintf(stderr, "obfusc: write: %s\n", strerror(errno));
            return 1;
        }
    } else {
	int i;
	for (i = 0; i < datalen; i++)
	    printf("%02x", data[i]);
	printf("\n");
    }

    return 0;
}