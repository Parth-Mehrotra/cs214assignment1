# Word Cases

## First character is alphabetic (a-z)
Delimeters:

* Anything that's not alphanumeric

## First character is a non-zero numeric character (1-9)
Delimeters:

* Anything that's not numeric
* A period is a *special special* character - it may indicate a floating point number.
	* If the character that follows the period is non-numeric, then it's a decimal
	
## First character is zero
Maybe decimal, octal, float or hexadecimal.

Delimeters: 

* Depends on the character after 0.
	* if it's anything that's non-numeric, not the letter x, the number `0`, or not a period. Then it is the decimal number `0`
	* if it's a number less than 8, it's octal.
		* then anything non-numeric or greater than 7 ends the token.
	* if it's the letter x
		* and then the following character is 0-9 || a-f, then it's hexadecimal.
			* continue until, you reach a non-alphanumeric, or letters greater than f.
		* If it's alphabetic, *not* a-f then it's decimal 0 + the word case.
		* special character decimal 0 + word x + special character.