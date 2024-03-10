#ifndef GRAPHVIZ_FUNCTIONS_H
#define GRAPHVIZ_FUNCTIONS_H

//===============================================================================================================

#include "../src/ListFunctions.h"

//===============================================================================================================
/**
 * @brief Creating images when compiling a dot file. 
 * 
 * @param lst the list pointer
 * @param function the name of the function where the make_graphviz was called from.
 * @return int error_code
 */
int make_graphviz(struct list* lst, char* function);

//===============================================================================================================
/**
 * @brief Printing to a file graphviz.txt in the dot markup language.
 * 
 * @param lst the list pointer
 * @return int error_code
 */
int graph_print(struct list* lst);

//===============================================================================================================
/**
 * @brief Filling and connecting pictures into an HTML file.
 * 
 * @param lst the list pointer
 * @param function the name of the function where the make_graphviz was called from.
 * @return int error_code
 */
int html_print(struct list* lst, char* function);

//===============================================================================================================
/**
 * @brief Connecting the function name and arguments into one line for further output to a HTML-file.
 * 
 * @param lst the list pointer
 * @param function_name the name of the function where the make_graphviz was called from.
 * @param node_index the parameter node_index of the function where the make_graphviz was called from.
 * @param value the parameter value of the function where the make_graphviz was called from.
 * @param string the string to which the connection is made by the parameter.
 * @return int error_code
 */
// int get_function(struct list* lst, const char* function_name, int node_index, elem_t value, char* string);

//===============================================================================================================

#endif //GRAPHVIZ_FUNCTIONS_H
