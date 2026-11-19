from setuptools import setup, Extension
from Cython.Build import cythonize
import numpy as np

extensions = [
    Extension(
    "TareaCython",
    sources=["TareaCython.pyx"],
    language="c++",
    extra_compile_args=["-march=native", "-O3"],
    define_macros=[("CYTHON_COMPILING", "1")],
    )
]

setup(
    name="TareaCython",
    ext_modules=cythonize(extensions),
    include_dirs=[np.get_include()]
)