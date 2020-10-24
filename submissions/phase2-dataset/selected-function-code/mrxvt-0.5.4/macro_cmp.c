int
macro_cmp( const void *p1, const void *p2)
{
    const macros_t  *macro1 = p1,
	     	    *macro2 = p2;

    /* First compare keysyms, then modflags. Ignore the "Primary" modifier */
    if( macro1->keysym == macro2->keysym )
    {
	if(
	     (macro1->modFlags & ~MACRO_PRIMARY & MACRO_MODMASK)
		== (macro2->modFlags & ~MACRO_PRIMARY & MACRO_MODMASK)
	  )
	    return MACRO_GET_NUMBER( macro1->modFlags )
			- MACRO_GET_NUMBER( macro2->modFlags );
	else
	    return (macro1->modFlags & ~MACRO_PRIMARY & MACRO_MODMASK)
			- (macro2->modFlags & ~MACRO_PRIMARY & MACRO_MODMASK);
    }
    else
	return macro1->keysym - macro2->keysym;
}