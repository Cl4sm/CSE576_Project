static void load_error(char *name, char *filename, int val)
{
	char *msg;

	val = -val;
	switch (val) {
	case XMP_ERROR_FORMAT:
		msg = "Unrecognized file format";
		break;
	case XMP_ERROR_DEPACK:
		msg = "Error depacking file";
		break;
	case XMP_ERROR_LOAD:
		msg = "Error loading module";
		break;
	case XMP_ERROR_SYSTEM:
		msg = strerror(errno);
		break;
	default:
		msg = "Unknown error";
	}

	fprintf(stderr, "%s: %s: %s\n", name, filename, msg);
}