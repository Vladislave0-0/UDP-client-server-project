#include "ListFunctions.h"

//=====================================================================================================================================

int list_ctor(struct list* lst)
{
    if(lst == nullptr)
    {
        RETURN_LIST_ERROR(ERROR_LIST_STRUCT_NULLPTR);

        return ERROR_LIST_STRUCT_NULLPTR;
    }


    lst->capacity = INITIAL_NODES_NUM;

    if(lst->capacity <= 0)
    {
        lst->error_code = ERROR_BAD_CAPACITY;

        RETURN_LIST_ERROR(lst->error_code);
    }


    lst->nodes_arr = (node*)calloc(lst->capacity, sizeof(node));

    if(lst->nodes_arr == nullptr)
    {
        lst->error_code = ERROR_NODES_CALLOC_NULLPTR;

        RETURN_LIST_ERROR(lst->error_code);
    }


    lst->html_file = fopen("graph_log.html", "w");

    if(lst->html_file == nullptr)
    {
        lst->error_code = ERROR_HTML_FILE_NULLPTR;

        RETURN_LIST_ERROR(lst->error_code);
    }


    fill_with_POISON(lst, 0, lst->capacity);
    lst->head_node = -1;
    lst->tail_node = -1;

    lst->is_linearized = 1;


    MAKE_GRAPHVIZ("list_ctor", -1, -1);

    return _SUCCESS;
}

//=====================================================================================================================================

int fill_with_POISON(struct list* lst, int start, int finish)
{
    CHECK_ERROR;

    for(int i = start; i < finish - 1; i++)
    {
        lst->nodes_arr[i].value = POISON;
        lst->nodes_arr[i].next  = i + 1;
        lst->nodes_arr[i].prev  = -1;
    }

    lst->nodes_arr[finish - 1].value = POISON;
    lst->nodes_arr[finish - 1].next = -1;
    lst->nodes_arr[finish - 1].prev = -1;

    return _SUCCESS;
}

//=====================================================================================================================================

int push_after(struct list* lst, int node_index, Message* message)
{
    CHECK_ERROR;
    int value = 0;

    if(node_index > lst->capacity - 1)
    {
        PRINT_INFO("You are off the list!");

        return _SUCCESS;
    }

    if(lst->head_node == -1)
    {
        lst->head_node = 0;
        lst->tail_node = 0;
        lst->free_node = lst->nodes_arr[lst->free_node].next;
        lst->nodes_arr[lst->head_node].next = -1;

        strcpy(lst->nodes_arr[lst->head_node].login, message->sender.login);
        strcpy(lst->nodes_arr[lst->head_node].ip_str, message->sender.ip_str);
        lst->nodes_arr[lst->head_node].port = message->sender.port;
        lst->nodes_arr[lst->head_node].socketfd = message->sender.socketfd;
        lst->nodes_arr[lst->head_node].cliaddr.sin_addr = message->sender.cliaddr.sin_addr;
        lst->nodes_arr[lst->head_node].cliaddr.sin_family = message->sender.cliaddr.sin_family;
        lst->nodes_arr[lst->head_node].cliaddr.sin_port = message->sender.cliaddr.sin_port;
        lst->size++;
    }

    else
    {
        if((node_index != lst->head_node) && (lst->nodes_arr[node_index].prev == -1))
        {
            PRINT_INFO("Referencing a non-existent cell of the list!");

            return _SUCCESS;
        }

        if(lst->size >= lst->capacity - 1)
        {
            int new_node_index = 0;

            if(translate_physical_adress_to_logical_position(lst, node_index, &new_node_index) != _SUCCESS)
            {
                RETURN_LIST_ERROR(lst->error_code);
            }

            if(list_resize(lst, (int)(lst->capacity * RESIZE_FACTOR)) != _SUCCESS)
            {
                RETURN_LIST_ERROR(lst->error_code);
            }

            node_index = new_node_index;
        }

        int cur_node = lst->free_node;
        lst->free_node = lst->nodes_arr[lst->free_node].next;
        lst->size++;
        
        strcpy(lst->nodes_arr[cur_node].login, message->sender.login);
        strcpy(lst->nodes_arr[cur_node].ip_str, message->sender.ip_str);
        lst->nodes_arr[cur_node].port = message->sender.port;
        lst->nodes_arr[cur_node].socketfd = message->sender.socketfd;
        lst->nodes_arr[cur_node].cliaddr.sin_addr = message->sender.cliaddr.sin_addr;
        lst->nodes_arr[cur_node].cliaddr.sin_family = message->sender.cliaddr.sin_family;
        lst->nodes_arr[cur_node].cliaddr.sin_port = message->sender.cliaddr.sin_port;

        if(node_index == lst->tail_node)
        {
            lst->nodes_arr[cur_node].prev = lst->tail_node;
            lst->nodes_arr[cur_node].next = -1;
            lst->nodes_arr[lst->tail_node].next = cur_node;
            lst->tail_node = cur_node;
        }

        else
        {
            lst->nodes_arr[cur_node].prev = node_index;
            lst->nodes_arr[cur_node].next = lst->nodes_arr[node_index].next;
            lst->nodes_arr[lst->nodes_arr[cur_node].next].prev = cur_node;
            lst->nodes_arr[lst->nodes_arr[cur_node].prev].next = cur_node;
        }
    }

    linearized_check(lst);


    MAKE_GRAPHVIZ("push_after", node_index, value);

    return _SUCCESS;
}

//=====================================================================================================================================

int push_before(struct list* lst, int node_index, elem_t value)
{
    CHECK_ERROR;


    if(node_index > lst->capacity - 1)
    {
        PRINT_INFO("You are off the list!");
    }  


    if(lst->head_node == -1)
    {
        lst->head_node = 0;

        lst->tail_node = 0;

        lst->free_node = lst->nodes_arr[lst->free_node].next;

        lst->nodes_arr[lst->head_node].next = -1;

        lst->nodes_arr[lst->head_node].value = value;

        lst->size++; 
    }

    else
    {
        if((node_index != lst->head_node) && (lst->nodes_arr[node_index].prev == -1))
        {
            PRINT_INFO("Referencing a non-existent cell of the list!");
        }

        if(lst->size >= lst->capacity - 1)
        {
            int new_node_index = 0;

            if(translate_physical_adress_to_logical_position(lst, node_index, &new_node_index) != _SUCCESS)
            {
                RETURN_LIST_ERROR(lst->error_code);
            }

            if(list_resize(lst, (int)(lst->capacity * RESIZE_FACTOR)) != _SUCCESS)
            {
                RETURN_LIST_ERROR(lst->error_code);
            }

            node_index = new_node_index;
        }

        int cur_node = lst->free_node;

        lst->free_node = lst->nodes_arr[lst->free_node].next;

        lst->nodes_arr[cur_node].value = value;

        lst->size++;

        if(node_index == lst->head_node)
        {
            lst->nodes_arr[cur_node].prev = -1;

            lst->nodes_arr[cur_node].next = lst->head_node;

            lst->nodes_arr[lst->head_node].prev = cur_node;

            lst->head_node = cur_node;
        }

        else
        {
            lst->nodes_arr[cur_node].next = node_index;

            lst->nodes_arr[cur_node].prev = lst->nodes_arr[node_index].prev;

            lst->nodes_arr[node_index].prev = cur_node;

            lst->nodes_arr[lst->nodes_arr[cur_node].prev].next = cur_node;
        }
    }

    linearized_check(lst);


    MAKE_GRAPHVIZ("push_before", node_index, value);

    return _SUCCESS;
}

//=====================================================================================================================================

int node_remove(struct list* lst, int node_index, elem_t* arg)
{
    CHECK_ERROR;

    if(node_index > lst->capacity - 1)
    {
        PRINT_INFO("You are off the list!");

        return _SUCCESS;
    }

    if(lst->head_node == -1)
    {
        PRINT_INFO("The list is empty!");

        return _SUCCESS;
    }

    else
    {
        if((lst->nodes_arr[node_index].prev == -1) && node_index != lst->head_node)
        {
            PRINT_INFO("You can't remove node_");

            printf("%d! It is free node!", node_index);

            return _SUCCESS;
        }

        if(lst->size * LIST_REMOVE_RESIZE <= lst->capacity)
        {
            if(node_index == lst->head_node)
            {
                if(list_resize(lst, (int)(lst->capacity / RESIZE_FACTOR)) != _SUCCESS)
                {
                    RETURN_LIST_ERROR(lst->error_code);
                }

                node_index = lst->head_node;
            }

            else if(node_index == lst->tail_node)
            {
                if(list_resize(lst, (int)(lst->capacity / RESIZE_FACTOR)) != _SUCCESS)
                {
                    RETURN_LIST_ERROR(lst->error_code);
                }

                node_index = lst->tail_node;       
            }

            else
            {
                int new_node_index = 0;

                if(translate_physical_adress_to_logical_position(lst, node_index, &new_node_index) != _SUCCESS)
                {
                    RETURN_LIST_ERROR(lst->error_code);
                }

                if(list_resize(lst, (int)(lst->capacity / RESIZE_FACTOR)) != _SUCCESS)
                {
                    RETURN_LIST_ERROR(lst->error_code);
                }

                node_index = new_node_index;
            }
        }

        *arg = lst->nodes_arr[node_index].value;

        lst->nodes_arr[node_index].value = POISON;

        lst->size--;

        if(node_index == lst->head_node)
        {
            lst->nodes_arr[lst->nodes_arr[node_index].next].prev = -1;

            lst->head_node = lst->nodes_arr[node_index].next;

            lst->nodes_arr[node_index].next = lst->free_node;

            lst->free_node = node_index;
        }

        else if(node_index == lst->tail_node)
        {
            lst->nodes_arr[lst->nodes_arr[node_index].prev].next = -1;

            lst->tail_node = lst->nodes_arr[node_index].prev;

            lst->nodes_arr[node_index].prev = -1;

            lst->nodes_arr[node_index].next = lst->free_node;

            lst->free_node = node_index;
        }

        else
        {
            lst->nodes_arr[lst->nodes_arr[node_index].prev].next = lst->nodes_arr[node_index].next;

            lst->nodes_arr[lst->nodes_arr[node_index].next].prev = lst->nodes_arr[node_index].prev;

            lst->nodes_arr[node_index].prev = -1;

            lst->nodes_arr[node_index].next = lst->free_node;

            lst->free_node = node_index;
        }
    }

    linearized_check(lst);


    MAKE_GRAPHVIZ("node_remove", node_index, POISON);

    return _SUCCESS;
}

//=====================================================================================================================================

int head_remove(struct list* lst, elem_t* arg)
{
    CHECK_ERROR; 

    if(node_remove(lst, lst->head_node, arg) != _SUCCESS)
    {
        printf(WHT "Function node_remove(list*, int, elem_t*) has been called by %s at %s(%u)\n" RESET, __PRETTY_FUNCTION__, __FILE__, __LINE__);
    }

    return _SUCCESS;
}

//=====================================================================================================================================

int tail_remove(struct list* lst, elem_t* arg)
{
    CHECK_ERROR;

    if(node_remove(lst, lst->tail_node, arg) != _SUCCESS)
    {
        printf(WHT "Function node_remove(list*, int, elem_t*) has been called by %s at %s(%u)\n" RESET, __PRETTY_FUNCTION__, __FILE__, __LINE__);
    }

    return _SUCCESS;
}

//=====================================================================================================================================

int list_linearize(struct list* lst)
{
    CHECK_ERROR;

    if(lst->is_linearized == 1)
    {
        return _SUCCESS;
    }
    
    node* new_nodes_arr = (node*)calloc(lst->capacity, sizeof(node));

    if(new_nodes_arr == nullptr)
    {
        lst->error_code = ERROR_BAD_CAPACITY;

        return lst->error_code;
    }


    elem_t cur_node = lst->head_node;

    for(int i = 0; i < lst->size; i++)
    {
        new_nodes_arr[i].next  = i + 1;
        new_nodes_arr[i].value = lst->nodes_arr[cur_node].value;
        new_nodes_arr[i].prev  = i - 1;

        cur_node = lst->nodes_arr[cur_node].next;
    }


    cur_node = lst->free_node;


    for(int i = lst->size; i < lst->capacity; i++)
    {
        new_nodes_arr[i].next  = i + 1;
        new_nodes_arr[i].value = POISON;
        new_nodes_arr[i].prev  = -1;
    }

    new_nodes_arr[lst->capacity - 1].next = -1;


    lst->head_node = 0;
    lst->tail_node = lst->size - 1;
    lst->free_node = lst->size;
    new_nodes_arr[lst->tail_node].next = -1;

    free(lst->nodes_arr);
    lst->nodes_arr = new_nodes_arr;

    lst->is_linearized = 1;

    MAKE_GRAPHVIZ("list_linearize", -1, -1);

    return _SUCCESS;
}

//=====================================================================================================================================

int list_resize(struct list* lst, int new_capacity)
{
    CHECK_ERROR;

    if(lst->size * LIST_REMOVE_RESIZE <= lst->capacity)
    {
        lst->capacity = new_capacity;

        if(list_linearize(lst) != _SUCCESS)
        {
            return lst->error_code;
        }
    }

    else
    {
        lst->capacity = new_capacity;

        if(list_linearize(lst) != _SUCCESS)
        {
            return lst->error_code;
        }

        if(lst->nodes_arr == nullptr)
        {
            lst->error_code = ERROR_RESIZE_NULLPTR;

            return lst->error_code;     
        }
    }

    if(lst->free_node == -1)
    {
        lst->free_node = lst->size;
    }

    MAKE_GRAPHVIZ("list_resize", -1, -1);

    return _SUCCESS;
}

//=====================================================================================================================================

int linearized_check(struct list* lst)
{
    CHECK_ERROR;

    if((lst->size == 1) && (lst->nodes_arr[0].value != POISON))
    {
        lst->is_linearized = 1;

        return _SUCCESS;
    }

    int index = 0;

    if(lst->nodes_arr[0].prev == -1)
    {
        index++;
    }

    for(int i = 0; i < lst->size; i++)
    {
        if(lst->nodes_arr[i].next != i + 1)
        {
            break;
        }

        index++;
    }

    if(lst->nodes_arr[index].next == -1)
    {
        index++;
    }

    if(index == lst->size)
    {
        lst->is_linearized = 1;

        return _SUCCESS;
    }

    lst->is_linearized = 0;

    return _SUCCESS;
}

//=====================================================================================================================================

int translate_logical_position_to_physical_adress(struct list* lst, int logic_index, int* phys_adress)
{
    CHECK_ERROR;

    if(logic_index > lst->capacity)
    {
        *phys_adress = -1;

        lst->error_code = ERROR_TRANSLATE_LOG_TO_PHYS;

        return lst->error_code;
    }

    if(lst->is_linearized == 1)
    {
        *phys_adress = logic_index;

        return _SUCCESS;
    }

    int cur_node = lst->head_node;

    for(int i = 1; i < logic_index; i++)
    {
        cur_node = lst->nodes_arr[cur_node].next;
    }

    *phys_adress = cur_node;

    return _SUCCESS;
}

//=====================================================================================================================================

int translate_physical_adress_to_logical_position(struct list* lst, int phys_adress, int* logic_index)
{
    CHECK_ERROR;

    if((phys_adress > lst->capacity) || ((lst->nodes_arr[phys_adress].prev == -1) && (phys_adress != lst->head_node)))
    {
        *logic_index = -1;

        lst->error_code = ERROR_TRANSLATE_PHYS_TO_LOG;

        return lst->error_code;
    }
    
    if(lst->is_linearized == 1)
    {
        *logic_index = phys_adress;

        return _SUCCESS;
    }

    int nodes_counter = 0;

    for(int cur_node = phys_adress; cur_node != lst->head_node; nodes_counter++)
    {
        cur_node = lst->nodes_arr[cur_node].prev;
    }

    *logic_index = nodes_counter;

    return _SUCCESS;
}

//=====================================================================================================================================

void list_dtor(struct list* lst)
{
    switch(lst->error_code)
    {
        case(_SUCCESS):
        {
            for(int i = 0; i < lst->capacity; i++)
            {
                lst->nodes_arr[i].value = POISON;
                lst->nodes_arr[i].prev  = 0;
                lst->nodes_arr[i].next  = 0;
            }

            lst->free_node  = 0;
            lst->head_node  = 0;
            lst->tail_node  = 0;
            lst->size       = 0;
            lst->capacity   = 0;

            free(lst->nodes_arr);
            lst->nodes_arr = nullptr; 

            fclose(lst->html_file);
            lst->html_file = nullptr;

            printf(CYN "_SUCCESS list_dtor!\n" RESET);

            break;
        }

        case(ERROR_LIST_STRUCT_NULLPTR):
        {
            break;
        }

        case(ERROR_BAD_CAPACITY):
        {
            lst->capacity = 0;

            break;
        }

        case(ERROR_NODES_CALLOC_NULLPTR):
        {
            lst->capacity = 0;

            break;
        }

        case(ERROR_HTML_FILE_NULLPTR):
        {
            lst->capacity = 0;

            free(lst->nodes_arr);
            lst->html_file = nullptr;     

            break; 
        }

        case(ERROR_GRAPH_FILE_NULLPTR):
        {
            for(int i = 0; i < lst->capacity; i++)
            {
                lst->nodes_arr[i].value = POISON;
                lst->nodes_arr[i].prev  = 0;
                lst->nodes_arr[i].next  = 0;
            }

            lst->free_node  = 0;
            lst->head_node  = 0;
            lst->tail_node  = 0;
            lst->size       = 0;
            lst->capacity   = 0;

            free(lst->nodes_arr);
            lst->nodes_arr = nullptr;

            fclose(lst->html_file);
            lst->html_file = nullptr;

            break;
        }

        case(ERROR_LINEARIZE_NULLPTR):
        {
            for(int i = 0; i < lst->capacity; i++)
            {
                lst->nodes_arr[i].value = POISON;
                lst->nodes_arr[i].prev  = 0;
                lst->nodes_arr[i].next  = 0;
            }

            lst->free_node  = 0;
            lst->head_node  = 0;
            lst->tail_node  = 0;
            lst->size       = 0;
            lst->capacity   = 0;

            free(lst->nodes_arr);
            lst->nodes_arr = nullptr;

            fclose(lst->html_file);
            lst->html_file = nullptr;

            break;
        }

        case(ERROR_RESIZE_NULLPTR):
        {
            lst->free_node  = 0;
            lst->head_node  = 0;
            lst->tail_node  = 0;
            lst->size       = 0;
            lst->capacity   = 0;

            fclose(lst->html_file);
            lst->html_file = nullptr;

            break; 
        }

        case(ERROR_TRANSLATE_PHYS_TO_LOG):
        {
            for(int i = 0; i < lst->capacity; i++)
            {
                lst->nodes_arr[i].value = POISON;
                lst->nodes_arr[i].prev  = 0;
                lst->nodes_arr[i].next  = 0;
            }

            lst->free_node  = 0;
            lst->head_node  = 0;
            lst->tail_node  = 0;
            lst->size       = 0;
            lst->capacity   = 0;

            free(lst->nodes_arr);
            lst->nodes_arr = nullptr;

            fclose(lst->html_file);
            lst->html_file = nullptr;

            break;  
        }

        case(ERROR_TRANSLATE_LOG_TO_PHYS):
        {
            for(int i = 0; i < lst->capacity; i++)
            {
                lst->nodes_arr[i].value = POISON;
                lst->nodes_arr[i].prev  = 0;
                lst->nodes_arr[i].next  = 0;
            }

            lst->free_node  = 0;
            lst->head_node  = 0;
            lst->tail_node  = 0;
            lst->size       = 0;
            lst->capacity   = 0;

            free(lst->nodes_arr);
            lst->nodes_arr = nullptr;  

            fclose(lst->html_file);
            lst->html_file = nullptr;

            break;
        }

        default:
        {
            printf("What the hell kind of error is this?!\n");

            break;
        }
    }
}
