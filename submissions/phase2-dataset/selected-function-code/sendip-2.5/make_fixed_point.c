u_int32_t make_fixed_point(double n, bool issigned, int totbits, int intbits) {
	u_int32_t intpart;
	u_int32_t fracpart;
	u_int32_t result;
	bool signbit;
	double intpartd, fracpartd;
	int fracbits;

	if(issigned) totbits--;

	fracbits=totbits-intbits;

	if(issigned && n<0) {
		signbit=TRUE;
	} else {
		signbit=FALSE;
	} /* else, signbit is unused */

	n=fabs(n);

/*
	fracpartd=floor(ldexp(modf(n,&intpartd),ldexp(2.0,fracbits)));
*/
	fracpartd=floor(ldexp(modf(n,&intpartd),32));
	intpart=(u_int32_t)intpartd;
	fracpart=(u_int32_t)fracpartd;
 
	
	if(issigned&&signbit) {
		result=1<<totbits;
	} else {
		result=0;
	}

	if(intbits!=0) {
		intpart&=(1<<intbits)-1;
		intpart<<=(totbits-intbits);
		result|=intpart;
	}
	if(intbits!=totbits) {
		if(fracbits!=32) {
			fracpart&=((1<<fracbits)-1)<<intbits;
			fracpart>>=intbits;
		}
		result|=fracpart;
	}
	return htonl(result);
}
