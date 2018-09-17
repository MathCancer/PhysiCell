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
% out = read_microenvironment( filename ) 
%
% The data in filename must be a BioFVM .mat file (not MultiCellDS)
%
% Note: This will eventually be deprecated. 
%
% Copyright 2015-2017 Paul Macklin / BioFVM project
% Licensed under 3-Clause BSD
%

function out = read_microenvironment( filename )

str = sprintf('Reading microenvironment scale stored in %s.\n\tTo access data field (n) at (i,j,k), use out.data{n}(i,j,k). \n' , filename);
disp(str);  

A = struct2array( load( filename ) );

% figure out X, Y, Z, and size

% out = struct([]); 
 
xmin = A(1,1); 
ymin = A(2,1); 
zmin = A(3,1); 

n = size(A,2);
xmax = A(1,n); 
ymax = A(2,n); 
zmax = A(3,n); 

% figure out number of x nodes  
xnodes = 1; 
while( A(1,xnodes) < xmax - eps )
xnodes = xnodes+1; 
end

out.X = A(1,1:xnodes); 

% figure out number of y nodes 
ynodes = 1; 

while( A(2,ynodes*xnodes) < ymax - eps )
ynodes = ynodes + 1;
end

out.Y = A(2,1:xnodes:xnodes*ynodes);

% figure out number of z nodes 

znodes = 1; 
while( A(3,ynodes*xnodes*znodes) < zmax - eps )
znodes = znodes + 1; 
end

out.Z = A(3,1:xnodes*ynodes:xnodes*ynodes*znodes);

% read in data 

temp = zeros(xnodes,ynodes,znodes);
% out.data = [] ; 
% out.data = temp; 
% out.data(1)  = {temp}; 

% out.data = struct([]);


% to access x(i),y(k),z(k) of data field n: 
% out.data{n}(i,j,k) 

for n=5:size(A,1)
    
    
    % fill in the data 
    m = 1; 
    for k=1:znodes
        for j=1:ynodes
            for i=1:xnodes 
                temp(i,j,k) = A(n,m); 
                m = m+1; 
            end
        end
    end

    % allocate the spot 
	out.data(n-4) = {temp}; % {temp}; 
    
    
end









return ;