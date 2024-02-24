/*-------------------------------------------------------------------------
 * File:  Lab_02_1.c
 * Description: This C program will print the sizes and ranges of common data types.
 * Author(s):   Min Han
 * Date:       08.28.23
 *-----------------------------------------------------------------------*/
#include <limits.h>
#include <stdio.h>
#include <float.h>


void printChar(char c, int count) {
    while (count > 0) {
        printf("%c", c);
        count--;
    }
    printf("\n");
}


int main() {

    printChar('-', 90);

    //formating
    printf("| %-20s | %-15s | %-21s | %-21s |\n", "Data Type", "Size (in bytes)", "Minimum", "Maximum");


    printChar('-', 90);

    // char 
    printf("| %-20s | %-15d | %-21d | %-21d |\n", "char", sizeof(char), CHAR_MIN, CHAR_MAX);

    // short 
    printf("| %-20s | %-15d | %-21d | %-21d |\n", "short int", sizeof(short), SHRT_MIN, SHRT_MAX);

    // int 
    printf("| %-20s | %-15d | %-21d | %-21d |\n", "int", sizeof(int), INT_MIN, INT_MAX);

    // float
    printf("| %-20s | %-15d | %-21e | %-21e |\n", "float", sizeof(float), FLT_MIN, FLT_MAX);

    // double
    printf("| %-20s | %-15d | %-21e | %-21e |\n", "double", sizeof(double), DBL_MIN, DBL_MAX);

    // long 
    printf("| %-20s | %-15d | %-21ld | %-21ld |\n", "long int", sizeof(long), LONG_MIN, LONG_MAX);

    // long long 
    printf("| %-20s | %-15d | %-21lld | %-21lld |\n", "long long int", sizeof(long long), LLONG_MIN, LLONG_MAX);

    // char unsigned
    printf("| %-20s | %-15d | %-21u | %-21u |\n", "unsigned char", sizeof(unsigned char), 0, UCHAR_MAX);

    // short unsigned
    printf("| %-20s | %-15d | %-21u | %-21u |\n", "unsigned short int", sizeof(unsigned short), 0, USHRT_MAX);

    // int unsigned
    printf("| %-20s | %-15d | %-21u | %-21u |\n", "unsigned int", sizeof(unsigned int), 0, UINT_MAX);

    // long unsigned
    printf("| %-20s | %-15d | %-21lu | %-21lu |\n", "unsigned long int", sizeof(unsigned long), 0UL, ULONG_MAX);

    // long long unsigned
    printf("| %-20s | %-15d | %-21llu | %-21llu |\n", "unsigned long long", sizeof(unsigned long long), 0ULL, ULLONG_MAX);




    printChar('-', 90);



    return 0;
}
