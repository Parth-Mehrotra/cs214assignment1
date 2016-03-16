# A better malloc and free.

## Introduction

We created a malloc and free that detects common errors. 

We detect
+ When a program runs out of memory
+ Trying to free a pointer that wasn't returned by malloc
+ Double Free

## How memory is stored

We store a *mementry* for everything that's been malloc'd. Each *mementry* holds:
+ A confirmationCode - to check wether a certain block of memory has been initialized
+ A pointer to the block of memory
+ A boolean to indicate wether it's free or not
+ A pointer to the previous mementry
+ A pointer to the next mementry

## Mechanics of the malloc and free

Before we malloc, we check a few things. Is the space requested available? Is there a block of memory of this size available? Once we find a block of memory that's suitable we return that pointer.

When we free, we check if the pointer that we're freeing is valid, we check if it's been freed already. 

Once we free a pointer, we check to see that if there are any adjacent free blocks. If so we combine them.
