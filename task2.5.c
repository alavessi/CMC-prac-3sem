#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node
{
    char* data;
    struct Node* next;
};

struct String
{
    char* data;
    int capacity;
    int length;
};

void Init_String(struct String* s)
{
    s->length = 0;
    s->capacity = 1;
    s->data = (char*) malloc(s->capacity * sizeof(char));
}

void Append_String(struct String* s, char c)
{
    if (s->length == s->capacity)
    {
        s->capacity *= 2;
        char* copy = (char*)malloc((s->capacity) * sizeof(char));
        for (int i = 0; i < s->length; i++)
        {
            copy[i] = s->data[i];
        }
        free(s->data);
        s->data = copy;
    }
    s->data[s->length++] = c;
}

char* C_String(const struct String* s)
{
    char* result = (char*) malloc((s->length + 1) * sizeof(char));
    for (int i = 0; i < s->length; i++)\
    {
        result[i] = s->data[i];
    }
    result[s->length] = '\0';
    return result;
}

void Clear_String(struct String* s)
{
    free(s->data);
    s->capacity = 2;
    s->length = 0;
}

struct Node* Create_Node(char* data)
{
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    node->data = data;
    node->next = NULL;
    return node;
}

void Print_List(struct Node* head)
{
    while (head != NULL)
    {
        printf("%s",head -> data);
        if (head->next != NULL)
        {
            printf(" ");
        }
        head = head->next;
    }
    printf("\n");
}

void Delete_Head(struct Node** head)
{
    struct Node* p = (*head);
    *head=(*head)->next;
    free(p->data);
    free(p);
}

void Delete_Node(struct Node** prev)
{
    struct Node* p = (*prev)->next;
    (*prev)->next = ((*prev)->next)->next;
    free(p->data);
    free(p);
}

bool equal_words(char* data1, char* data2)
{
    int i = 0;
    while (data1[i] != '\0' && data2[i] != '\0' && data1[i] == data2[i])
    {
        i++;
    }
    return data1[i] == '\0' && data2[i] == '\0';
}

int main()
{
    bool begin_word = true, first_word = true;
    struct Node* head = NULL;
    struct Node* current = NULL;
    struct String s;
    char c = getchar(), clast;
    while (c == ' ' || c == '\t')
    {
        c = getchar();
    }
    bool spaces = false;
    bool empty = c == '\n';
    while (c != '\n')
    {
        if (spaces)
        {
            if (c == ' ' || c == '\t')
            {
                c = getchar();
                if (c == '\n')
                {
                    break;
                }
            }
            else
            {
                spaces = false;
            }
        }
        if (c != ' ' && c != '\t')
        {
            empty = false;
            spaces = false;
            if (begin_word)
            {
                Init_String(&s);
            }
            begin_word = false;
            Append_String(&s, c);
        }
        else
        {
            if (spaces)
            {
                c = getchar();
                if (c == '\n')
                {
                    break;
                }
                if (c != ' ' && c != '\t')
                {
                    empty = false;
                    spaces = false;
                    if (begin_word)
                    {
                        Init_String(&s);
                    }
                    begin_word = false;
                    Append_String(&s, c);
                }
            }
            else
            {
                if (first_word)
                {
                    head = Create_Node(C_String(&s));
                    current = head;
                    first_word = false;
                }
                else
                {
                    current->next = Create_Node(C_String(&s));
                    current = current->next;
                }
                Clear_String(&s);
                begin_word = true;
                spaces = true;
            }
        }
        clast = c;
        c = getchar();
    }
    if (clast != ' ' && clast != '\t' && !empty)
    {
        if (first_word)
        {
            head = Create_Node(C_String(&s));
            current = head;
            first_word = false;
        }
        else
        {
            current->next = Create_Node(C_String(&s));
            current = current->next;
        }
        Clear_String(&s);
    }
    struct Node* last = current;
    current = head;
    while (current != last && equal_words(current->data, last->data))
    {
        Delete_Head(&head);
        current = head;
    }
    if (current != last)
    {
        while (current->next != last)
        {
            if (equal_words((current->next)->data, last->data))
            {
                Delete_Node(&current);
            }
            else
            {
                current = current->next;
            }
        }
        current = head;
    }
    Print_List(current);
    while (head != NULL)
    {
        Delete_Head(&head);
    }
    return 0;
}
