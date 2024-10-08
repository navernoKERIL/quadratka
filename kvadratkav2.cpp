#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#define TESTS 1

enum Num_of_roots{
    NO_ROOTS = 0,
    ONE_ROOT,
    TWO_ROOTS,
    ANY_ROOT,
    LESS = -1,
    EQUAL,
    GREATER
};

const double EPS = 1e-9;

int  get_square_root(double*, double*, double*);
void print_roots(int, double*, double*);
void get_coefs(double*);
void get_coef(double*, char*);
bool is_fin(double*, int);
void buff_clear(void);
void main_testings();
int  testing(int, double *, double, double, int);
int  compare_doubles(double, double);
int  symbol_search(int);
int  get_lin_root(double*, double*, double*);
void print_equation(double *);
bool compare_pairs(double, double, double, double);


int main(void)
{
#if TESTS == 1
    main_testings();
#else
    double coefs[3] = {}, x1 = 0, x2 = 0;

    get_coefs(coefs);

    int roots = get_square_root(coefs, &x1, &x2);

    print_roots(roots, &x1, &x2);
#endif
    return 0;
}

int get_square_root(double *coefs, double *x1, double *x2)
{
    assert(coefs!=0 && x1!=0 && x2!=0);
    assert(is_fin(coefs, 3));

    double a = coefs[0], b = coefs[1], c = coefs[2];
    if (compare_doubles(a, 0)==EQUAL)
        get_lin_root(coefs, x1, x2);

    else{
        double d = b*b - 4*a*c;

        if (compare_doubles(d, 0)==LESS)
            return NO_ROOTS;
        *x1 = (-b + sqrt(d))/(2*a);
        *x2 = (-b - sqrt(d))/(2*a);

        if (compare_doubles(d, 0)==EQUAL)
            return ONE_ROOT;

        return TWO_ROOTS;
    }
}

void print_roots(int roots, double *x1, double *x2)
{
    assert(x1!=0 && x2!=0 && x1!=x2);
    switch(roots){
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
    }
}

void get_coefs(double *coefs)
{
    assert(coefs!=0);

    puts("ax^2 + bx + c = 0");
    get_coef(&coefs[0], "a");
    get_coef(&coefs[1], "b");
    get_coef(&coefs[2], "c");

    print_equation(coefs);
}

void print_equation(double *coefs)
{
    assert(coefs != 0);

    int i = 0;
    if (compare_doubles(coefs[0], 0)!=EQUAL)
        i += printf("%lgx^2 ", coefs[0]);
    if (compare_doubles(coefs[1], 0)!=EQUAL)
        i += printf("%c %lgx ", (compare_doubles(coefs[1], 0)==GREATER) ? ('+') : ('-'), fabs(coefs[1]));
    if (compare_doubles(coefs[2], 0)!=EQUAL)
        i += printf("%c %lg ",  (compare_doubles(coefs[2], 0)==GREATER) ? ('+') : ('-'), fabs(coefs[2]));
    if (i==0)
                printf("0 ");
    puts("= 0");
}

void get_coef(double *coef, char *str)
{
    assert(coef!=0 && str!=0);

    for(int i = 0; i < 1; i++){
        printf("write coefficient %s ", str);

        scanf("%lg", coef);

        i = symbol_search(i);
    }
}

bool is_fin(double *coefs, int n)
{
    assert(coefs!=0);

    for (int i = 0; i < n; i++)
        if (isfinite(coefs[i]) == 0)
            return false;
    return true;
}

void buff_clear(void)
{
    char ch = 0;
    while(ch!='\n' && ch!=EOF)
        ch = getchar();
}

int testing(int n, double *coefs, double exp1, double exp2, int exproots)
{
    assert(coefs != 0);

    double x1 = 0, x2 = 0;
    int roots = get_square_root(coefs, &x1, &x2);
    if (compare_doubles(roots, exproots)!= EQUAL ||
       compare_pairs(x1, x2, exp1, exp2) == false){
        printf("test %d failed, roots = %d, x1, x2: %lg, %lg, expected: %lg, %lg\n",
         n, roots, x1, x2, exp1, exp2);
        return n;
    }
    return 0;
}

void main_testings()
{
    double abc_x1_x2 [][5] = {{1,   2,  -3,   1, -3},
                              {0,   0,   0,   0,  0},
                              {1.1, 0,   0,   0,  0},
                              {0,   2.5, 0,   0,  0},
                              {1.1, 1.2, 7.5, 0,  0}};
    int roots[5] = {2, 3, 1, 1, 0};
    int err = 0;

    for (int i = 0; i < 5 && err==0; i++)
        err = testing((i + 1), abc_x1_x2[i], abc_x1_x2[i][3], abc_x1_x2[i][4], roots[i]);
    if (err==0)
        printf("no errors");
}

int compare_doubles(double a, double b)
{
    double dif = a - b;

    if (fabs(dif) < EPS)
        return EQUAL;
    if (dif>0)
        return GREATER;
    return LESS;
}

int symbol_search(int i)
{
    char ch  = getchar();
    if (ch!='\n' && ch!='\0' && ch!=EOF){
        if (isspace(ch)==0){
            printf("wrong symbol (%c)\n", ch);
            i--;
        }
        buff_clear();
    }
    return i;
}

int get_lin_root(double *coefs, double *x1, double *x2)
{
    assert(coefs != 0 && x1 != 0 && x2 != 0 && x1 != x2);

    double b = coefs[1], c = coefs[2];
#if TESTS == 0
    puts("not a quadratic equation");
#endif
    if(compare_doubles(b, 0)==EQUAL)
        return (compare_doubles(c, 0)==EQUAL) ? ANY_ROOT : NO_ROOTS;
    *x1 = -c/b;
    return ONE_ROOT;
}

bool compare_pairs(double a1, double a2, double b1, double b2)
{
    if ((compare_doubles(a1, b1)==EQUAL && compare_doubles(a2, b2)==EQUAL)
     || (compare_doubles(a1, b2)==EQUAL && compare_doubles(a2, b1)==EQUAL))
        return true;
    return false;
}
