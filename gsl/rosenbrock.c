// http://www.gnu.org/software/gsl/manual/html_node/Example-programs-for-Multidimensional-Root-finding.html

#include <stdlib.h>
#include <stdio.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_multiroots.h>


struct rparams
{
    double a;
    double b;
};

int rosenbrock_f(const gsl_vector *x, void *params, gsl_vector *f)
{
    double a = ((struct rparams *) params)->a;
    double b = ((struct rparams *) params)->b;

    const double x0 = gsl_vector_get(x, 0);
    const double x1 = gsl_vector_get(x, 1);

    const double y0 = a * (1 - x0);
    const double y1 = b * (x1 - x0 * x0);

    gsl_vector_set(f, 0, y0);
    gsl_vector_set(f, 1, y1);

    return GSL_SUCCESS;
}

int rosenbrock_df(const gsl_vector *x, void *params, gsl_matrix *J)
{
    const double a = ((struct rparams *) params)->a;
    const double b = ((struct rparams *) params)->b;

    const double x0 = gsl_vector_get(x, 0);

    const double df00 = -a;
    const double df01 = 0;
    const double df10 = -2 * b * x0;
    const double df11 = b;

    gsl_matrix_set(J, 0, 0, df00);
    gsl_matrix_set(J, 0, 1, df01);
    gsl_matrix_set(J, 1, 0, df10);
    gsl_matrix_set(J, 1, 1, df11);

    return GSL_SUCCESS;
}

int rosenbrock_fdf(const gsl_vector *x, void *params, gsl_vector *f, gsl_matrix *J)
{
    rosenbrock_f(x, params, f);
    rosenbrock_df(x, params, J);

    return GSL_SUCCESS;
}


int print_state(size_t iter, gsl_multiroot_fsolver *s)
{
    printf("iter = %3u  x = % .3f % .3f  f(x) = % .3e % .3e\n",
        iter,
        gsl_vector_get(s->x, 0),
        gsl_vector_get(s->x, 1),
        gsl_vector_get(s->f, 0),
        gsl_vector_get(s->f, 1));
}

int print_state2(size_t iter, gsl_multiroot_fdfsolver *s)
{
    printf("iter = %3u  x = % .3f % .3f  f(x) = % .3e % .3e\n",
        iter,
        gsl_vector_get(s->x, 0),
        gsl_vector_get(s->x, 1),
        gsl_vector_get(s->f, 0),
        gsl_vector_get(s->f, 1));
}


int solve_with_hybrids(void)
{
    const gsl_multiroot_fsolver_type *T;
    gsl_multiroot_fsolver *s;

    int status;
    size_t i, iter;

    const size_t n = 2;

    struct rparams p = {1.0, 10.0};
    gsl_multiroot_function f = {&rosenbrock_f, n, &p};

    double x_init[2] = {-10.0, -5.0};
    gsl_vector *x = gsl_vector_alloc(n);

    gsl_vector_set(x, 0, x_init[0]);
    gsl_vector_set(x, 1, x_init[1]);

    T = gsl_multiroot_fsolver_hybrids;
    s = gsl_multiroot_fsolver_alloc(T, 2);
    gsl_multiroot_fsolver_set(s, &f, x);

    printf("solving rosenbrock system using hybrids method\n");

    iter = 0;
    print_state(iter, s);

    do {
        iter++;

        status = gsl_multiroot_fsolver_iterate(s);

        print_state(iter, s);

        /* check if solver is stuck */
        if (status) break;

        status = gsl_multiroot_test_residual(s->f, 1e-7);

    } while ((status == GSL_CONTINUE) && (iter < 1000));

    printf("status = %s\n", gsl_strerror(status));
    printf("\n");

    gsl_multiroot_fsolver_free(s);
    gsl_vector_free(x);

    return 0;
}

int solve_with_gnewton(void)
{
    const gsl_multiroot_fdfsolver_type *T;
    gsl_multiroot_fdfsolver *s;

    int status;
    size_t i, iter;

    const size_t n = 2;
    struct rparams p = {1.0, 10.0};
    gsl_multiroot_function_fdf f = {&rosenbrock_f,
                                    &rosenbrock_df,
                                    &rosenbrock_fdf,
                                    n, &p};

    double x_init[2] = {-10.0, -5.0};
    gsl_vector *x = gsl_vector_alloc(n);

    gsl_vector_set(x, 0, x_init[0]);
    gsl_vector_set(x, 1, x_init[1]);

    T = gsl_multiroot_fdfsolver_gnewton;
    s = gsl_multiroot_fdfsolver_alloc(T, n);
    gsl_multiroot_fdfsolver_set(s, &f, x);

    printf("solving rosenbrock system using gnewton method\n");

    iter = 0;
    print_state2(iter, s);

    do {
        iter++;

        status = gsl_multiroot_fdfsolver_iterate(s);

        print_state2(iter, s);

        if (status) break;

        status = gsl_multiroot_test_residual(s->f, 1e-7);

    } while ((status == GSL_CONTINUE) && (iter < 1000));

    printf("status = %s\n", gsl_strerror(status));
    printf("\n");

    gsl_multiroot_fdfsolver_free(s);
    gsl_vector_free(x);

    return 0;
}

int main(void)
{
    printf("\n");
    solve_with_hybrids();
    solve_with_gnewton();
    return 0;
}
