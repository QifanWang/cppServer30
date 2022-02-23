// error macro symposim stdio.h, errno.h and 
#define errif(cond, errmsg)     \
    do{                         \
        if ((cond)){            \
            perror((errmsg));   \
            exit(EXIT_FAILURE); \
        }                       \
    } while (false);            \

#define UTIL_HEADER