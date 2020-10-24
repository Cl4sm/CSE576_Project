 ***************************************************/

static void __sort_reduce_rangearr(
    uint64_t  **   arr,
    unsigned       arr_len,
    uint64_t  ** * p_res_arr,
    unsigned     * p_res_arr_len )
{
    unsigned i = 0;
    unsigned j = 0;
    unsigned last_valid_ind = 0;
    unsigned valid_cnt = 0;
    uint64_t ** res_arr;
    boolean_t * is_valid_arr;

    *p_res_arr = NULL;
    *p_res_arr_len = 0;

    qsort(arr, arr_len, sizeof(uint64_t*), __cmp_num_range);

    is_valid_arr = (boolean_t *)malloc(arr_len * sizeof(boolean_t));
    is_valid_arr[last_valid_ind] = TRUE;
    valid_cnt++;
    for (i = 1; i < arr_len; i++)
    {
        if (arr[i][0] <= arr[last_valid_ind][1])
        {
            if (arr[i][1] > arr[last_valid_ind][1])
                arr[last_valid_ind][1] = arr[i][1];
            free(arr[i]);
            arr[i] = NULL;
            is_valid_arr[i] = FALSE;
        }
        else if ((arr[i][0] - 1) == arr[last_valid_ind][1])
        {
            arr[last_valid_ind][1] = arr[i][1];
            free(arr[i]);
            arr[i] = NULL;
            is_valid_arr[i] = FALSE;
        }
        else
        {
            is_valid_arr[i] = TRUE;
            last_valid_ind = i;
            valid_cnt++;
        }
    }

    res_arr = (uint64_t **)malloc(valid_cnt * sizeof(uint64_t *));
    for (i = 0; i < arr_len; i++)
    {
        if (is_valid_arr[i])
            res_arr[j++] = arr[i];
    }
    free(is_valid_arr);
    free(arr);

    *p_res_arr = res_arr;