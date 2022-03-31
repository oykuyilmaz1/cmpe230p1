#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

int hasExpression(char* string, char* pattern, regmatch_t* pmatch, int len) {
    regex_t preg;
    regcomp(&preg, pattern, REG_EXTENDED);
    int res = regexec(&preg, string, len, pmatch, 0);
    regfree(&preg);
    return res == 0;
}

struct variableType {
    char *name;
    int dim1;
    int dim2;
    float** value;
};

struct variableType VARIABLES[20];

void getChooseParameters(char* line,char* exp1, char* exp2, char* exp3, char* exp4){
    int isParant = 0, isSquareBracket = 0, whichExp = 1;
    char* ptr = line;
    char* currExp = NULL;
    char* currPtr = currExp;
    for(; *ptr!='\0'; ptr++){
        if(*ptr == '('){
            isParant++;
            *currPtr = *ptr;
            currPtr++;
        }
        else if (*ptr == ')'){
            isParant--;
            *currPtr = *ptr;
            currPtr++;
        }
        else if(*ptr == '['){
            isSquareBracket++;
            *currPtr = *ptr;
            currPtr++;
        }
        else if (*ptr == ']'){
            isSquareBracket--;
            *currPtr = *ptr;
            currPtr++;
        }
        else if (*ptr == ','){
            if(isParant == 0 && isSquareBracket == 0){
                switch (whichExp) {
                    case 1:
                        exp1 = currExp;
                        currExp = NULL;
                        currPtr = currExp;
                        whichExp++;
                        break;
                    case 2:
                        exp2 = currExp;
                        currExp = NULL;
                        currPtr = currExp;
                        whichExp++;
                        break;
                    case 3:
                        exp3 = currExp;
                        currExp = NULL;
                        currPtr = currExp;
                        whichExp++;
                        break;
                    default:
                        exp4 = currExp;
                        currExp = NULL;
                        currPtr = currExp;
                        break;
                }
            }
        }
        else {
            *currPtr = *ptr;
            currPtr++;
        }
    }
}

char* subStr(char* string,int start, int finish){
    char* r = (char*) malloc(sizeof(char) * (finish-start+1));
    strncpy(r,&string[start],finish-start);
    r[finish] = '\0';
    return r;
}
//int main(){
//    char str[] = "This is test";
//    char *ptr;
//    double ret;
//
//    ret = strtod(str, &ptr);
//    int x = atoi(str);
//    printf("atoi %d\n", x);
//    printf("String part is |%s|", ptr);
//    return 0;
//
//
//}
#include <stdio.h>

#include <string.h>
int hasFuncExpression(char *string, char* pmatch, char* funcName){
    char var[256];
    int ind = 0;
    int isStarted = 0;
    int i;
    for(i = 0; i < strlen(string); i++){
        if(string[i] == ' ' && !isStarted)
            continue;
        if(string[i] == ' ' && isStarted)
            return 0;
        if(string[i] == '(')
            break;
        if(!isStarted)
            isStarted = 1;
        else
            var[ind++] = string[i];
    }
    var[ind] = '\0';
    int found = 0;
    if(strcmp(var, funcName))
        return 0;
    char res[256];
    ind = 0;
    for(int j = i+1; j < strlen(string); j++){
        res[ind++] = string[j];
    }
    for(int j = (int) strlen(string) - 1; j >= 0; j--){
        if(string[j] == ')'){
            found = 1;
            break;
        }
    }
    if(found){
        res[ind] = '\0';
        pmatch = res;
        return 1;
    }
    else
        return 0;
}

int main() {
//    char pmatch[256];
//    return hasFuncExpression("choose(o,y,k,u)",pmatch, "choose");
    double x = 5.6;
    int res = (x == (int) x);
    printf("%d", res);
    return 0;

}