// Last Update:2019-08-05 14:57:46

#include <getopt.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "include.h"
#include "mpegts.h"

typedef struct {
    int check;
    int dump;
    char *output;
    char *input;
} param_t;

void show_usage( char *argv[] )
{
    printf("usage: %s -c -d -o [output file] inputfile\n", argv[0] );
}

int console_parse( int argc, char *argv[], param_t *param )
{
    int c = 0, option_index = 0, i = 0;
    struct option long_options[] = {
        {"check",     no_argument, 0, 'c' },
        {"dump",     no_argument, 0, 'd' },
        {"output",     required_argument, 0, 'd' },
        {0,         0,                 0,  0 }
    };

    if ( argc < 2 ) {
        show_usage( argv );
        exit(0);
    }

    while(1) {
        c = getopt_long(argc, argv, "cdo:", long_options, &option_index);
        if (c == -1)
            break;
        switch (c) {
        case 0:
            printf("option %s", long_options[option_index].name);
            if (optarg)
                printf(" with arg %s", optarg);
            printf("\n");
            break;
        case 'c':
            param->check = 1;
            break;
        case 'd':
            param->dump = 1;
            break;
        case 'o':
            param->output = strdup( optarg );
            break;
        default:
            printf("?? getopt returned character code 0%o ??\n", c);
            return -1;
        }
    }

    /*
     * ./tstool [input file] -c -d -o xxxx
     * optind point to [input file]
     * */
    param->input = strdup(argv[optind]);

    return 0;
}

int main( int argc, char *argv[] )
{
    param_t param;
    int fd;
    struct stat sb;
    char *addr;

    LOGI("enter main...\n");

    CALL( console_parse(argc, argv, &param) );
    CALL( fd = open(param.input, O_RDONLY) );
    CALL( fstat( fd, &sb ) );
    LOGI("%s size : %lld\n", param.input, sb.st_size);
    addr = mmap( NULL, sb.st_size, PROT_READ, MAP_SHARED,fd, 0);
    if ( addr == MAP_FAILED ) {
        LOGE("map failed\n");
        goto err;
    }
    parse_ts( addr, sb.st_size );
    munmap( addr, sb.st_size );

err:
    return 0;
}
