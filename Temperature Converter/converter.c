/*
 * Author: Neo Zhou - zhouaea@bc.edu
*/
#include <stdlib.h>
#include <stdio.h>

double convert(double temperature_celsius)
{
    return 9 * temperature_celsius / 5 + 32;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Usage: ./converter temperature_in_celsius");
        return 0;
    }

    double celsius = strtod(argv[1], NULL);
    double farenheit = convert(celsius);
    printf("%.2f degrees celsius is %.2f degrees farenheit", celsius, farenheit);
    return 0;
}
