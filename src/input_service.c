#include "../include/ecco/ecco.h"
#include "../include/tinyargs/tinyargs.h"
#include <stdio.h>
#include <string.h>

void get_arguments(struct arguments* args, int argc, char** argv) {
    ArgTable argTable;
	InitArgTable(&argTable,argv,argc);

    NewArgument("-k", "private or public key file or name in case of generation", 
        StrArg("keyring"), ARG_STRING, &argTable);
    NewArgument("-e", "file to be encrypted", 
        StrArg(""), ARG_STRING, &argTable);
    NewArgument("-d", "file to be decrypted", 
        StrArg(""), ARG_STRING, &argTable);
    NewArgument("-o", "output file", 
        StrArg(""), ARG_STRING, &argTable);

    Parse(&argTable);

    // if both -e andd -d are set - something's wrong
    if (strlen(GetStrArg(&argTable, "-e")) 
    && strlen(GetStrArg(&argTable, "-d"))) {
        perror("invalid arguments: can't encrypt and decrypt at the same time");        
    } else if (strlen(GetStrArg(&argTable, "-e"))) {
        
    }
}
