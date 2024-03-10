#include <string.h>

#include "GraphvizFunctions.h"
#include "Colors.h"

//=====================================================================================================================================

int photo_index = 0;

//=====================================================================================================================================

int make_graphviz(struct list* lst, char* function)
{
    CHECK_ERROR;
    
    graph_print(lst);
    html_print(lst, function);


    char dot_command[70] = "dot graphviz.txt -Tpng -o Images/graphviz_";
    char command_num[10];

    sprintf(command_num, "%d.png", photo_index++);
    strcat(dot_command, command_num);


    system(dot_command);

    return _SUCCESS;
}

//=====================================================================================================================================

int graph_print(struct list* lst)
{
    CHECK_ERROR;

    lst->graph_file = fopen("graphviz.txt", "w");

    if(lst->graph_file == nullptr)
    {
        lst->error_code = ERROR_GRAPH_FILE_NULLPTR;

        RETURN_LIST_ERROR(lst->error_code);
    }



    fprintf(lst->graph_file, "digraph photo_%d\n", photo_index);
    fprintf(lst->graph_file, "{\n");
    fprintf(lst->graph_file, "\trankdir = LR;\n\n");


    for(int i = 0; i < lst->capacity -1 ; i++)
    {
        fprintf(lst->graph_file, "\tnode%d -> node%d [color = invis, weight = 1000];\n", i, i + 1);
    }

    fprintf(lst->graph_file, "\n");

    for(int i = 0; i < lst->capacity; i++)
    {
        const char* ip_str = nullptr;

        if(strcmp(lst->nodes_arr[i].ip_str, "INADDR_LOOPBACK") == 0)
        {
            ip_str = "127.0.0.1";
        }
        else
        {
            ip_str = lst->nodes_arr[i].ip_str;
        }

        fprintf(lst->graph_file, "\tnode%d ", i);

        if((i == lst->head_node) || i == lst->tail_node)
        {
            if(i == lst->head_node)
            {
                // fprintf(lst->graph_file, "[style = filled, fillcolor = orange, color = black, shape = Mrecord, label = "
                //                          "\"{<f1> node_%d} | {<f2> prev = %d} | {<f3> value = %d }| {<f4> next = %d }\"];\n", 
                //                          i, lst->nodes_arr[i].prev, lst->nodes_arr[i].value, lst->nodes_arr[i].next);

                // fprintf(lst->graph_file, "\thead_node -> node%d:f1;\n", lst->head_node);
                fprintf(lst->graph_file, "[style = filled, fillcolor = orange, color = black, shape = Mrecord, label = "
                                         "\"{<f1> login: %s} | {<f2> IP = %s} | {<f3> port = %lu } | {<f4> sock_fd = %d }\"];\n", 
                                         lst->nodes_arr[i].login, ip_str, lst->nodes_arr[i].port, lst->nodes_arr[i].socketfd);
                fprintf(lst->graph_file, "\thead_node -> node%d:f1;\n", lst->head_node);
            }

            if(lst->tail_node == lst->head_node)
            {
                fprintf(lst->graph_file, "\ttail_node -> node%d:f1;\n", lst->tail_node); 

                continue;
            }

            if(i == lst->tail_node)
            {
                // fprintf(lst->graph_file, "[style = filled, fillcolor = orange, color = black, shape = Mrecord, label = "
                //                          "\"{<f1> node_%d} | {<f2> prev = %d} | {<f3> value = %d }| {<f4> next = %d }\"];\n", 
                //                          i, lst->nodes_arr[i].prev, lst->nodes_arr[i].value, lst->nodes_arr[i].next);
                // fprintf(lst->graph_file, "\ttail_node -> node%d:f1;\n", lst->tail_node);
                fprintf(lst->graph_file, "[style = filled, fillcolor = orange, color = black, shape = Mrecord, label = "
                                         "\"{<f1> login: %s} | {<f2> IP = %s} | {<f3> port = %lu } | {<f4> sock_fd = %d }\"];\n", 
                                         lst->nodes_arr[i].login, ip_str, lst->nodes_arr[i].port, lst->nodes_arr[i].socketfd);
                fprintf(lst->graph_file, "\ttail_node -> node%d:f1;\n", lst->tail_node);
            }

            continue;
        }

        if(lst->nodes_arr[i].prev != -1)
        {
            // fprintf(lst->graph_file, "[style = filled, fillcolor = orange, color = black, shape = Mrecord, label = "
            //                          "\"{<f1> node_%d} | {<f2> prev = %d} | {<f3> value = %d }| {<f4> next = %d }\"];\n", 
            //                          i, lst->nodes_arr[i].prev, lst->nodes_arr[i].value, lst->nodes_arr[i].next);
            fprintf(lst->graph_file, "[style = filled, fillcolor = orange, color = black, shape = Mrecord, label = "
                                        "\"{<f1> login: %s} | {<f2> IP = %s} | {<f3> port = %lu } | {<f4> sock_fd = %d }\"];\n", 
                                        lst->nodes_arr[i].login, ip_str, lst->nodes_arr[i].port, lst->nodes_arr[i].socketfd);
        }

        else
        {
            fprintf(lst->graph_file, "[style = filled, fillcolor = skyblue, color = black, shape = Mrecord, label =" 
                                     "\"{<f1> free_node} | {<f2> IP} | {<f3> port }| {<f4> sock_fd}\"];\n");
            // fprintf(lst->graph_file, "[style = filled, fillcolor = orange, color = black, shape = Mrecord, label = "
            //                             "\"{<f1> login: %s} | {<f2> IP = %s} | {<f3> port = %lu } | {<f4> sock_fd = %d }\"];\n", 
            //                             lst->nodes_arr[i].login, ip_str, lst->nodes_arr[i].port, lst->nodes_arr[i].socketfd);
        }
    }

    fprintf(lst->graph_file, "\n");

    if(lst->size != 0)
    {
        fprintf(lst->graph_file, "\thead_node [style = filled, fillcolor = red, color = black,"
                                " shape = Mrecord, label = \"Head node\"];\n");
        fprintf(lst->graph_file, "\ttail_node [style = filled, fillcolor = yellow, color = black,"
                                " shape = Mrecord, label = \"Tail node\"];\n");
    }


    if(lst->size < lst->capacity)
    {
        fprintf(lst->graph_file, "\tfree_node [style = filled, fillcolor = \"green\", color = \"black\","
                                 " shape = Mrecord, label = \"Free node\"]\n\n");
    }

    
    for(int i = 0; i < lst->capacity; i++)
    {
        if((lst->nodes_arr[i].next == -1) && (i != lst->head_node))
        {
            if(i == lst->tail_node)
            {
                fprintf(lst->graph_file, "\tnode%d:f2 -> node%d:f2 [color = \"invis\"];\n", i, lst->nodes_arr[i].prev);
            }

            continue;
        }

        if((lst->nodes_arr[i].prev != -1) || (i == lst->head_node))
        {
            if(lst->size == 1)
            {
                continue;
            }

            if(i != lst->head_node)
            {
                fprintf(lst->graph_file, "\tnode%d:f2 -> node%d:f2 [color = \"invis\", splines=ortho];\n", i, lst->nodes_arr[i].prev);
            }

            fprintf(lst->graph_file, "\tnode%d:f4 -> node%d:f4 [color = \"invis\", splines=ortho];\n", i, lst->nodes_arr[i].next);

            continue;
        }

        if(lst->nodes_arr[i].prev == -1)
        {
            fprintf(lst->graph_file, "\tnode%d:f1 -> node%d:f1 [color = \"seagreen\", splines=ortho];\n", i, lst->nodes_arr[i].next);
        }
    }

    if(lst->size < lst->capacity)
    {
        fprintf(lst->graph_file, "\tfree_node -> node%d:f1;\n", lst->free_node);
    }

    fprintf(lst->graph_file, "}\n\n");
    
    fclose(lst->graph_file);

    return _SUCCESS;
}

//=====================================================================================================================================

int html_print(struct list* lst, char* function)
{
    CHECK_ERROR;

    fprintf(lst->html_file, "<pre>\n\n");
    
    fprintf(lst->html_file, "\t<h2 style=\"color:" FUNC_CLR "\"><font style=\"font-size:42px\"> %s </font></h2>\n", function);

    fprintf(lst->html_file, "\t<h2>\n");
    fprintf(lst->html_file, "\t<font color = " WORD_STRUCT_CLR ">struct</font> <font color = " WORD_LIST_CLR "><u>List</u></font>\n");
    fprintf(lst->html_file, "\t{\n");
    fprintf(lst->html_file, "\t   head_node = <font color = " STRUCTS_VAL_CLR ">%d</font>;\n", lst->head_node);
    fprintf(lst->html_file, "\t   tail_node = <font color = " STRUCTS_VAL_CLR ">%d</font>;\n", lst->tail_node);
    fprintf(lst->html_file, "\t   free_node = <font color = " STRUCTS_VAL_CLR ">%d</font>;\n\n", lst->free_node);
    fprintf(lst->html_file, "\t   size      = <font color = " STRUCTS_VAL_CLR ">%d</font>;\n", lst->size);
    fprintf(lst->html_file, "\t   capacity  = <font color = " STRUCTS_VAL_CLR ">%d</font>;\n", lst->capacity);
    fprintf(lst->html_file, "\n\t   is_linearized  = <font color = " STRUCTS_VAL_CLR ">%d</font>;\n", lst->is_linearized);
    fprintf(lst->html_file, "\t};\n");
    fprintf(lst->html_file, "\t</h2>\n");

    fprintf(lst->html_file, "\t<img src = \"Images/graphviz_%d.png\">\n", photo_index);
    fprintf(lst->html_file, "\t<hr style=\"border-style: dotted\">\n\n");

    fprintf(lst->html_file, "</pre>\n\n");

    return _SUCCESS;
}

//=====================================================================================================================================

// int get_function(struct list* lst, const char* function_name, int node_index, elem_t value, char* string)
// {
//     CHECK_ERROR;

//     if(node_index != -1 && value != POISON)
//     {
//         char args_str[50] = {};

//         sprintf(args_str, "(node_index = %d, value = %d)", node_index, value);

//         strcat(string, function_name);
//         strcat(string, args_str);
//     }

//     else if(node_index != -1 && value == POISON)
//     {
//         char args_str[50] = {};

//         sprintf(args_str, "(node_index = %d)", node_index);

//         strcat(string, function_name);
//         strcat(string, args_str);
//     }

//     else
//     {
//         strcat(string, function_name); 
//     }

//     return _SUCCESS;
// }
