static void radeon_print_pp_lib_non_clock_class(uint16_t class)
{
    int class_type = (class & ATOM_PPLIB_CLASSIFICATION_UI_MASK) >> ATOM_PPLIB_CLASSIFICATION_UI_SHIFT;
    switch (class_type) {
    case ATOM_PPLIB_CLASSIFICATION_UI_NONE:
	printf("\t\tNONE\n");
	break;
    case ATOM_PPLIB_CLASSIFICATION_UI_BATTERY:
	printf("\t\tBATTERY\n");
	break;
    case ATOM_PPLIB_CLASSIFICATION_UI_BALANCED:
	printf("\t\tBALANCED\n");
	break;
    case ATOM_PPLIB_CLASSIFICATION_UI_PERFORMANCE:
	printf("\t\tPERFORMANCE\n");
	break;
    default:
	break;
    }
    if (class & ATOM_PPLIB_CLASSIFICATION_BOOT)
	printf("\t\tBOOT\n");
    if (class & ATOM_PPLIB_CLASSIFICATION_THERMAL)
	printf("\t\tTHERMAL\n");
    if (class & ATOM_PPLIB_CLASSIFICATION_LIMITEDPOWERSOURCE)
	printf("\t\tLIMITEDPOWERSOURCE\n");
    if (class & ATOM_PPLIB_CLASSIFICATION_REST)
	printf("\t\tREST\n");
    if (class & ATOM_PPLIB_CLASSIFICATION_FORCED)
	printf("\t\tFORCED\n");
    if (class & ATOM_PPLIB_CLASSIFICATION_3DPERFORMANCE)
	printf("\t\t3DPERFORMANCE\n");
    if (class & ATOM_PPLIB_CLASSIFICATION_OVERDRIVETEMPLATE)
	printf("\t\tOVERDRIVETEMPLATE\n");
    if (class & ATOM_PPLIB_CLASSIFICATION_UVDSTATE)
	printf("\t\tUVDSTATE\n");
    if (class & ATOM_PPLIB_CLASSIFICATION_3DLOW)
	printf("\t\t3DLOW\n");
    if (class & ATOM_PPLIB_CLASSIFICATION_ACPI)
	printf("\t\tACPI\n");
}