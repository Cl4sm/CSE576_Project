 ***************************************************/

static void __rangelist2rangearr(
    cl_list_t    * p_list,
    uint64_t  ** * p_arr,
    unsigned     * p_arr_len)
{
    cl_list_iterator_t list_iterator;
    unsigned len = cl_list_count(p_list);
    unsigned i = 0;
    uint64_t ** tmp_arr;
    uint64_t ** res_arr = NULL;
    unsigned res_arr_len = 0;

    tmp_arr = (uint64_t **)malloc(len * sizeof(uint64_t *));

    list_iterator = cl_list_head(p_list);
    while( list_iterator != cl_list_end(p_list) )
    {
       tmp_arr[i++] = (uint64_t *)cl_list_obj(list_iterator);
       list_iterator = cl_list_next(list_iterator);
    }
    cl_list_remove_all(p_list);

    __sort_reduce_rangearr( tmp_arr,
                            len,
                            &res_arr,
                            &res_arr_len );
    *p_arr = res_arr;