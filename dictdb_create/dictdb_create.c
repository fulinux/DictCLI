/* 
 * File:   dictdb_create.c
 * Author: lusaisai
 *
 * Created on December 5, 2013, 5:23 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>


#define WORD_BUFFER 255
#define MEANING_BUFFER 524288
#define DB_FILE "../dict/data/dictdb.sqlite"
#define META_TABLE_SQL_FILE "data/meta_tables.sql"
#define META_INDEX_SQL_FILE "data/meta_indexes.sql"
#define RAW_FILE "data/raw_data.txt"

typedef struct {
    char word[WORD_BUFFER];
    int length;
    char first_letter[2];
    char meanings[MEANING_BUFFER];
} db_record, *pdb_record;
    

/* function prototypes */
sqlite3 * connect_db();
void run_multiple_sql(sqlite3 *db, char *sql);
char * read_whole_file(char *filename);
void create_tables();
void create_indexes( sqlite3 *pdb );
void populate_db( sqlite3 *pdb );
void trim_trailing_spaces(char *line);


/*
 * Run to generate the dictionary database file
 */
int main(int argc, char** argv) {
    sqlite3 *pdb;
    pdb = connect_db();
    create_tables(pdb);
    populate_db(pdb);
    create_indexes(pdb);
    sqlite3_close(pdb);
    return (EXIT_SUCCESS);
}

/*
 * read data from the raw text file and insert into DB
 */
void populate_db( sqlite3 *pdb ) {
    db_record data;
    data.first_letter[1] = 0;
    FILE *pfile;
    char *line;
    sqlite3_stmt *stmt;
    char *sql = "insert into oxford ( word, length, first_letter, meanings ) VALUES (?1, ?2, ?3, ?4)";
    
    pfile = fopen(RAW_FILE , "r");
    if (pfile == NULL) { 
        perror("Error opening the raw dictionary file");
        exit(1);
    }
    
    sqlite3_prepare_v2(pdb, sql, -1, &stmt, NULL);
    
    while ( 1 ) {
        // read the word
        line = fgets( data.word, WORD_BUFFER, pfile );
        if ( line == NULL ) break;
        trim_trailing_spaces(data.word);
        data.first_letter[0] = data.word[0];
        data.length = strlen(data.word);
        
        // read the meaning
        line = fgets( data.meanings, MEANING_BUFFER, pfile );
        if ( line == NULL ) break;
        trim_trailing_spaces(data.meanings);
        
        // bind data
        sqlite3_bind_text(stmt, 1, data.word, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, data.length);
        sqlite3_bind_text(stmt, 3, data.first_letter, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, data.meanings, -1, SQLITE_STATIC);
        
        // run statement
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            perror("Error inserting data");
        } else {
            printf("Insert word %s.\n", data.word);
        }
        
        // reset
        sqlite3_reset(stmt);
    }
    
    sqlite3_finalize(stmt);
    
}


/* remove the trailing newline and spaces*/
void trim_trailing_spaces(char *line) {
    for ( int index = strlen(line) - 1; index >= 0; index-- ) {
        if ( line[index] == '\n' || line[index] == ' ' ) {
            line[index] = 0;
        } else {
            break;
        }
    }
}

/* Connect the database file and return a pointer */
sqlite3 * connect_db() {
    sqlite3 *pdb;
    int rc;

    rc = sqlite3_open(DB_FILE, &pdb);
    if( rc ){
        fprintf(stderr, "Can't connect to database: %s\n", sqlite3_errmsg(pdb));
        exit(1);
    }else{
        fprintf(stdout, "Connected to database successfully\n");
    }
    return pdb;
}

/*
 * read the whole file, user is responsible for freeing the memory
 */
char * read_whole_file(char *filename) {
    long size = 0;
    FILE *pfile = fopen( filename, "rb" );
    char *data = NULL;
    size_t result;
    
    if (pfile==NULL) { 
        perror ("Error opening file");
    } else {
        fseek(pfile, 0, SEEK_END);
        size = ftell(pfile);
        rewind (pfile);
        data = malloc(size+1);
        
        result = fread (data,1,size,pfile);
        if (result != size) perror ("Error reading file");
        
        data[size] = 0;
    }
    
    return data;
}

void run_multiple_sql(sqlite3 *db, char *sql) {
   char *zErrMsg = 0;
   int  rc;
   rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "SQL finished successfully\n");
   }
}

void create_tables( sqlite3 *pdb )
{
    char *sql;
    sql = read_whole_file(META_TABLE_SQL_FILE);
    run_multiple_sql( pdb, sql );
    free(sql);
}

void create_indexes( sqlite3 *pdb ) {
    char *sql;
    sql = read_whole_file(META_INDEX_SQL_FILE);
    run_multiple_sql( pdb, sql );
    free(sql);
}
