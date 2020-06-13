
/* "controlfile.h" */
/*{{{
    This header provides:
    * the definition of a value for maximum line length
    * the prototype of the ParseControlFile() function
      and its associated free_keywordlistvalues()
    * the definition of the struct "keywords":
        explanation of struct entries:
          keyword:
            Pointer to the actual keyword string
          identifier:
            Can be used to individually identify the given struct
            by using integer values or chars (e.g. 'a')
            or to group a set of them, e.g. all integer values
          set:
            An integer which should be initialized to 0. The control
            file parsing function will increment this value every time
            the given keyword was set
          value:
            A pointer of type char**. Each time a given keyword = value
            pair is found, the ParseControlFile() function will assign an
            additional buffer of _MaxEntryLength_.
              i.e.
                keywordlist[i].value[keywordlist[i].set][_MaxEntryLength_]

            The values corresponding to the given keyword will be stored
            in this array in raw char type for later conversion.

    Invocation:
      A pointer "keywordlist" of the first element of an array of struct
      keywords is passed as an argument of the parsing function.
      The last entry of this element has to be filled with zeros (and NULL).

        keyword keywordlist[] = {
            { "first_keyword",   0,  'a',  NULL },
            { "second_keyword",  0,  'b',  NULL },
            { "third_keyword",   0,  'c',  NULL },
            { NULL , 0 , 0 , NULL }
        };

        keyword * kl = ControlFileParser(filename, keywordlist);

    // iterate over keyword list entries
        int counter = 0;
        while( kl[counter].keyword != NULL ){

        // ignore entries not set by ControlFileParser
            if(kl[i].value == NULL){ counter++; continue; }

            char ** optarg = kl[i].value;

        // do conversion and assignment //
        //------------------------------//
        //     to do so use one of      //
        //      kl[i].keyword           //
        //      kl[i].identifier        //
        //------------------------------//
        // do conversion and assignment //

        }

        free_keywordlistvalues(kl);

//}}}*/

#ifndef _CONTROL_FILE_H
#define _CONTROL_FILE_H

#define _MaxLineLength_ 2048
#define _MaxEntryLength_ 512

typedef struct keyword {
    const char * keyword;
    int          set;
    const int    identifier;
    char      ** value;
}keyword;

keyword* ControlFileParser(char* filename, keyword* keywordlist);
void free_keywordlistvalues(keyword* keylist);

#endif
