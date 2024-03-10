#ifndef LIST_FUNCTIONS_H
#define LIST_FUNCTIONS_H

//==============================================================================================================

#include "ListInfo.h"
#include "../Graphviz/GraphvizFunctions.h"
#include "../Graphviz/Colors.h"
// #include "../common.h"

//==============================================================================================================

#define RETURN_LIST_ERROR(error_code)                                                                       \
        printf(RED "In function %s at %s(%u):\nError code: %d. Check file \"ListInfo.h\" to decipher "      \
                   "the error code.\n" RESET, __PRETTY_FUNCTION__, __FILE__, __LINE__, error_code);         \
        list_dtor(lst);                                                                                     \
        return error_code;                                                                                  \

//==============================================================================================================

#define PRINT_INFO(string)                                                                                                      \
        printf(BLU "In function %s at %s(%u):\n" RESET WHT "%s\n\n" RESET, __PRETTY_FUNCTION__, __FILE__, __LINE__, string);    \

//==============================================================================================================

#define CHECK_ERROR                         \
        if(lst->error_code != _SUCCESS)     \
        {                                   \
            return lst->error_code;         \
        }                                   \

//==============================================================================================================

#define MAKE_GRAPHVIZ(func_name, node_index, value)                     \
        char string[50] = {};                                           \
        make_graphviz(lst, string);                                     \
        // get_function(lst, func_name, node_index, value, string);        \

//==============================================================================================================
/**
 * @brief Create a list structure.
 * 
 * @param lst the list pointer
 * @return int error_code
 */
int list_ctor(struct list* lst);

//==============================================================================================================
/**
 * @brief Filling the array of node structures with at ctor
 * 
 * @param lst the list pointer
 * @param start start-of-fill index
 * @param finish end-of-fill index
 * @return int error_code
 */
int fill_with_POISON(struct list* lst, int start, int finish);

//==============================================================================================================
/**
 * @brief Pushes value to the list after a certain cell.
 * 
 * @param lst the list pointer
 * @param node_index 
 * @param value 
 * @return int error_code
 */
int push_after(struct list* lst, int node_index, Message* message);

//==============================================================================================================
/**
 * @brief Pushes value to the list before a certain cell.
 * 
 * @param lst the list pointer
 * @param node_index 
 * @param value 
 * @return int error_code
 */
int push_before(struct list* lst, int node_index, elem_t value);

//==============================================================================================================
/**
 * @brief Removing a node from the list.
 * 
 * @param lst the list pointer
 * @param node_index 
 * @param arg the variable that the value removed from the list will fall into
 * @return int error_code
 */
int node_remove(struct list* lst, int node_index, elem_t* arg);

//==============================================================================================================
/**
 * @brief Special case of the function node_remove.
 * 
 * @param lst the list pointer
 * @param node_index 
 * @param arg the variable that the value removed from the list will fall into
 * @return int error_code
 */
int head_remove(struct list* lst, elem_t* arg);

//==============================================================================================================
/**
 * @brief Special case of the function node_remove.
 * 
 * @param lst the list pointer
 * @param node_index 
 * @param arg the variable that the value removed from the list will fall into
 * @return int error_code
 */
int tail_remove(struct list* lst, elem_t* arg);

//==============================================================================================================
/**
 * @brief Alignment of logical and physical addresses, taking into account the preservation 
 *        of logical connections of nodes.
 * 
 * @param lst the list pointer
 * @return int error_code
 */
int list_linearize(struct list* lst);

//==============================================================================================================
/**
 * @brief Changing the list size when filling or large reduction.
 * 
 * @param lst the list pointer
 * @param new_capacity 
 * @return int error_code
 */
int list_resize(struct list* lst, int new_capacity);

//==============================================================================================================
/**
 * @brief Finding the physical index of a node by its logical value.
 * 
 * @param lst the list pointer
 * @param logic_index 
 * @param phys_adress 
 * @return int error_code
 */
int translate_logical_position_to_physical_adress(struct list* lst, int logic_index, int* phys_adress);

//==============================================================================================================
/**
 * @brief Finding the logical index of a node by its physical value.
 * 
 * @param lst the list pointer
 * @param phys_adress 
 * @param logic_index 
 * @return int error_code
 */
int translate_physical_adress_to_logical_position(struct list* lst, int phys_adress, int* logic_index);

//==============================================================================================================
/**
 * @brief Deleting list structure elements, freeing memory.
 * 
 * @param lst the list pointer
 */
void list_dtor(struct list* lst);

//==============================================================================================================
/**
 * @brief Checks if a list is linear.
 * 
 * @param lst the list pointer
 * @return int error_code
 */
int linearized_check(struct list* lst);

//==============================================================================================================

#endif //LIST_FUNCTIONS_H
