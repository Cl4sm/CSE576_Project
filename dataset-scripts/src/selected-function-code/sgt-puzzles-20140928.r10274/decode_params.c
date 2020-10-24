static void decode_params(game_params *params, char const *string)
{
    char const *p = string;
    game_params *defs = default_params();

    *params = *defs; free_params(defs);

    while (*p) {
	switch (*p++) {
	case 'c':
	    params->ncolours = atoi(p);
	    while (*p && isdigit((unsigned char)*p)) p++;
	    break;

	case 'p':
	    params->npegs = atoi(p);
	    while (*p && isdigit((unsigned char)*p)) p++;
	    break;

	case 'g':
	    params->nguesses = atoi(p);
	    while (*p && isdigit((unsigned char)*p)) p++;
	    break;

        case 'b':
            params->allow_blank = 1;
            break;

        case 'B':
            params->allow_blank = 0;
            break;

        case 'm':
            params->allow_multiple = 1;
            break;

        case 'M':
            params->allow_multiple = 0;
            break;

	default:
            ;
	}
    }
}