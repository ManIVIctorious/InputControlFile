
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>

#include "typedefinitions.h"
#include "controlfile.h"

static void ThrowInputError(char* filename, int linenumber, char* format,...);
static char * PreprocessBuffer(char* filename, int linenumber, char* buffer, const char* comment);

static int SetKeyValue(char* filename, int linenumber, keyword* keywordlist, char* kw, char* val);

// Provided prototypes
int ControlFileParser(char* filename, keyword* keywordlist);


int ControlFileParser(char* filename, keyword* keywordlist){

    const char * comment = "%#\n";
    char * token;
    char * pos;
    char * eqsgn;
    char * end;

    FILE * fd = fopen(filename, "r");
    if(fd == NULL){ perror(filename); exit(errno); }

    char * buffer = malloc((_MaxLineLength_) * sizeof(char));
    if(buffer == NULL){ perror("Control file buffer"); exit(errno); }

    int linenumber = 0;
    while( (fgets(buffer, _MaxLineLength_, fd)) != NULL ){

        linenumber++;
        pos = PreprocessBuffer(filename, linenumber, buffer, comment);
        if(pos == NULL){ continue; }

    // buffer now contains a full (non empty) line of the control file, stripped of
    //  comments and *pos points to the first, non white space character of buffer
    //--------------------------------------------------------------------------------

    //  &token --> token --> char array;
    //                       ↑
    //                      pos
    // split at every occurrence of semicolon
        token = pos;
        while( (pos = strsep(&token, ";")) != NULL ){

        // remove leading white spaces
            while( isspace(*pos) ) { pos++; }

            if(strlen(pos) > 0){

            // search for an equality sign after word:
                eqsgn = end = pos;
                while( (*eqsgn != '=') && (*eqsgn != '\0') ){
                // first set <end> to start of word and iterate to its end
                    end = pos;
                    while( !isspace(*end)
                        && (*end != '=')
                        && (*end != '\0')
                    ){ end++; }

                // then move <eqsgn> forward over eventual white spaces
                    eqsgn = end;
                    while( isspace(*eqsgn)
                        && (*eqsgn != '=')
                        && (*eqsgn != '\0')
                    ){ eqsgn++; }

                // and check if <eqsgn> now points to an equality sign
                //  if not ignore the word and check if there is a valid keyword
                //  afterwards (set pos to new word start and reiterate loop)
                    if( *eqsgn != '=' ){ pos = eqsgn; }
                }

                if( (*eqsgn == '=') && (strlen(eqsgn) > 1) ){
                // set char after end of word to \0 (delimit pos)
                // and move eqsgn one char behind the actual '='
                    *end = '\0';
                    eqsgn++;

                    SetKeyValue(
                        filename,
                        linenumber,
                        keywordlist,
                        pos,
                        eqsgn
                    );
                }

            }
        }
    }

// free memory
    free(buffer); buffer = NULL;

    return 1;
}


static void ThrowInputError(char* filename, int linenumber, char* format,...){

    fprintf(stderr,
        "\n (-) Error in control file \"%s\", line %d."
        ,filename,linenumber
    );

    if( (format != NULL) && (strlen(format) > 0) ){
        fprintf(stderr, "\n     ");
        va_list ap;
        va_start(ap, format);
        vfprintf(stderr, format, ap);
        va_end(ap);
    }
    fprintf(stderr, "\n     Please check your input.");
    fprintf(stderr, "\n     Aborting...\n\n");

    exit(EXIT_FAILURE);
}


static char * PreprocessBuffer(char* filename, int linenumber, char* buffer, const char* comment){

    long unsigned i;
    int control;
    char * token;
    char * pos;

// check for existence of newline character,
//  if not found the line is not fully inside of the buffer
//  and therefore exceeding maximum line length
    for(i = 0, control = 0; i < strlen(buffer); ++i){
        if(buffer[i] == '\n') control = 1;
    }
    if(control == 0){
        ThrowInputError(filename, linenumber,
            "Line too long (no newline char '\\n' found)."
        );
    }

// strip buffer from comments
    token = buffer;
    pos   = strsep(&token, comment);
    if(pos == NULL){ return pos; }

// to lower
    i = 0;
    while( pos[i] != '\0' ){
        pos[i] = tolower(pos[i]);
        ++i;
    }

// remove leading white spaces and empty lines
    while( isspace( *pos ) && (*pos != '\0') ) { pos++; }
    if(strlen(pos) < 1){ pos = NULL; }

    return pos;
}


static int SetKeyValue(char* filename, int linenumber, keyword* keywordlist, char* kw, char* val){

//             val    '\0'
//              ↓       ↓
//␣␣␣␣keyword0␣=␣␣value␣;
//    ↑      ↑
//  start   end

    int i = 0;
    int control = 0;
    int keyword_recognised = 0;
    long unsigned int j;
    char * end;

// iterate over list of keywords and check if found word is a valid keyword
    while( (keywordlist [i] .keyword) != NULL ){

    // check name of keyword
        if( strcasecmp(keywordlist[i].keyword, kw) == 0 ){

            ++keyword_recognised;

        // remove leading and trailing white spaces from <value>:
            if( strlen(val) > 0 ){
            // trailing: point <end> to end of <val>, iterate backwards
            //  until non white space is found and set the next char to \0
                end = val + strlen(val) - 1;
                while( isspace(*end) && (end > val) ){ end--; }
                end++;
                *end = '\0';

            // leading: move <val> to start of value
                while( isspace(*val) && (*val != '\0') ) { val++; }

            }

            if( strlen(val) < 1 ){
                ThrowInputError(filename, linenumber,
                    "Keyword \"%s\" does not contain any information."
                );
            }

        // check if <value> contains any white spaces and throw a warning if true
            for(j = 0, control = 0; j < strlen(val); ++j){
                if( isspace(val[j]) ){ control = 1; }
            }
            if(control == 1){
                fprintf(stderr,
                    " (-) Warning: Control file \"%s\", line \"%d\": "
                    "Value of keyword \"%s\" contains white spaces\n"
                    , filename, linenumber, keywordlist[i].keyword
                );
            }

        // <val> now contains the value in raw character array form
        //  copy value to keywordlist array and ensure \0 termination
            strncpy( keywordlist[i].value, val, _MaxEntryLength_ );
            if(keywordlist[i].value[_MaxEntryLength_ - 1] != '\0'){
                ThrowInputError(filename, linenumber,
                    "Keyword \"%s\": Value exceeding maximum entry length (%d)."
                    , _MaxEntryLength_ - 1
                );
            }

        // keyword found: increment keywordlist[i].set and number of found
        // keywords, then break the loop keyword comparison loop
            ++keywordlist[i].set;
            break;
        }

        ++i;
    }

    if(!keyword_recognised){
    // throw a warning if no keyword recognised
        fprintf(stderr,
            " (-) Warning: Control file \"%s\", line \"%d\": "
            "Ignoring unrecognised keyword \"%s\"\n"
            , filename, linenumber, kw
        );
    }

    return keyword_recognised;
}
