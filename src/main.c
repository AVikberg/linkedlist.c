#define __STDC_WANT_LIB_EXT2__ 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

#define S_RST "\033[0m"         // Reset
#define S_BLD "\033[0;1m"       // Bold
#define S_DIM "\033[0;2m"       // Dim
#define S_ITA "\033[0;3m"       // Italics
#define S_ERR "\033[0;1;31m"    // Bold + Bright red
#define S_WRN "\033[0;2;33m"    // Dim + Bright yellow
#define S_NFO "\033[0;36m"      // Cyan


#ifdef _WIN32
#include <wchar.h>
#include <windows.h>
#endif

// Non-zero for failure. Assumes enabled for non-Windows
int virtualTerminalEnabled()
{
#ifdef _WIN32
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
        return GetLastError();

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
        return GetLastError();

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
        return GetLastError();

    return 0;
#else
    return 0;
#endif
}


void removeTrailingLinebreak(char* str)
{
    if (str[strlen(str) - 1] == '\n')
        str[strlen(str) - 1] = '\0';
}


void handleArgs(int argc, char** argv, struct LinkedList* list)
{
    for (int i = 1; i < argc; i++)
    {
        if (strncmp(argv[i], "--help", 6) == 0 || strncmp(argv[i], "-h", 2) == 0 || strncmp(argv[i], "-?", 2) == 0)
        {
            printf(
                "Linked List Interactive.c\n"
                "Usage: linked [initial node values...]\n"
                "Options\n"
                "--version  -v     Display application version\n"
                "--help     -h     Show this message\n"
            );
            exit(0);
        }

        if (strncmp(argv[i], "--version", 9) == 0 || strncmp(argv[i], "-v", 2) == 0)
        {
            printf("Linked List Interactive.c v1.0.0\n");
            exit(0);
        }
        
        if (strncmp(argv[i], "-", 1) == 0)
        {
            fprintf(stderr, S_ERR "Unknown argument \"%s\"\n" S_RST, argv[i]);
            exit(1);
        }

        createNode(list, argv[i]);
    }
}

void printHelp()
{
    printf(
        S_BLD "Linked List Interactive.c\n\n" S_RST
        S_ITA "Commands:\n" S_RST
        S_BLD "stop|exit|q" S_RST "              Stops the application\n"
        S_BLD "help|?" S_RST "                   Display this message\n"
        S_BLD "print" S_RST "                    Print the entire list\n"
        S_BLD "get " S_DIM "<index>" S_RST "              Prints the value of the node at index\n"
        S_BLD "set " S_DIM "<index> <value>" S_RST "      Sets the value of the node at the index\n"
        S_BLD "add " S_DIM "<value>" S_RST "              Adds a node with the specified value at the end\n"
        S_BLD "add first " S_DIM "<value>" S_RST "        Adds a node with the value at the beginning\n"
        S_BLD "add at " S_DIM "<index> <value>" S_RST "   Adds a node with the value at the index\n"
        S_BLD "remove " S_DIM "<index>" S_RST "           Removes the node at the index\n"
        S_BLD "remove all" S_RST "               Remove all nodes\n" S_RST
    );
}


int main(int argc, char** argv)
{
    if (virtualTerminalEnabled())
    {
        printf("Could not enable virtual terminal\n");
        return 1;
    }

    struct LinkedList list = { NULL };
    handleArgs(argc, argv, &list);

    int index;
    char lineBuffer[MAX_NODEVALUE_LENGTH], command[MAX_NODEVALUE_LENGTH], value[MAX_NODEVALUE_LENGTH];
    int read = 0;
    
    while (1)
    {
        printf("> ");
        
        size_t offset = 0;
        if (!fgets(lineBuffer, sizeof(lineBuffer), stdin))
        {
            fprintf(stderr, S_ERR "Could not get input\n" S_RST);
            continue;
        }

        if (sscanf(lineBuffer, "%255s %n", command, &read) < 1)
            continue;
        
        if (strcmp(command, "stop") == 0 || strcmp(command, "exit") == 0 || strcmp(command, "q") == 0)
        {
            removeAll(&list);
            break;
        }

        if (strcmp(command, "help") == 0 || strcmp(command, "?") == 0)
        {
            printf("\033[1A\033[2K");
            printHelp();
            continue;
        }

        if (strcmp(command, "print") == 0)
        {
            struct Node* current = list.first;
            if (!current)
            {
                printf(S_WRN "List is empty\n" S_RST);
                continue;
            }

            printf(S_NFO);
            while (current)
            {
                printf("%s\n", current->value);
                current = current->next;
            }
            printf(S_RST);
            continue;
        }

        
        int endOfLine = (read + 1) >= strlen(lineBuffer);
        offset += read;

        if (strcmp(command, "get") == 0)
        {
            if (endOfLine)
            {
                fprintf(stderr, S_ERR "Missing index argument for get command\n" S_RST);
                continue;
            }

            if (sscanf(lineBuffer + offset, " %d%n", &index, &read) < 1)
            {
                fprintf(stderr, S_ERR "Invalid index argument for get command\n" S_RST);
                continue;
            }
            struct Node* node = getNode(&list, index);
            if (node)
                printf(S_NFO "%s\n" S_RST, node->value);
            else
                fprintf(stderr, S_ERR "No node at index %i\n" S_RST, index);
            
            continue;
        }

        if (strcmp(command, "set") == 0)
        {
            if (endOfLine)
            {
                fprintf(stderr, S_ERR "Missing arguments for set command\n" S_RST);
                continue;
            }

            if (sscanf(lineBuffer + offset, " %d %n", &index, &read) < 1)
            {
                fprintf(stderr, S_ERR "Invalid index argument for set command\n" S_RST);
                continue;
            }

            offset += read;
            endOfLine = (offset + 1) >= strlen(lineBuffer);
            
            if (endOfLine)
            {
                fprintf(stderr, S_ERR "Missing value argument for set command\n" S_RST);
                continue;
            }

            // Check if value exists, but read rest of line buffer instead (preserves spaces)
            if (sscanf(lineBuffer + offset, " %255s", value) < 1)
            {
                fprintf(stderr, S_ERR "Invalid value argument for set command\n" S_RST);
                continue;
            }

            removeTrailingLinebreak(lineBuffer);
            struct Node* node = getNode(&list, index);
            if (node)
            {
                node->value = malloc(sizeof(char[MAX_NODEVALUE_LENGTH]));
                strcpy(node->value, lineBuffer + offset);
            }
            else
                fprintf(stderr, S_ERR "No node at index %i\n" S_RST, index);
            
            continue;
        }

        if (strcmp(command, "add") == 0)
        {
            if (endOfLine || sscanf(lineBuffer + offset, "%255s %n", command, &read) < 1)
            {
                fprintf(stderr, S_ERR "Missing arguments for add command\n" S_RST);
                continue;
            }

            if (strcmp(command, "at") == 0)
            {
                offset += read;
                endOfLine = (offset + 1) >= strlen(lineBuffer);

                if (endOfLine)
                {
                    fprintf(stderr, S_ERR "Missing index argument for add at command\n" S_RST);
                    continue;
                }

                if (sscanf(lineBuffer + offset, " %d %n", &index, &read) < 1)
                {
                    fprintf(stderr, S_ERR "Invalid index argument for add at command\n" S_RST);
                    continue;
                }

                offset += read;
                endOfLine = (offset + 1) >= strlen(lineBuffer);

                // Check if value exists, but read rest of line buffer instead (preserves spaces)
                if (sscanf(lineBuffer + offset, " %255s", value) < 1)
                {
                    fprintf(stderr, S_ERR "Invalid value argument for add at command\n" S_RST);
                    continue;
                }

                removeTrailingLinebreak(lineBuffer);

                if (!createNodeAt(&list, lineBuffer + offset, index))
                    fprintf(stderr, S_ERR "Could not add node at index %i\n" S_RST, index);

                continue;
            }
            
            if (strcmp(command, "first") == 0)
            {
                offset += read;
                endOfLine = (offset + 1) >= strlen(lineBuffer);

                // Check if value exists, but read rest of line buffer instead (preserves spaces)
                if (sscanf(lineBuffer + offset, " %255s", value) < 1)
                {
                    fprintf(stderr, S_ERR "Invalid value argument for add at command\n" S_RST);
                    continue;
                }

                removeTrailingLinebreak(lineBuffer);
                createNodeFirst(&list, lineBuffer + offset);

                continue;
            }
            
            removeTrailingLinebreak(lineBuffer);
            createNode(&list, lineBuffer + offset);

            continue;
        }

        if (strcmp(command, "remove") == 0)
        {
            if (endOfLine || sscanf(lineBuffer + offset, "%255s %n", command, &read) < 1)
            {
                fprintf(stderr, S_ERR "Missing arguments for remove command\n" S_RST);
                continue;
            }

            if (strcmp(command, "all") == 0)
            {
                removeAll(&list);
                continue;
            }
            

            if (sscanf(lineBuffer + offset, " %d", &index) < 1)
            {
                fprintf(stderr, S_ERR "Invalid index argument for remove command\n" S_RST);
                continue;
            }

            removeNodeAt(&list, index);
            continue;
        }
    }

    return 0;
}
