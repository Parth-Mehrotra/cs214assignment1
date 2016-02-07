# Simple word case
```
-bash-4.1$ ./a.out "hello how are you"
Word "hello"
Word "how"
Word "are"
Word "you"
```

# Different delimeters
```
-bash-4.1$ ./a.out "hello#how?are%you"
Word "hello"
Hashtag "#"
Word "how"
Question Mark "?"
Word "are"
Percent "%"
Word "you"
```

# Number then word
```
-bash-4.1$ ./a.out "123hello"
Decimal Number "123"
Word "hello"
```

# Hex then word
```
-bash-4.1$ ./a.out "0x1342hello"
Hexadecimal Number "0x1342"
Word "hello"
```

# Octal then word
```
-bash-4.1$ ./a.out "0234asdflksjfd"
Octal Number "0234"
Word "asdflksjfd"
```

# Octal then decimal
```
-bash-4.1$ ./a.out "012899"
Octal Number "012"
Decimal Number "899"
```

# Words then float
```
-bash-4.1$ ./a.out "zero point five 0.5"
Word "zero"
Word "point"
Word "five"
float "0.5"
```

# C Keywords
```
-bash-4.1$ ./a.out "some C keywords char float"
Word "some"
Word "C"
Word "keywords"
C Keyword "char"
C Keyword "float"
```

# Ignoring block comments
```
-bash-4.1$ ./a.out "oh theres no comment here /*shhh Im invisible*/. None at all"
Word "oh"
Word "theres"
Word "no"
Word "comment"
Word "here"
Period "."
Word "None"
Word "at"
Word "all"
```

# Ignoring line comments
```
-bash-4.1$ ./a.out "some C keywords char//this should not show up\n float"
Word "some"
Word "C"
Word "keywords"
C Keyword "char"
C Keyword "float"
```

# Identifying strings with apostrophes
```
-bash-4.1$ ./a.out "this is a string with apostrophe: 'with apostrophes'"
Word "this"
Word "is"
Word "a"
Word "string"
Word "with"
Word "apostrophe"
Colon ":"
String "'with apostrophes'"
```

# Identifying strings with quotations
```
-bash-4.1$ ./a.out "this is a string with quotations: \"with quotations\""
Word "this"
Word "is"
Word "a"
Word "string"
Word "with"
Word "quotations"
Colon ":"
String ""with quotations""
```
