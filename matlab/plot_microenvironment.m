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
% If you want to put titles on the plots, use this syntax: 
% 
% M = read_microenvironment( 'some_file.mat' ); 
% titles{1} = 'cells'; 
% titles{2} = 'blood vessels';
% titles{3} = 'oxygen'; 
% plot_microenvironment( M , titles );  
% 
% To go without the labels, use: 
% 
% M = read_microenvironment( 'some_file.mat' ); 
% plot_microenvironment( M ); 
% 
% The data in filename must be a BioFVM .mat file (not MultiCellDS)
%
% Note: This will eventually be deprecated. 
%
% Copyright 2015-2017 Paul Macklin / BioFVM project
% Licensed under 3-Clause BSD
%

function plot_microenvironment( M , titles )
plot_titles = [];
if( nargin > 1 )
    plot_titles = titles;  
end

number_of_fields = length( M.data ); 
number_of_plots = number_of_fields; 

if( nargin == 1 )
    for i=1:number_of_fields
        plot_titles{i} = sprintf('field %d',i); 
    end
end


if( mod(number_of_plots,2) == 1 && number_of_fields > 1 )
    number_of_plots = number_of_plots + 1; 
end

width = ceil( sqrt( number_of_plots )) ;
height = number_of_plots / width ;

[x,y,z] = size( M.data{1} ) ;

mid_index = 1+floor( z/2.0 ) ;

for i=1:number_of_fields
    subplot(height,width,i,'align') ; 
    % figure(1) ; 
    h = contourf( M.X , M.Y, M.data{i}(:,:,mid_index)' , 30 ,  'linecolor', 'none'); axis image;  ;   title( plot_titles{i} ); colorbar; 
    xlabel( 'x' ); 
    ylabel( 'y' ); 
    % set( gca ,'CLim', [0 1] ); 
end

return; 