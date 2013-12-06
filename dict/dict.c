/* 
 * File:   dict.c
 * Author: lusaisai
 *
 * Created on December 6, 2013, 11:11 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sqlite3.h>
#include "config.h"

#define WORD_BUFFER 255


/*function prototypes*/
sqlite3 * connect_db();
char * rl_gets ( char *line_read );
void start_up();
void shutdown();
void run();
char ** dict_completion( const char *text, int start, int end );
char * find_word (const char * text, int state);
void query_meaning(char *word);
void trim_trailing_spaces(char *line);


/* global variable */
sqlite3 * p_dict_db = NULL;

/*
 * 
 */
int main(int argc, char** argv) {
    p_dict_db = connect_db();
    run();
    return (EXIT_SUCCESS);
}


/* Connect the database file and return a pointer */
sqlite3 * connect_db() {
    sqlite3 *pdb;
    int rc;

    rc = sqlite3_open(DICT_FILE, &pdb);
    if( rc ){
        fprintf(stderr, "Can't connect to database: %s\n", sqlite3_errmsg(pdb));
        exit(1);
    }
    return pdb;
}

/*
 * The replace of gets
 */
char * rl_gets ( char *line_read ) {
  if (line_read) {
      free(line_read);
      line_read = (char *)NULL;
  }

  line_read = readline("> ");

  if (line_read && *line_read) add_history(line_read);
  
  return line_read;
}


void initialize_readline() {
  rl_attempted_completion_function = dict_completion;
}

char ** dict_completion( const char *text, int start, int end ) {
    char **matches = NULL;
    matches = rl_completion_matches (text, find_word);
    return matches;
}



char * find_word (const char * text, int state) {
    static int word_index = 1;
    static char **result = NULL;
    static int row_count = 0;
    static int column_count = 0;
    static char *err_msg = NULL;
    static int rc = 0;
    
    char *word = malloc(WORD_BUFFER);
    
    if ( !state ) {
        word_index = 1;
        if ( result != NULL ) sqlite3_free_table(result);
        char *osql = sqlite3_mprintf("select word from oxford where word like '%q%%'", text);
        rc = sqlite3_get_table(p_dict_db, osql, &result, &row_count, &column_count, &err_msg );
        sqlite3_free(osql);
        if ( rc != SQLITE_OK ) {
            perror(err_msg);
            return NULL;
        }
    }
    
    if ( word_index <= row_count ) {
        strncpy( word, result[word_index], WORD_BUFFER );
        word_index++;
        return word;
    } else {
        return NULL;
    }
}

/*
 * The start up informations
 */
void start_up() {
    printf("Dict %s\n", VERSION);
    puts("Type the English word and get the English/Chinese explanations");
    printf ("Current avaliable dictionaries: %s\n", "Oxford");
    puts("Press Tab for word suggestions");
    printf("Press Ctrl+D to quit\n");
    initialize_readline();
}

/*
 * The quit informations
 */
void shutdown() {
    printf("Exit Dict %s\n", VERSION);
    sqlite3_close(p_dict_db);
}


/*
 * an infinite loop of running
 */
void run() {
    start_up();
    char *line = NULL;
    while( 1 ) {
        line = rl_gets(line);
        if ( !line ) {
            shutdown();
            break;
        }
        query_meaning(line);
    }
}

/*
 * query the meaning of the word
 */
void query_meaning(char *word) {
    trim_trailing_spaces(word);
    if ( strlen(word) == 0 ) return;
    char *sql = sqlite3_mprintf("select meanings from oxford where word = '%q' limit 1", word);
    char **result = NULL;
    int row_count = 0;
    int column_count = 0;
    char *err_msg = NULL;
    
    sqlite3_get_table(p_dict_db, sql, &result, &row_count, &column_count, &err_msg );

    if ( row_count == 0 ) {
        puts("Word Not Found");
    } else {
        for( int i = 1; i <= row_count; i++ ) {
            puts(result[i]);
        }
    }
    sqlite3_free_table(result);
    sqlite3_free(sql); 
}

/* remove the trailing newline and spaces*/
void trim_trailing_spaces(char *line) {
    for ( int index = strlen(line) - 1; index >= 0; index-- ) {
        if ( line[index] == '\n' || line[index] == ' ' || line[index] == '\t' ) {
            line[index] = 0;
        } else {
            break;
        }
    }
}
