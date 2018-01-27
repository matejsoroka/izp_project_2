/*
 *    @file     proj2.c
 *    @author   Matej Soroka <xsorok02>
 *    @date     2017-10-25
 *    @brief    1BIT Project 2
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define CYAN "\x1b[36m"
#define CFRAC_ITERATIONS 10

/**
 *  Structure for Arguments
 */
struct Arguments
{
    bool help;
    bool tangens;
    bool distance;
    bool height;
};

/**
 *  Taylor polynome for tanges approx.
 *  @param x radius
 *  @param n number of iterations
 */
double taylor_tan(double x, unsigned n)
{

    double cit[30] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162, 18888466084, 113927491862, 58870668456604, 8374643517010684, 689005380505609448, 129848163681107301953., 1736640792209901647222., 418781231495293038913922.};
    double men[30] = {1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875, 1856156927625, 194896477400625, 49308808782358125, 3698160658676859375, 1298054391195577640625., 263505041412702261046875.};
    double s = 0;
    double xsqr = x*x;

    for(unsigned i = 0; i <= n; i++)
    {
        s += x * cit[i] / men[i];
        x *= xsqr;
    }

    return s;
}

/**
 *  Continued fraction for tanges approx.
 *  @param x radius
 *  @param n number of iterations
 */
double cfrac_tan(double x, unsigned int n)
{
    double cf = 0.;
    double a, b;

    for (unsigned i = n; i > 0; i--)
    {
        a = (i * 2 - 1) / x;
        b = 1;
        cf = b / (a - cf);
    }

    return cf;
}


/**
 *  Switching bool values in structure by arguments
 *  @param argc
 *  @param argv
 *  @param arguments
 */
void checkArguments(int argc, char *argv[], struct Arguments *arguments)
{
    arguments->help = false;
    arguments->tangens = false;
    arguments->distance = false;
    arguments->height = false;

    if(argc > 1)
    {
        for(int i = 1; i < argc; i++)
        {
            if(!strcmp(argv[i], "--help"))
                arguments->help = true;
            if(!strcmp(argv[i], "--tan"))
                arguments->tangens = true;
            if(!strcmp(argv[i], "-m"))
                arguments->distance = true;
            if(!strcmp(argv[i], "-c"))
                arguments->height = true;
        }
    }
}

/**
 *  Prints program instructions
 */
void printHelp() {
    printf(
    "\n======================================================================\n"
    "             Vypocet vysky objektu a vzdialenosti od neho\n"
    "             Autor: Matej Soroka <xsorok02>"
    "\n======================================================================\n\n"
    GREEN"\n--tan \n"RESET
    "porovna presnosti vypoctu tangens uhlu A (v radianech) mezi volanim\n"
    "tan z matematicke knihovny, a vypoctu tangens pomoci Taylorova polynomu\n"
    "a zretezeneho zlomku. Argumenty "CYAN"N"RESET" a "CYAN"M"RESET" udavaji, ve kterych iteracich \n"
    "iteracniho vypoctu ma porovnani probihat. 0 < N <= M < 14\n\n"
    GREEN"-m"RESET
    "\nvypocita a zmeri vzdalenosti.\n\n"
    "Uhel "CYAN"A"RESET" je dan v radianech. Program vypocita a vypise vzdalenost\n"
    "mereneho objektu. 0 < A <= 1.4 < Ï€/2.\n\n"
    "Uhel "CYAN"B"RESET" pokud je zadan, tak v radianech. Program vypocita a vypise\n"
    "i vysku mereneho objektu. 0 < B <= 1.4 < Ï€/2\n\n"
    "Argument "GREEN"-c"RESET" nastavuje vysku mericiho pristroje c pro vypocet. \n"
    "Vyska c je dana argumentem "CYAN"X"RESET" (0 < X <= 100). Argument je volitelny\n"
    "- implicitni vyska je 1.5 metru.\n\n"
    GREEN"EXAMPLES\n\n"RESET
    "./proj2 ["GREEN"-c"CYAN " X"RESET"] "GREEN"-m"RESET" "CYAN"A"RESET" ["CYAN"B"RESET"]\n\n"
    "./proj2 "GREEN"--tan"RESET" "CYAN"A N M"RESET"\n\n"
);
}

/**
 *  Checks if iterations are from interval
 *  @param j
 *  @param k
 */
bool checkIterations(int j, int k)
{

    bool interval = j > 0 && k < 14;
    if(!interval)
    {
        fprintf(stderr, "Numbers of iterations must be in interval (0, 14)\n");
        return false;
    }

    bool expression = j <= k;
    if(!expression)
    {
        fprintf(stderr, "Second iteration digit must be lower then first one\n");
        return false;
    }

    return true;
}

/**
 *  Checks if tangens paraeters are valid
 *  @param argc
 */
bool tangensError(int argc)
{
    bool error = false;
    if(argc < 3)
    {
        fprintf(stderr, "Radius of tangens is not set\n");
        error = true;
    }
    if(argc < 4)
    {
        fprintf(stderr, "First digit from iteration is not set\n");
        error = true;
    }
    if(argc < 5)
    {
        fprintf(stderr, "Second digit from iteration is not set\n");
        error = true;
    }
    return error;
}

/**
 *  Prints calculated iterations to stdout
 *  @param x radius
 *  @param j first digit of iteration
 *  @param k second digit of iteration
 */
void printIterations(double x, int j, int k)
{
    double m, t, te, c, ce;
    for (int i = 0; i <= k; i++)
    {

        m = tan(x);
        t = taylor_tan(x, i);
        te = fabs(m - t);
        c = cfrac_tan(x, i);
        ce = fabs(m - c);
        if((i >= j) && (i <= k))
            printf("%d %.10e %.10e %.10e %.10e %.10e\n", i, m, t, te, c, ce);

    }
}

/**
 *  Sets values for counting
 *  @param x pointer for setting radius
 *  @param j pointer for setting first digit of iteration
 *  @param k pointer for setting second digit of iteration
 *  @param argv
 */
bool setValues(double *x, int *j, int *k, char **argv)
{
    char *check;
    *x = strtod(argv[2], &check);

    if(strlen(check) > 0)
    {
        fprintf(stderr, "Radius is not a number\n");
        return false;
    }

    *j = strtod(argv[3], &check);

    if(strlen(check) > 0)
    {
        fprintf(stderr, "First number from iteration is not a number\n");
        return false;
    }

    *k = strtod(argv[4], &check);

    if(strlen(check) > 0)
    {
        fprintf(stderr, "Second number from iteration is not a number\n");
        return false;
    }

    return true;
}

/**
 *  Prints tangens approx. in interval of iterations
 *  @param argv
 *  @param j
 *  @param k
 */
bool printTangens(char *argv[], int argc) {

    int j, k;
    double x;

    if(tangensError(argc))
        return false;

    if(!setValues(&x, &j, &k, argv))
        return false;

    if(!checkIterations(j, k))
        return false;

    printIterations(x, j, k);

    return true;
}

/**
 *  Checking if height is in interval
 *  @param height
 *  @param errorCount
 */
bool checkHeight(double height, int *errorCount)
{
    if (height < 0 || height >= 100)
    {
        fprintf(stderr, "Height of object must be in interval (0, 100>\n");
        *errorCount += 1;
    }
    return height > 0 && height < 100;
}

/**
 *  Checking if radius is in interval
 *  @param radius
 *  @param errorCount
 */
bool checkRadius(double radius, int *errorCount)
{
    if (radius < 0 || radius > 1.4)
    {
        fprintf(stderr, "Radius must be in interval (0, 1.4>\n");
        *errorCount += 1;
    }
    return radius > 0 || radius <= 1.4;
}

/**
 *  Triggers functions for checking input data
 *  @param height
 *  @param alpha
 *  @param beta
 *  @param errorCount
 */
void checkValues(double height, double alpha, double beta, int *errorCount)
{
    checkHeight(height, &*errorCount);
    checkRadius(alpha, &*errorCount);
    checkRadius(beta, &*errorCount);
}

/**
 *  Returns beta value
 *  @param arguments
 *  @param argv
 *  @param argc
 */
double getBeta(struct Arguments *arguments, char **argv, int argc)
{
    double beta;
    char *check = "\0";

    if(arguments->height)
        beta = argc > 5 ? strtod(argv[5], &check) : 0;
    else
        beta = argc > 3 ? strtod(argv[3], &check) : 0;

    if(strlen(check) > 0)
    {
        fprintf(stderr, "Beta is not a number\n");
        return -1;
    }

    return beta;
}

/**
 *  Returns alpha value
 *  @param arguments
 *  @param argv
 */
double getHeight(struct Arguments *arguments, char **argv)
{
    char *check = "\0";
    double height = arguments->height ? strtod(argv[2], &check) : 1.5;
    if(strlen(check) > 0)
    {
        fprintf(stderr, "Height is not a number\n");
        return -1;
    }
    return height;
}

/**
 *  Returns alpha value
 *  @param arguments
 *  @param argv
 */
double getAlpha(struct Arguments *arguments, char **argv)
{
    char *check = "\0";
    double alpha = arguments->height ? strtod(argv[4], &check) : strtod(argv[2], &check);
    if(strlen(check) > 0)
    {
        fprintf(stderr, "Alpha is not a number\n");
        return -1;
    }
    return alpha;
}

/**
 *  Prints calculated distance and heights
 *  @param alpha
 *  @param beta
 *  @param height
 */
void distanceOutput(double alpha, double beta, double height)
{
    double distance = height / cfrac_tan(alpha, CFRAC_ITERATIONS);
    printf("%.10e\n", distance);

    if(beta)
    {
        double objectHeight = cfrac_tan(beta, CFRAC_ITERATIONS) * distance + height;
        printf("%.10e\n", objectHeight);
    }
}

/**
 *  Prints distance by tangens and height
 *  @param argc
 *  @param argv
 *  @param arguments
 */
bool printDistance(int argc, char **argv, struct Arguments *arguments)
{
    if(argc < 3)
    {
        fprintf(stderr, "Alpha is not set\n");
        return false;
    }

    double alpha = getAlpha(arguments, argv);
    double beta = getBeta(arguments, argv, argc);
    double height = getHeight(arguments, argv);

    if(alpha < 0 || beta < 0 || height < 0)
        return false;

    int errorCount = 0;
    checkValues(height, alpha, beta, &errorCount);

    if(errorCount)
        return false;

    distanceOutput(alpha, beta, height);

    return true;
}

/**
 *  Main function
 *  @param argc count of arguments
 *  @param argv array of arguments
 */
int main(int argc, char *argv[])
{

    struct Arguments arguments;
    bool error = 0;               // error handler

    checkArguments(argc, argv, &arguments);

    if(arguments.help)
        printHelp();

    else if(arguments.tangens)
    {
        error = printTangens(argv, argc);
        if(error) return 1;
    }

    else if(arguments.distance)
    {
        error = printDistance(argc, argv, &arguments);
        if(error) return 1;
    }

    else
    {
        printf("Program has no arguments to work with, use --help for more instructions\n");
    }

    return 0;
}
