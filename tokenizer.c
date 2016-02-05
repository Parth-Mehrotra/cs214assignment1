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
Token* wordCase(char* currentString){
	int i = 0;
	while(isalnum(*(currentString + i)))
	{
		i++;
	}
	char* temp = (char*) calloc((i+1), sizeof(char));
	temp = strncpy(temp, currentString, i);
	return newToken(temp, "word");
}

Token* decimalCase(char* currentString){
	
}

Token* periodCase(char* currentString){
	
}

Token* minusCase(char* currentString){
	
}

Token* floatCase(char* currentString){
	
}

Token* zeroCase(char* currentString){
	
}

Token* xCase(char* currentString){
	
}

Token* octalCase(char* currentString){
	
}

Token* hexCase(char* currentString){
	
}

void commentCase(char* currentString){
	
}

Token* quoteCase(char* currentString){
	
}

Token* specialCase(char* currentString){
	
}

Token* badTokenCase(char* currentString){
	
}

/*
 * Takes the first character of the input string and determines which of the initial cases it belongs to.
 * Then, it calls the corresponding function of that case.
 */
Token* getInit(char* string) {

	// For example, a == 'T'. Calls wordCase().
	char a = *string;
	if(isalpha(a)) {
		return wordCase(string);
	}
	else if(a == '0') {
		return zeroCase(string);
	}
	else if(isdigit(a)) {
		return decimalCase(string);
	}
	else if(isspace(a)) {
		return NULL;
	}
	else if(a == '.') {
		return periodCase(string);
	}
	else if(a == '-') {
		return minusCase(string);
	}
	else {
		return specialCase(string);
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
