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

char PRINT_ARRAY[MAX_NUMBER_OF_LINES][MAX_LINE_LENGTH];
int PRINT_COUNT = 0;

char FOR_ARRAY[MAX_NUMBER_OF_LINES][MAX_LINE_LENGTH];
int FOR_COUNT = 0;



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

int hasExpression(char *string, char *pattern, regmatch_t *pmatch, int len) {
    regex_t preg;
    regcomp(&preg, pattern, REG_EXTENDED);
    int res = regexec(&preg, string, len, pmatch, 0);
    regfree(&preg);
    return res == 0;
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
    for(int j = (int) strlen(res) - 1; j >= 0; j--){
        if(res[j] == ')'){
            found = 1;
            res[j] = '\0';
            break;
        }
        else if(res[j] == ' '){
            continue;
        }
        else {
            return 0;
        }
    }
    if(found){
        strcpy(*pmatch, res);
//        *pmatch = res;
        return 1;
    }
    else
        return 0;
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
    *pmatch = removePostWhiteSpaces(res);
    return 1;
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
    *varName = finalVar;
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
    *varName = finalVar;
    *index1 = subStr(string, startInd+1, len1);
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
    *ind1 = _d1;
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
    *ind1 = returnInd;
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
    *ind1 = arr;
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
    *exp = str;
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
    *exp = str;
    *ind1 = _ind1;
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
    *exp = str;
    *ind1 = _ind1;
    *ind2 = _ind2;
    *varName = _varName;
    return 1;
}


char* removeComments(char *_string) {
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





//void processCodeLines() {
//    int i;
//    for (i = 0; i < NUMBER_OF_LINES; i++) {
//        evaluateExpression(LINE_ARRAY[i]);
//    }
//}

int isOperator(char c){
//    if(c == '^'){
//        return 3;
//    }
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
                numOfSquare--;
            }
            else if(exp[i] == ']'){
                numOfSquare--;
            }
            currLen++;
        }
    }
    char* x = malloc(currLen+1);
    strncpy(x,&exp[strlen(exp)-currLen],currLen);
    x[currLen] = '\0';
    push(&stackNode, x);
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
            char* varName;
            if(hasParanthesisExpression(exp,&varName)) {
                struct StackNode* s = infixToPostFix(varName);
                struct StackNode* temp = NULL;
                while (!isEmpty(s)){
                    push(&temp, pop(&s));
                }
                while (!isEmpty(temp)){
                    push(&res, pop(&temp));
                }
                int r = 0;
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
            returnErrorVar();
        }
        int dim = res.dim1*res.dim2;
        for(int j = 0; j < dim; j++){
            res.value[j] = sqrt(res.value[j]);
        }
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
            returnErrorVar();
        }
        struct variableType x2 = evaluateExpression(exp2);
        if(x2.error){
            returnErrorVar();
        }
        struct variableType x3 = evaluateExpression(exp3);
        if(x3.error){
            returnErrorVar();
        }
        struct variableType x4 = evaluateExpression(exp4);
        if(x4.error){
            returnErrorVar();
        }
        if(x1.dim1 == x1.dim2 == x2.dim1 == x2.dim2 == x3.dim1 == x3.dim2 == x4.dim1 == x4.dim2 != 1){
            struct variableType x = {.name="", 0, 0, 0, 1, -1};
            return x;
        }
        if(x1.value==0){
            return x2;
        }
        else if(x1.value > 0){
            return x3;
        }
        else {
            return x4;
        }
    }
    else if(hasTransposeExpression(exp, &varName)){
        struct variableType res = evaluateExpression(varName);
        if(res.error){
            returnErrorVar();
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
            return res;
        }
        struct variableType x = {.name="", 0, 0, 0, 1,-1};
        return x;
    }
    else if(hasVectorPointExpression(exp,&varName, &ind1)) {
        struct variableType x = evaluateExpression(ind1);
        if(x.error){
            returnErrorVar();
        }
        if(x.dim1 != 1 && x.dim2 != 1){
            returnErrorVar();
        }
        double dimDouble = x.value[0];
        if(dimDouble <= 0 || !isInteger(dimDouble)) {
            returnErrorVar();
        }
        int dim1 = (int) dimDouble;
        struct variableType s = evaluateExpression(varName);
        if(s.error){
            returnErrorVar();
        }
        if(dim1 > s.dim1) {
            returnErrorVar();
        }
        double* ans  = (double *) malloc(sizeof (double ));
        ans[0] = s.value[dim1-1];
        struct variableType res = {.name="", 1, 1, ans, 0, s.ind};
        return res;
    }
    else if(hasMatrixPointExpression(exp, &varName, &ind1, &ind2)) {
        struct variableType x1 = evaluateExpression(ind1);
        if(x1.error){
            returnErrorVar();
        }
        if(x1.dim1 != 1 && x1.dim2 != 1){
            returnErrorVar();
        }
        struct variableType x2 = evaluateExpression(ind2);
        if(x2.error){
            returnErrorVar();
        }
        if(x2.dim1 != 1 && x2.dim2 != 1){
            returnErrorVar();
        }
        double dim1Double = atoi((const char *) x1.value);
        double dim2Double = atoi((const char *) x2.value);


        if(dim1Double <= 0 || dim2Double <= 0 || !isInteger(dim1Double) || !isInteger(dim2Double)) {
            returnErrorVar();
        }
        int dim1 = (int) dim1Double;
        int dim2 = (int) dim2Double;
        struct variableType s = evaluateExpression(varName);
        if(s.error){
            returnErrorVar();
        }
        if(dim1 > s.dim1 || dim2 > s.dim2) {
            returnErrorVar();
        }
        double* ans  = (double *) malloc(sizeof (double ));
        ans[0] = s.value[(dim1-1) * s.dim1 + dim2 - 1];
        struct variableType res = {.name="", 1, 1, ans, 0, s.ind};
        return res;
    }
    else if(hasDigitExpression(exp, &ind1)){
        double* arr = (double *) malloc(sizeof (double ) * 1);
        arr[0] = atoi(ind1);
        struct variableType s = {"", 1, 1, arr, 0, -1};
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
                    returnErrorVar();
                }
                struct variableType x1 = popVarStack(&evalStack);
                if(x1.error){
                    returnErrorVar();
                }
                if(*curr == '+'){
                    if(x1.dim1 != x2.dim1 || x1.dim2 != x2.dim2){
                        returnErrorVar();
                    }
                    else {
                        int d = x1.dim1 * x1.dim2;
                        double* arr = (double *) malloc(sizeof (double ) *d);
                        for (int i = 0; i < d; i++) {
                            arr[i] = x1.value[i] + x2.value[i];
                        }
                        struct variableType s = {"", x1.dim1, x1.dim2, arr, 0};
                        pushVarStack(&evalStack, s);
                    }
                }
                else if(*curr == '-'){
                    if(x1.dim1 != x2.dim1 || x1.dim2 != x2.dim2){
                        returnErrorVar();
                    }
                    else {
                        int d = x1.dim1 * x1.dim2;
                        double* arr = (double *) malloc(sizeof (double ) *d);
                        for (int i = 0; i < d; i++) {
                            arr[i] = x1.value[i] - x2.value[i];
                        }
                        struct variableType s = {"", x1.dim1, x1.dim2, arr, 0, -1};
                        pushVarStack(&evalStack, s);
                    }
                }
                else if(*curr == '*'){
                    if(x1.dim2 != x2.dim1){
                        returnErrorVar();
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
                        pushVarStack(&evalStack, s);
                    }
                }
            }
            else {
                struct variableType s = evaluateExpression(curr);
                pushVarStack(&evalStack, s);
            }
        }
        return popVarStack(&evalStack);
    }
}




int getLineType(char *line) {
    char* varName =  (char *) malloc(MAX_LINE_LENGTH * sizeof (char ));
    char* ind1 =  (char *) malloc(MAX_LINE_LENGTH * sizeof (char ));
    char* ind2 =  (char *) malloc(MAX_LINE_LENGTH * sizeof (char ));
    char* exp =  (char *) malloc(MAX_LINE_LENGTH * sizeof (char ));
    if (hasScalarDeclarationExpression(line, &varName)) {
        double* y = (double *) malloc(sizeof (double ) *1);
        y[0] = 0;
        struct variableArrayType a = {.name = varName, .dim1 =1, .dim2 = 1, .value = y};
        VARIABLES[variableNum] = a;
        variableNum++;
    }
    else if (hasVectorDeclarationExpression(line, &varName, &ind1)) {
        double dim1Double = atoi(ind1);
        if(!isInteger(dim1Double))
            return 0;
        int dim1 = (int) dim1Double;
        if(dim1 == 0) {
            return 0;
        }
        double* _y = (double *) malloc(sizeof (double ) * dim1);
        for(int i = 0; i < dim1; i++){
            _y[i] = 0;
        }
        double *y = copyValueArray(_y, dim1);

        struct variableArrayType b = {.name = varName, .dim1 =dim1, .dim2 = 1, .value =  y};
        VARIABLES[variableNum] = b;
        variableNum++;
    }
    else if (hasMatrixDeclarationExpression(line, &varName, &ind1, &ind2)) {
        double dim1Double = atoi(ind1);
        if(!isInteger(dim1Double))
            return 0;
        int dim1 = (int) dim1Double;

        double dim2Double = atoi(ind2);
        if(!isInteger(dim2Double))
            return 0;
        int dim2 = (int) dim2Double;

        if(dim1 == 0 || dim2 == 0){
            return 0;
        }
        double* y = (double *) malloc(sizeof (double ) *dim1*dim2);
        for(int i = 0; i < dim1*dim2; i++)
            y[i] = 0;
        struct variableArrayType c = { .name = varName, .dim1 = dim1, .dim2 = dim2, .value = y };
        VARIABLES[variableNum] = c;
        variableNum++;
    }
    else if(hasVectorDefinitionExpression(line, &varName,&ind1)) {
        struct variableType var = evaluateExpression(varName);
        if(var.error || var.ind == -1){
            return 0;
        }
        double arr[256];
        int dimVar = 0;
        char* curr;
        double currVal = 0;
        while( (curr = strsep(&ind1," ")) != NULL ) {
            if(strlen(curr) == 0){
                continue;
            }
            currVal = atoi(curr);
            arr[dimVar] = currVal;
            dimVar++;
        }
        if(var.dim1 * var.dim2 != dimVar)
            return 0;
        double* resArr = copyValueArray(arr, dimVar);
        struct variableArrayType finalRes = {var.name, var.dim1, var.dim2, resArr};
        VARIABLES[var.ind] = finalRes;
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
        double val = atoi(ind1);
        double* resArr = (double *) malloc(sizeof (double ) );
        resArr[0] = val;
        struct variableArrayType s = {var.name, var.dim1, var.dim2, copyValueArray(resArr, 1)};
        VARIABLES[var.ind] = s;
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

        double* valDouble = copyValueArray(val.value, originalDim);

        struct variableArrayType s = {var.name, var.dim1, var.dim2, valDouble};
        VARIABLES[var.ind] = s;
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
                sprintf(str, "%f", var.value[i]);
                strcpy(PRINT_ARRAY[PRINT_COUNT++], str);
            }
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
        else if(line[i] == '}')
            found = 1;
        else
            return 0;
    }
    return found;
}

int hasOneForExp(char* exp, char **varName, char** start, char** end, char** step){

    char* _exp = removePreWhiteSpaces(exp);
    char* _varName = strsep(&_exp, " ");
    if(_varName == NULL || strlen(_varName) == 0)
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

int processForLines(char* exp) {
    char *varName1, * start1, * end1, * step1;
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
        double _times = (endVar.value[0] - startVar.value[0]) / stepVar.value[0];
        int times = (int ) _times + 1;
        char* startExp = createAssignmentExpression(varName1, start1);
        int x = getLineType(startExp);
        if(!x)
            return 0;
        for(int i = 0; i <= times; i++){
            var = evaluateExpression(varName1);
            if(var.error || var.ind == -1 || var.dim1 != 1 || var.dim2 != 1)
                return 0;
            if(var.value[0] > endVar.value[0])
                break;
            for(int j = 0; j < FOR_COUNT; j++){
                int res = getLineType(FOR_ARRAY[j]);
                if(!res)
                    return 0;
            }
            char* stepExp = createStepExpression(varName1, step1);
            x = getLineType(stepExp);
            if(!x)
                return 0;
        }

    }
    return 1;
}

void processCodeLines() {
    int i, res;
    int lineLength = sizeof (char) * MAX_LINE_LENGTH;

    for (i = 0; i < NUMBER_OF_LINES; i++) {
        char * reduced = removeComments(LINE_ARRAY[i]);
        if(reduced == NULL) {
            continue;
        }
        int found = 0;
        char* forExp = (char *) malloc(lineLength);
        if(getForLoop(reduced, &forExp)){
            while (i<NUMBER_OF_LINES){
                i++;
                char* red = removeComments(LINE_ARRAY[i]);
                found = checkEndForLoop(red);
                if(found) {
                    int r = processForLines(forExp);
                    if(!r) {
                        printf("Error in code line %d", i+1);
                        return;
                    }
                    for(int y = 0; y < FOR_COUNT; y++){
                        strcpy(FOR_ARRAY[y], "\0");
                    }
                    FOR_COUNT = 0;
                    break;
                }
                else{
                    strcpy(FOR_ARRAY[FOR_COUNT++], red);
                }
            }
            continue;
        }
        res = getLineType(reduced);
        if(!res) {
            printf("Error in code line %d", i+1);
            return;
        }
    }

}




int main() {
    int hasReadFile = readFile();
    if (!hasReadFile) {
        return 1;
    }
//    struct StackNode* expStack = infixToPostFix("a*(b*c)");
    processCodeLines();
    return 0;
}
