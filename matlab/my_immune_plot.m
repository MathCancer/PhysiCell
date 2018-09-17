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

function MCDS = my_immune_plot( i )
filename = sprintf( 'output%08u.xml', i ); 
MCDS = read_MultiCellDS_xml( filename ); 
ind0 = find( MCDS.discrete_cells.metadata.type == 0 );
ind1 = find( MCDS.discrete_cells.metadata.type == 1 );

ind0_live = intersect( MCDS.discrete_cells.live_cells , ind0 );
ind0_dead = intersect( MCDS.discrete_cells.dead_cells , ind0 );

clf; 
simple_cutaway_plot( MCDS, ind0_live , 'r' );
simple_cutaway_plot( MCDS, ind0_dead , 'b' );
simple_cutaway_plot( MCDS, ind1 , 'g' );

axis( 500*[-1 1 -1 1 -1 1])

title( sprintf( '%3.2f days\n%u tumor cells (%3.2f%% live)\n%u immune cells' , MCDS.metadata.current_time / (60*24) , length(ind0), 100*length(ind0_live)/length(ind0), length(ind1)  ), 'fontsize', 14 )

xlabel( 'x (\mum)' , 'fontsize', 12 ); 
ylabel( 'y (\mum)' , 'fontsize', 12 ); 
zlabel( 'z (\mum)' , 'fontsize', 12 ); 
