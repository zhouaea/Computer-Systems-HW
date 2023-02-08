/*
 * Author: Neo Zhou - zhouaea@bc.edu
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ERROR (1)
#define SUCCESS (0)
#define TRUE (1)
#define FALSE (0)
#define USAGE "Usage: ./convert temperature temperature_scale\n" \
"Ex: ./convert 32F\n" \
"Ex: ./convert 100.7C\n"

/*
 * Check whether the value of the character c is one of the 10 decimal digits
 * 0 through 9 by determining if the ASCII value of the character is in the
 * range of the ASCII values for the numeric characters. Return 1 for true, 0 for false.
*/
int is_digit(char c)
{
   return c >= '0' && c <= '9';
}

/*
 * Check whether the input string represents a floating point number.
 * Return FALSE if the first character is not a sign, numeral, or dot.
 * Return FALSE if all other characters are not numerals or dots.
 * If a character is a dot and no dots came before it, note a dot has been found.
 * If there was already a dot before, there are multiple dots so return FALSE.
 * If a dot hasn't been found after reading the string, return FALSE.
 * Return TRUE if input meets all conditions.
*/
int is_floating_point_number(char * input)
{

    int decimal_point_found = 0;

    for (int i = 0; input[i] != '\0'; i++)
    {
        if (i == 0 && (input[i] != '+' && input[i] != '-' && !is_digit(input[i]) && input[i] != '.'))
        {
            return FALSE;
        }
        else if (i != 0 && !is_digit(input[i]) && input[i] != '.')
            return FALSE;

        if (input[i] == '.')
        {
            if (!decimal_point_found)
                decimal_point_found = 1;
            else if (decimal_point_found)
                return FALSE;
        }
    }

    if (!decimal_point_found) {
        return FALSE;
    }

    return TRUE;
}

/*
 * Check whether the input string represents an integer.
 * Return FALSE if the first character is not a sign or numeral.
 * Return FALSE if all other characters are not numerals.
 * Return TRUE if input meets all conditions.
*/
int is_integer(char * input)
{
    printf("modified input: %s\n", input);
    for (int i = 0; input[i] != '\0'; i++)
    {
        if (i == 0 && (input[i] != '+' && input[i] != '-' && !is_digit(input[i])))
            return FALSE;
        else if (i != 0 && !is_digit(input[i]))
            return FALSE;
    }

    return TRUE;
}

/* PARSES TEMPERATURE SCALE
 * Check whether the last character in the string is one of the two allowable characters, 'C' or 'F',
 * using strlen() and either store the character in a character pointer or return an error.
 *
 * PARSES TEMPERATURE
 * Replace temperature scale character with nul terminator to analyze numeric temperature values only.
 * Check whether the parsed input is a float or an integer, and if they are, cast them to strings.
 * If the parsed input is neither float or integer, return an error.
*/
int validate_and_parse_input(char * input, double * temperature, char * scale)
{
    char last_character = input[strlen(input) - 1];

    if (last_character != 'C' && last_character != 'F')
        return ERROR;
    *scale = last_character;

    input[strlen(input) - 1] = '\0';
    
    if (is_floating_point_number(input))
    {
        *temperature = strtod(input, NULL);
    }
    else if (is_integer(input))
    {
        *temperature = (double) atoi(input);
    }
    else
    {
        return ERROR;
    }

    return SUCCESS;
}

/*
 * Convert a temperature in celsius to farenheit using the formula C * 9/5 + 32.
*/
double celsius_to_farenheit(double celsius)
{
    return 9 * celsius / 5 + 32;
}

/*
 * Convert a temperature in celsius to fahrenheit using the formula(F - 32) * 5/9.
*/
double fahrenheit_to_celsius(double fahrenheit)
{
    return (fahrenheit - 32) * 5 / 9;
}

/*
 * Print a usage message to the user in respone to erroneous input.
*/
void print_usage()
{
    printf(USAGE);
}

/*
 * Print an output depending on the format of the user's input.
 * If scale is the character 'C', then print Celsius first and Fahrenheit second.
 * If scale is 'F', print the results in the reverse order.
 * Handle the unlikely case of any other character in scale by printing an "internal" error message
 * (meant for the developer) and return without further action.
*/
void print_output(double celsius, double fahrenheit, char scale)
{
    if (scale == 'C')
        printf("Celsisus: %.2f Fahrenheit: %.2f\n", celsius, fahrenheit);
    else if (scale == 'F')
        printf("Fahrenheit: %.2f Celsisus: %.2f\n", fahrenheit, celsius);
    else
        fprintf(stderr, "%c is not a valid scale!\n", scale);
        exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
    // Make sure user inputs valid number of command-line arguments.
    if (argc != 2)
    {
        print_usage();
        return 0;
    }
    
    // Store user input.
    char* input = argv[1];
    
    // Parse the input string and get the temperature and scale.
    double temperature;
    char scale;
    int result = validate_and_parse_input(input, &temperature, &scale);
    
    // Ensure user correctly formatted input.
    if (result == ERROR)
    {
        print_usage();
        return 0;
    }
    
    // Obtain temperature in both scales.
    double fahrenheit;
    double celsius;
    
    if (scale == 'C')
    {
        celsius = temperature;
        fahrenheit = celsius_to_farenheit(temperature);
    }
    else if (scale == 'F')
    {
        fahrenheit = temperature;
        celsius = fahrenheit_to_celsius(temperature);
    }
    
    print_output(celsius, fahrenheit, scale);
    return 0;
}
