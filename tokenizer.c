/*
 * @author Wael Ayadi, Parth Mehrotra
 *
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Token_ {
    char* string;
    char* type;
    struct Token_* next;
};
typedef struct Token_ Token;

struct TokenizerT_ {
    char* inputString;
    Token* head;
    Token* current;
};
typedef struct TokenizerT_ TokenizerT;

// Token Constuctor
Token* newToken(char* input, char* type) {
    Token* token = (Token*) malloc(sizeof(Token));
    token->string = input;
    token->type = type;
    token->next = NULL;
    return token;
}

// List of case functions:
Token* wordCase(char* currentString, int startIndex) {
    int i = startIndex;
    while(isalnum(*(currentString + i)))
    {
        i++;
    }
    char* temp = (char*) calloc((i+1), sizeof(char));
    temp = strncpy(temp, currentString, i);
    if(strcmp(temp, "auto") == 0 || strcmp(temp, "break") == 0 || strcmp(temp, "case") == 0 || strcmp(temp, "char") == 0 ||
    strcmp(temp, "const") == 0 || strcmp(temp, "continue") == 0 || strcmp(temp, "default") == 0 || strcmp(temp, "do") == 0 ||
    strcmp(temp, "double") == 0 || strcmp(temp, "else") == 0 || strcmp(temp, "enum") == 0 || strcmp(temp, "extern") == 0 ||
    strcmp(temp, "float") == 0 || strcmp(temp, "for") == 0 || strcmp(temp, "goto") == 0 || strcmp(temp, "if") == 0 ||
    strcmp(temp, "int") == 0 || strcmp(temp, "long") == 0 || strcmp(temp, "register") == 0 || strcmp(temp, "return") == 0 ||
    strcmp(temp, "short") == 0 || strcmp(temp, "signed") == 0 || strcmp(temp, "sizeof") == 0 || strcmp(temp, "static") == 0 ||
    strcmp(temp, "struct") == 0 || strcmp(temp, "switch") == 0 || strcmp(temp, "typedef") == 0 || strcmp(temp, "union") == 0 ||
    strcmp(temp, "unsigned") == 0 || strcmp(temp, "void") == 0 || strcmp(temp, "volatile") == 0 || strcmp(temp, "while") == 0){
	return newToken(temp, "C Keyword");
    }

    return newToken(temp, "Word");
}

Token* floatCase(char* currentString, int startIndex) {
    int i = startIndex;
    while(isdigit(*(currentString + i))) {
        i++;
    }
    if(*(currentString + i) != '\0') {
        if(*(currentString + i) == 'e' || *(currentString + i) == 'E') {
            if(*(currentString + i + 1) != '\0') {
                if(isdigit(*(currentString + i + 1))) {
                    i = i + 1;
                    while(isdigit(*(currentString + i))) {
                        i++;
                    }
                }
                else if(*(currentString + i + 1) == '+' || *(currentString + i + 1) == '-') {
                    if(*(currentString + i + 2) != '\0') {
                        if(isdigit(*(currentString + i + 2))) {
                            i = i + 2;
                            while(isdigit(*(currentString + i))) {
                                i++;
                            }
                        }

                    }
                }
            }
        }
    }

    char* temp = (char*) calloc((i+1), sizeof(char));
    temp = strncpy(temp, currentString, i);
    return newToken(temp, "float");
}

Token* commentCase(char* currentString, int startIndex) {
    int i = startIndex;
    if(*(currentString + 1) == '/') {
        while(1) {
            if(*(currentString + i) == '\0') {
                break;
            }
            if(*(currentString + i) == '\n') {
		i++;
                break;
            }
            if(*(currentString + i) == '\\') {
                if(*(currentString + i + 1) != '\0') {
                    if(*(currentString + i + 1) == 'n') {
			i = i + 2;
                        break;
                    }
                }
            }
            i++;
        }
        char* temp = (char*) calloc((i+1), sizeof(char));
        temp = strncpy(temp, currentString, i);
        return newToken(temp, "Comment");
    }
    else { // if the second character is *
        while(1) {
	    if(*(currentString + i) == '\0'){
		break;
	    }
            if(*(currentString + i) == '*') {
                if(*(currentString + i + 1) != '\0') {
                    if(*(currentString + i + 1) == '/') {
			i = i + 2;
                        break;
                    }
                }
            }
            i++;
        }
        char* temp = (char*) calloc((i+1), sizeof(char));
        temp = strncpy(temp, currentString, i);
        return newToken(temp, "Comment");
    }
}

Token* quoteCase(char* currentString, int startIndex) {
    int i = startIndex;
    if(*(currentString) == '\''){
    	while(1){
    		if(*(currentString + i) == '\0'){
   			break;
    		}
    		if(*(currentString + i) == '\''){
			i++;
   			break;
    		}
    		i++;
    	}
    	char* temp = (char*) calloc((i+1), sizeof(char));
    	temp = strncpy(temp, currentString, i);
    	return newToken(temp, "String");
    }
    else{ // if the second character is "
    	while(1){
    		if(*(currentString + i) == '\0'){
   			break;
    		}
    		if(*(currentString + i) == '\"'){
			i++;
    			break;
    		}
    		i++;
    	}
    	char* temp = (char*) calloc((i+1), sizeof(char));
   	temp = strncpy(temp, currentString, i);
    	return newToken(temp, "String");
    }
    return NULL;
}


Token* specialCase(char* currentString, int startIndex) {
    int i = startIndex;
    char a = *(currentString + i);
    char typeString[30];
    int j = 0;
    for(j = 0; j < 30; j++) {
        typeString[j] = '\0';
    }
    int numChars = 1;
    while(1) {
        if(isspace(a)) {
            strcpy(typeString, "Whitespace");
            numChars = 1;
            break;
        }
        else if(a == '\\') {
            if(*(currentString + i + 1) != '\0') {
                if(*(currentString + i + 1) == '0' || *(currentString + i + 1) == 't' ||
                        *(currentString + i + 1) == 'n' || *(currentString + i + 1) == 'v' ||
                        *(currentString + i + 1) == 'f' || *(currentString + i + 1) == 'r') {
                    strcpy(typeString, "Whitespace");
                    numChars = 2;
                    break;
                }
            }
            strcpy(typeString, "Backslash");
            numChars = 1;
            break;
        }
        else if(a == '(') {
            strcpy(typeString, "Left Parenthesis");
            numChars = 1;
            break;
        }
        else if(a == ')') {
            strcpy(typeString, "Right Parenthesis");
            numChars = 1;
            break;
        }
        else if(a == '[') {
            strcpy(typeString, "Left Bracket");
            numChars = 1;
            break;
        }
        else if(a == ']') {
            strcpy(typeString, "Right Bracket");
            numChars = 1;
            break;
        }
        else if(a == '.') {
            strcpy(typeString, "Period");
            numChars = 1;
            break;
        }
        else if(a == '*') {
            if(*(currentString + i + 1) != '\0') {
                if(*(currentString + i + 1) == '=') {
                    strcpy(typeString, "TimesEquals");
                    numChars = 2;
                    break;
                }
            }
            strcpy(typeString, "Asterisk");
            numChars = 1;
            break;
        }
        else if(a == '&') {
            if(*(currentString + i + 1) != '\0') {
                if(*(currentString + i + 1) == '&') {
                    strcpy(typeString, "Logical AND");
                    numChars = 2;
                    break;
                }
                if(*(currentString + i + 1) == '=') {
                    strcpy(typeString, "BinaryANDEquals");
                    numChars = 2;
                    break;
                }
            }
            strcpy(typeString, "Ampersand");
            numChars = 1;
            break;
        }
        else if(a == '!') {
            if(*(currentString + i + 1) != '\0') {
                if(*(currentString + i + 1) == '=') {
                    strcpy(typeString, "Not Equal");
                    numChars = 2;
                    break;
                }
            }
            strcpy(typeString, "Exclamation Point");
            numChars = 1;
            break;
        }
        else if(a == '~') {
            strcpy(typeString, "Tilda");
            numChars = 1;
            break;
        }
        else if(a == '+') {
            if(*(currentString + i + 1) != '\0') {
                if(*(currentString + i + 1) == '+') {
                    strcpy(typeString, "Increment");
                    numChars = 2;
                    break;
                }
                if(*(currentString + i + 1) == '=') {
                    strcpy(typeString, "PlusEquals");
                    numChars = 2;
                    break;
                }
            }
            strcpy(typeString, "Plus");
            numChars = 1;
            break;
        }
        else if(a == '-') {
            if(*(currentString + i + 1) != '\0') {
                if(*(currentString + i + 1) == '-') {
                    strcpy(typeString, "Decrement");
                    numChars = 2;
                    break;
                }
                if(*(currentString + i + 1) == '=') {
                    strcpy(typeString, "MinusEquals");
                    numChars = 2;
                    break;
                }
            }
            strcpy(typeString, "Dash");
            numChars = 1;
            break;
        }
        else if(a == '%') {
            if(*(currentString + i + 1) != '\0') {
                if(*(currentString + i + 1) == '=') {
                    strcpy(typeString, "ModuloEquals");
                    numChars = 2;
                    break;
                }
            }
            strcpy(typeString, "Percent");
            numChars = 1;
            break;
        }
        else if(a == '>') {
            if(*(currentString + i + 1) != '\0') {
                if(*(currentString + i + 1) == '>') {
                    if(*(currentString + i + 2) != '\0') {
                        if(*(currentString + i + 2) == '=') {
                            strcpy(typeString, "RightShiftEquals");
                            numChars = 3;
                            break;
                        }
                    }
                    strcpy(typeString, "Shift Right");
                    numChars = 2;
                    break;
                }
                if(*(currentString + i + 1) == '=') {
                    strcpy(typeString, "Greater or Equals");
                    numChars = 2;
                    break;
                }
            }
            strcpy(typeString, "Greater Than");
            numChars = 1;
            break;
        }
        else if(a == '<') {
            if(*(currentString + i + 1) != '\0') {
                if(*(currentString + i + 1) == '<') {
                    if(*(currentString + i + 2) != '\0') {
                        if(*(currentString + i + 2) == '=') {
                            strcpy(typeString, "LeftShiftEquals");
                            numChars = 3;
                            break;
                        }
                    }
                    strcpy(typeString, "Shift Left");
                    numChars = 2;
                    break;
                }
                if(*(currentString + i + 1) == '=') {
                    strcpy(typeString, "Less than or Equals");
                    numChars = 2;
                    break;
                }
            }
            strcpy(typeString, "Less Than");
            numChars = 1;
            break;
        }
        else if(a == '=') {
            if(*(currentString + i + 1) != '\0') {
                if(*(currentString + i + 1) == '=') {
                    strcpy(typeString, "Equals");
                    numChars = 2;
                    break;
                }
            }
            strcpy(typeString, "Equal");
            numChars = 1;
            break;
        }
        else if(a == '^') {
            if(*(currentString + i + 1) != '\0') {
                if(*(currentString + i + 1) == '=') {
                    strcpy(typeString, "BinaryExclusiveOREquals");
                    numChars = 2;
                    break;
                }
            }
            strcpy(typeString, "Caret");
            numChars = 1;
            break;
        }
        else if(a == '|') {
            if(*(currentString + i + 1) != '\0') {
                if(*(currentString + i + 1) == '|') {
                    strcpy(typeString, "Logical OR");
                    numChars = 2;
                    break;
                }
                if(*(currentString + i + 1) == '=') {
                    strcpy(typeString, "BinaryOREquals");
                    numChars = 2;
                    break;
                }
            }
            strcpy(typeString, "Vertical Bar");
            numChars = 1;
            break;
        }
        else if(a == '?') {
            strcpy(typeString, "Question Mark");
            numChars = 1;
            break;
        }
        else if(a == ',') {
            strcpy(typeString, "Comma");
            numChars = 1;
            break;
        }
        else if(a == '`') {
            strcpy(typeString, "Grave Accent");
            numChars = 1;
            break;
        }
        else if(a == '@') {
            strcpy(typeString, "At Sign");
            numChars = 1;
            break;
        }
        else if(a == '#') {
            strcpy(typeString, "Hashtag");
            numChars = 1;
            break;
        }
        else if(a == '$') {
            strcpy(typeString, "Dollar Sign");
            numChars = 1;
            break;
        }
        else if(a == '_') {
            strcpy(typeString, "Underscore");
            numChars = 1;
            break;
        }
        else if(a == '{') {
            strcpy(typeString, "Left Curly Bracket");
            numChars = 1;
            break;
        }
        else if(a == '}') {
            strcpy(typeString, "Right Curly Bracket");
            numChars = 1;
            break;
        }
        else if(a == '/') {
            if(*(currentString + i + 1) != '\0') {
                if(*(currentString + i + 1) == '/') {
			return commentCase(currentString, i + 1);
                }
		if(*(currentString + i + 1) == '*') {
			return commentCase(currentString, i + 1);
                }
		if(*(currentString + i + 1) == '=') {
                    strcpy(typeString, "DivideEquals");
                    numChars = 2;
                    break;
                }
            }

            strcpy(typeString, "Forward Slash");
            numChars = 1;
            break;
        }
        else if(a == ':') {
            strcpy(typeString, "Colon");
            numChars = 1;
            break;
        }
        else if(a == ';') {
            strcpy(typeString, "Semicolon");
            numChars = 1;
            break;
        }
        else if(a == '\'') {
	    return quoteCase(currentString, i+1);
        }
        else if(a == '\"') {
	    return quoteCase(currentString, i+1);
        }
        else {
            strcpy(typeString, "Bad Token");
            numChars = 1;
            break;
        }
    }
    char* temp = (char*) calloc(numChars + 1, sizeof(char));
    temp = strncpy(temp, currentString, numChars);
    return newToken(temp, typeString);
}

Token* decimalCase(char* currentString, int startIndex) {
    int i = startIndex;
    while(isdigit(*(currentString + i)))
    {
        i++;
    }
    if(*(currentString + i) == '.')	{
        if(*(currentString + i + 1) != '\0') {
            if(isdigit(*(currentString + i + 1))) {
                return floatCase(currentString, i + 1);
            }
        }
    }
    char* temp = (char*) calloc((i+1), sizeof(char));
    temp = strncpy(temp, currentString, i);
    return newToken(temp, "Decimal Number");
}

Token* periodCase(char* currentString, int startIndex) {
    int i = startIndex;
    if(*(currentString + i) == '.')	{
        if(*(currentString + i + 1) != '\0') {
            if(isdigit(*(currentString + i + 1))) {
                return floatCase(currentString, i + 1);
            }
        }
        return specialCase(currentString, i);
    }
    else { // if the first character is a dash
        if(*(currentString + i + 2) != '\0') {
            if(isdigit(*(currentString + i + 2))) {
                return floatCase(currentString, i + 2);
            }
        }
        return specialCase(currentString, i);
    }
}

Token* minusCase(char* currentString, int startIndex) {
    int i = startIndex;
    if(*(currentString + i + 1) != '\0') {
        if(isdigit(*(currentString + i + 1))) {
            return decimalCase(currentString, i + 1);
        }
        if(*(currentString + i + 1) == '.') {
            return periodCase(currentString, i + 1);
        }
    }
    return specialCase(currentString, i);
}

Token* octalCase(char* currentString, int startIndex) {
    int i = startIndex;
    while(isdigit(*(currentString + i)))
    {
        if(*(currentString + i) > '7')
            break;
        i++;
    }
    char* temp = (char*) calloc((i+1), sizeof(char));
    temp = strncpy(temp, currentString, i );
    return newToken(temp, "Octal Number");
}

Token* hexCase(char* currentString, int startIndex) {
    int i = startIndex;
    while(isalnum(*(currentString + i)))
    {
        if(isalpha(*(currentString + i))) {
            if(!((*(currentString + i) >= 'A' && *(currentString + i) <= 'F') ||
                    (*(currentString + i) >= 'a' && *(currentString + i) <= 'f'))) {
                break;
            }
        }
        i++;
    }
    char* temp = (char*) calloc((i+1), sizeof(char));
    temp = strncpy(temp, currentString, i);
    return newToken(temp, "Hexadecimal Number");
}

Token* xCase(char* currentString, int startIndex) {
    int i = startIndex;
    if(*(currentString + i + 1) != '\0')	{
        if(isdigit(*(currentString + i + 1)) || (*(currentString + i + 1) >= 'A' && *(currentString + i + 1) <= 'F')
                || (*(currentString + i + 1) >= 'a' && *(currentString + i + 1) <= 'f')) {
            return hexCase(currentString, i + 1);
        }
    }
    char* temp = (char*) calloc((i+1), sizeof(char));
    temp = strncpy(temp, currentString, i);
    return newToken(temp, "Decimal Number");
}

Token* zeroCase(char* currentString, int startIndex) {
    int i = startIndex;
    if(*(currentString + i + 1) != '\0')	{
        if(isdigit(*(currentString + i + 1))) {
            if(*(currentString + i + 1) < '8' && *(currentString + i + 1) != '0') {
                return octalCase(currentString, i+1);
            }
        }
        else if(*(currentString + i + 1) == 'x' || *(currentString + i + 1) == 'X') {
            return xCase(currentString, i+1);
        }
        else if(*(currentString + i + 1) == '.') {
            if(*(currentString + i + 2) != '\0') {
                if(isdigit(*(currentString + i + 2))) {
                    return floatCase(currentString, i + 2);
                }
            }
        }
    }
    char* temp = (char*) calloc((i+2), sizeof(char));
    temp = strncpy(temp, currentString, i + 1);
    return newToken(temp, "Decimal Number");

}


/*
 * Takes the first character of the input string and determines which of the initial cases it belongs to.
 * Then, it calls the corresponding function of that case.
 */
Token* getInit(char* string) {

    // For example, a == 'T'. Calls wordCase().
    char a = *string;
    if(isalpha(a)) {
        return wordCase(string, 0);
    }
    else if(a == '0') {
        return zeroCase(string, 0);
    }
    else if(isdigit(a)) {
        return decimalCase(string, 0);
    }
    else if(a == '.') {
        return periodCase(string, 0);
    }
    else if(a == '-') {
        return minusCase(string, 0);
    }
    else {
        return specialCase(string, 0);
    }
}

void printToken(Token* token) {
    if(strcmp(token->type, "Whitespace") != 0 && strcmp(token->type, "Comment") != 0) 
	    printf("%s \"%s\"\n", token->type, token->string);
}

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 *
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.	(In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */
TokenizerT* TKCreate(char* inputString) {
    TokenizerT* tokenizer = (TokenizerT*) malloc(sizeof(TokenizerT));
    tokenizer->inputString = inputString;
    tokenizer->head = NULL;
    tokenizer->current = NULL;

    int isNext = 1;
    int strOffset = 0;

    while (strOffset < strlen(inputString)) {

        char* currentStr = (char*) calloc(strlen(inputString) - strOffset, sizeof(char));
        strcpy(currentStr, &inputString[strOffset]);

        Token* temp = getInit(currentStr);
        printToken(temp);
        strOffset += strlen(temp -> string);

        if(tokenizer->head == NULL) {
            tokenizer -> head = temp;
            tokenizer -> current = temp;
        } else {
            tokenizer -> current -> next = temp;
            tokenizer -> current = temp;
        }
        free(currentStr);
    }
	tokenizer -> current = tokenizer -> head;

    return tokenizer;
}

/*
 * TKDestroy destroys a TokenizerT object.	It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */
void TKDestroy( TokenizerT * tk ) {
    while (tk -> head -> next != NULL) {
        Token* temp = tk -> head;
        while (temp -> next -> next != NULL) {
            temp = temp -> next;
        }
        free(temp -> next);
		temp -> next = NULL;
    }
	free(tk->head);
    free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.	Space for the returned token should be dynamically
 * allocated.	The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.	Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */
char *TKGetNextToken( TokenizerT * tk ) {

    char* temp = tk -> current -> string;
    if (tk -> current -> next != NULL) {
        tk -> current = tk -> current -> next;
    } else {
        tk -> current = tk -> head;
    }

    return temp;
}

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 * TODO: check argc
 */
int main(int argc, char **argv) {
    if(argc != 2 || strlen(argv[1]) < 1) {
        printf("bad input\n");
        return 1;
    }

    TokenizerT* tokenizer = TKCreate(argv[1]);
	TKDestroy(tokenizer);

    return 0;
}
