#ifndef LIST_INFO_H
#define LIST_INFO_H

//================================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../common.h"

//================================================================================================

typedef int elem_t;  // Type of data storage in the list.

//================================================================================================

const int INITIAL_NODES_NUM  = 1 << 3;
const int POISON             = 0xDEAD;
const int LIST_REMOVE_RESIZE = 4;
const size_t RESIZE_FACTOR   = 2;
const int _MAX_LOGIN_LENGTH  = 24;
const int _INET_ADDRSTRLEN   = 16;

//================================================================================================

// Structure for an array of node structures.
struct node
{
    int next = 0;           //| Index of the node following the current one.
    int prev = 0;           //| Index of the node preceding the current one.

    elem_t value = 0;

    struct sockaddr_in cliaddr    = {0};
    char login[_MAX_LOGIN_LENGTH] = {0};
    char ip_str[_INET_ADDRSTRLEN] = {0};
    size_t port  = 0;
    int socketfd = 0;
};

// The list structure.
struct list
{
    FILE* graph_file = nullptr;         //| Pointer to graphviz file
    FILE* html_file  = nullptr;         //| Pointer to html file
    node* nodes_arr = nullptr;          //| Pointer to an array of node structures.

    int head_node = 0;                  //| The physical index of the beginning of the list.
    int tail_node = 0;                  //| Physical index of the end of the list.
    int free_node = 0;                  //| Physical index of the first free cell.

    int capacity = 0;                   //| Current list capacity.
    int size     = 0;                   //| Current list size.

    int error_code = 0;                 //| List error code.

    bool is_linearized = 0;             //| List linearization state.
};

//================================================================================================
/**
 * @brief Notes the type of error in list.
 * 
 */
enum ListErrors
{
    _SUCCESS                    = 0,    //| no error
    ERROR_LIST_STRUCT_NULLPTR   = 1,    //| lst = nulltpr
    ERROR_NODES_CALLOC_NULLPTR  = 2,    //| nodes_arr == nullptr
    ERROR_GRAPH_FILE_NULLPTR    = 3,    //| graphviz_txt == nullptr.
    ERROR_BAD_CAPACITY          = 4,    //| capacity <= 0.
    ERROR_LINEARIZE_NULLPTR     = 5,    //| list linearization error.
    ERROR_RESIZE_NULLPTR        = 6,    //| list resize error.
    ERROR_TRANSLATE_PHYS_TO_LOG = 7,    //| error when translating from a physical node to a logical one.
    ERROR_TRANSLATE_LOG_TO_PHYS = 8,    //| error when translating from a logical node to a physical one
    ERROR_HTML_FILE_NULLPTR     = 9,    //| graph_log.html == nulltpr
};

//================================================================================================

#endif //LIST_INFO_H
