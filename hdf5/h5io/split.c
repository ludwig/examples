#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "split.h"

/*
 * Data structure for our basic string tokenizer. Basically, the strategy
 * is to copy the desired string into our buffer, where we can zero out
 * the separator characters in-place. Using a struct allows us to avoid
 * defining global variables like in the version of split.c from the project
 * http://www.nongnu.org/uri/ on which this code is inspired.
 */

typedef struct {
    char separator;         /* separator character */
    char **tokens;          /* tokens array */
    int token_count;        /* real size of tokens array */
    int max_token_count;    /* size of tokens array */
    char *buffer;           /* buffer string */
    int buffer_length;      /* length of buffer string */
} strtok_t;



/*
 * In this section, we define the methods for our string tokenizer
 * object. Namely, a constructor, a destructor, and then the actual
 * routine to split the string.
 */
static void strtok_init(strtok_t *st, char sep)
{
    /* remember the separator character */
    st->separator = sep;

    /* starting up with 0 tokens */
    st->token_count = 0;

    /* setup the initial array sizes */
    st->buffer_length = 512;        /* 512 chars in buffer */
    st->max_token_count = 16;       /* 16 tokens in array */

    /* allocate enough tokens and initialize buffer */
    st->buffer = (char *)malloc(st->buffer_length * sizeof(char));
    st->tokens = (char **)malloc(st->max_token_count * sizeof(char *));
    
    return;
}

static void strtok_free(strtok_t *st)
{
    if (st != NULL)
    {
        free(st->buffer);
        free(st->tokens);
    }
}

static void strtok_split(strtok_t *st, const char *s, int len)
{
    /* Quick validation */
    assert(st->separator != '\0');

    /*
     * First, check whether our buffer is large enough to manipulate
     * the string s, and if not, reallocate enough memory.
     */
    if (st->buffer_length < len)
    {
        st->buffer_length = (len < 512) ? 512 : len+1;
        st->buffer = (char *)realloc(st->buffer, st->buffer_length * sizeof(char));
    }

    /*
     * Next, copy the string s into our buffer and tokenize it in-place.
     * Essentially, zero out the locations where we find the separator
     * character, while remembering the beginning of each string.
     */
    memcpy(st->buffer, s, len);
    st->buffer[len] = '\0';
    {
        char *first, *p;
        int index, last;

        first = st->buffer;
        last  = st->buffer_length - 1;

        /* remove trailing separators */
        while (last >= 0 && st->buffer[last] == st->separator)
        {
            st->buffer[last] = '\0';
            last--;
        }

        /* remove leading separators */
        while(*first == st->separator)
        {
            first++;
        }

        /* store first token */
        index = 0;
        st->tokens[index++] = first;

        /* keep tokenizing the buffer */
        for (p = strchr(first, st->separator);
             p != NULL;
             p = strchr(p, st->separator))
        {
            /* separator found -- zero it out */
            *p = '\0';

            /* make p point to next char */
            p++;

            /* store the next token */
            if ((*p != st->separator) && (*p != '\0'))
            {
                st->tokens[index++] = p;

                /* check whether we need to expand our tokens array,
                 * to make room for the next batch of tokens */
                if (index >= (st->max_token_count))
                {
                    st->max_token_count += 16;
                    st->tokens = (char **)realloc(st->tokens,
                                                  st->max_token_count * sizeof(char *));
                }
            }
        }

        /* store the final count */
        st->token_count = index;
    }

    return;
}



/*
 * Finally, we provide a public interface to our string tokenizer.
 * The caller subsumes the responsibility of freeing the newly allocated
 * list, as well as each individual string in that list.
 */
void split(const char *str, int len,
           char ***split_list, int *split_count, char sep)
{
    int i;
    strtok_t tok;

    /* guard against NULL separator */
    if (sep == '\0')
    {
        *split_list = NULL;
        *split_count = 0;
        return;
    }

    strtok_init(&tok, sep);
    strtok_split(&tok, str, len);

    *split_list = (char **)malloc(tok.token_count * sizeof(char *));
    *split_count = tok.token_count;

    for (i = 0; i < tok.token_count; i++)
    {
        (*split_list)[i] = strdup(tok.tokens[i]);
    }

    strtok_free(&tok);
}

void split_free(char **split_list, int split_count)
{
    int i;
    if (split_list != NULL)
    {
        for (i = 0; i < split_count; i++)
        {
            free(split_list[i]);
        }
        free(split_list);
    }
}
