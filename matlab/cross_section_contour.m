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


clear
file_name='output_4.000000_0.000100_10.000000.mat';
x=1;y=2;z=3;

needed_plane=[x,y];

crossesction_index=setdiff([1,2,3], needed_plane);
labels={'x','y','z'};
load(file_name);

for i=5:size(multiscale_microenvironment,1)
    m=multiscale_microenvironment;
    
    temp= unique(sort(m(crossesction_index,:)));
	if length(temp)==1 %2D
        temp_median_index=1;
    else
        temp_median_index=floor(length(temp)/2);
    end
    temp_median= temp(temp_median_index);	
    
    m=m(:,m(crossesction_index,:)==temp_median);
    
    tempx= unique(sort(m(needed_plane(1),:)));
    stepx= abs(tempx(1)-tempx(2));
    minx=tempx(1)-stepx/2;
    maxx=tempx(end)+stepx/2;
    tempy= unique(sort(m(needed_plane(2),:)));
    stepy= abs(tempy(1)-tempy(2));
    miny=tempy(1)-stepy/2;
    maxy=tempy(end)+stepy/2;
    
    num_rows= length(tempx);
    num_cols= length(tempy);
    % scaling the x values to range [1:numrows], the y values to range [1:numcols]
    x_scaled= 1+ floor(num_rows*((m(needed_plane(1),:)-minx)/(maxx-minx)));
    y_scaled= 1+ floor(num_cols*((m(needed_plane(2),:)-miny)/(maxy-miny)));
    
    c1=sparse(y_scaled, x_scaled,m(i,:));
    
    full_matrix=full(c1);
    
    figure
    contourf(full_matrix)
    axis image
    shading flat
    colorbar('FontSize', 14)
    title(['substrate' num2str(i-4)], 'FontSize', 14)
    h = colorbar();
    h1 = get( h , 'ylabel' );
    set( h1 , 'string' , 'concentration' )
    set( h1, 'fontsize', 12 )
    set(gca,'XTick',1:num_rows/2-0.5:num_rows)
    set(gca,'YTick',1:num_cols/2-0.5:num_cols)
    set(gca,'YTickLabel',{num2str(miny),num2str((miny+maxy)/2),num2str(maxy)})
    set(gca,'XTickLabel',{num2str(minx),num2str((minx+maxx)/2),num2str(maxx)})
    xlabel([labels{needed_plane(1)},' (\mum)'])
    ylabel([labels{needed_plane(2)},' (\mum)'])
end
