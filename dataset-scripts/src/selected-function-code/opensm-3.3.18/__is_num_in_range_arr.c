static boolean_t
__is_num_in_range_arr(uint64_t ** range_arr,
		  unsigned range_arr_len, uint64_t num)
{
	unsigned ind_1 = 0;
	unsigned ind_2 = range_arr_len - 1;
	unsigned ind_mid;

	if (!range_arr || !range_arr_len)
		return FALSE;

	while (ind_1 <= ind_2) {
	    if (num < range_arr[ind_1][0] || num > range_arr[ind_2][1])
		return FALSE;
	    else if (num <= range_arr[ind_1][1] || num >= range_arr[ind_2][0])
		return TRUE;

	    ind_mid = ind_1 + (ind_2 - ind_1 + 1)/2;

	    if (num < range_arr[ind_mid][0])
		ind_2 = ind_mid;
	    else if (num > range_arr[ind_mid][1])
		ind_1 = ind_mid;
	    else
		return TRUE;

	    ind_1++;
	    ind_2--;
	}

	return FALSE;
}