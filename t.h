#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// A structure to represent a stack


#define MAX_LINE_LENGTH 256
#define MAX_NUMBER_OF_LINES 400

#define SCALAR_REGEX "[[:space:]]*scalar[[:space:]]+([[:alpha:]_$][[:alpha:]_$[:digit:]]*)[[:space:]]*$"
#define VECTOR_REGEX "[[:space:]]*vector[[:space:]]+([[:alpha:]_$][[:alpha:]_$[:digit:]]*)\\[[[:space:]]*([[:digit:]]+)[[:space:]]*\\][[:space:]]*$"
#define MATRIX_REGEX "[[:space:]]*matrix[[:space:]]+([[:alpha:]_$][[:alpha:]_$[:digit:]]*)\\[[[:space:]]*([[:digit:]]+)[[:space:]]*,[[:space:]]*([[:digit:]]+)[[:space:]]*\\][[:space:]]*$"
#define ASSIGNMENT_REGEX "^[[:space:]]*([[:alpha:]_$][[:alpha:]_$[:digit:]]*)[[:space:]]*=[[:space:]]*(.+)[[:space:]]*$"
#define DEFINITION_REGEX_VECTOR_MATRIX "^[[:space:]]*([[:alpha:]_$][[:alpha:]_$[:digit:]]*)[[:space:]]*=[[:space:]]*{(.+)}[[:space:]]*"
#define DIGIT_REGEX "^[[:space:]]*([[:digit:]]+)[[:space:]]*$"
#define SQRT_REGEX "[[:space:]]*sqrt[[:space:]]*\\((.+)\\)[[:space:]]*$"
#define CHOOSE_REGEX "[[:space:]]*choose[[:space:]]*\\((.+)\\)[[:space:]]*$"
#define VECTOR_POINT_REGEX "^[[:space:]]*([[:alpha:]_$][[:alpha:]_$[:digit:]]*)\\[[[:space:]]*([[:digit:]]+)[[:space:]]*\\][[:space:]]*$"
#define MATRIX_POINT_REGEX "^[[:space:]]*([[:alpha:]_$][[:alpha:]_$[:digit:]]*)\\[[[:space:]]*([[:digit:]]+)[[:space:]]*,[[:space:]]*([[:digit:]]+)[[:space:]]*\\][[:space:]]*$"
#define PARANTH_REGEX "^[[:space:]]*\\((.+)\\)[[:space:]]*"
#define TRANSPOSE_REGEX "[[:space:]]*tr[[:space:]]*\\((.+)\\)[[:space:]]*$"
#define VARIABLE_REGEX "^[[:space:]]*([[:alpha:]_$][[:alpha:]_$[:digit:]]*)[[:space:]]*$"
