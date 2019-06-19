#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue May  7 15:42:00 2019

@author: gionuno
"""

from distutils.core import setup;
from distutils.extension import Extension;
from Cython.Build import cythonize;

e_modules = cythonize([Extension("libdt",["libdt.pyx"],extra_compile_args=['-std=c++14'],libraries=[],language="c++")]);
setup(name="libdt",ext_modules = e_modules)