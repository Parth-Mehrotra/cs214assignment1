# Character Read Flow:

## First character is alphabetic (a-z)

*Word case*

## First character is a non-zero numeric character (1-9)

* Assume *Decimal case*.
	
## First character is zero

Maybe decimal, octal, float or hexadecimal.
*Zero case*

## First character is a special character

//TODO
*Special Character case*

# CASE LIST

## Word Case

* Upon reaching anything that's not alphanumeric, *END*.

## Decimal Case

* if reaches anything that's not numeric, *END*
* if reaches a period: 
	* If the character that follows the period is non-numeric, then it's a *END* + *Special Character 'period'*
	* Otherwise, it is *Float case*.

## Float Case

* 

## Zero Case

* Look at the character after 0.
	* if it's anything that's non-numeric, not the letter x, the number `0`, or not a period. Then it is the *Decimal `0`*
	* if it's a number less than 8, *Octal case*.
	* if it's the letter x, *X case*

## X Case

* If then the following character is 0-9 || a-f, then *Hex case*.
		* If it's alphabetic and NOT a-f then it's *Decimal '0'* + *Word case*.
		* If special character, *Decimal '0'* + *Word case 'x'* + *Special Character case*.
	
## Octal Case

* If anything non-numeric or greater than 7, *END*.

## Hex Case

* If anything non-alphanumeric or letters greater than f, *END*.
	
## Special Character Case

* 
