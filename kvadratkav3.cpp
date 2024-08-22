#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#define TESTS 0

#define CHECK_ERROR(error)\
{                         \
if (error != OK)          \
{                         \
    check_errors(error);  \
    return error;         \
}                         \
}                         \

struct Data
{
    double a;
    double b;
    double c;

    double x1;
    double x2;

    int roots;
    int n;
};

enum Num_of_roots{
    NO_ROOTS = 0,
    ONE_ROOT,
    TWO_ROOTS,
    ANY_ROOT
};

enum Compare{
    LESS = -1,
    EQUAL,
    GREATER
};

enum Error_codes{
    OK = 0,
    COEF_INPUT,
    PRINT_ERR
};

const double EPS = 1e-9;

Error_codes get_square_root(Data *);
Error_codes get_lin_root(Data *, double, double);
Error_codes get_coefs(Data *);
Compare compare_doubles(double, double);
void check_errors(int);
void print_roots(Data *);
void get_coef(double*, char*);
void buff_clear(void);
void main_testings();
int  one_test(Data *);
int  symbol_verification(int);
void print_equation(Data *);
bool compare_pairs(double, double, double, double);
void struct_initialize(Data *);
void test_err(Data *, double, double);





int main(void)
{
#if TESTS
    main_testings();
#else
    Data coefs = {0, 0, 0, 0, 0, 0, 0};
    struct_initialize(&coefs);
    int error = get_coefs(&coefs);
    CHECK_ERROR(error);

    error = get_square_root(&coefs);
    CHECK_ERROR(error);

    print_roots(&coefs);
    CHECK_ERROR(error);
#endif
    return 0;
}





Error_codes get_coefs(Data *coefs)
{
    assert(coefs != nullptr);

    puts("ax^2 + bx + c = 0");
    get_coef(&coefs->a, "a");
    get_coef(&coefs->b, "b");
    get_coef(&coefs->c, "c");

    if (coefs->a == NAN || coefs->b == NAN || coefs->c == NAN){
        check_errors(COEF_INPUT);
        return COEF_INPUT;
    }

    print_equation(coefs);
    return OK;
}

void get_coef(double *coef, char *str)
{
    assert(coef != nullptr && str != nullptr);

    for(int i = 0; i < 1; i++){
        printf("write coefficient %s ", str);

        scanf("%lg", coef);

        i = symbol_verification(i);
    }
}

int symbol_verification(int i)
{
    char ch  = getchar();
    if (ch!='\n' && ch != EOF){
        if (isspace(ch) == 0){
            printf("wrong symbol (%c)\n", ch);
            i--;
        }
        buff_clear();
    }
    return i;
}

inline void buff_clear(void)
{
    char ch = 0;
    while(ch!='\n' && ch!=EOF)
        ch = getchar();
}





Error_codes get_square_root(Data *coefs)
{
    assert(coefs != nullptr);

    double a = coefs->a;
    double b = coefs->b;
    double c = coefs->c;

    if (compare_doubles(a, 0) == EQUAL)
        get_lin_root(coefs, b, c);

    else{
        double d = b*b - 4*a*c;

        if (compare_doubles(d, 0) == LESS){
            coefs->roots = NO_ROOTS;
            return OK;
        }

        coefs->x1 = (-b + sqrt(d))/(2*a);
        coefs->x2 = (-b - sqrt(d))/(2*a);

        if (compare_doubles(d, 0) == EQUAL){
            coefs->roots = ONE_ROOT;
            return OK;
            }

        coefs->roots = TWO_ROOTS;
        return OK;
    }
}

Error_codes get_lin_root(Data *coefs, double b, double c)
{
    assert(coefs != nullptr);

    #if ! TESTS
    puts("not a quadratic equation");

    #endif

    if(compare_doubles(coefs->b, 0) == EQUAL){
        coefs->roots = (compare_doubles(coefs->c, 0) == EQUAL) ? ANY_ROOT : NO_ROOTS;
        return OK;
    }

    coefs->x1 = -c/b;
    coefs->x2 = -c/b;

    coefs->roots = ONE_ROOT;
    return OK;
}






void print_equation(Data *coefs)
{
    assert(coefs != nullptr);

    int i = 0;
    if (compare_doubles(coefs->a, 0) != EQUAL)
        i += printf("%lgx^2", coefs->a);
    if (compare_doubles(coefs->b, 0) != EQUAL)
        i += printf("%+lgx",  coefs->b);
    if (compare_doubles(coefs->c, 0) != EQUAL)
        i += printf("%+lg", coefs->c);
    if (i==0)
        printf("0");
    puts(" = 0");
}

void print_roots(Data *coefs)
{
    assert(coefs != nullptr);

    switch(coefs->roots){
        case NO_ROOTS:
            printf("no roots");
            break;
        case ONE_ROOT:
            printf("x = %lg", coefs->x1);
            break;
        case TWO_ROOTS:
            printf("x1 = %lg, X2 = %lg", coefs->x1, coefs->x2);
            break;
        case ANY_ROOT:
            printf("any root");
            break;
        default:
            check_errors(PRINT_ERR);
    }
}





Compare compare_doubles(double a, double b)
{
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
    coefs->n     = NAN;
    coefs->roots = NAN;
}

void check_errors(int err_code)
{
    switch(err_code)
    {
        case COEF_INPUT:
            printf("error with input coefficients");
        case PRINT_ERR:
            printf("error with print answer");
        default:
            printf("? error");

    }
}





#if TESTS
int one_test(Data *coefs)
{
    assert(coefs != nullptr);

    double exp1 = coefs->x1, exp2 = coefs->x2;
    int exproots = coefs->roots;

    if (compare_doubles(coefs->roots, exproots) != EQUAL ||
       compare_pairs(coefs->x1, coefs->x2, exp1, exp2) == false)
    {

        test_err(coefs, exp1, exp2);

        return coefs->n;
    }
    return OK;
}

void main_testings()
{
    int err = 0;
    Data test1 = {1,   2,  -3,
                  1,   -3,
                  1,   2};
    Data test2 = {0,   0,   0,
                  0,   0,
                  2,   3};
    Data test3 = {1.1, 0,   0,
                  0,   0,
                  3,   1};
    Data test4 = {0,   2.5, 0,
                  0,   0,
                  4,   1};
    Data test5 = {1.1, 1.2, 7.5,
                  0,   0,
                  5,   0};

    err = one_test(&test1);
    if (err == 0)
        err = one_test(&test2);
    if (err == 0)
        err = one_test(&test3);
    if (err == 0)
        err = one_test(&test4);
    if (err == 0)
        err = one_test(&test5);
    if (err == 0)
        printf("no errors");
}

inline void test_err(Data *coefs, double exp1, double exp2)
{
    assert(coefs != nullptr);

    printf("test %d failed, roots = %d, x1, x2: %lg, %lg, expected: %lg, %lg\n",
         coefs->n, coefs->roots, coefs->x1, coefs->x2, exp1, exp2);
}

#endif
