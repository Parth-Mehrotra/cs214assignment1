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
Token* wordCase(char* currentString, int startIndex){
	int i = startIndex;
	while(isalnum(*(currentString + i)))
	{
		i++;
	}
	char* temp = (char*) calloc((i+2), sizeof(char));
	temp = strncpy(temp, currentString, i + 1);
	return newToken(temp, "word");
}

Token* floatCase(char* currentString, int startIndex){
	int i = startIndex;
	while(isdigit(*(currentString + i)))
	{
		i++;
	}
	if(*(currentString + i) != '\0'){
		if(*(currentString + i) == 'e' || *(currentString + i) == 'E'){
			if(*(currentString + i + 1) != '\0'){
				if(isdigit(*(currentString + i + 1))){
					i = i + 1;
					while(isdigit(*(currentString + i))){
						i++;
					}
				}
				else if(*(currentString + i + 1) == '+' || *(currentString + i + 1) == '-'){
					if(*(currentString + i + 2) != '\0'){
						if(isdigit(*(currentString + i + 2))){
							i = i + 2;
							while(isdigit(*(currentString + i))){
								i++;
							}
						}

					}
				}
			}
		}
	}

	char* temp = (char*) calloc((i+2), sizeof(char));
	temp = strncpy(temp, currentString, i + 1);
	return newToken(temp, "float");
}

Token* specialCase(char* currentString, int startIndex){
	
}

Token* decimalCase(char* currentString, int startIndex){
	int i = startIndex;
	while(isdigit(*(currentString + i)))
	{
		i++;
	}
	if(*(currentString + i) == '.')	{
		if(*(currentString + i + 1) != '\0'){
			if(isdigit(*(currentString + i + 1))){
				return floatCase(currentString, i + 1);
			}
		}
	}
	char* temp = (char*) calloc((i+2), sizeof(char));
	temp = strncpy(temp, currentString, i + 1);
	return newToken(temp, "decimal");
}

Token* periodCase(char* currentString, int startIndex){
	int i = startIndex;
	if(*(currentString + i) == '.')	{
		if(*(currentString + i + 1) != '\0'){
			if(isdigit(*(currentString + i + 1))){
				return floatCase(currentString, i + 1);
			}
		}
		return specialCase(currentString, i);
	}
	else{ // if the first character is a dash
		if(*(currentString + i + 2) != '\0'){
			if(isdigit(*(currentString + i + 2))){
				return floatCase(currentString, i + 2);
			}
		}
		return specialCase(currentString, i);
	}
}

Token* minusCase(char* currentString, int startIndex){
	int i = startIndex;
	if(*(currentString + i + 1) != '\0'){
		if(isdigit(*(currentString + i + 1))){
			return decimalCase(currentString, i + 1);
		}
		if(*(currentString + i + 1) == '.'){
			return periodCase(currentString, i + 1);
		}
	}
	return specialCase(currentString, i);
}

Token* octalCase(char* currentString, int startIndex){
	int i = startIndex;
	while(isdigit(*(currentString + i)))
	{
		if(*(currentString + i) > '7')
			break;
		i++;
	}
	char* temp = (char*) calloc((i+2), sizeof(char));
	temp = strncpy(temp, currentString, i + 1);
	return newToken(temp, "octal");
}

Token* hexCase(char* currentString, int startIndex){
	int i = startIndex;
	while(isalnum(*(currentString + i)))
	{
		if(isalpha(*(currentString + i))){
			if(!((*(currentString + i) > 'A' && *(currentString + i) < 'F') || 
			(*(currentString + i) > 'a' && *(currentString + i) < 'f'))){
				break;
			}
		}
		i++;
	}
	char* temp = (char*) calloc((i+2), sizeof(char));
	temp = strncpy(temp, currentString, i + 1);
	return newToken(temp, "hex");
}

Token* xCase(char* currentString, int startIndex){
	int i = startIndex;
	if(*(currentString + i + 1) != '\0')	{
		if(isdigit(*(currentString + i + 1)) || (*(currentString + i + 1) > 'A' && *(currentString + i + 1) < 'F') 
			|| (*(currentString + i + 1) > 'a' && *(currentString + i + 1) < 'f')){
			return hexCase(currentString, i + 1);
		}
	}
	char* temp = (char*) calloc((i+1), sizeof(char));
	temp = strncpy(temp, currentString, i);
	return newToken(temp, "decimal");
}

Token* zeroCase(char* currentString, int startIndex){
	int i = startIndex;
	if(*(currentString + i + 1) != '\0')	{
		if(isdigit(*(currentString + i + 1))){
			if(*(currentString + i + 1) < '8' && *(currentString + i + 1) != '0'){
				return octalCase(currentString, i+1);
			}
		}
		else if(*(currentString + i + 1) == 'x' || *(currentString + i + 1) == 'X'){
			return xCase(currentString, i+1);
		}
		else if(*(currentString + i + 1) == '.'){
			if(*(currentString + i + 2) != '\0'){
				if(isdigit(*(currentString + i + 2))){
					return floatCase(currentString, i + 2);
				}
			}
		}
	}
	char* temp = (char*) calloc((i+2), sizeof(char));
	temp = strncpy(temp, currentString, i + 1);
	return newToken(temp, "decimal");

}

Token* commentCase(char* currentString, int startIndex){
	int i = startIndex;
	if(*(currentString + 1)	== '/'){
		while(1){
			if(*(currentString + i) == '\n'){
				break;
			}
			if(*(currentString + i) == '\\'){
				if(*(currentString + i + 1) != '\0'){
						if(*(currentString + i + 1) == 'n'){
							break;
						}
				}
			}
			i++;
		}
		char* temp = (char*) calloc((i+2), sizeof(char));
		temp = strncpy(temp, currentString, i + 1);
		return newToken(temp, "comment");
	}
	else{ // if the second character is *
		while(1){
			if(*(currentString + i) == '*'){
				if(*(currentString + i + 1) != '\0'){
						if(*(currentString + i + 1) == '/'){
							break;
						}
				}
			}
			i++;
		}
		char* temp = (char*) calloc((i+2), sizeof(char));
		temp = strncpy(temp, currentString, i + 1);
		return newToken(temp, "comment");

	}
}

Token* quoteCase(char* currentString, int startIndex){
	int i = startIndex;
	if(*(currentString) == '''){
		while(1){
			if(*(currentString + i) == '''){
				break;
			}
			i++;
		}
		char* temp = (char*) calloc((i+2), sizeof(char));
		temp = strncpy(temp, currentString, i + 1);
		return newToken(temp, "quote");
	}
	else{ // if the second character is "
		while(1){
			if(*(currentString + i) == '"'){
				break;
			}
			i++;
		}
		char* temp = (char*) calloc((i+2), sizeof(char));
		temp = strncpy(temp, currentString, i + 1);
		return newToken(temp, "quote");
	}
}

Token* badTokenCase(char* currentString, int startIndex){
	int i = startIndex;
	char* temp = (char*) calloc((i+2), sizeof(char));
	temp = strncpy(temp, currentString, i + 1);
	return newToken(temp, "bad token");

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
	else if(isspace(a)) {
		return NULL;
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

	while (isNext) {

		char* currentStr = (char*) calloc(strlen(inputString), sizeof(char));	
		strcpy(currentStr, inputString);

		Token* temp = getInit(currentStr);
		if(tokenizer->head == NULL) {
			tokenizer->head = temp;
		}
		tokenizer->current = temp;
		free(currentStr);
	}

	return tokenizer;
}

/*
 * TKDestroy destroys a TokenizerT object.	It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */
void TKDestroy( TokenizerT * tk ) {
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
	
	// Takes tk->current and prints it to outputStream

	return NULL;
}

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 * TODO: check argc
 */
int main(int argc, char **argv) {
	if(argc != 2){printf("bad input\n");}
	TokenizerT* tokenizer = TKCreate(argv[1]);
	
	
	return 0;
}
