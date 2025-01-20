/*
#############################################################################
# If you use BioFVM in your project, please cite BioFVM and the version     #
# number, such as below:                                                    #
#                                                                           #
# We solved the diffusion equations using BioFVM (Version 1.1.7) [1]        #
#                                                                           #
# [1] A. Ghaffarizadeh, S.H. Friedman, and P. Macklin, BioFVM: an efficient #
#    parallelized diffusive transport solver for 3-D biological simulations,#
#    Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730 #
#                                                                           #
#############################################################################
#                                                                           #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   #
#                                                                           #
# Copyright (c) 2015-2025, Paul Macklin and the BioFVM Project              #
# All rights reserved.                                                      #
#                                                                           #
# Redistribution and use in source and binary forms, with or without        #
# modification, are permitted provided that the following conditions are    #
# met:                                                                      #
#                                                                           #
# 1. Redistributions of source code must retain the above copyright notice, #
# this list of conditions and the following disclaimer.                     #
#                                                                           #
# 2. Redistributions in binary form must reproduce the above copyright      #
# notice, this list of conditions and the following disclaimer in the       #
# documentation and/or other materials provided with the distribution.      #
#                                                                           #
# 3. Neither the name of the copyright holder nor the names of its          #
# contributors may be used to endorse or promote products derived from this #
# software without specific prior written permission.                       #
#                                                                           #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       #
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED #
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           #
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER #
# OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  #
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       #
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        #
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    #
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      #
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        #
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              #
#                                                                           #
#############################################################################
*/

#ifndef __BioFVM_solvers_h__
#define __BioFVM_solvers_h__

#include "BioFVM_microenvironment.h" 

namespace BioFVM{
// /*! diffusion-decay solvers for the equation du/dt = D*Laplacian(u) - lambda*u - U(x)*u + M(X)*(uT-u) */ 

// /*! diffusion-decay solver: 3D LOD implicit (stable method). D and r uniform */  
void diffusion_decay_solver__constant_coefficients_LOD_3D( Microenvironment& M, double dt ); // done
// /*! diffusion-decay solver: 2D LOD implicit (stable method). D and r uniform */  
void diffusion_decay_solver__constant_coefficients_LOD_2D( Microenvironment& M, double dt ); // done
void diffusion_decay_solver__constant_coefficients_LOD_1D( Microenvironment& M, double dt ); // done

/*! This solves for constant diffusion coefficients on a general mesh using the 
    explicit stepping for the diffusion operator, and implicit stepping for all 
    other terms to increase stability. It is suitable for a general mesh. */ 

// /*! diffusion-decay solver: 3D explicit method -- suitable to a general mesh if necessary */  
void diffusion_decay_explicit_uniform_rates( Microenvironment& M , double dt );  // it exists 

void diffusion_decay_solver__constant_coefficients_explicit( Microenvironment& M, double dt ); 
void diffusion_decay_solver__constant_coefficients_explicit_uniform_mesh( Microenvironment& M, double dt ); 
};

#endif 