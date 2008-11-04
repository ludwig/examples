from distutils.core import setup, Extension
import sipdistutils

setup(
    name = 'hello',
    version = '1.0',
    ext_modules = [
        Extension("hello",
            sources=["hello.sip", "hello.cpp"],
            include_dirs=['.'],
            ),
        ],
    cmdclass = {'build_ext': sipdistutils.build_ext}
    )
