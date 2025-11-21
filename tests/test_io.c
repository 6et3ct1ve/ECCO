#include "../include/ecco/ecco.h"

int main(int argc,char** argv) {
    struct arguments args;
    get_arguments(&args, argc, argv);
    print_arguments(&args);
}
