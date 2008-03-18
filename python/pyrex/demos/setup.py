from distutils.core import setup
#from distutils.extension import Extension
from Pyrex.Distutils.extension import Extension
from Pyrex.Distutils import build_ext

setup(
    name='Demos',
    ext_modules=[
        Extension("primes", ["primes.pyx"]),
        ],
    cmdclass={'build_ext':build_ext}
)

