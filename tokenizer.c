/*
 * @author Wael Ayadi, Parth Mehrotra
 *
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

global char* inputString;

struct Token_ {
	char* token;
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
Token* newToken(char* input) {
	Token* token = (Token*) malloc(sizeof(Token));
	// TODO figure out type and all that
	return token;
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
	tokenizer -> inputString = inputString;
	// TODO do computation
	return return tokenizer;
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
 * Takes the first character of the input string and determines which of the initial cases it belongs to.
 * Then, it calls the corresponding function of that case.
 */
void spec(char a) {

	// For example, a == 'T'. Calls wordCase().
}

// List of case functions:
Token* wordCase(){
	
}

Token* decimalCase(){
	
}

Token* periodCase(){
	
}

Token* minusCase(){
	
}

Token* floatCase(){
	
}

Token* zeroCase(){
	
}

Token* xCase(){
	
}

Token* octalCase(){
	
}

Token* hexCase(){
	
}

void commentCase(){
	
}

Token* quoteCase(){
	
}

Token* specialCase(){
	
}

Token* badTokenCase(){
	
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
 */

int main(int argc, char **argv) {
	printf("%s\n", argv[1]);
	int len = strlen(argv[1]);
	printf("%d\n", len);
	return 0;
}
