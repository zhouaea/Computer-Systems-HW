/*
 * Program to parse a float into its sign, exponent, and mantissa, in both decimal and binary.
 * Author: Neo Zhou - zhouaea@bc.edu
 */

#include <stdio.h>
#define TRUE 1
#define FALSE 0

typedef union converter {
    float f;
    unsigned int u;
} converter;

/*
 * Extract the sign from a 32 bit float.
 */
void obtain_sign(unsigned int input, unsigned int *sign_ptr) {
    // Create a mask with a 1 in the 31st place followed by a string of 0's.
    int mask = 1 << 31;
    // Extract the 31st place by using an and operation with the mask and omit all bits except
    // the bit in the 31st place.
    *sign_ptr = (input & mask) >> 31;
}

/*
 * Extract the exponent from a 32 bit float.
 */
void obtain_exponent(unsigned int input, unsigned int *exponent_ptr) {
    // Create a mask 8 bits long filled with all 1's, shift it 23 places to the left so that the
    // 1's inhabit the bits from the 30th place to the 23rd place.
    int mask = 0xff << 23;
    // Extract the 30th to 23rd places by using an and operation with the mask and omit all bits
    // except the bits from the 23rd place to the 31st place. Since the 31st place is guaranteed to
    // be 0, since the mask does not cover the 31st place, we are essentially only extracting the
    // 30th place to the 23rd place.
    *exponent_ptr = (input & mask) >> 23;
}

/*
 * Extract the mantissa from a 32 bit float.
 */
void obtain_mantissa(unsigned int input, unsigned int *mantissa_ptr) {
    // Create a mask that inhabits the 22nd place to the 0th place.
    int mask = 0x7fffff;
    // Extract the 22nd to 0th places by using an and operation with the mask.
    *mantissa_ptr = input & mask;
}

/*
 * Parse a 32 bit float into a sign, exponent, and mantissa.
 */
void parse_input(unsigned int input, unsigned int *sign_ptr, unsigned int *exponent_ptr,
                 unsigned int *mantissa_ptr) {
    obtain_sign(input, sign_ptr);
    obtain_exponent(input, exponent_ptr);
    obtain_mantissa(input, mantissa_ptr);
}

/*
 * Print the floating point input (interpreted as an unsigned integer) in binary
 * without removing leading or trailing zeroes.
 */
void print_input_binary(unsigned int decimal) {
    int reverse_bits[32];
    
    // Initialize reverse bits array to all 0's.
    for (int i = 0; i < 32; i++) {
        reverse_bits[i] = 0;
    }
    
    int i = 0;
    
    // Get the remainder of dividing the decimal by 2 in order to get reverse binary digits.
    while (decimal > 0) {
        reverse_bits[i] = decimal % 2;
        decimal /= 2;
        i++;
    }
    
    // Print the reverse binary digits in reverse to get the binary representation of the decimal.
    printf("\nBinary Representation:\n");
    
    
    for (int i = 31; i >= 0; i--) {
        printf("%d", reverse_bits[i]);
    }
    printf("\n\n");
}

/*
 * Print an unsigned decimal integer in binary, removing the leading zeroes;
 * this will be used to print the exponent in binary.
 */
void print_binary(unsigned int decimal) {
    int reverse_bits[32];
    
    // Initialize reverse bits array to all 0's.
    for (int i = 0; i < 32; i++) {
        reverse_bits[i] = 0;
    }
    
    int i = 0;
    
    // Get the remainder of dividing the decimal by 2 in order to get reverse binary digits.
    while (decimal > 0) {
        reverse_bits[i] = decimal % 2;
        decimal /= 2;
        i++;
    }
    
    // Print the reverse binary digits in reverse to get the binary representation of the decimal.
    printf("\t\tbinary: ");
    
    int leading_zeroes = TRUE;
    
    for (int i = 31; i >= 0; i--) {
        // If there are no more uneccesary leading zeroes, print the binary digit.
        if (!leading_zeroes) {
            printf("%d", reverse_bits[i]);
            continue;
        }
        
        // Once the rightmost one is reached, we know there can no longer be any leading zeroes
        if (reverse_bits[i] == 1) {
            leading_zeroes = FALSE;
            printf("%d", reverse_bits[i]);
        }
    }
    printf("\n");
}

/*
 * Print numbers in binary and decimal that add a leading '1.' to a decimal input;
 * this will be used to print the mantissa in binary and decimal while restoring the leading '1.'.
 * When it prints the binary number, it removes trailing zeroes.
 */
void print_binary_and_decimal_with_leading_one(unsigned int decimal) {
    int reverse_bits[23];
    
    // Initialize reverse bits array to all 0's.
    for (int i = 0; i < 23; i++) {
        reverse_bits[i] = 0;
    }
    
    int i = 0;
    
    // Get the remainder of dividing the decimal by 2 in order to get reverse binary digits.
    while (decimal > 0) {
        reverse_bits[i] = decimal % 2;
        decimal /= 2;
        i++;
    }
    
    // Mark the trailing zeroes as threes.
    for (int i = 0; i < 23; i++) {
        // Mark any zeroes at the end of the floating point as a three, until a 1 is found.
        if (reverse_bits[i] == 0) {
            reverse_bits[i] = 3;
        } else {
            break;
        }
    }
    
    // Print the reverse binary digits in reverse to get the binary representation of the decimal.
    printf("\t\tbinary (with added one): 1.");
    for (int i = 22; i >= 0; i--) {
        // Print the binary digit if it is not a trailing zero.
        if (reverse_bits[i] != 3) {
            printf("%d", reverse_bits[i]);
        }
    }
    printf("\n");
    
    // Print the decimal representation of adding a 1. to the binary of the decimal input.
    float decimal_with_leading_one = 1;
    float power_of_2 = 0.5;
    for (int i = 22; i >= 0; i--) {
        // Do not calculate a bit if it is a trailing 0.
        if (reverse_bits[i] != 3) {
            decimal_with_leading_one += power_of_2 * reverse_bits[i];
        }
        
        power_of_2 *= 0.5;
    }
    printf("\t\tdecimal (with added one): %f\n", decimal_with_leading_one);
}


/*
 * Print the sign of a floating point number in binary and decimal using a clean text format.
 */
void print_sign(unsigned int sign) {
    printf("Sign:\n");
    // Since a sign can only be a 0 or a 1, its representation in decimal is equivalent to its
    // representation in binary.
    printf("\t\tbinary: %d\n", sign);
    
    printf("\t\tdecimal: %d\n", sign);
}

/*
 * Print in a clean format the exponent of a floating point number in binary and decimal,
 * calculating the decimal value with bias and without.
 */
void print_exponent(unsigned int exponent) {
    printf("Exponent:\n");
    
    print_binary(exponent);
    
    printf("\t\tdecimal: %d\n", exponent);
    
    // Calculate the actual value of the exponent by subtracting the bias, which is 2^k - 1.
    int unbiased_exponent = exponent - 127;
    printf("\t\twithout bias: %d\n", unbiased_exponent);
}

/*
 * Print in a clean format the mantissa of a floating point number in binary and decimal,
 * calculating them by restoring the leading one.
 */
void print_mantissa(unsigned int mantissa) {
    printf("Mantissa:\n");
    print_binary_and_decimal_with_leading_one(mantissa);
}

/*
 * Print the sign, exponent, and mantissa of a floating point number in a clean format.
 */
void print_components(unsigned int sign, unsigned int exponent, unsigned int mantissa) {
    print_sign(sign);
    print_exponent(exponent);
    print_mantissa(mantissa);
}

int main() {
    converter input;

    printf("Please input a floating point number: ");
    // Continue to scan while user input is a float. Exit if otherwise.
    while (scanf("%f", &input.f)) {
        print_input_binary(input.u);
        
        // Parse 32 bit binary number into three components (sign, exponent, and mantissa).
        unsigned int sign;
        unsigned int exponent;
        unsigned int mantissa;
        parse_input(input.u, &sign, &exponent, &mantissa);

        // Print the decimal and binary representations of the sign, exponent, and mantissa.
        print_components(sign, exponent, mantissa);
        printf("\nPlease input a floating point number: ");
    }
    
    return 0;
}
