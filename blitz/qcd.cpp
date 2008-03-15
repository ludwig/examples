/* From http://www.oonumerics.org/blitz/examples/Blitz++/qcd.cpp
 *
 * This example illustrates the TinyMatrix<T,N,M> class.  TinyMatrix can be
 * used for small matrices whose dimensions are known at compile time.  The
 * application is from lattice quantum chromodynamics.
 *
 * See: S. Booth, Lattice QCD simulation programs on the Cray T3D,
 * Edinburgh Parallel Computing Centre Technical Report EPCC-TR96-03.
 * http://www.epcc.ed.ac.uk/t3d/documents/techreports.html
 */

#include <blitz/tinymat.h>
#include <blitz/vector.h>

using namespace blitz;

/*
 * This example is drawn from a technical report by Stephen Booth
 * of the Edinburgh Parallel Computing Centre.  In his group's implementation
 * of lattice QCD, most of the CPU time is taken up by a group of routines
 * which multiplies 2-spinors by SU(3) gauge elements.  In Fortran, this
 * code looks like:
 *
 *   COMPLEX M(V,3,3), res(V,3,2), src(V,3,2)
 *   DO spin=1,2
 *       DO col=1,3
 *           DO site=1,V
 *               res(site,col,spin)=
 *                        M(site,col,1) * src(site,1,spin)
 *                      + M(site,col,2) * src(site,2,spin)
 *                      + M(site,col,3) * src(site,3,spin)
 *           END DO
 *       END DO
 *   END DO
 *
 * This is a situation in which TinyMatrix can be used to good effect:
 * each element of `src' is a 3x2 complex matrix, and each element
 * of `M' is a 3x3 complex matrix.
 */


typedef TinyMatrix<complex<double>, 3, 3> gauge;
typedef TinyMatrix<complex<double>, 3, 2> spinor;


/*
 * The following two-line function is equivalent to the Fortran
 * version above.
 */

inline void qcdUpdate(Vector<spinor>& res,
                      Vector<gauge>& M,
                      Vector<spinor>& src)
{
    for (int i = 0; i < res.length(); ++i)
    {
        res[i] = product(M[i], src[i]);
    }
}


int main(void)
{
    const int numElements = 100;    // Some number of elements

    Vector<gauge> M(numElements);
    Vector<spinor> res(numElements), src(numElements);

    qcdUpdate(res, M, src);

    return 0;
}

