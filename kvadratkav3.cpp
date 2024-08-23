#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>

#define TESTS 1

#define CHECK_ERROR(error)    \
do                            \
{                             \
    if (error != OK)          \
    {                         \
        check_errors(error);  \
        return error;         \
    }                         \
}while (0)                    \

struct Data
{
    double a;
    double b;
    double c;

    double x1;
    double x2;

    int roots;
};

enum NumOfRoots{
    NO_ROOTS = 0,
    ONE_ROOT,
    TWO_ROOTS,
    ANY_ROOT
};

enum Compare{
    LESS = -1,
    EQUAL,
    GREATER,
    NaN
};

enum ErrorCodes{
    OK = 0,
    COEF_INPUT,
    PRINT_ERR,
    BUFF_ERR,
    NAN_X,
    NO_COEF
};

const double EPS = 1e-9;

ErrorCodes get_square_root(Data *);
ErrorCodes get_lin_root(Data *);
ErrorCodes get_coefs(Data *);
ErrorCodes print_roots(Data *);
ErrorCodes buff_clear(void);
ErrorCodes get_coef(double*, char*);
ErrorCodes symbol_verification();
Compare compare_doubles(double, double);
void check_errors(int);
void main_testings();
int  one_test(int, Data *);
void print_equation(Data *);
bool compare_pairs(double, double, double, double);
void struct_initialize(Data *);
void test_err(int, Data *, double, double);





int main(void)
{
#if TESTS
    main_testings();
#else
    Data coefs = {};
    struct_initialize(&coefs);

    int error = OK;

    error = get_coefs(&coefs);
    CHECK_ERROR(error);

    error = get_square_root(&coefs);
    CHECK_ERROR(error);

    error = print_roots(&coefs);
    CHECK_ERROR(error);
#endif
    return OK;
}


//-------------------------------------------------------------------------------


ErrorCodes get_coefs(Data *coefs)
{
    assert(coefs != nullptr);

    double a = NAN, b = NAN, c = NAN;

    puts("ax^2 + bx + c = 0");

    ErrorCodes err = get_coef(&a, "a");
    if (err)
        return err;

    err = get_coef(&b, "b");
    if (err)
        return err;

    err = get_coef(&c, "c");
    if (err)
        return err;

    if (isnan(a) || isnan(b) || isnan(c))
        return COEF_INPUT;

    coefs->a = a;
    coefs->b = b;
    coefs->c = c;

    print_equation(coefs);
    return OK;
}

ErrorCodes get_coef(double *coef, char *str)
{
    assert(coef != nullptr);
    assert(str  != nullptr);

    int i = 0;
    for(; i < 3; i++){
        printf("write coefficient %s ", str);

        scanf("%lg", coef);

        int result = symbol_verification();
        if (result == BUFF_ERR)
            return BUFF_ERR;
        if (result == OK)
            break;
    }
    if (i == 3)
        return NO_COEF;
    return OK;
}

ErrorCodes symbol_verification()
{
    char ch  = getchar();
    if (ch!='\n' && ch != EOF){
        if (isspace(ch) == 0)
            printf("wrong symbol (%c)\n", ch);

        if (buff_clear() == BUFF_ERR)
            return BUFF_ERR;
        return NO_COEF;
    }
    return OK;
}

ErrorCodes buff_clear(void)
{
    int ch       = 0;
    int symb_amt = 0;
    while(ch != '\n' && ch != EOF){
        ch = getchar();
        if (symb_amt++ > 1000)
            return BUFF_ERR;
        }
    return OK;
}


//-------------------------------------------------------------------------------


ErrorCodes get_square_root(Data *coefs)
{
    assert(coefs != nullptr);

    double a = coefs->a;
    double b = coefs->b;
    double c = coefs->c;

    if (compare_doubles(a, 0) == EQUAL)
        get_lin_root(coefs);

    else{
        double d = b*b - 4*a*c;

        if (compare_doubles(d, 0) == LESS){
            coefs->roots = NO_ROOTS;
            return OK;
        }

        coefs->x1 = (-b + sqrt(d))/(2*a);

        if (compare_doubles(d, 0) == EQUAL){
            coefs->roots = ONE_ROOT;
            return OK;
            }

        coefs->x2 = (-b - sqrt(d))/(2*a);

        coefs->roots = TWO_ROOTS;
        return OK;
    }
}

ErrorCodes get_lin_root(Data *coefs)
{
    assert(coefs != nullptr);

    double b = coefs->b;
    double c = coefs->c;

    #if ! TESTS
    puts("not a quadratic equation");
    #endif

    if(compare_doubles(b, 0) == EQUAL){
        coefs->roots = (compare_doubles(c, 0) == EQUAL) ? ANY_ROOT : NO_ROOTS;

        return OK;
    }

    coefs->x1    = -c/b;
    coefs->roots = ONE_ROOT;

    return OK;
}



//-------------------------------------------------------------------------------


void print_equation(Data *coefs)
{
    assert(coefs != nullptr);

    int coefs_amt = 0;
    if (compare_doubles(coefs->a, 0) != EQUAL)
        coefs_amt += printf("%lgx^2", coefs->a);

    if (compare_doubles(coefs->b, 0) != EQUAL)
        coefs_amt += printf("%+lgx",  coefs->b);

    if (compare_doubles(coefs->c, 0) != EQUAL)
        coefs_amt += printf("%+lg",   coefs->c);

    if (coefs_amt==0)
        printf("0");

    puts(" = 0");
}

ErrorCodes print_roots(Data *coefs)
{
    assert(coefs != nullptr);

    switch(coefs->roots){
        case NO_ROOTS:
            printf("no roots");
            break;
        case ONE_ROOT:
            if (isnan(coefs->x1))
                return NAN_X;
            printf("x = %lg", coefs->x1);
            break;
        case TWO_ROOTS:
            if (isnan(coefs->x1) || isnan(coefs->x2))
                return NAN_X;
            printf("x1 = %lg, X2 = %lg", coefs->x1, coefs->x2);
            break;
        case ANY_ROOT:
            printf("any root");
            break;
        default:
            return PRINT_ERR;
    }
    return OK;
}


//-------------------------------------------------------------------------------


Compare compare_doubles(double a, double b)
{
    if (isnan(a))
        return (isnan (b)) ? EQUAL : NaN;

    double dif = a - b;

    if (fabs(dif) < EPS)
        return EQUAL;
    if (dif > 0)
        return GREATER;
    return LESS;
}

inline bool compare_pairs(double a1, double a2, double b1, double b2)
{
    if ((compare_doubles(a1, b1) == EQUAL && compare_doubles(a2, b2) == EQUAL)
     || (compare_doubles(a1, b2) == EQUAL && compare_doubles(a2, b1) == EQUAL))
        return true;
    return false;
}

void struct_initialize(Data *coefs)
{
    assert(coefs != nullptr);

    coefs->a     = NAN;
    coefs->b     = NAN;
    coefs->c     = NAN;
    coefs->x1    = NAN;
    coefs->x2    = NAN;
    coefs->roots = NAN;
}

void check_errors(int err_code)
{
    switch(err_code)
    {
        case COEF_INPUT:
            fprintf(stderr, "error with input coefficients");
            break;
        case PRINT_ERR:
            fprintf(stderr, "error with print answer");
            break;
        case BUFF_ERR:
            fprintf(stderr, "buff error or to much symbols");
            break;
        case NAN_X:
            fprintf(stderr, "x is NaN");
            break;
        case NO_COEF:
            fprintf(stderr, "to much attempts");
            break;
        default:
            fprintf(stderr, "? error");
    }
}



 //-------------------------------------------------------------------------------


int one_test(int n_test, Data *coefs)
{
    assert(coefs != nullptr);

    double exp1 = coefs->x1, exp2 = coefs->x2;
    coefs->x1   = NAN;
    coefs->x2   = NAN;

    int exproots = coefs->roots;
    coefs->roots = NAN;

    get_square_root(coefs);

    if (coefs->roots != exproots || compare_pairs(coefs->x1, coefs->x2, exp1, exp2) == false)
    {

        test_err(n_test, coefs, exp1, exp2);

        return n_test;
    }
    return OK;
}

void main_testings()
{
    int     err = 0;
    const int test_amt = 5;
//                           a    b    c    x1    x2  roots
    Data tests[test_amt] = {{1,   2,  -3,   1,   -3,    2},
                            {0,   0,   0,   NAN,  NAN,  3},
                            {1.1, 0,   0,   0,    NAN,  1},
                            {0,   2.5, 0,   0,    NAN,  1},
                            {1.1, 1.2, 7.5, NAN,  NAN,  0}};


    for (int i = 0; i < test_amt; i++)
        err += one_test((i+1), &tests[i]);

    if (err == OK)
        printf("all tests completed");
}

inline void test_err(int n_test, Data *coefs, double exp1, double exp2)
{
    assert(coefs != nullptr);

    printf("test %d failed, roots = %d, x1, x2: %lg, %lg, expected: %lg, %lg\n",
         n_test, coefs->roots, coefs->x1, coefs->x2, exp1, exp2);
}
