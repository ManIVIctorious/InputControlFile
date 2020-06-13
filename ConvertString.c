
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>
#include <limits.h>
#include <math.h>

#include "ConvertString.h"

long int convertstring_to_long(char* optarg){

    long int value = 0;
    char * endptr  = NULL;

    errno = 0;

    value = strtol(optarg, &endptr, 10);

    /* check for various possible errors */

// out of bounds check
    if(    ( (errno == ERANGE) && (value == LONG_MAX) )
        || ( (errno == ERANGE) && (value == LONG_MIN) )
        || ( (errno !=    0  ) && (value ==     0   ) )
    ){
        perror("strtol");
        exit(errno);
    }

// check if conversion did actually happen
    if( endptr == optarg ){
        fprintf(stderr, "No digits were found\n");
        exit(EXIT_FAILURE);
    }

// check if whole string was converted, i.e. until NULL termination
    if( *endptr != '\0' ){
        fprintf(stderr, "Bad number\n");
        exit(EXIT_FAILURE);
    }

    return value;
}


long int convertstring_to_bool(char* optarg){

    long int value = 0;

// set value to 1 if optarg equals to "true" (case insensitive)
// set it to 0 if optarg equals to "false" and else assume it to
// be set as a number. In this case do a integer comparison and
// return the appropriate value.
    if      ( strcasecmp( optarg, "true"  ) == 0 ){ value = 1; }
    else if ( strcasecmp( optarg, "false" ) == 0 ){ value = 0; }
    else{
        value = convertstring_to_long(optarg);
    }

    return value;
}


double convertstring_to_double(char* optarg){

    double value  = 0;
    char * endptr = NULL;

    errno = 0;

    value = strtod(optarg, &endptr);

    /* check for various possible errors */

// out of bounds check
    if(    ( (errno == ERANGE) && (value ==  HUGE_VAL) )
        || ( (errno == ERANGE) && (value == -HUGE_VAL) )
        || ( (errno == ERANGE) && (value ==    0.0   ) )
        || ( (errno != 0) )
    ){
        perror("strtol");
        exit(errno);
    }

// check if conversion did actually happen
    if( endptr == optarg ){
        fprintf(stderr, "No digits were found\n");
        exit(EXIT_FAILURE);
    }

// check if whole string was converted, i.e. until NULL termination
    if( *endptr != '\0' ){
        fprintf(stderr, "Bad number\n");
        exit(EXIT_FAILURE);
    }

    return value;
}
