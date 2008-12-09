/* A program to print out various machine-dependent constants */
/* Michael Ashley / UNSW / 03-Mar-2003                        */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>     /* for CHAR_MIN, CHAR_MAX, etc */
#include <float.h>      /* for FLT_DIG, DBL_DIG, etc   */

int main(int argc, char *argv[])
{
    printf("char                %d bytes %d to %d \n",     sizeof(char               ), CHAR_MIN , CHAR_MAX  );
    printf("unsigned char       %d bytes %d to %d \n",     sizeof(unsigned char      ), 0        , UCHAR_MAX );
    printf("short               %d bytes %hi to %hi \n",   sizeof(short              ), SHRT_MIN , SHRT_MAX  );
    printf("unsigned short      %d bytes %hu to %hu \n",   sizeof(unsigned short     ), 0        , USHRT_MAX );
    printf("int                 %d bytes %i to %i \n",     sizeof(int                ), INT_MIN  , INT_MAX   );
    printf("unsigned int        %d bytes %u to %u \n",     sizeof(unsigned int       ), 0        , UINT_MAX  );
    printf("long                %d bytes %li to %li \n",   sizeof(long               ), LONG_MIN , LONG_MAX  );
    printf("unsigned long       %d bytes %lu to %lu \n",   sizeof(unsigned long      ), 0        , ULONG_MAX );
    printf("long long           %d bytes %lli to %lli \n", sizeof(long long          ), LLONG_MIN, LLONG_MAX );
    printf("float               %d bytes %e to %e \n",     sizeof(float              ), FLT_MIN  , FLT_MAX   );
    printf("double              %d bytes %e to %e \n",     sizeof(double             ), DBL_MIN  , DBL_MAX   );
    printf("precision of float  %d digits\n", FLT_DIG);
    printf("precision of double %d digits\n",  DBL_DIG);
    return EXIT_SUCCESS;
}
