from setuptools import setup, Extension
from Cython.Build import cythonize
import numpy as np

extensions = [
    "wrapper_avx",
    sources=["wrapper_avx_pyx"],
    language="c++",
    extra_compile_args=["-march=native"m "-O3"],
]

setup(
    name="WrapperAVX",
    ext_modules=cythonize(extensions),
    include_dirs=[np.get_include]
)