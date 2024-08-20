#include <stdio.h>
#include <math.h>
#include <assert.h>

enum Num_of_roots{
    NEGATIVE_D = -1,
    NO_ROOTS,
    ONE_ROOT,
    TWO_ROOTS,
    ANY_ROOT
};

const double EPS = 1e-9;

int get_square_root(double, double, double, double*, double*);

int is_equal (double, double);

void print_roots(int, double*, double*);

void get_coefs(double *, char*);

int main(void)
{
    double coefs[3], x1=0, x2=0;
    char coef_names[] = "abc";
    get_coefs(coefs, coef_names);
    int roots = get_square_root(coefs[0], coefs[1], coefs[2], &x1, &x2);
    print_roots(roots, &x1, &x2);
    return 0;
}

int get_square_root(double a, double b, double c, double *x1, double *x2)
{
    if (is_equal(a, 0)){
        if(is_equal(b, 0))
            return (is_equal(c, 0)) ? 3 : 0;
        *x1 = -c/b;
        return 1;
    }
    double d = b*b-4*a*c;
    if (d<0)
        return -1;
    *x1 = (-b + sqrt(d))/(2*a);
    *x2 = (-b - sqrt(d))/(2*a);
    if (is_equal(d, 0))
        return 1;
    return 2;
}

int is_equal(double a, double b)
{
    double dif = a-b;
    if (dif<0)
        dif = -dif;
    if (dif<EPS)
        return 1;
    return 0;
}

void print_roots(int roots, double *x1, double *x2)
{
    switch(roots){
        case NEGATIVE_D:
            printf("d<0");
            break;
        case NO_ROOTS:
            printf("no roots");
            break;
        case ONE_ROOT:
            printf("x = %lg", *x1);
            break;
        case TWO_ROOTS:
            printf("x1 = %lg, X2 = %lg", *x1, *x2);
            break;
        case ANY_ROOT:
            printf("any root");
            break;
        default:
            puts("print error");
            assert(0);
    }
}

void get_coefs(double* coefs, char* coef_names)
{
    for (int i=0;i<3; i++){
        printf("print coefficient %c ", coef_names[i]);
        int err = scanf("%lg", &coefs[i]);
        assert(err == 1);
        char ch = getchar();
        while (ch != '\n' && ch != '\0')
            ch = getchar();
    }
}
