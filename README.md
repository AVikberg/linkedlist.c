# LinkedList.c

A simple linked list implementation written as a coding exercise in the C language.

`src/linkedlist.c` and its header file provide structs and an interface for
adding, removing, and getting elements to and from the list.

In this implementation the node value was chosen to just be a char pointer (string),
offering no benefit over a regular array of char array pointers.
In a practical setting, a value type of uncertain sizes (or even different types)
would be a better use case for a linked list.<br>
This is merely a proof of concept and therefore the char pointer suffices.

---

A sort of "frontend" for testing or interacting with a linked list is implemented in
the form of a REPL-like application (`main.c`) that runs in a terminal.

While running the application, enter "help" to get a list of available commands.
All values are regular C-strings of maximum length 255 characters.
