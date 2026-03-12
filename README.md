> ⚠️ **Disclaimer:** This project is in development.
# Dynamic Memory Allocation
In this project I'm making malloc, calloc, realloc in c using sbrk.
This readme can be used as a some kind of totorial.

## Malloc
malloc is a function that dynamiclly allocate memory in the heap and return a pointer to the start.
I'll implement malloc using [sbrk](#sbrk).



### brk, sbrk
```
int brk(void *addr);
void *sbrk(intptr_t increment);
```

brk, sbrk are functions that change the process data segment size.
in the end these functions change the location of the **program break**,
this location if the first location after the end of data segment.
This implies that increasing the program break allocated memory and decreasing
the program break deallocate memory.


