#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "t.h"
#include <ctype.h>



struct StackNode {
    char* data;
    struct StackNode* next;
};

struct StackNode* newNode(char* data)
{
    struct StackNode* stackNode =
            (struct StackNode*)
                    malloc(sizeof(struct StackNode));
    stackNode->data = data;
    stackNode->next = NULL;
    return stackNode;
}

int isEmpty(struct StackNode* root)
{
    return !root;
}

void push(struct StackNode** root, const char* data)
{
    struct StackNode* stackNode = newNode(data);
    stackNode->next = *root;
    *root = stackNode;
}

char* pop(struct StackNode** root)
{
    if (isEmpty(*root))
        return "";
    struct StackNode* temp = *root;
    *root = (*root)->next;
    char* popped = temp->data;
    free(temp);

    return popped;
}

char* peek(struct StackNode* root)
{
    if (isEmpty(root))
        return "";
    return root->data;
}

struct variableType {
    char *name;
    int dim1;
    int dim2;
    double * value;
    int error;
    int ind;
    char* cName;
};

struct variableArrayType {
    char *name;
    int dim1;
    int dim2;
    double * value;
};



struct StackVarNode {
    struct variableType data;
    struct StackVarNode* next;
};

struct StackVarNode* newVarNode(struct variableType data)
{
    struct StackVarNode* stackNode =
            (struct StackVarNode*)
                    malloc(sizeof(struct StackVarNode));
    stackNode->data = data;
    stackNode->next = NULL;
    return stackNode;
}

int isVarStackEmpty(struct StackVarNode* root)
{
    return !root;
}

void pushVarStack(struct StackVarNode** root, struct variableType data)
{
    struct StackVarNode* sNode = newVarNode(data);
    sNode->next = *root;
    *root = sNode;
}

struct variableType popVarStack(struct StackVarNode** root)
{
    if (isVarStackEmpty(*root)) {
        struct variableType s = {"", 0, 0, NULL, 1, -1};
        return s;
    }
    struct StackVarNode* temp = *root;
    *root = (*root)->next;
    struct variableType popped = temp->data;
    free(temp);
    return popped;
}

struct variableType peekVarStack(struct StackVarNode* root)
{
    if (isVarStackEmpty(root)) {
        struct variableType s = {"", 0, 0, NULL, 1, -1};
        return s;
    }
    return root->data;
}


char LINE_ARRAY[MAX_NUMBER_OF_LINES][MAX_LINE_LENGTH];
int NUMBER_OF_LINES = 0;

struct variableArrayType VARIABLES[40];
int variableNum = 0;

char PRINT_ARRAY[MAX_NUMBER_OF_LINES][5000];
int PRINT_COUNT = 0;

char FOR_ARRAY[MAX_NUMBER_OF_LINES][5000];
int FOR_COUNT = 0;


char C_CODE_ARRAY[MAX_NUMBER_OF_LINES][5000];
int  C_CODE_COUNT = 0;

char C_START_ARRAY[7][5000];
char C_END_ARRAY[1][5000];

int HAS_ERROR = 0;

char C_ERROR_ARRAY[5][MAX_LINE_LENGTH];

double* copyValueArray(double* arr, int len ){
    double *newArr = (double *) malloc(sizeof (double )*len);
    for(int i = 0; i < len; i++){
        newArr[i] = arr[i];
    }
    return newArr;
}

int getFirstIndex(char *string, char c) {
    char *e = strchr(string, c);
    if (e == NULL) {
        return -1;
    }
    return (int) (e - string);
}
int getLastIndex(char *string, char c) {
    char *e = strrchr(string, c);
    if (e == NULL) {
        return -1;
    }
    return (int) (e - string);
}

char* subStr(char* string,int start, int len){
    char* r = (char*) malloc(sizeof(char) * (len+1));
    strncpy(r,&string[start],len);
    r[len] = '\0';
    return r;
}

void getChooseParametersLengths(char* line,int* exp1, int* exp2, int* exp3, int* exp4){
    int isParant = 0, isSquareBracket = 0, whichExp = 1;
    int len1 = 0, len2 = 0, len3 = 0, len4 = 0;
    int curLen = 0;
    for(size_t i = 0; i < strlen(line); i++) {
        if (line[i] == ','){
            if(isParant == 0 && isSquareBracket == 0){
                switch (whichExp) {
                    case 1:
                        whichExp++;
                        len1 = curLen;
                        curLen = 0;
                        break;
                    case 2:
                        whichExp++;
                        len2 = curLen;
                        curLen = 0;
                        break;
                    case 3:
                        whichExp++;
                        len3 = curLen;
                        curLen = 0;
                        break;
                    case 4:
                        whichExp++;
                        len4 = curLen;
                        curLen = 0;
                    default:
                        break;
                }
            }
            else {
                curLen++;
            }
        }
        else {
            curLen++;
            if (line[i] == '(') {
                isParant++;
            } else if (line[i] == ')') {
                isParant--;
            } else if (line[i] == '[') {
                isSquareBracket++;
            } else if (line[i] == ']') {
                isSquareBracket--;
            }
        }
    }
    * exp1 = len1;
    * exp2 = len2;
    * exp3 = len3;
    * exp4 = curLen;
}


void getChooseParameters2(char* line ,char** exp1, char** exp2, char** exp3, char ** exp4){
    int len1, len2,len3, len4;
    getChooseParametersLengths(line, &len1,&len2,&len3,&len4);
    *exp1 = subStr(line,0,len1);
    *exp2 = subStr(line,len1+1,len2);
    *exp3 = subStr(line,len1+len2+2,len3);
    *exp4 = subStr(line,len1+len2+len3+3,len4);
}


int readFile() {
    FILE *file = fopen("t.txt", "r");

    if (!file) {
        printf("File not found!");
        return 0;
    }
    int len = 0;
    while (fgets(LINE_ARRAY[NUMBER_OF_LINES], MAX_LINE_LENGTH, file)) {
        len = strlen(LINE_ARRAY[NUMBER_OF_LINES]);
        if(LINE_ARRAY[NUMBER_OF_LINES][len-1] == '\n')
            LINE_ARRAY[NUMBER_OF_LINES][len-1] = '\0';
        NUMBER_OF_LINES++;
    }
    fclose(file);
    return 1;
}

int checkParanthesis(const char* exp){
    char paran[MAX_LINE_LENGTH];
    int ind = 0;
    for(int i = 0; i < strlen(exp); i++){
        if (exp[i] == '(') {
            paran[++ind] = '(';
        } else if (exp[i] == ')') {
            if(paran[ind] == '(')
                ind --;
            else
                return 0;
        } else if (exp[i] == '[') {
            paran[++ind] = '[';
        } else if (exp[i] == ']') {
            if(paran[ind] == '[')
                ind --;
            else
                return 0;
        }
    }
    return 1;
}

char* removePostWhiteSpaces(char* str){
    return strsep(&str," ");
}

char* removePreWhiteSpaces(char* string){
    int isStarted = 0;
    int i;
    int len = strlen(string) + 1;
    char* res = (char*) malloc(sizeof (char ) * MAX_LINE_LENGTH);
    int ind = 0;
    for(i = 0; i < strlen(string); i++){
        if(string[i] == ' ' && !isStarted)
            continue;
        if(!isStarted){
            isStarted = 1;
        }
        res[ind++] = string[i];
    }
    res[ind] = '\0';
    return res;
}

int hasFuncExpression(char *string, char** pmatch, char* funcName){
    char var[256];
    int ind = 0;
    int isStarted = 0;
    int i;
    for(i = 0; i < strlen(string); i++){
        if(string[i] == ' ' && !isStarted)
            continue;
        if(string[i] == '(')
            break;
        if(!isStarted){
            var[ind++] = string[i];
            isStarted = 1;
        }

        else
            var[ind++] = string[i];
    }
    var[ind] = '\0';
    int found = 0;
    char* variable = removePostWhiteSpaces(var);
    if(strcmp(variable, funcName))
        return 0;
    char res[256];
    ind = 0;
    for(int j = i+1; j < strlen(string); j++){
        res[ind++] = string[j];
    }
    res[ind] = '\0';
    int isParant = 1, isSquareBracket = 0;
    int lastParant;
    for(lastParant =0; lastParant <  (int) strlen(res); lastParant++){
        if (res[lastParant] == '(') {
            isParant++;
        } else if (res[lastParant] == ')') {
            isParant--;
        } else if (res[lastParant] == '[') {
            isSquareBracket++;
        } else if (res[lastParant] == ']') {
            isSquareBracket--;
        }
        if(isParant == 0 && isSquareBracket == 0) {
            found = 1;
            break;
        }
    }
    for(int j = lastParant + 1; j <  (int) strlen(res); j++){
        if(res[j] == ' '){
            continue;
        }
        else {
            return 0;
        }
    }
    if(found){
        res[lastParant] = '\0';
        if(pmatch != NULL)
            strcpy(*pmatch, res);
        return 1;
    }
    else
        return 0;
}
int parseSquareBracket(char* string, int *len1, int *len2) {
    int isParant = 0, isSquareBracket = 0, whichExp = 1;
    int _len1 = 0, _len2 = 0;
    int curLen = 0;
    for(size_t i = 0; i < strlen(string); i++) {
        if (string[i] == ','){
            if(isParant == 0 && isSquareBracket == 0){
                switch (whichExp) {
                    case 1:
                        whichExp++;
                        _len1 = curLen;
                        curLen = 0;
                        break;
                    case 2:
                        whichExp++;
                        _len2 = curLen;
                        curLen = 0;
                        break;
                    default:
                        break;
                }
            }
            else {
                curLen++;
            }
        }
        else {
            curLen++;
            if (string[i] == '(') {
                isParant++;
            } else if (string[i] == ')') {
                isParant--;
            } else if (string[i] == '[') {
                isSquareBracket++;
            } else if (string[i] == ']') {
                isSquareBracket--;
            }
        }
    }
    if(whichExp == 1){
        _len1 = curLen;
    }
    else if(whichExp == 2) {
        _len2 = curLen;
    }
    else {
        return 0;
    }
    *len1 = _len1;
    *len2 = _len2;
    return 1;
}

int hasChooseExpression(char *string, char** exp){
    return hasFuncExpression(string, exp, "choose");
}
int hasTransposeExpression(char *string, char** exp){
    return hasFuncExpression(string, exp, "tr");
}
int hasSqrtExpression(char *string, char** exp){
    return hasFuncExpression(string, exp, "sqrt");
}
int hasPrintExpression(char *string, char** exp){
    return hasFuncExpression(string, exp, "print");
}
int hasPrintSepExpression(char *string, char** exp){
    return hasFuncExpression(string, exp, "printsep");
}

int hasParanthesisExpression(char *string, char** pmatch) {
    return hasFuncExpression(string, pmatch, "");
}

int hasVariableExpression(char* string, char** varName) {
    char* res = removePreWhiteSpaces(string);
    int hasEnded = 0;
    for(int i = 0; i < strlen(res); i++){
        if(res[i] == ' ' && hasEnded)
            continue;
        if(res[i] == ' ' && !hasEnded) {
            hasEnded = 1;
            continue;
        }
        if(hasEnded) {
            return 0;
        }
        if(isalnum(res[i]) || res[i] == '_' || res[i] == '$')
            continue;
        return 0;
    }
    if(isalpha(res[0]) || res[0] == '_' || res[0] == '$') {
        char* finalRes = removePostWhiteSpaces(res);
        if(varName != NULL)
            *varName = finalRes;
        return 1;
    }
    return 0;

}
int hasDigitExpression(char* string, char** pmatch){
    char* res = removePreWhiteSpaces(string);
    int hasEnded = 0;
    for(int i = 0; i < strlen(res); i++){
        if(res[i] == ' ' && hasEnded)
            continue;
        if(res[i] == ' ' && !hasEnded) {
            hasEnded = 1;
            continue;
        }
        if(hasEnded) {
            return 0;
        }
        if(isdigit(res[i]) || res[i] == '.')
            continue;
        return 0;
    }
    if(pmatch != NULL)
        *pmatch = removePostWhiteSpaces(res);
    return 1;
}

int hasVectorPointExpression(char* string, char** varName, char** index){
    int startInd = getFirstIndex(string, '[');
    if(startInd == -1)
        return 0;
    int endInd = getLastIndex(string, ']');
    if(endInd == -1)
        return 0;
    for(int j = endInd+1; j < strlen(string); j++ ){
        if(string[j] != ' '){
            return 0;
        }
    }
    int len1 = 0, len2 = 0;
    char* range = subStr(string, startInd+1, endInd-startInd-1);
    int p = parseSquareBracket(range, &len1, &len2);
    if(p == 0 | len1 == 0 || len2 != 0)
        return 0;
    char* var = subStr(string, 0, startInd);
    char* finalVar = (char *) malloc(sizeof (char )* strlen(var));
    int x = hasVariableExpression(var, &finalVar);
    if(!x)
        return 0;
    if(varName != NULL)
        *varName = finalVar;
    if(index != NULL)
        *index = subStr(string, startInd+1, endInd-startInd-1);
    return 1;
}
int hasMatrixPointExpression(char* string, char** varName, char** index1, char** index2){
    int startInd = getFirstIndex(string, '[');
    if(startInd == -1)
        return 0;
    int endInd = getLastIndex(string, ']');
    if(endInd == -1)
        return 0;
    for(int j = endInd+1; j < strlen(string); j++ ){
        if(string[j] != ' '){
            return 0;
        }
    }
    int len1 = 0, len2 = 0;
    char* range = subStr(string, startInd+1, endInd-startInd-1);
    int p = parseSquareBracket(range, &len1, &len2);
    if(p == 0 | len1 == 0 || len2 == 0)
        return 0;
    char* var = subStr(string, 0, startInd);
    char* finalVar = (char *) malloc(sizeof (char )* strlen(var));
    int x = hasVariableExpression(var, &finalVar);
    if(!x)
        return 0;
    if(varName != NULL)
        *varName = finalVar;
    if(index1 != NULL)
        *index1 = subStr(string, startInd+1, len1);
    if(index2 != NULL)
        *index2 = subStr(string, startInd+len1+2, endInd-startInd-len1-2);
    return 1;
}

int hasScalarDeclarationExpression(char* string, char** varName){
    char* str = removePreWhiteSpaces(string);
    char* prefix = strsep(&str, " ");
    if(strcmp("scalar", prefix))
        return 0;
    int r = hasVariableExpression(str, varName);
    return r;
}
int hasVectorDeclarationExpression(char* string, char** varName, char** ind){
    char* str = removePreWhiteSpaces(string);
    char* prefix = strsep(&str, " ");
    if(strcmp("vector", prefix))
        return 0;
    char* range;
    int r = hasVectorPointExpression(str, varName, &range);
    if(!r){
        return 0;
    }
    char* _d;
    int d = hasDigitExpression(range, &_d);
    if(!d){
        return 0;
    }
    if(ind != NULL)
        *ind = _d;
    return 1;
}
int hasMatrixDeclarationExpression(char* string, char** varName, char** ind1, char** ind2){
    char* str = removePreWhiteSpaces(string);
    char* prefix = strsep(&str, " ");
    if(strcmp("matrix", prefix))
        return 0;
    char* range1, *range2;
    int r = hasMatrixPointExpression(str, varName, &range1, &range2);
    if(!r){
        return 0;
    }
    char* _d1, *_d2;
    int d1 = hasDigitExpression(range1, &_d1);
    if(!d1){
        return 0;
    }
    int d2 = hasDigitExpression(range2, &_d2);
    if(!d2){
        return 0;
    }
    if(ind1 != NULL)
        *ind1 = _d1;
    if(ind2 != NULL)
        *ind2 = _d2;
    return 1;
}

int hasVectorDefinitionExpression(char* string, char** varName, char** ind1) {
    char* str = removePreWhiteSpaces(string);
    char* var = strsep(&str, "=");
    char* _varName;
    int v = hasVariableExpression(var, &_varName);
    if(!v)
        return 0;
    char arr[256];
    int ind = 0;
    int len = 0;
    int found1 = 0, found2 = 0;
    for(int i = 0; i < strlen(str); i++) {
        if(str[i] == '{') {
            if(!found1 && !found2)
                found1 = 1;
            else
                return 0;
        }
        else if(str[i] == ' '){
            if(found2)
                continue;
            if(found1 && !found2) {
                arr[ind++] = str[i];
                len++;
            }
        }
        else if(isdigit(str[i]) || str[i] == '.'){
            if(found1 && !found2) {
                arr[ind++] = str[i];
                len++;
            }
            else
                return 0;
        }
        else if(str[i] == '}'){
            if(found1 && !found2)
                continue;
            else
                return 0;
        }
        else {
            return 0;
        }
    }
    char* returnInd = (char *) malloc(sizeof (char ) * len + 1);
    returnInd = arr;
    returnInd[len] = '\0';
    if(ind1 != NULL)
        *ind1 = returnInd;
    if(varName != NULL)
        *varName = _varName;
    return 1;

}
int hasScalarDefinitionExpression(char* string, char** varName, char** ind1){
    char* str = removePreWhiteSpaces(string);
    char* var = strsep(&str, "=");
    char* _varName;
    int v = hasVariableExpression(var, &_varName);
    if(!v)
        return 0;
    char* arr;
    int hasD = hasDigitExpression(str, &arr);
    if(!hasD){
        return 0;
    }
    if(ind1 != NULL)
        *ind1 = arr;
    if(varName != NULL)
        *varName = _varName;
    return 1;
}

int hasAssignmentExpression(char* string, char** varName, char** exp){
    char* str = removePreWhiteSpaces(string);
    char* var = strsep(&str, "=");
    char* _varName;
    int v = hasVariableExpression(var, &_varName);
    if(!v)
        return 0;
    if(exp != NULL)
        *exp = str;
    if(varName != NULL)
        *varName = _varName;
    return 1;
}
int hasVectorPointAssignmentExpression(char* string, char** varName, char** ind1, char** exp){
    char* str = removePreWhiteSpaces(string);
    char* var = strsep(&str, "=");
    char* _varName, *_ind1;
    int v = hasVectorPointExpression(var, &_varName, &_ind1);
    if(!v)
        return 0;
    if(exp != NULL)
        *exp = str;
    if(ind1 != NULL)
        *ind1 = _ind1;
    if(varName != NULL)
        *varName = _varName;
    return 1;
}
int hasMatrixPointAssignmentExpression(char* string, char** varName, char** ind1, char** ind2, char** exp){
    char* str = removePreWhiteSpaces(string);
    char* var = strsep(&str, "=");
    char* _varName, *_ind1, *_ind2;
    int v = hasMatrixPointExpression(var, &_varName, &_ind1, &_ind2);
    if(!v)
        return 0;
    if(exp != NULL)
        *exp = str;
    if(ind1 != NULL)
        *ind1 = _ind1;
    if(ind2 != NULL)
        *ind1 = _ind2;
    if(varName != NULL)
        *varName = _varName;
    return 1;
}

int canBeProcessed(char* exp){
    return hasChooseExpression(exp, NULL) || hasTransposeExpression(exp,NULL)
    || hasSqrtExpression(exp,NULL) || hasVariableExpression(exp,NULL)
    || hasDigitExpression(exp,NULL) || hasVectorPointExpression(exp, NULL, NULL)
    || hasMatrixPointExpression(exp, NULL, NULL, NULL);
}

char* handleComments(char *_string) {
    char* string = (char*) malloc(sizeof (char ) * strlen(_string));
    strcpy(string,_string);
    if (*string == '\0') {
        return NULL;
    }
    char* newString = strsep(&string, "#");
    int len = (int) strlen(newString);
    if(len == 0)
        return NULL;
    for(int i = 0; i < len; i++){
        if(newString[i] != ' ')
            return newString;
    }
    return NULL;
}

int isOperator(char c){

    if(c == '*'){
        return 2;
    }
    if(c == '+' | c == '-'){
        return 1;
    }
    return 0;
}

int isInteger(double num){
    return num == (int) num;
}
struct variableType returnErrorVar() {
    struct variableType s = {"", 0, 0, NULL, 1, -1};
    return s;
}

struct StackNode* infixToPostFix(char* exp){
    struct StackNode* stackNode =  NULL;
    int totalNum = 0;
    int ind = 0;
    int currLen = 0;
    int numOfParan = 0, numOfSquare = 0;
    for(size_t i = 0; i < strlen(exp); i++){
        if(isOperator(exp[i])){
            if(numOfSquare == 0 && numOfParan == 0){
                totalNum++;
                char* x = malloc(currLen+1);
                strncpy(x,&exp[i-currLen],currLen);
                x[currLen] = '\0';
                push(&stackNode, x);
                currLen = 0;
                char *y = malloc(2);
                y[0] = exp[i];
                y[1] = '\0';
                push(&stackNode, y);
//                free(y);
                ind++;
            }
            else {
                currLen++;
            }
        }
        else {
            if(exp[i] == '('){
                numOfParan++;
            }
            else if(exp[i] == ')') {
                numOfParan--;
            }
            else if(exp[i] == '[') {
                numOfSquare++;
            }
            else if(exp[i] == ']'){
                numOfSquare--;
            }
            currLen++;
        }
    }
    if(currLen != 0){
        char* x = malloc(currLen+1);
        strncpy(x,&exp[strlen(exp)-currLen],currLen);
        x[currLen] = '\0';
        push(&stackNode, x);
    }

    struct StackNode* stack =  NULL;
    while(!isEmpty(stackNode)){
        push(&stack, pop(&stackNode));
    }
    struct StackNode* res = NULL;
    struct StackNode* pStack = NULL;
    while(!isEmpty(stack)){
        char* curr = pop(&stack);
        int prec = isOperator(*curr);
        if(prec){
            while (!isEmpty(pStack) && prec <= isOperator(*peek(pStack))){
                push(&res, pop(&pStack));
            }
            push(&pStack, curr);
        }
        else {
            char* varName = (char *) malloc(sizeof (char ) * MAX_LINE_LENGTH);
            if(hasParanthesisExpression(exp,&varName)) {
                struct StackNode* s = infixToPostFix(varName);
                while (!isEmpty(s)){
                    push(&res, pop(&s));
                }
            }
            else {
                push(&res, curr);

            }
        }
    }
    while (!isEmpty(pStack)){
        push(&res, pop(&pStack));
    }
    struct StackNode* _res = NULL;

    while (!isEmpty(res)){
        push(&_res, pop(&res));
    }
    return _res;

}

struct variableType evaluateExpression(char* exp) {
    char* varName =  (char *) malloc(MAX_LINE_LENGTH * sizeof (char ));
    char* ind1 =  (char *) malloc(MAX_LINE_LENGTH * sizeof (char ));
    char* ind2 =  (char *) malloc(MAX_LINE_LENGTH * sizeof (char ));
    if(hasSqrtExpression(exp, &varName)){
        struct variableType res = evaluateExpression(varName);
        if(res.error){
            return returnErrorVar();
        }
        int dim = res.dim1*res.dim2;
        for(int j = 0; j < dim; j++){
            res.value[j] = sqrt(res.value[j]);
        }
        char *l = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
        sprintf(l, "func_sqrt(%s, %d);\n", res.cName, dim);
        strcpy(res.cName, l);
        free(l);
        return res;
    }
    else if(hasChooseExpression(exp, &varName)){
        char* exp1;
        char* exp2;
        char* exp3;
        char* exp4;
        getChooseParameters2(varName, &exp1, &exp2,&exp3,&exp4);
        struct variableType x1 = evaluateExpression(exp1);
        if(x1.error){
            return returnErrorVar();
        }
        struct variableType x2 = evaluateExpression(exp2);
        if(x2.error){
            return returnErrorVar();
        }
        struct variableType x3 = evaluateExpression(exp3);
        if(x3.error){
            return returnErrorVar();
        }
        struct variableType x4 = evaluateExpression(exp4);
        if(x4.error){
            return returnErrorVar();
        }
        if(x1.dim1 == x1.dim2 == x2.dim1 == x2.dim2 == x3.dim1 == x3.dim2 == x4.dim1 == x4.dim2 != 1){
            struct variableType x = {.name="", 0, 0, 0, 1, -1};
            return x;
        }
        char *l = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
        struct variableType res;
        sprintf(l, "func_choose(%s,%s,%s,%s)", x1.cName,x2.cName,x3.cName,x4.cName);

        if(x1.value[0]==0){
            strcpy(x2.cName, l);
            free(l);
            return x2;
        }
        else if(x1.value[0] > 0){
            strcpy(x3.cName, l);
            free(l);
            return x3;
        }
        else {
            strcpy(x4.cName, l);
            free(l);
            return x4;
        }
    }
    else if(hasTransposeExpression(exp, &varName)){
        struct variableType res = evaluateExpression(varName);
        if(res.error){
            return returnErrorVar();
        }
        int dim1 = res.dim1;
        int dim2 = res.dim2;
        int i, j;
        double* resArr = (double *) malloc(dim1*dim2*sizeof(double ));
        for(i = 0; i < dim1; i++){
            for(j = 0; j < dim2; j++){
                resArr[i*dim2+j] = res.value[j*dim1+i];
            }
        }

        struct variableType s = {"", dim2, dim1, copyValueArray(resArr, dim1*dim2), 0, res.ind};
        char *l = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
        sprintf(l, "func_tr(%s, %d, %d);\n", res.cName, dim1, dim2);
        strcpy(s.cName, l);
        free(l);
        return s;
    }
    else if(hasVariableExpression(exp, &varName)){
        int i;
        struct variableArrayType s;
        int found = 0;
        for(i = 0; i < variableNum; i++) {
            if(!strcmp(VARIABLES[i].name, varName)){
                s =  VARIABLES[i];
                found = i+1;
                break;
            }
        }
        double* arr = copyValueArray(s.value, s.dim1*s.dim2);
        if (found){
            struct variableType res = {.name=s.name, s.dim1, s.dim2, arr, 0,found-1};
            res.cName = (char*) malloc(sizeof (char )* MAX_LINE_LENGTH);
            strcpy(res.cName, s.name);
            return res;
        }
        struct variableType x = {.name="", 0, 0, 0, 1,-1};
        return x;
    }
    else if(hasVectorPointExpression(exp,&varName, &ind1)) {
        struct variableType x = evaluateExpression(ind1);
        if(x.error){
            return returnErrorVar();
        }
        if(x.dim1 != 1 && x.dim2 != 1){
            return returnErrorVar();
        }
        double dimDouble = x.value[0];
        if(dimDouble <= 0 || !isInteger(dimDouble)) {
            return returnErrorVar();
        }
        int dim1 = (int) dimDouble;
        struct variableType s = evaluateExpression(varName);
        if(s.error){
            return returnErrorVar();
        }
        if(dim1 > s.dim1) {
            return returnErrorVar();
        }
        double* ans  = (double *) malloc(sizeof (double ));
        ans[0] = s.value[dim1-1];
        struct variableType res = {.name="", 1, 1, ans, 0, s.ind};
        char *l = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
        sprintf(l, "%s[%d]", s.cName, dim1-1);
        strcpy(res.cName, l);
        free(l);
        return res;
    }
    else if(hasMatrixPointExpression(exp, &varName, &ind1, &ind2)) {
        struct variableType x1 = evaluateExpression(ind1);
        if(x1.error){
            return returnErrorVar();
        }
        if(x1.dim1 != 1 && x1.dim2 != 1){
            return returnErrorVar();
        }
        struct variableType x2 = evaluateExpression(ind2);
        if(x2.error){
            return returnErrorVar();
        }
        if(x2.dim1 != 1 && x2.dim2 != 1){
            return returnErrorVar();
        }
        double dim1Double = x1.value[0];
        double dim2Double = x2.value[0];


        if(dim1Double <= 0 || dim2Double <= 0 || !isInteger(dim1Double) || !isInteger(dim2Double)) {
            return returnErrorVar();
        }
        int dim1 = (int) dim1Double;
        int dim2 = (int) dim2Double;
        struct variableType s = evaluateExpression(varName);
        if(s.error){
            return returnErrorVar();
        }
        if(dim1 > s.dim1 || dim2 > s.dim2) {
            return returnErrorVar();
        }
        double* ans  = (double *) malloc(sizeof (double ));
        ans[0] = s.value[(dim1-1) * s.dim1 + dim2 - 1];
        struct variableType res = {.name="", 1, 1, ans, 0, s.ind, ""};
        char *l = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
        sprintf(l, "%s[%d]", s.cName, (dim1-1) * s.dim1 + dim2 - 1);
        res.cName = (char*) malloc(sizeof (char ) * MAX_LINE_LENGTH);
        strcpy(res.cName, l);
        free(l);
        return res;
    }
    else if(hasDigitExpression(exp, &ind1)){
        double* arr = (double *) malloc(sizeof (double ) * 1);
        arr[0] = (double ) atof(ind1);
        struct variableType s = {"", 1, 1, copyValueArray(arr, 1), 0, -1,""};
        char *l = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
        sprintf(l, "%f", arr[0]);
        s.cName = (char*) malloc(sizeof (char ) * MAX_LINE_LENGTH);
        strcpy(s.cName, l);
        free(l);
        return s;
    }
    else {
        struct StackNode* expStack = infixToPostFix(exp);
        struct StackVarNode* evalStack = NULL;
        while (!isEmpty(expStack)){
            char* curr = pop(&expStack);

            if(isOperator(*curr)){
                struct variableType x2 = popVarStack(&evalStack);
                if(x2.error){
                    return returnErrorVar();
                }
                struct variableType x1 = popVarStack(&evalStack);
                if(x1.error){
                    return returnErrorVar();
                }
                if(*curr == '+'){
                    if(x1.dim1 != x2.dim1 || x1.dim2 != x2.dim2){
                        return returnErrorVar();
                    }
                    else {
                        int d = x1.dim1 * x1.dim2;
                        double* arr = (double *) malloc(sizeof (double ) *d);
                        for (int i = 0; i < d; i++) {
                            arr[i] = x1.value[i] + x2.value[i];
                        }
                        struct variableType s = {"", x1.dim1, x1.dim2, arr, 0};
                        char *l = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
                        sprintf(l, "func_add(%s, %s, %d)", x1.cName, x2.cName, d);
                        s.cName = (char*) malloc(sizeof (char )* MAX_LINE_LENGTH);
                        strcpy(s.cName, l);
                        free(l);
                        pushVarStack(&evalStack, s);
                    }
                }
                else if(*curr == '-'){
                    if(x1.dim1 != x2.dim1 || x1.dim2 != x2.dim2){
                        return returnErrorVar();
                    }
                    else {
                        int d = x1.dim1 * x1.dim2;
                        double* arr = (double *) malloc(sizeof (double ) *d);
                        for (int i = 0; i < d; i++) {
                            arr[i] = x1.value[i] - x2.value[i];
                        }
                        struct variableType s = {"", x1.dim1, x1.dim2, copyValueArray(arr, d), 0, -1};
                        char *l = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
                        sprintf(l, "func_subtract(%s, %s, %d)", x1.cName, x2.cName, d);
                        s.cName = (char*) malloc(sizeof (char )* MAX_LINE_LENGTH);
                        strcpy(s.cName, l);
                        free(l);
                        pushVarStack(&evalStack, s);
                    }
                }
                else if(*curr == '*'){
                    if(x1.dim2 != x2.dim1){
                        return returnErrorVar();
                    }
                    else {
                        int i,j,k;
                        int d = x1.dim1;
                        int d2 = x2.dim2;
                        int d3 = x1.dim2;
                        double *  _arr = (double *) malloc(sizeof (double ) * d * d2);
                        for(int w = 0; w < d*d2; w++){
                            _arr[w] = 0;
                        }
                        for(i = 0; i <d; i++){
                            for(j = 0; j < d2; j++){
                                for(k = 0; k < d3; k++){
                                    _arr[i*d2+j] += x1.value[i*d3+k] * x2.value[k*d2+j];
                                }
                            }
                        }
                        struct variableType s = {"", d, d2, copyValueArray(_arr, d*d2), 0,-1};
                        char *l = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
                        sprintf(l, "func_multiply(%s, %s, %d, %d, %d)", x1.cName, x2.cName, d, d2, d3);
                        strcpy(s.cName, l);
                        free(l);
                        pushVarStack(&evalStack, s);
                    }
                }
            }
            else {
                int isTerm = canBeProcessed(curr);
                if(!isTerm)
                    return returnErrorVar();
                struct variableType s = evaluateExpression(curr);
                pushVarStack(&evalStack, s);
            }
        }
        return popVarStack(&evalStack);
    }
}

int processCodeLine(char *line, int shouldPrint) {
    char* varName =  (char *) malloc(MAX_LINE_LENGTH * sizeof (char ));
    char* ind1 =  (char *) malloc(MAX_LINE_LENGTH * sizeof (char ));
    char* ind2 =  (char *) malloc(MAX_LINE_LENGTH * sizeof (char ));
    char* exp =  (char *) malloc(MAX_LINE_LENGTH * sizeof (char ));
    if (hasScalarDeclarationExpression(line, &varName)) {
        double* y = (double *) calloc(1, sizeof (double ));
        struct variableArrayType a = {.name = varName, .dim1 =1, .dim2 = 1, .value = y};
        VARIABLES[variableNum] = a;
        variableNum++;
        if(shouldPrint) {
            char *l = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
            sprintf(l, "double * %s  = (double *) calloc(1, sizeof(double));\n", varName);
            strcpy(C_CODE_ARRAY[C_CODE_COUNT++], l);
            free(l);
        }
    }
    else if (hasVectorDeclarationExpression(line, &varName, &ind1)) {
        double dim1Double = (double ) atof(ind1);
        if(!isInteger(dim1Double))
            return 0;
        int dim1 = (int) dim1Double;
        if(dim1 == 0) {
            return 0;
        }
        double* _y = (double *) calloc(dim1,sizeof (double ));

        struct variableArrayType b = {.name = varName, .dim1 =dim1, .dim2 = 1, .value =  _y};
        VARIABLES[variableNum] = b;
        variableNum++;
        if(shouldPrint) {
            char *l = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
            sprintf(l, "double * %s  = (double *) calloc(%d, sizeof(double));\n", varName, dim1);
            strcpy(C_CODE_ARRAY[C_CODE_COUNT++], l);
            free(l);
        }
    }
    else if (hasMatrixDeclarationExpression(line, &varName, &ind1, &ind2)) {
        double dim1Double = (double ) atof(ind1);
        if(!isInteger(dim1Double))
            return 0;
        int dim1 = (int) dim1Double;

        double dim2Double = (double ) atof(ind2);
        if(!isInteger(dim2Double))
            return 0;
        int dim2 = (int) dim2Double;

        if(dim1 == 0 || dim2 == 0){
            return 0;
        }
        double* y = (double *) calloc(dim1*dim2, sizeof (double ));
        struct variableArrayType c = { .name = varName, .dim1 = dim1, .dim2 = dim2, .value = y };
        VARIABLES[variableNum] = c;
        variableNum++;
        if(shouldPrint) {
            char *l = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
            sprintf(l, "double * %s  = (double *) calloc(%d, sizeof(double));\n", varName, dim1 * dim2);
            strcpy(C_CODE_ARRAY[C_CODE_COUNT++], l);
            free(l);
        }
    }
    else if(hasVectorDefinitionExpression(line, &varName,&ind1)) {
        struct variableType var = evaluateExpression(varName);
        if(var.error || var.ind == -1){
            return 0;
        }
        double arr[var.dim1 * var.dim2];
        int dimVar = 0;
        char* curr;
        struct variableType currVal;
        while( (curr = strsep(&ind1," ")) != NULL ) {
            if(strlen(curr) == 0){
                continue;
            }

            currVal = evaluateExpression(curr);
            if(currVal.error || currVal.dim1 != 1 || currVal.dim2 != 1)
                return 0;
            arr[dimVar] =  currVal.value[0];
            dimVar++;
        }
        if(var.dim1 * var.dim2 != dimVar)
            return 0;
        struct variableArrayType finalRes = {var.name, var.dim1, var.dim2, copyValueArray(arr, dimVar)};
        VARIABLES[var.ind] = finalRes;
        if(shouldPrint) {
            for (int i = 0; i < dimVar; i++) {
                char *l = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
                sprintf(l, "%s[%d]  = %f;\n", varName, i, arr[i]);
                strcpy(C_CODE_ARRAY[C_CODE_COUNT++], l);
                free(l);
            }
        }
        return 1;
    }
    else if(hasScalarDefinitionExpression(line, &varName, &ind1)){
        struct variableType var = evaluateExpression(varName);
        if(var.error || var.ind == -1){
            return 0;
        }
        if(var.dim1 != 1 || var.dim2 != 1){
            return 0;
        }
        double val = (double ) atof(ind1);
        double* resArr = (double *) malloc(sizeof (double ) );
        resArr[0] = val;
        struct variableArrayType s = {var.name, var.dim1, var.dim2, copyValueArray(resArr, 1)};
        VARIABLES[var.ind] = s;
        if(shouldPrint) {
            char *l = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
            sprintf(l, "%s[0]  = %f;\n", varName, resArr[0]);
            strcpy(C_CODE_ARRAY[C_CODE_COUNT++], l);
            free(l);
        }
        return 1;
    }
    else if(hasAssignmentExpression(line, &varName, &exp)){
        struct variableType var = evaluateExpression(varName);
        if(var.error || var.ind == -1){
            return 0;
        }
        struct variableArrayType originalVar = VARIABLES[var.ind];
        int originalDim = originalVar.dim1 * originalVar.dim2;

        struct variableType val = evaluateExpression(exp);
        if(val.error)
            return 0;

        int newDim = val.dim1 * val.dim2;
        if(newDim != originalDim)
            return 0;

        struct variableArrayType s = {var.name, var.dim1, var.dim2, copyValueArray(val.value, originalDim)};
        VARIABLES[var.ind] = s;
        if(shouldPrint) {
            char *l = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
            sprintf(l, "%s  = %s;\n", varName, val.cName);
            strcpy(C_CODE_ARRAY[C_CODE_COUNT++], l);
            free(l);
        }
        return 1;
    }
    else if(hasVectorPointAssignmentExpression(line, &varName,&ind1, &exp)) {
        struct variableType var = evaluateExpression(varName);
        if(var.error || var.ind == -1){
            return 0;
        }
        struct variableType ind = evaluateExpression(ind1);
        if(ind.error || ind.dim1 != 1 || ind.dim2 != 1){
            return 0;
        }
        struct variableArrayType originalVar = VARIABLES[var.ind];
        int originalDim = originalVar.dim1 * originalVar.dim2;

        struct variableType val = evaluateExpression(exp);
        if(val.error || val.dim1 != 1 || val.dim2 != 1)
            return 0;

        double* valDouble = copyValueArray(originalVar.value, originalDim);
        if(!isInteger(*ind.value))
            return 0;
        valDouble[(int) *ind.value] = val.value[0];
        struct variableArrayType s = {var.name, var.dim1, var.dim2, valDouble};
        VARIABLES[var.ind] = s;
        if(shouldPrint) {
            char *l = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
            sprintf(l, "%s[%d]  = %s;\n", varName, (int) *ind.value, val.cName);
            strcpy(C_CODE_ARRAY[C_CODE_COUNT++], l);
            free(l);
        }
        return 1;
    }
    else if(hasMatrixPointAssignmentExpression(line, &varName,&ind1,&ind2, &exp)) {
        struct variableType var = evaluateExpression(varName);
        if(var.error || var.ind == -1){
            return 0;
        }
        struct variableType ind1varType = evaluateExpression(ind1);
        if(ind1varType.error || ind1varType.dim1 != 1 || ind1varType.dim2 != 1 || !isInteger(*ind1varType.value) || *ind1varType.value <= 0){
            return 0;
        }
        struct variableType ind2varType = evaluateExpression(ind2);
        if(ind2varType.error || ind2varType.dim1 != 1 || ind2varType.dim2 != 1|| !isInteger(*ind2varType.value || *ind2varType.value <= 0)){
            return 0;
        }

        struct variableArrayType originalVar = VARIABLES[var.ind];
        if((*ind1varType.value - 1) >= originalVar.dim1 || (*ind2varType.value - 1) >= originalVar.dim2)
            return 0;
        int originalDim = originalVar.dim1 * originalVar.dim2;
        int changedInd = (int) ((*ind1varType.value - 1) * originalVar.dim1 + (*ind2varType.value - 1));

        struct variableType val = evaluateExpression(exp);
        if(val.error || val.dim1 != 1 || val.dim2 != 1)
            return 0;

        double* valDouble = copyValueArray(originalVar.value, originalDim);

        valDouble[changedInd] = val.value[0];
        struct variableArrayType s = {var.name, var.dim1, var.dim2, valDouble};
        VARIABLES[var.ind] = s;
        if(shouldPrint) {
            char *l = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
            sprintf(l, "%s[%d]  = %s;\n", varName, changedInd, val.cName);
            strcpy(C_CODE_ARRAY[C_CODE_COUNT++], l);
            free(l);
        }

        return 1;
    }
    else if(hasPrintExpression(line, &varName)){
        struct variableType var = evaluateExpression(varName);
        if(var.error || var.dim1 < 1 || var.dim2 < 1)
            return 0;
        int dim = var.dim1 * var.dim2;
        for(int i = 0; i < dim; i++) {
            char str[MAX_LINE_LENGTH];
            if (isInteger(var.value[i])) {
                sprintf(str, "%d", (int) var.value[i]);
                strcpy(PRINT_ARRAY[PRINT_COUNT++], str);
            }
            else {
                sprintf(str, "%.7f", var.value[i]);
                strcpy(PRINT_ARRAY[PRINT_COUNT++], str);
            }
            if(shouldPrint) {
                char *l = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
                sprintf(l, "printf(\"%s[%d]\");\n", var.cName, i);
                strcpy(C_CODE_ARRAY[C_CODE_COUNT++], l);
                free(l);
            }
        }
    }
    else if(hasPrintSepExpression(line, &varName)){
        for(int i = 0; i < strlen(varName); i++){
            if(varName[i] != ' ')
                return 0;
        }
        if(shouldPrint) {
            char *printSepChar = "----------";
            strcpy(PRINT_ARRAY[PRINT_COUNT++], printSepChar);
            strcpy(C_CODE_ARRAY[C_CODE_COUNT++], "printf(\"----------\")");
        }
    }
    else {
        return 0;
    }
    return 1;
}

int getForLoop(char* line, char** exp){
    char *lineCp = (char *) malloc(sizeof (char ) * strlen(line));
    strcpy(lineCp, line);
    int len = getLastIndex(lineCp, '{');
    for(int i = len+1; i < strlen(lineCp); i++){
        if(lineCp[i] != ' ')
            return 0;
    }
    char* sub = subStr(lineCp, 0, len-1);
    char* _exp ;//= (char*) malloc(sizeof (char ) * strlen(line));
    int r =  hasFuncExpression(sub, exp, "for");
    if(!r)
        return 0;
    return 1;
}
int checkEndForLoop(char* line){
    int found  = 0;
    for(int i = 0; i < strlen(line); i++) {
        if(line[i] ==' ')
            continue;
        else if(line[i] == '}' && found == 0)
            found = 1;
        else
            return 0;
    }
    return found;
}

int hasOneForExp(char* exp, char **varName, char** start, char** end, char** step){
    char* _exp = removePreWhiteSpaces(exp);
    char* _varName = strsep(&_exp, " ");
    if(_varName == NULL || strlen(_varName) == 0 || strlen(_varName) == strlen(exp))
        return 0;
    char* _temp;
    if(!hasVariableExpression(_varName, &_temp))
        return 0;
    *varName = _varName;
    int i;
    for(i = 0; i < strlen(_exp); i++){
        if(_exp[i] == ' ')
            continue;
        if(_exp[i] == 'i') {
            if(i+1 < strlen(_exp) && _exp[i+1] == 'n') {
                if (i+2 < strlen(_exp) && _exp[i+2] == ' ')
                    break;
                else
                    return 0;
            }
            else
                return 0;
        }
    }
    char* range = subStr(_exp, i+3, (int)strlen(_exp) - i - 3);
    char* _start = strsep(&range, ":");
    if(_start != NULL)
        *start = _start;
    else
        return 0;
    char* _end = strsep(&range, ":");
    if(_end != NULL)
        *end = _end;
    else
        return 0;
    char* _step = (char *) malloc(sizeof (char ) * strlen(range));
    strcpy(_step, range);
    if(strlen(_step) != 0)
        *step = _step;
    else
        return 0;
    return 1;
}
int hasTwoForExp(char* exp, char **varName1, char** start1, char** end1, char** step1,
                 char **varName2, char** start2, char** end2, char** step2){
    char* _exp = removePreWhiteSpaces(exp);
    char* _varName1 = strsep(&_exp, ",");
    if(_varName1 == NULL || strlen(_varName1) == 0 || strlen(_varName1) == strlen(exp))
        return 0;
    *varName1 = _varName1;
    char* _varName2 = strsep(&_exp, " ");
    if(_varName2 == NULL || strlen(_varName2) == 0 || strlen(_varName2) == strlen(exp))
        return 0;
    *varName2 = _varName2;
    int i;
    for(i = 0; i < strlen(_exp); i++){
        if(_exp[i] == ' ')
            continue;
        if(_exp[i] == 'i') {
            if(i+1 < strlen(_exp) && _exp[i+1] == 'n') {
                if (i+2 < strlen(_exp) && _exp[i+2] == ' ')
                    break;
                else
                    return 0;
            }
            else
                return 0;
        }
    }
    char* range = subStr(_exp, i+3, (int)strlen(_exp) - i - 3);
    int hasChar = getFirstIndex(range, ':');
    if(hasChar == -1)
        return 0;
    char* _start1 = strsep(&range, ":");
    if(_start1 != NULL)
        *start1 = _start1;
    else
        return 0;
    hasChar = getFirstIndex(range, ':');
    if(hasChar == -1)
        return 0;
    char* _end1 = strsep(&range, ":");
    if(_end1 != NULL)
        *end1 = _end1;
    else
        return 0;
    hasChar = getFirstIndex(range, ',');
    if(hasChar == -1)
        return 0;
    char* _step1 = strsep(&range, ",");
    if(strlen(_step1) != 0)
        *step1 = _step1;
    else
        return 0;

    hasChar = getFirstIndex(range, ':');
    if(hasChar == -1)
        return 0;
    char* _start2 = strsep(&range, ":");
    if(_start2 != NULL)
        *start2 = _start2;
    else
        return 0;
    hasChar = getFirstIndex(range, ':');
    if(hasChar == -1)
        return 0;
    char* _end2 = strsep(&range, ":");
    if(_end2 != NULL)
        *end2 = _end2;
    else
        return 0;
    char* _step2 = (char *) malloc(sizeof (char ) * strlen(range));
    strcpy(_step2, range);
    if(strlen(_step2) != 0)
        *step2 = _step2;
    else
        return 0;
    return 1;
}

char* createStepExpression(char* varName, char* step){
    char* res = (char*) malloc(sizeof (char ) * MAX_LINE_LENGTH);
    int ind = 0;
    for(int i = 0; i < strlen(varName); i++){
        res[ind++] = varName[i];
    }
    res[ind++] = '=';
    for(int i = 0; i < strlen(varName); i++){
        res[ind++] = varName[i];
    }
    res[ind++] = '+';
    for(int i = 0; i < strlen(step); i++){
        res[ind++] = step[i];
    }
    res[ind] = '\0';
    return res;
}
char* createAssignmentExpression(char* varName, char* start){
    char* res = (char*) malloc(sizeof (char ) * MAX_LINE_LENGTH);
    int ind = 0;
    for(int i = 0; i < strlen(varName); i++){
        res[ind++] = varName[i];
    }
    res[ind++] = '=';
    for(int i = 0; i < strlen(start); i++){
        res[ind++] = start[i];
    }
    res[ind] = '\0';
    return res;
}

int processForLines(char* exp, int* errorLine) {
    char *varName1, * start1, * end1, * step1;
    char *varName2, * start2, * end2, * step2;
    if(hasOneForExp(exp, &varName1, &start1, &end1, &step1) ){
        struct variableType var = evaluateExpression(varName1);
        if(var.error || var.ind == -1 || var.dim1 != 1 || var.dim2 != 1)
            return 0;
        struct variableType startVar = evaluateExpression(start1);
        if(startVar.error || startVar.dim1 != 1 || startVar.dim2 != 1)
            return 0;
        struct variableType endVar = evaluateExpression(end1);
        if(endVar.error || endVar.dim1 != 1 || endVar.dim2 != 1)
            return 0;
        struct variableType stepVar = evaluateExpression(step1);
        if(stepVar.error || stepVar.dim1 != 1 || stepVar.dim2 != 1)
            return 0;
        char* l = (char *) malloc(sizeof (char ) * MAX_LINE_LENGTH);
        sprintf(l, "for(%s[0] = %s; %s[0] < %s, %s[0] += %s) {\n", var.cName, startVar.cName, var.cName, endVar.cName, var.cName, stepVar.cName);
        strcpy(C_CODE_ARRAY[C_CODE_COUNT++], l);
        free(l);
        double _times = (endVar.value[0] - startVar.value[0]) / stepVar.value[0];
        int times = (int ) _times + 1;
        char* startExp = createAssignmentExpression(varName1, start1);
        int x = processCodeLine(startExp,0);
        if(!x)
            return 0;

        for(int i = 0; i <= times; i++){
            var = evaluateExpression(varName1);
            if(var.error || var.ind == -1 || var.dim1 != 1 || var.dim2 != 1)
                return 0;
            if(var.value[0] > endVar.value[0])
                break;
            for(int j = 0; j < FOR_COUNT; j++){
                if(strlen((FOR_ARRAY[j])) == 0)
                    continue;
                int res = processCodeLine(FOR_ARRAY[j], i==0 && j==0);
                if(!res) {
                    *errorLine += j + 1;
                    return 0;
                }
            }
            char* stepExp = createStepExpression(varName1, step1);
            x = processCodeLine(stepExp, 0);
            if(!x)
                return 0;
        }
        strcpy(C_CODE_ARRAY[C_CODE_COUNT++], "}\n");
    }
    else if(hasTwoForExp(exp, &varName1, &start1, &end1, &step1,
                         &varName2, &start2, &end2, &step2)) {
        struct variableType var1 = evaluateExpression(varName1);
        if(var1.error || var1.ind == -1 || var1.dim1 != 1 || var1.dim2 != 1)
            return 0;
        struct variableType startVar1 = evaluateExpression(start1);
        if(startVar1.error || startVar1.dim1 != 1 || startVar1.dim2 != 1)
            return 0;
        struct variableType endVar1 = evaluateExpression(end1);
        if(endVar1.error || endVar1.dim1 != 1 || endVar1.dim2 != 1)
            return 0;
        struct variableType stepVar1 = evaluateExpression(step1);
        if(stepVar1.error || stepVar1.dim1 != 1 || stepVar1.dim2 != 1)
            return 0;

        struct variableType var2 = evaluateExpression(varName2);
        if(var2.error || var2.ind == -1 || var2.dim1 != 1 || var2.dim2 != 1)
            return 0;
        struct variableType startVar2 = evaluateExpression(start2);
        if(startVar2.error || startVar2.dim1 != 1 || startVar2.dim2 != 1)
            return 0;
        struct variableType endVar2 = evaluateExpression(end2);
        if(endVar2.error || endVar2.dim1 != 1 || endVar2.dim2 != 1)
            return 0;
        struct variableType stepVar2 = evaluateExpression(step2);
        if(stepVar2.error || stepVar2.dim1 != 1 || stepVar2.dim2 != 1)
            return 0;

        double _times1 = (endVar1.value[0] - startVar1.value[0]) / stepVar1.value[0];
        double _times2 = (endVar2.value[0] - startVar2.value[0]) / stepVar2.value[0];
        int times1 = (int ) _times1 + 1;
        int times2 = (int ) _times2 + 1;

        char* l = (char *) malloc(sizeof (char ) * MAX_LINE_LENGTH);
        sprintf(l, "for(%s[0] = %s; %s[0] < %s; %s[0] += %s) {\n", var1.cName, startVar1.cName, var1.cName, endVar1.cName, var1.cName, stepVar1.cName);
        strcpy(C_CODE_ARRAY[C_CODE_COUNT++], l);
        free(l);
        char* l2 = (char *) malloc(sizeof (char ) * MAX_LINE_LENGTH);
        sprintf(l2, "for(%s[0] = %s; %s[0] < %s; %s[0] += %s) {\n", var2.cName, startVar2.cName, var2.cName, endVar2.cName, var2.cName, stepVar2.cName);
        strcpy(C_CODE_ARRAY[C_CODE_COUNT++], l2);
        free(l2);
        char* startExp1 = createAssignmentExpression(varName1, start1);
        char* startExp2 = createAssignmentExpression(varName2, start2);
        int x1 = processCodeLine(startExp1, 0);
        if(!x1)
            return 0;
        int x2 = processCodeLine(startExp2, 0);
        if(!x2)
            return 0;
        char* stepExp1 = createStepExpression(varName1, step1);
        char* stepExp2 = createStepExpression(varName2, step2);
        for(int i = 0; i <= times1; i++){
            var1 = evaluateExpression(varName1);
            if(var1.error || var1.ind == -1 || var1.dim1 != 1 || var1.dim2 != 1)
                return 0;
            if(var1.value[0] > endVar1.value[0])
                break;
            x2 = processCodeLine(startExp2, 0);
            if(!x2)
                return 0;
            for(int j = 0; j <= times2; j++){
                var2 = evaluateExpression(varName2);
                if(var2.error || var2.ind == -1 || var2.dim1 != 1 || var2.dim2 != 1)
                    return 0;
                if(var2.value[0] > endVar2.value[0])
                    break;
                for(int k = 0; k < FOR_COUNT; k++){
                    if(strlen((FOR_ARRAY[k])) == 0)
                        continue;
                    int res = processCodeLine(FOR_ARRAY[k], i==0 && j== 0);
                    if(!res){
                        *errorLine += k + 1;
                        return 0;
                    }
                }
                x2 = processCodeLine(stepExp2,0);
                if(!x2)
                    return 0;
            }
            x1 = processCodeLine(stepExp1,0);
            if(!x1)
                return 0;
        }
        strcpy(C_CODE_ARRAY[C_CODE_COUNT++], "}\n");
        strcpy(C_CODE_ARRAY[C_CODE_COUNT++], "}\n");

    }
    else {
        return 0;
    }
    return 1;
}
void createErrorProgram(int line) {
    HAS_ERROR = 1;
    strcpy(C_ERROR_ARRAY[0], "#include <stdio.h>\n");
    strcpy(C_ERROR_ARRAY[1], "int main(){\n");
    char err[] = "printf(\"ERROR: Line      \");\n";
    strcpy(C_ERROR_ARRAY[2], err);
    strcpy(C_ERROR_ARRAY[3], "return 0;");
    strcpy(C_ERROR_ARRAY[4], "}");

}
int processCodeLines() {
    int i, res;
    int lineLength = sizeof (char) * MAX_LINE_LENGTH;

    for (i = 0; i < NUMBER_OF_LINES; i++) {
        char * reduced = handleComments(LINE_ARRAY[i]);
        if(reduced == NULL) {
            continue;
        }
        if(!checkParanthesis(reduced)){
            createErrorProgram(i+1);
            return 0;
        }
        int found = 0;
        char* forExp = (char *) malloc(lineLength);
        if(getForLoop(reduced, &forExp)){
            int errorLine = i;
            while (i<NUMBER_OF_LINES){
                i++;
                char* red = handleComments(LINE_ARRAY[i]);
                if(red == NULL) {
                    strcpy(FOR_ARRAY[FOR_COUNT++], "\0");
                    continue;
                }
                found = checkEndForLoop(red);
                if(found) {
                    int r = processForLines(forExp, &errorLine);
                    if(!r) {
                        printf("Error in code line %d", errorLine + 1);
                        return 0;
                    }
                    for(int y = 0; y < FOR_COUNT; y++){
                        strcpy(FOR_ARRAY[y], "\0");
                    }
                    FOR_COUNT = 0;
                    found = 0;
                    break;
                }
                else{
                    if(!checkParanthesis(red)){
                        printf("Error in code line %d", i+1);
                        return 0;
                    }
                    strcpy(FOR_ARRAY[FOR_COUNT++], red);
                }
            }
            continue;
        }
        res = processCodeLine(reduced, 1);
        if(!res) {
            printf("Error in code line %d", i+1);
            return 0;
        }
    }
    if(FOR_COUNT != 0) {
        printf("Your for loop has no ending char }");
        return 0;
    }
    return 1;
}


void prepareCFile() {
    strcpy(C_START_ARRAY[0], "#include <stdio.h>\n"
                             "#include <stdlib.h>\n"
                             "#include <string.h>\n"
                             "#include <limits.h>\n"
                             "#include <math.h>\n"
                             "#include \"t.h\"\n"
                             "#include <ctype.h>\n");
    strcpy(C_START_ARRAY[1], "double* func_sqrt(double* param, int dim) {\n"
                             "        double* res = (double*) calloc(dim, sizeof(double));\n"
                             "        for(int j = 0; j < dim; j++){\n"
                             "            res[j] = sqrt(param[j]);\n"
                             "        }\n"
                             "        return res;\n"
                             "}\n");
    strcpy(C_START_ARRAY[2], "double* func_choose(double* x1, double* x2, double* x3, double* x4) {\n"
                             "        if(x1[0]==0){\n"
                             "            return x2;\n"
                             "        }\n"
                             "        else if(x1[0] > 0){\n"
                             "            return x3;\n"
                             "        }\n"
                             "        else {\n"
                             "            return x4;\n"
                             "        }\n"
                             "}\n");
    strcpy(C_START_ARRAY[3], "double* func_tr(double* param, int dim1, int dim2) {\n"
                             "        int i, j;\n"
                             "        double* resArr = (double *) calloc(dim1*dim2, sizeof(double));\n"
                             "        for(i = 0; i < dim1; i++){\n"
                             "            for(j = 0; j < dim2; j++){\n"
                             "                resArr[i*dim2+j] = param[j*dim1+i];\n"
                             "            }\n"
                             "        }\n"
                             "        return resArr;\n"
                             "}\n");
    strcpy(C_START_ARRAY[4], "double* func_add(double* x1, double* x2, int d) {\n"
                             "    double* arr = (double *) calloc(d,sizeof (double ));\n"
                             "    for (int i = 0; i < d; i++) {\n"
                             "        arr[i] = x1[i] + x2[i];\n"
                             "    }\n"
                             "    return arr;\n"
                             "}\n");
    strcpy(C_START_ARRAY[5], "double* func_subtract(double* x1, double* x2, int d) {\n"
                             "    double* arr = (double *) calloc(d,sizeof (double ));\n"
                             "    for (int i = 0; i < d; i++) {\n"
                             "        arr[i] = x1[i] - x2[i];\n"
                             "    }\n"
                             "    return arr;\n"
                             "}\n");
    strcpy(C_START_ARRAY[6], "double* func_multiply(double* x1, double* x2, int d, int d2, int d3) {\n"
                             "    int i,j,k;\n"
                             "    double* arr = (double *) calloc(d,sizeof (double ));\n"
                             "    for(i = 0; i <d; i++){\n"
                             "          for(j = 0; j < d2; j++){\n"
                             "               for(k = 0; k < d3; k++){\n"
                             "                    arr[i*d2+j] += x1[i*d3+k] * x2[k*d2+j];\n"
                             "               }\n"
                             "          }\n"
                             "     }"
                             "    return arr;\n"
                             "}\n");

    strcpy(C_END_ARRAY[0], "return 0; \n}\n");

}

void prepareErrorCFile(){
    strcpy(C_START_ARRAY[0], "#include <stdio.h>\n"
                             "#include <stdlib.h>\n"
                             "#include <string.h>\n"
                             "#include <limits.h>\n"
                             "#include <math.h>\n"
                             "#include \"t.h\"\n"
                             "#include <ctype.h>");
    strcpy(C_END_ARRAY[0], "return 0; \n}\n");

}


int writeToErrorCFile() {
    FILE *file = fopen("output.c", "w");

    if (!file) {
        printf("File not found!");
        return 0;
    }
    fprintf(file, "%s", C_START_ARRAY[0]);
    for(int i = 0; i < 5;i++){
        fprintf(file, "%s", C_ERROR_ARRAY[i]);
    }

    fprintf(file, "%s", C_END_ARRAY[0]);
    return 1;
}

int writeToCFile() {
    FILE *file = fopen("output.c", "w");

    if (!file) {
        printf("File not found!");
        return 0;
    }
    for(int i = 0; i < 7; i++){
        fprintf(file, "%s", C_START_ARRAY[i]);
    }
    for(int i = 0; i < C_CODE_COUNT; i++){
        fprintf(file, "%s", C_CODE_ARRAY[i]);
    }
    fprintf(file, "%s", C_END_ARRAY[0]);
    fclose(file);
    return 1;
}

int main() {
    int hasReadFile = readFile();
    if (!hasReadFile) {
        return 1;
    }
    strcpy(C_CODE_ARRAY[C_CODE_COUNT++], "int main() {\n");
    int res = processCodeLines();
    if(res){
        for(int i = 0; i < PRINT_COUNT; i++)
            printf("%s\n", PRINT_ARRAY[i]);
        prepareCFile();
        int hasWrittenToFile = writeToCFile();
        if (!hasWrittenToFile) {
            return 1;
        }
    }
    else {
        prepareErrorCFile();
        int hasWrittenToFile = writeToErrorCFile();
        if (!hasWrittenToFile) {
            return 1;
        }
    }


    return 0;
}
