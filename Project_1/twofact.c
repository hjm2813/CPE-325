/*------------------------------------------------------------------------------
 * File:        twofact.c
 * Function:    Subroutines int_fact and long_int_fact for Lab01 demo 
 * Description: This C code calculates the factorial of a given number
                int_fact gives an int output; long_int_fact gives a long int.
 * Input:       Integer variable base
 * Output:      Factorial of input (integer and long integer)
 * Author(s):   Aleksandar Milenkovic, milenkovic@computer.org
 * Date:        Aug 16, 2017
 * ---------------------------------------------------------------------------*/
int int_fact(int base)
{
    int power, total = 1;
    if(base == 0 || base == 1)
    {
        return 1;               // Factorial of 0 and 1 is 1 by definition
    }
    else
    {
        for(power = 2; power <= base; power++)
        {
            total *= power;     // Do the factorial multiplication
        }
        return total;
    }
}


long int long_int_fact(int base)
{
    int power = 1;
    long int total = 1;
    if(base == 0 || base == 1)
    {
        return 1;               // Factorial of 0 and 1 is 1 by definition
    }
    else
    {
        for(power = 2; power <= base; power++)
        {
            total *= power;     // Do the factorial multiplication
        }
        return total;
    }
}
