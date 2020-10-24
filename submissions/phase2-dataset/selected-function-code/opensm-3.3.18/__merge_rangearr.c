 ***************************************************/

static void __merge_rangearr(
    uint64_t  **   range_arr_1,
    unsigned       range_len_1,
    uint64_t  **   range_arr_2,
    unsigned       range_len_2,
    uint64_t  ** * p_arr,
    unsigned     * p_arr_len )
{
    unsigned i = 0;
    unsigned j = 0;
    unsigned len = range_len_1 + range_len_2;
    uint64_t ** tmp_arr;
    uint64_t ** res_arr = NULL;
    unsigned res_arr_len = 0;

    *p_arr = NULL;
    *p_arr_len = 0;

    tmp_arr = (uint64_t **)malloc(len * sizeof(uint64_t *));

    for (i = 0; i < range_len_1; i++)
       tmp_arr[j++] = range_arr_1[i];
    for (i = 0; i < range_len_2; i++)
       tmp_arr[j++] = range_arr_2[i];
    free(range_arr_1);
    free(range_arr_2);

    __sort_reduce_rangearr( tmp_arr,
                            len,
                            &res_arr,
                            &res_arr_len );
    *p_arr = res_arr;