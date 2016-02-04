Code needs to detect comments and single & double quotes

# Character Read Flow:

## First character is alphabetic (a-z)

* *Word case*

## First character is a non-zero numeric character (1-9)

* Assume *Decimal case*.
	
## First character is zero

Maybe decimal, octal, float or hexadecimal.
* *Zero case*

## First character is a whitespace character

* Ignore this character. Skip to the next character.

## First character is a period

Maybe character period or start of a float.
* *Period Case*

## First character is a minus sign.

Maybe negative decimal, negative float, or Special character '-'
* *Minus case*

## First character is a special character (excluding '.' and '-')

* *Special Character case*

# CASE LIST

## Word Case

EX: "ABC123" is one word.
* Upon reaching anything that's not alphanumeric, *END*.

## Decimal Case

EX: "12354" is a decimal.
* if reaches anything that's not numeric, *END*
* if reaches a period: 
	* If the character that follows the period is non-numeric, then it's a *END* + *Special Character 'period'*  
	EX: "456." is *Decimal 456* + *Special Character '.'*
	* Otherwise, it is *Float case*.

## Period Case

NOTE: Period case is not a Token. It is either part of *Float case* or *Special Character case*  
EX: "." is period case. "-." is also period case.
* Check following character:
	* if it is a numeric, *Float case*
	* else, *Special character '.'* or *Special character '-'* + *Special character '.'*

## Minus Case

NOTE: Minus case is not a Token. It is either a *Decimal*, *Float*, or *Special Character*  
EX: "-" is minus case.
* Check the following character:
	* if it is numeric, *Decimal case*.
	* if it is a period, *Period case*.
	* if it is a '>', then *Special Character "Structure Pointer"*

## Float Case

EX: "3.14" is one float.
* Continue until reaching any non-numeric character or the letter "e".
	* if it is not the letter e, *END*
	* else, check the following character
		* if it is non-numeric or not '-', *END* + continue with character e.
		* if it is '-', check the next character
			* if it is not numeric, *END* + continue with character e.
			* else, continue until hitting something non-numeric
		* if it is numeric, continue until hitting something non-numeric.

## Zero Case

NOTE: Zero Case is not a Token.  
EX: "0" is zero case.
* Look at the character after 0.
	* if it's anything that's non-numeric, not the letter x, the number `0`, or not a period. Then it is the *Decimal `0`*  
	EX: "0ABC" is *Decimal 0* + *Word ABC*
	* if it's a number less than 8, *Octal case*.
	* if it's the letter x, *X case*
	* if it's a period, check the following character:
		* if it is numeric, *Float case*
		* else, *Decimal '0'* + *Special Character '.'*

## X Case

NOTE: X Case is not a Token.  
EX: "0x" is X case.
* If then the following character is 0-9 || a-f, then *Hex case*.
	* If it's alphabetic and NOT a-f then it's *Decimal '0'* + *Word case*.
	* If special character, *Decimal '0'* + *Word case 'x'* + *Special Character case*.
	
## Octal Case

EX: "0456" is Octal case.
* If anything non-numeric or greater than 7, *END*.

NOTE: There is no octal quantity "0" with this implementation. "00" = *Decimal '0'* + *Decimal '0'*

## Hex Case

EX: "0x45AF" is Hex case.
* If anything non-alphanumeric or letters greater than f, *END*.
	
## Special Character Case

List of Special characters:
* '(' Left Parenthesis
* ')' Right Parenthesis
* '[' Left Bracket
* ']' Right Bracket
* '.' Period or Structure Member
* '->' Structure Pointer
* '*' Asterisk or Multiply or D Reference
* '&' Ampersand or Address or Bitwise AND
* '-' Dash or Minus sign
* '!' Negate or Exclamation Point
* '~' One's Complement or Tilda
* '++' Increment
* '--' Decrement
* '/' Divide or Forward Slash
* '%' Modulus or Percent Symbol
* '+' Add
* '-' Subtract
* '>>' Shift right
* '<<' Shift left
* '<' Less than
* '>' Greater than
* '<=' Less or equal
* '>=' Greater or equal
* '=' Equal
* '==' Equals
* '!=' Not equals
* '^' Bitwise Exclusive OR
* '|' Bitwise OR
* '&&' Logical AND
* '||' Logical OR
* '?' Exclamation Point
* '+=" PlusEquals
* '-=' MinusEquals
* '*=' TimesEquals
* '/=' DivideEquals
* '%=' ModuloEquals
* '>>=' RightShiftEquals
* '<<=' LeftShiftEquals
* '&=' BinaryANDEquals
* '^=' BinaryExclusiveOREquals
* '|=' BinaryOREquals
* ',' Comma
* '`' Grave Accent
* '@' At
* '#' Hashtag or Pound
* '$' Dollar Sign
* '_' Underscore
* '{' Left Curly Bracket
* '}' Right Curly Bracket
* '\' Back Slash
* ':' Colon
* ';' Semicolon
* ''' Single Quotation Mark
* '"' Double Quotation Mark

## List of C Keywords
auto
break
case
char
const
continue
default
do
double
else
enum
extern
float
for
goto
if
int
long
register
return
short
signed
sizeof
static
struct
switch
typedef
union
unsigned
void
volatile
while
