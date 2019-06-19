#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun May  5 20:59:42 2019

@author: gionuno
"""

import numpy as np;
import numpy.random as rd;

from libdt import dist_trans;

import matplotlib.pyplot as plt;
import matplotlib.image  as img;

X = img.imread("lizard500.jpg")/255.0;
#"""
mX = np.mean(X,axis=(0,1));
cX = 1e-4*np.eye(3);
for i in range(X.shape[0]):
    for j in range(X.shape[1]):
        aX = np.reshape(X[i,j,:],(-1,))-mX;
        cX += np.outer(aX,aX)/(X.shape[0]*X.shape[1]);

[eX,vX] = np.linalg.eig(cX);
ieX = 1.0/np.sqrt(1e-15+eX);
wX = np.zeros(X.shape);
for i in range(X.shape[0]):
    for j in range(X.shape[1]):
        aX = ieX*np.dot(vX.T,np.reshape(X[i,j,:],(-1,))-mX);        
        wX[i,j,:] = aX;

sX = np.zeros((wX.shape[0],wX.shape[1]));
sX[98,178] = 1.0;

dX = dist_trans(wX,sX,0.995);

rwX = np.copy(wX);
rwX[:,:,0] -= np.min(wX[:,:,0]);
rwX[:,:,1] -= np.min(wX[:,:,1]);
rwX[:,:,2] -= np.min(wX[:,:,2]);
rwX[:,:,0] /= np.max(rwX[:,:,0])+1e-14;
rwX[:,:,1] /= np.max(rwX[:,:,1])+1e-14;
rwX[:,:,2] /= np.max(rwX[:,:,2])+1e-14;

f,ax = plt.subplots(1,3);
ax[0].imshow(X);
ax[0].set_axis_off();
ax[1].imshow(rwX,cmap='seismic');
ax[1].set_axis_off();
ax[2].imshow(dX,cmap='terrain');
ax[2].set_axis_off();
plt.show();