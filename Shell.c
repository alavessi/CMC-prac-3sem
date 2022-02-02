#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>

struct Input
{
    bool is_eof;
    const char* error;
    char** words;
};

struct String
{
    char* data;
    int capacity;
    int length;
};

struct Array
{
    char** arrchar;
    int capacity;
    int size;
};

void Init_String(struct String* s)
{
    s->length = 0;
    s->capacity = 2;
    s->data = (char*) malloc(s->capacity * sizeof(char));
}

void Append_String(struct String* s, char c)
{
    if (s->length >= s->capacity)
    {
        char* copy = (char*) malloc((s->capacity *= 2) * sizeof(char));
        for (int i = 0; i < s->length; i++)
        {
            copy[i] = s->data[i];
        }
        free(s->data);
        s->data = copy;
    }
    s->data[s->length++] = c;
}

char* Get_C_String(const struct String* s)
{
    char* result = (char*) malloc((s->length + 1) * sizeof(char));
    for (int i = 0; i < s->length; i++)
        result[i] = s->data[i];
    result[s->length] = '\0';
    return result;
}

void Clear_String(struct String* s)
{
    free(s->data);
    s->capacity = 0;
    s->length = 0;
}

void Init_Array(struct Array* arr)
{
    arr->size = 0;
    arr->capacity = 2;
    arr->arrchar = (char**)malloc(arr->capacity * sizeof(char*));
}

void Append_Array(struct Array* arr, char* data)
{
    if (arr->size + 1 >= arr->capacity)
    {
        char** copy = (char**)malloc((arr->capacity *= 2) * sizeof(char*));
        for (int i = 0; i < arr->size; i++)
        {
            copy[i] = arr->arrchar[i];
        }
        free(arr->arrchar);
        arr->arrchar = copy;
    }
    arr->arrchar[arr->size++] = data;
}

char** Get_Array(struct Array* arr)
{
    arr->arrchar[arr->size] = NULL;
    return arr->arrchar;
}

bool Is_words_equal(char* data1, char* data2)
{
    int i = 0;
    while (data1[i] != '\0' && data1[i] == data2[i])
    {
        i++;
    }
    return data1[i] == data2[i];
}

char* ReadWord(int* pc)
{
    int c = *pc;
    while (c == ' ' || c == '\t')
    {
        c = getchar();
    }
    *pc = c;
    if (c == '\n' || c == EOF)
    {
        return NULL;
    }
    enum {regular, after_slash, in_apos, in_quotes} state = regular;
    if (c == '\\')
    {
        state = after_slash;
        c = getchar();
        *pc = c;
    }
    if (state == regular && c == '\'')
    {
        state = in_apos;
        c = getchar();
        *pc = c;
    }
    if (state == regular && (c == '&' || c == '<' || c == '>' || c == '|'))
    {
        int c1 = c;
        c = getchar();
        bool f = (c == '>' && c1 == '>');
        char* result = (char*)malloc((2 + f) * sizeof(char));
        result[0] = c1;
        result[f] = c1;
        result[1 + f] = '\0';
        if (f)
        {
            c = getchar();
        }
        *pc = c;
        return result;
    }
    struct String word;
    Init_String(&word);
    bool back_to_quotes = false;
    while (state != regular || (c != '&' && c != '<' && c != '>' && c != '|' && c != ' ' && c != '\t' && c != '\n' && c != EOF))
    {
        if(state == regular)
        {
            if (c != '\\' && c != '\'' && c != '\"')
                Append_String(&word, c);
            if (c == '\\')
            {
                state = after_slash;
            }
            if (c == '\'')
            {
                state = in_apos;
            }
            if (c == '\"')
            {
                state = in_quotes;
            }
        }
        else if (state == after_slash)
        {
            state = back_to_quotes ? in_quotes : regular;
            if (back_to_quotes && c != '\"' && c != '&' && c != '<' && c != '>' && c != '|' && c != ' ' && c != '\t' && c != '\n' && c != EOF)
            {
                Append_String(&word, '\\');
            }
            if (c != '\n' && c != EOF)
            {
                Append_String(&word, c);
            }
        }
        else if (state == in_apos)
        {
            if (c == '\'')
            {
                state = regular;
            }
            else if (c != EOF)
            {
                Append_String(&word, c);
            }
        }
        else if (state == in_quotes)
        {
            if (c == '\"')
            {
                state = regular;
            }
            if (c == '\\')
            {
                state = after_slash;
                back_to_quotes = true;
            }
            else if (c != '\\' && c != '\"' && c != EOF)
            {
                Append_String(&word, c);
            }
        }
        c = getchar();
    }
    *pc = c;
    char* result = Get_C_String(&word);
    Clear_String(&word);
    return result;
}

void ReadInput(struct Input* input)
{
    input->is_eof = false;
    input->error = NULL;
    input->words = NULL;
    struct Array words;
    Init_Array(&words);
    int c = getchar();
    while (c != '\n' && c != EOF)
    {
        char* word = ReadWord(&c);
        if (word != NULL)
        {
            Append_Array(&words, word);
        }
    }
    input->words = Get_Array(&words);
    input->is_eof = (c == EOF);
}

bool Run_Input_Embedded(struct Input* input)
{
    if (Is_words_equal(input->words[0], "exit"))
    {
        if (input->words[1] == NULL)
        {
            input->is_eof = true;
        }
        else
        {
            input->error = "exit does not expect arguments";
        }
        return true;
    }
    if (Is_words_equal(input->words[0], "cd"))
    {
        if (input->words[1] == NULL || input->words[2] != NULL) // ���� ��� ���������� ��� ���������� ����� ������
        {
            input->error = "cd expects exactly one argument";
        }
        else if (chdir(input->words[1]) == -1)
        {
                input->error = "failed to change current working dir";
        }
        return true;
    }
    return false;
}

void Run_Input(struct Input* input)
{
    if (input->error != NULL || input->words[0] == NULL || Run_Input_Embedded(input))
    {
        return;
    }
    int child = fork();
    if (child == -1)
    {
        input->error = "failed to fork proccess";
        return;
    }
    if (child == 0) // �������� ������� // �������� "���������" �������� ��������, �� ������
    {
        execvp(input->words[0], input->words);
        input->error = "failed to run command";
        input->is_eof = true;
    }
    else // ������������ �������
    {
        wait(NULL);
    }

    #if 0
    for (int i = 0; input->words[i] != NULL; i++)
    {
        printf("%d: %s\n", i, input->words[i]);
    }
    printf("#\n");
    #endif
}

void Print_Error(struct Input* input)
{
    if (input->error == NULL)
    {
        return;
    }
    printf("Error: %s\n", input->error);
}

void Print_Input(const struct Input* input)
{
    for (int i = 0; input->words[i] != NULL; i++)
    {
        printf("%s ", input->words[i]);
    }
}

void Free_Input(struct Input* input)
{
    for (int i = 0; input->words[i] != NULL; i++)
    {
        free(input->words[i]);
    }
    free(input->words);
}

int main()
{
    struct Input input;
    do
    {
        printf("%s\n$ ", get_current_dir_name());
        ReadInput(&input);
        Run_Input(&input);
        Print_Error(&input);
        Free_Input(&input);
    }
    while (!input.is_eof);
    return 0;
}
