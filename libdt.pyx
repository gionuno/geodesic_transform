#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue May  7 15:27:20 2019

@author: gionuno
"""

import cython;
from libcpp.vector cimport vector;
import numpy as np;

cdef extern from "libdt.hpp":
    cdef vector[vector[double]] dist_trans_cpp(vector[vector[vector[double]]] &,vector[vector[double]] &,double);

def dist_trans(I,S,alpha):
    return np.array(dist_trans_cpp(I,S,alpha));

