%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% If you use PhysiCell in your project, please cite PhysiCell and the version %
% number, such as below:                                                      %
%                                                                             %
% We implemented and solved the model using PhysiCell (Version x.y.z) [1].    %
%                                                                             %
% [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, %
%     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  %
%     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   %
%     DOI: 10.1371/journal.pcbi.1005991                                       %
%                                                                             %
% See VERSION.txt or call get_PhysiCell_version() to get the current version  %
%     x.y.z. Call display_citations() to get detailed information on all cite-%
%     able software used in your PhysiCell application.                       %
%                                                                             %
% Because PhysiCell extensively uses BioFVM, we suggest you also cite BioFVM  %
%     as below:                                                               %
%                                                                             %
% We implemented and solved the model using PhysiCell (Version x.y.z) [1],    %
% with BioFVM [2] to solve the transport equations.                           %
%                                                                             %
% [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, %
%     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  %
%     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   %
%     DOI: 10.1371/journal.pcbi.1005991                                       %
%                                                                             %
% [2] A Ghaffarizadeh, SH Friedman, and P Macklin, BioFVM: an efficient para- %
%     llelized diffusive transport solver for 3-D biological simulations,     %
%     Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730  %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
% BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)     %
%                                                                             %
% Copyright (c) 2015-2018, Paul Macklin and the PhysiCell Project             %
% All rights reserved.                                                        %
%                                                                             %
% Redistribution and use in source and binary forms, with or without          %
% modification, are permitted provided that the following conditions are met: %
%                                                                             %
% 1. Redistributions of source code must retain the above copyright notice,   %
% this list of conditions and the following disclaimer.                       %
%                                                                             %
% 2. Redistributions in binary form must reproduce the above copyright        %
% notice, this list of conditions and the following disclaimer in the         %
% documentation and/or other materials provided with the distribution.        %
%                                                                             %
% 3. Neither the name of the copyright holder nor the names of its            %
% contributors may be used to endorse or promote products derived from this   %
% software without specific prior written permission.                         %
%                                                                             %
% THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" %
% AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   %
% IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  %
% ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   %
% LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         %
% CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF        %
% SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    %
% INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN     %
% CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)     %
% ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE  %
% POSSIBILITY OF SUCH DAMAGE.                                                 %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Usage: 
%
% [h,indicator,fv,p] = simple_cutaway_plot( MCDS , ind , c )  
%
% This function creates a 3-D cutaway plot of the cells in 
% MCDS.discrete_cells, restricted to the indices in ind. 
%
% For example, use:
%    ind = MCDS.discrete_cells.live_cells 
%    [h,indicator,fv,p] = simple_cutaway_plot( MCDS , ind , c ); 
%
% or use: 
%    constants = set_MCDS_constants(); 
%    ind = find( ...
%    MCDS.discrete_cells.phenotype.cycle.current_phase == constants.apoptotic); 
%
%    [h,indicator,fv,p] = simple_cutaway_plot( MCDS , ind , c ) ; 
%
% Copyright 2016-2017 Paul Macklin / PhysiCell project
% Licensed under 3-Clause BSD

function [h,indicator,fv,p] = simple_cutaway_plot( MCDS , ind , c ) 

if( nargin == 1 )
    ind = union( MCDS.discrete_cells.live_cells , MCDS.discrete_cells.dead_cells ); 
end

if( nargin < 3 )
    c = 'r' ; 
end

indicator = zeros( size( MCDS.continuum_variables(1).data )); 
positions = MCDS.discrete_cells.state.position; 

h = gcf; 

for n=1:length( ind )
    m = ind(n); 
    [i,j,k,m] = nearest_index( MCDS , positions(m,:) );
    
    indicator(i,j,k) = 1; 
    x = MCDS.mesh.voxels(m).center; 
    if( x(1) <=0  && x(2) <= 0 && x(3) >= 0 )
        indicator(i,j,k) = 0; 
    end
    
end

fv = isosurface(MCDS.mesh.X,MCDS.mesh.Y,MCDS.mesh.Z,indicator,0.5); 

p = patch(fv);
isonormals(MCDS.mesh.X,MCDS.mesh.Y,MCDS.mesh.Z,indicator,p);
p.FaceColor = c;
p.EdgeColor = 'none';
daspect([1 1 1])
view(3); 
axis tight
camlight 
lighting gouraud


return; 

% subfunctions 

function [i,j,k,n] = nearest_index( MCDS, position )
Nx = length( MCDS.mesh.X_coordinates ); 
Ny = length( MCDS.mesh.Y_coordinates ); 
Nz = length( MCDS.mesh.Z_coordinates ); 

dx = MCDS.mesh.X_coordinates(2) - MCDS.mesh.X_coordinates(1); 
dy = MCDS.mesh.Y_coordinates(2) - MCDS.mesh.Y_coordinates(1) ;
dz = 1; 
if( Nz > 1 )
    dz = MCDS.mesh.Z_coordinates(2) - MCDS.mesh.Z_coordinates(1) ;
end

bounding_box = [ MCDS.mesh.X_coordinates(1)-dx/2, MCDS.mesh.Y_coordinates(1)-dy/2 , MCDS.mesh.Z_coordinates(1)-dz/2, ...
    MCDS.mesh.X_coordinates(Nx)+dx/2, MCDS.mesh.Y_coordinates(Ny)+dy/2 , MCDS.mesh.Z_coordinates(Nz)+dz/2 ];

% i,j,k coordinates
i = 1+ floor( (position(1)-bounding_box(1))/dx ) ;
j = 1+ floor( (position(2)-bounding_box(2))/dy ) ;
k = 1+ floor( (position(3)-bounding_box(3))/dz ) ;

% add some bounds checking -- truncate to inside the computational domain   
if( i > Nx ) 
    i = Nx; 
end
if( i < 1 )
    i = 1; 
end

if( j > Ny ) 
    j = Ny; 
end
if( j < 1 )
    j = 1; 
end

if( k > Nz ) 
    k = Nz; 
end
if( k < 1 )
    k = 1; 
end

n = ((k-1)*Ny+(j-1))*Nx + i; 
return; 
% 	return ( k*y_coordinates.size() + j )*x_coordinates.size() + i; 
% 
% 
% 
% 
% 
