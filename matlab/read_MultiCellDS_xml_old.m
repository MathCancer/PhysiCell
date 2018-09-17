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
% MCDS = read_MultiCellDS_xml_old( filename )
%
% MCDS.metadata has key metadata information
%
% MCDS.mesh has the mesh, including: 
%           X_coordinates, Y_coordinates, Z_coordinates
%           and a meshgrid [X,Y,Z]
%
% MCDS.continuum_variables has densities. 
%           Access MCDS.continuum_variables(1), etc. 
%
% MCDS.discrete_cells has individual cells 
%           Access MCDS.discrete_cells.cells(1), etc. 
%
% This function is much slower than the current read_MultiCellDS_xml(). 
% It will probably be deprecated in a later release. 
%
%           MCDS.discrete_cells.positions, 
%           MCDS.discrete_cells.radii, 
%           MCDS.discrete_cells.volumes, 
%           might be useful for plotting
%
% Copyright 2016 Paul Macklin / BioFVM project
% Licensed under 3-Clause BSD
%

function MCDS = read_MultiCellDS_xml_old( filename )
tic; 

inputfile = filename; 

tree = xmlread( filename ); 
node = tree.getFirstChild(); %   MultiCellDS

node = node.getElementsByTagName('microenvironment').item(0); 
node = node.getParentNode; 

% some key metadata
metadata_node = node.getParentNode.getElementsByTagName('metadata').item(0); 
node1 = metadata_node.getElementsByTagName( 'current_time' ).item(0); 
MCDS.metadata.current_time = str2num( node1.getTextContent ); 
MCDS.metadata.time_units = char( node1.getAttribute( 'units' ) ); 

node1 = metadata_node.getElementsByTagName( 'current_runtime' ).item(0); 
MCDS.metadata.current_runtime = str2num( node1.getTextContent ); 
MCDS.metadata.runtime_units = char( node1.getAttribute( 'units' ) ); 

MCDS.metadata.spatial_units = []; 

% mylist = node.getElementsByTagName('microenvironment');
microenvironment_node = node.getElementsByTagName('microenvironment').item(0);

node = microenvironment_node.getElementsByTagName( 'mesh' ).item(0); % mesh level 
MCDS.metadata.spatial_units = char( node.getAttribute( 'units' ) );  

% first, read the mesh 

meshtype = node.getAttribute( 'type' ); 
Cartesian = false; 

MCDS.mesh.X_coordinates = []; 
MCDS.mesh.Y_coordinates = []; 
MCDS.mesh.Z_coordinates = []; 

if( strcmp( meshtype, 'Cartesian' ) )
    str = node.getElementsByTagName( 'x_coordinates' ).item(0).getTextContent; 
    MCDS.mesh.X_coordinates = str2num( str ); 

    str = node.getElementsByTagName( 'y_coordinates' ).item(0).getTextContent; 
    MCDS.mesh.Y_coordinates = str2num( str ); 

    str = node.getElementsByTagName( 'z_coordinates' ).item(0).getTextContent; 
    MCDS.mesh.Z_coordinates = str2num( str ); 
    
    Cartesian = true; 
end

blank_voxel = []; 
blank_voxel.center = [0 0 0];
blank_voxel.volume = 0; 

MCDS.mesh.voxels = []; 

voxeltype = node.getElementsByTagName( 'voxels' ).item(0).getAttribute( 'type' ); 

if( strcmp( voxeltype , 'xml' ) )  
    % if voxels stored in the XML
    mylist = node.getElementsByTagName( 'voxel' ); 

    numvoxels = mylist.getLength; 
    MCDS.mesh.voxels = repmat( blank_voxel , 1 , numvoxels ); 
   
    for i=0:mylist.getLength - 1
        MCDS.mesh.voxels(i+1).center = str2num( mylist.item(i).getElementsByTagName( 'center' ).item(0).getTextContent );
        MCDS.mesh.voxels(i+1).volume = str2num( mylist.item(i).getElementsByTagName( 'volume' ).item(0).getTextContent );   
    end
else
    % voxels are stored in a mat file 
    filename = node.getElementsByTagName('voxels' ).item(0).getElementsByTagName( 'filename' ).item(0).getTextContent ; 
    MAT = struct2array( load( char(filename) ) ); 
    [m,numvoxels] = size(MAT);
    
    MCDS.mesh.voxels = repmat( blank_voxel , 1 , numvoxels ); 
    
    for i=1:numvoxels
        MCDS.mesh.voxels(i).center = MAT(1:3,i)'; 
        MCDS.mesh.voxels(i).volume = MAT(4,i); 
    end
    clear MAT; 
end

% meshgrid 
[MCDS.mesh.X , MCDS.mesh.Y , MCDS.mesh.Z] = meshgrid( MCDS.mesh.X_coordinates , MCDS.mesh.Y_coordinates , MCDS.mesh.Z_coordinates ); 

% now, read the various densities 
node = node.getParentNode;
node = node.getElementsByTagName( 'variables' ).item(0); 

mylist = node.getElementsByTagName( 'variable' ); 

MCDS.continuum_variables = []; 

for i=0:mylist.getLength - 1
   MCDS.continuum_variables(i+1).name = char( mylist.item(i).getAttribute('name' ) ); 
   MCDS.continuum_variables(i+1).units = char( mylist.item(i).getAttribute('units' ) ); 
   MCDS.continuum_variables(i+1).diffusion_coefficient = str2num( mylist.item(i).getElementsByTagName( 'diffusion_coefficient' ).item(0).getTextContent ); 
   MCDS.continuum_variables(i+1).decay_rate = str2num( mylist.item(i).getElementsByTagName( 'decay_rate' ).item(0).getTextContent ); 
   
   if( Cartesian )
       MCDS.continuum_variables(i+1).data = zeros( size(MCDS.mesh.X) ); % only for Cartesian 
       MCDS.continuum_variables(i+1).raw_data = []; % only for non-Cartesian 
   else
       MCDS.continuum_variables(i+1).data = []; % only for Cartesian 
       MCDS.continuum_variables(i+1).raw_data = zeros( 1 , length( MCDS.mesh.voxels) ); % only for non-Cartesian 
   end
end

% now get the actual data 
node = node.getParentNode; % scale 
node = node.getElementsByTagName( 'data' ).item(0); 
datatype = node.getAttribute( 'type' ); 

if( strcmp( datatype, 'xml' ) )
   % data stored in xml data vectors  
    mylist = node.getElementsByTagName( 'data_vector' ); 
    numvars = length( MCDS.continuum_variables ); 
    temp = zeros( 1 , numvars ); 
    xyz = zeros(1,3); 
    
    if( Cartesian )
        for i=0:mylist.getLength - 1
            n = str2num( mylist.item(i).getAttribute( 'voxel_ID' ) )+1; % which voxel 
            xyz = MCDS.mesh.voxels(n).center ; % get its center coordinate 
            temp = str2num( mylist.item(i).getTextContent ) ; % get the data vector 

            % figure out the X,Y,Z indices
            ii = find( abs( MCDS.mesh.X_coordinates - xyz(1) ) < 1e-10 , 1); 
            jj = find( abs( MCDS.mesh.Y_coordinates - xyz(2) ) < 1e-10 , 1); 
            kk = find( abs( MCDS.mesh.Z_coordinates - xyz(3) ) < 1e-10 , 1); 

            for j=1:numvars
                MCDS.continuum_variables(j).data(jj,ii,kk) = temp(j); 
                % Matlab is STOOOPID. data d_ijk at (x(i), y(j) , z(k) ) is
                % stored in data(j,i,k) instead of data(i,j,k). 
            end
        end
    else
        % non-Cartesian -- just keep the pointcloud of data
        for i=0:mylist.getLength - 1
            n = str2num( mylist.item(i).getAttribute( 'voxel_ID' ) )+1; % which voxel 
            temp = str2num( mylist.item(i).getTextContent ) ; % get the data vector 

            for j=1:numvars
                MCDS.continuum_variables(j).raw_data(n) = temp(j); 
            end
        end
    end
    
end    
    
% data stored in a matlab file 
if( strcmp( datatype, 'matlab' ) )
   % data stored in a matlab file data vectors  

   
   % voxels are stored in a mat file 
   filename = node.getElementsByTagName( 'filename' ).item(0).getTextContent;
   MAT = struct2array( load( char(filename) ) ); 
   [m,n] = size(MAT);
   
   numvars = length( MCDS.continuum_variables ); 
   numvoxels = length( MCDS.mesh.voxels ); 
   xyz = zeros(1,3);  
       
   if( Cartesian )
       for i=1:numvoxels
           xyz = MCDS.mesh.voxels(i).center; % get its center coordinate 

           % figure out the X,Y,Z indices
           ii = find( abs( MCDS.mesh.X_coordinates - xyz(1) ) < 1e-10 , 1); 
           jj = find( abs( MCDS.mesh.Y_coordinates - xyz(2) ) < 1e-10 , 1); 
           kk = find( abs( MCDS.mesh.Z_coordinates - xyz(3) ) < 1e-10 , 1); 

           for j=1:numvars
                MCDS.continuum_variables(j).data(jj,ii,kk) = MAT(4+j,i); 
                % Matlab is STOOOPID. data d_ijk at (x(i), y(j) , z(k) ) is
                % stored in data(j,i,k) instead of data(i,j,k). 
           end
       end
   end
   
   if( Cartesian == false )
       % non-Cartesian -- just keep the pointcloud of data
       for i=1:numvoxels
            for j=1:numvars
                MCDS.continuum_variables(j).raw_data(i) = MAT(4+j,i); 
            end
       end
   end
    
end    

% read cell populations

node = node.getParentNode.getParentNode.getParentNode; 
node = node.getElementsByTagName( 'cell_populations' ).item(0) ; 

% creating "blank" structures and preallocating memory cuts 
% processing time. 

blank_phenotype.geometrical_properties.volumes.total_volume = 0; 
blank_phenotype.geometrical_properties.lengths.radius = 0; 

blank_transport_variable.name = '';
blank_transport_variable.export_rate = 0;
blank_transport_variable.import_rate = 0;
blank_transport_variable.saturation_density = 0;

num_substrates = length( MCDS.continuum_variables );                
for k=1:num_substrates
    blank_phenotype.transport_processes.variable(k) = blank_transport_variable; 
    blank_phenotype.transport_processes.variable(k).name = MCDS.continuum_variables(k).name; 
end

blank_state.position = [0 0 0];

blank_cell.phenotype = blank_phenotype; 
blank_cell.state = blank_state;

mylist = []; 
if( isempty( node ) == false )
    mylist = node.getElementsByTagName( 'cell_population' ); 
end

% for easy scatter plots 
MCDS.discrete_cells.X_coordinates = []; 
MCDS.discrete_cells.Y_coordinates = []; 
MCDS.discrete_cells.Z_coordinates = [];
MCDS.discrete_cells.radii = []; 

MCDS.discrete_cells = []; 

if( isempty( mylist ) == false )
    
    for i=0:mylist.getLength-1
        node1 = mylist.item(i); % cell_population 
        poptype = char( node1.getAttribute('type') ); 
        
        individual_type = false; 
        if( strcmp( poptype , 'individual' ) )
            individual_type = true; 
        end
        
        custom_node = node1.getElementsByTagName( 'custom' ).item(0); 
        custom = false; 
        if( isempty( custom_node ) == false )
            custom = true;
        end
            
        if( individual_type == true && custom == false )
            mylist1 = mylist.item(i).getElementsByTagName( 'cell' ); 

            numcells = mylist1.getLength; 
            % this is helpful for scatter plots -- some more basic details in
            % big arrays, instead of cell-by-cell
            MCDS.discrete_cells.positions = zeros(numcells,3); 
            MCDS.discrete_cells.radii = zeros(1,numcells); 

            % detailed cells, MultiCellDS data structures 
            MCDS.discrete_cells.cells = repmat( blank_cell , 1 , numcells ); 
               
            for j=0:mylist1.getLength-1
                node2 = mylist1.item(j).getElementsByTagName('phenotype').item(0); 
                node3 = node2.getElementsByTagName('geometrical_properties').item(0); 
                node3 = node3.getElementsByTagName('volumes').item(0).getElementsByTagName('total_volume').item(0); 
                vol = str2num( node3.getTextContent ); 

                MCDS.discrete_cells.radii(j+1) = ( 3/(4*vol*pi) )^(1/3);
                MCDS.discrete_cells.volumes(j+1) = vol; 

                % read all the geometrical properties 
                MCDS.discrete_cells.cells(j+1).phenotype.geometrical_properties.volumes.total_volume = vol; 
                MCDS.discrete_cells.cells(j+1).phenotype.geometrical_properties.lengths.radius = MCDS.discrete_cells.radii(j+1); 


                % transport processes 
                node3 = node2.getElementsByTagName('transport_processes' ).item(0);
                mylist2 = node3.getElementsByTagName( 'variable' );
                for k = 0:mylist2.getLength -1
                    % MCDS.discrete_cells.cells(j+1).phenotype.transport_processes.variable(k+1).name = char( mylist2.item(k).getAttribute( 'name' ) );
                    MCDS.discrete_cells.cells(j+1).phenotype.transport_processes.variable(k+1).export_rate = str2num( mylist2.item(k).getElementsByTagName('export_rate').item(0).getTextContent ); 
                    MCDS.discrete_cells.cells(j+1).phenotype.transport_processes.variable(k+1).import_rate = str2num( mylist2.item(k).getElementsByTagName('import_rate').item(0).getTextContent ); 
                    MCDS.discrete_cells.cells(j+1).phenotype.transport_processes.variable(k+1).saturation_density = str2num( mylist2.item(k).getElementsByTagName('saturation_density').item(0).getTextContent ); 
                end

%                MCDS.discrete_cells.cells(j+1).phenotype = phenotype; 

                node2 = node2.getParentNode.getParentNode; 
                node3 = node2.getElementsByTagName( 'state' ).item(0);

%                state = []; 
                MCDS.discrete_cells.cells(j+1).state.position = str2num( node3.getElementsByTagName( 'position' ).item(0).getTextContent ); 
%                MCDS.discrete_cells.cells(j+1).state = state; 

                MCDS.discrete_cells.positions(j+1,:) = MCDS.discrete_cells.cells(j+1).state.position; 
            end
        end

        % deal with the case where we stored a simplified data structure 
        % as a matlab file
        if( individual_type == true && custom == true )
            
            mylist_temp = custom_node.getElementsByTagName( 'simplified_data' ); 
            filename = []; 
            if( isempty( mylist_temp ) == false )
                filename = char( mylist_temp.item(0).getElementsByTagName( 'filename').item(0).getTextContent ); 
            end
            
            if( isempty( filename ) == false )
                % load the matlab file, and determine the number of cells 
                MAT = struct2array( load( filename ) ); 
                [m,numcells] = size(MAT); 
               
                % ID, x,y,z, volume,  src,sink,saturation (multiple) 
                
                % allocate space 
                % this is helpful for scatter plots -- some more basic details in
                % big arrays, instead of cell-by-cell
                MCDS.discrete_cells.positions = zeros(numcells,3); 
                MCDS.discrete_cells.radii = zeros(1,numcells); 

                % detailed cells, MultiCellDS data structures 
                MCDS.discrete_cells.cells = repmat( blank_cell , 1 , numcells ); 
                
                % fill out the cells 
                MCDS.discrete_cells.positions(:,1:3) = MAT(2:4,:)'; 
                MCDS.discrete_cells.volumes = MAT(5,:)'; 
                my_constant = 3.0 / 4.0 / pi; 
                MCDS.discrete_cells.radii = ( my_constant * MCDS.discrete_cells.volumes ).^(1/3); 
                
                num_substrates = length( MCDS.continuum_variables );                

                
                for j=1:numcells
                    % read all the geometrical properties 
                    MCDS.discrete_cells.cells(j).phenotype.geometrical_properties.volumes.total_volume = MCDS.discrete_cells.volumes(j); 
                    MCDS.discrete_cells.cells(j).phenotype.geometrical_properties.lengths.radius = MCDS.discrete_cells.radii(j); 
                    
                    % transport processes 
                    ind = 6; 
                    for k = 1:num_substrates
                        % MCDS.discrete_cells.cells(j).phenotype.transport_processes.variable(k).name = MCDS.continuum_variables(k).name; 
                        MCDS.discrete_cells.cells(j).phenotype.transport_processes.variable(k).export_rate = MAT(ind,j); ind = ind+1;  
                        MCDS.discrete_cells.cells(j).phenotype.transport_processes.variable(k).import_rate = MAT(ind,j); ind = ind+1;  
                        MCDS.discrete_cells.cells(j).phenotype.transport_processes.variable(k).saturation_density = MAT(ind,j) ; ind = ind+1; 
                    end

                    % MCDS.discrete_cells.cells(j).phenotype = phenotype; 

                    MCDS.discrete_cells.cells(j).state.position = MCDS.discrete_cells.positions(j,:); 
                    % MCDS.discrete_cells.cells(j).state = state; 
                end                
                
                

                clear MAT; 
                
            end
        end


    end

end 

clear tree 
toc

disp( sprintf('\nSummary for file %s:' , inputfile ) ); 
disp( sprintf('Voxels: %u', length(MCDS.mesh.voxels) ) )
disp( sprintf('Substrates: %u' , length( MCDS.continuum_variables ) ) );
str = sprintf( '  %s (%s)' , MCDS.continuum_variables(1).name , MCDS.continuum_variables(1).units ); 
for i=2:length( MCDS.continuum_variables )
    str = sprintf( '%s, %s (%s)' , str, MCDS.continuum_variables(i).name , MCDS.continuum_variables(i).units ); 
end
disp(str); 
disp( sprintf('Cells: %u' , length( MCDS.discrete_cells.cells ) ) );

return; 
