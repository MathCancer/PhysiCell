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
% MCDS = read_MultiCellDS_xml( filename ) OR
%
% MCDS = read_MultiCellDS_xml( filename , directory )
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
%           Access MCDS.discrete_cells.phenotype. etc. to get to arrays
%               of cell phenotype properties. e.g.,
%
% MCDS.discrete_cells.phenotype.geometrical_properties.volumes.total_volume(i)
%
%           MCDS.discrete_cells.state.position(i) is position of cell i
%
% Copyright 2016-2017 Paul Macklin / PhysiCell project
% Licensed under 3-Clause BSD
%

function MCDS = read_MultiCellDS_xml_vdb( filename , directory )
tic;

if( nargin == 1 )
    directory = '.';
end

MCDS.constants = set_MCDS_constants();

inputfile = sprintf('%s/%s', directory , filename);

tree = xmlread( inputfile );
node = tree.getFirstChild(); %   MultiCellDS

MCDS = locfn__read_microenvironment( MCDS, node, directory );
MCDS = locfn__read_cellular_information( MCDS, node, directory );

toc

%

 fprintf('\nSummary for file %s:\n' , inputfile ) ;
 fprintf('Voxels: %u\n', length(MCDS.mesh.voxels) ) 
 fprintf('Substrates: %u\n' , length( MCDS.continuum_variables ) ) ;
str = sprintf( '  %s (%s)' , MCDS.continuum_variables(1).name , MCDS.continuum_variables(1).units );
for i=2:length( MCDS.continuum_variables )
    str = sprintf( '%s, %s (%s)' , str, MCDS.continuum_variables(i).name , MCDS.continuum_variables(i).units );
end
disp(str);
if( ~isempty( MCDS.discrete_cells ) )
     fprintf('Cells: %u\n' , size( MCDS.discrete_cells.state.position , 1) ) ;
else
    disp( 'Cells: 0' );
end

end

%% Local functions

function MCDS = locfn__read_microenvironment( MCDS, multicell_ds_node, directory )

node = multicell_ds_node.getElementsByTagName('microenvironment').item(0);
node = node.getParentNode;

% some key metadata
metadata_node = node.getParentNode.getElementsByTagName('metadata').item(0);
node1 = metadata_node.getElementsByTagName( 'current_time' ).item(0);
MCDS.metadata.current_time = str2double( node1.getTextContent );
MCDS.metadata.time_units = char( node1.getAttribute( 'units' ) );

node1 = metadata_node.getElementsByTagName( 'current_runtime' ).item(0);
MCDS.metadata.current_runtime = str2double( node1.getTextContent );
MCDS.metadata.runtime_units = char( node1.getAttribute( 'units' ) );

MCDS.metadata.spatial_units = [];

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
    MCDS.mesh.X_coordinates = str2num( str ); %#ok<ST2NM> str is vector of doubles

    str = node.getElementsByTagName( 'y_coordinates' ).item(0).getTextContent;
    MCDS.mesh.Y_coordinates = str2num( str );

    str = node.getElementsByTagName( 'z_coordinates' ).item(0).getTextContent;
    MCDS.mesh.Z_coordinates = str2num( str ); %#ok<ST2NM> str is vector of doubles

    Cartesian = true;
end

blank_voxel = [];
blank_voxel.center = [0 0 0];
blank_voxel.volume = 0;

MCDS.mesh.voxels = [];

voxeltype = node.getElementsByTagName( 'voxels' ).item(0).getAttribute( 'type' );

if( strcmp( voxeltype , 'xml' ) )
    % if voxels stored in the XML
    voxel_tags = node.getElementsByTagName( 'voxel' );

    numvoxels = voxel_tags.getLength;
    MCDS.mesh.voxels = repmat( blank_voxel , 1 , numvoxels );

    for i=0:voxel_tags.getLength - 1
        MCDS.mesh.voxels(i+1).center = str2double( voxel_tags.item(i).getElementsByTagName( 'center' ).item(0).getTextContent );
        MCDS.mesh.voxels(i+1).volume = str2double( voxel_tags.item(i).getElementsByTagName( 'volume' ).item(0).getTextContent );
    end
else
    % voxels are stored in a mat file
    filename = node.getElementsByTagName('voxels' ).item(0).getElementsByTagName( 'filename' ).item(0).getTextContent ;
    %MAT = struct2array( load( char(filename) ) );

    filename = sprintf( '%s/%s', directory , char(filename));
    MAT = load(filename); % load(char(filename));
    MAT = MAT.mesh; % use this instead of struct2array for better octave compatibility
    numvoxels = size(MAT, 2);

    MCDS.mesh.voxels = repmat( blank_voxel , 1 , numvoxels );

    for i=1:numvoxels
        MCDS.mesh.voxels(i).center = MAT(1:3,i)';
        MCDS.mesh.voxels(i).volume = MAT(4,i);
    end
end

% meshgrid
[MCDS.mesh.X , MCDS.mesh.Y , MCDS.mesh.Z] = meshgrid( MCDS.mesh.X_coordinates , MCDS.mesh.Y_coordinates , MCDS.mesh.Z_coordinates );

% now, read the various densities
node = node.getParentNode;
node = node.getElementsByTagName( 'variables' ).item(0);

variable_tags = node.getElementsByTagName( 'variable' );

MCDS.continuum_variables = [];

for i=0:variable_tags.getLength - 1
    MCDS.continuum_variables(i+1).name = char( variable_tags.item(i).getAttribute('name' ) );
    MCDS.continuum_variables(i+1).units = char( variable_tags.item(i).getAttribute('units' ) );
    MCDS.continuum_variables(i+1).diffusion_coefficient = str2double( variable_tags.item(i).getElementsByTagName( 'diffusion_coefficient' ).item(0).getTextContent );
    MCDS.continuum_variables(i+1).decay_rate = str2double( variable_tags.item(i).getElementsByTagName( 'decay_rate' ).item(0).getTextContent );

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
    data_vector_tags = node.getElementsByTagName( 'data_vector' );
    numvars = length( MCDS.continuum_variables );

    if( Cartesian )
        for i=0:data_vector_tags.getLength - 1
            n = str2double( data_vector_tags.item(i).getAttribute( 'voxel_ID' ) )+1; % which voxel
            xyz = MCDS.mesh.voxels(n).center ; % get its center coordinate
            temp = str2double( data_vector_tags.item(i).getTextContent ) ; % get the data vector

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
        for i=0:data_vector_tags.getLength - 1
            n = str2double( data_vector_tags.item(i).getAttribute( 'voxel_ID' ) )+1; % which voxel
            temp = str2double( data_vector_tags.item(i).getTextContent ) ; % get the data vector

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
    % MAT = struct2array( load( char(filename) ) );
    filename = sprintf( '%s/%s', directory , char(filename));
    MAT = load(filename); % load(char(filename));
    MAT = MAT.multiscale_microenvironment;

    numvars = length( MCDS.continuum_variables );
    numvoxels = length( MCDS.mesh.voxels );

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

    if( ~Cartesian )
        % non-Cartesian -- just keep the pointcloud of data
        for i=1:numvoxels
            for j=1:numvars
                MCDS.continuum_variables(j).raw_data(i) = MAT(4+j,i);
            end
        end
    end

end

end

function MCDS = locfn__read_cellular_information( MCDS, node, directory )

% read cell populations

% node = node.getParentNode.getParentNode.getParentNode;
node = node.getElementsByTagName( 'cellular_information' ).item(0) ;
node = node.getElementsByTagName( 'cell_populations' ).item(0);

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

cell_population_tags = [];
if( ~isempty( node ) )
    cell_population_tags = node.getElementsByTagName( 'cell_population' );
end

MCDS.discrete_cells = blank_cell;

if( ~isempty( cell_population_tags ) )

    for cell_pop_ind=0:cell_population_tags.getLength-1
        node1 = cell_population_tags.item(cell_pop_ind); % cell_population
        poptype = char( node1.getAttribute('type') );

        is_individual_type = strcmp( poptype , 'individual' );

        custom_node = node1.getElementsByTagName( 'custom' ).item(0);
        has_custom = ~isempty( custom_node );

        if( is_individual_type && ~has_custom )
            cell_tags = cell_population_tags.item(cell_pop_ind).getElementsByTagName( 'cell' );

            numcells = cell_tags.getLength;

            MCDS.discrete_cells.phenotype.geometrical_properties.volumes.total_volume = zeros( 1, numcells );
            MCDS.discrete_cells.phenotype.geometrical_properties.lengths.radius = zeros( 1, numcells );
            for j=1:num_substrates
                MCDS.discrete_cells.phenotype.transport_processes.variable(j).export_rate = zeros(1,numcells);
                MCDS.discrete_cells.phenotype.transport_processes.variable(j).import_rate = zeros(1,numcells);
                MCDS.discrete_cells.phenotype.transport_processes.variable(j).saturation_density = zeros(1,numcells);
            end

            MCDS.discrete_cells.state.position = zeros( numcells , 3 );

            for j=0:cell_tags.getLength-1
                node2 = cell_tags.item(j).getElementsByTagName('phenotype').item(0);
                node3 = node2.getElementsByTagName('geometrical_properties').item(0);
                node3 = node3.getElementsByTagName('volumes').item(0).getElementsByTagName('total_volume').item(0);
                vol = str2double( node3.getTextContent );

                MCDS.discrete_cells.phenotype.geometrical_properties.volumes.total_volume(j+1) = vol;
                MCDS.discrete_cells.phenotype.geometrical_properties.lengths.radius(j+1) = ( 3/(4*vol*pi) )^(1/3);

                % transport processes
                node3 = node2.getElementsByTagName('transport_processes' ).item(0);
                mylist2 = node3.getElementsByTagName( 'variable' );
                for k = 0:mylist2.getLength -1
                    % MCDS.discrete_cells.cells(j+1).phenotype.transport_processes.variable(k+1).name = char( mylist2.item(k).getAttribute( 'name' ) );
                    MCDS.discrete_cells.phenotype.transport_processes.variable(k+1).export_rate(j+1) = str2double( mylist2.item(k).getElementsByTagName('export_rate').item(0).getTextContent );
                    MCDS.discrete_cells.phenotype.transport_processes.variable(k+1).import_rate(j+1) = str2double( mylist2.item(k).getElementsByTagName('import_rate').item(0).getTextContent );
                    MCDS.discrete_cells.phenotype.transport_processes.variable(k+1).saturation_density(j+1) = str2double( mylist2.item(k).getElementsByTagName('saturation_density').item(0).getTextContent );
                end

                node2 = node2.getParentNode.getParentNode;
                node3 = node2.getElementsByTagName( 'state' ).item(0);

                MCDS.discrete_cells.state.position(j+1,:) = str2double( node3.getElementsByTagName( 'position' ).item(0).getTextContent );
            end
        end

        % deal with the case where we stored a simplified data structure
        % as a matlab file
        if( is_individual_type && has_custom )

            % first, get the BioFVM stuff
            simplified_data_tags = custom_node.getElementsByTagName( 'simplified_data' );
            filename = [];
            if( ~isempty( simplified_data_tags ) )
                filename = char( simplified_data_tags.item(0).getElementsByTagName( 'filename').item(0).getTextContent );
            end

            if( ~isempty( filename ) )
                % load the matlab file, and determine the number of cells
                % MAT = struct2array( load( filename ) );
                filename = sprintf( '%s/%s', directory , char(filename));
                MAT = load(filename); % load(char(filename));
                MAT = MAT.cells; % use this instead of struct2array for better octave compatibility

                numcells = size(MAT, 2);

                % ID, x,y,z, volume,  src,sink,saturation (multiple)

                MCDS.discrete_cells.phenotype.geometrical_properties.volumes.total_volume = zeros( 1, numcells );
                MCDS.discrete_cells.phenotype.geometrical_properties.lengths.radius = zeros( 1, numcells );
                for j=1:num_substrates
                    MCDS.discrete_cells.phenotype.transport_processes.variable(j).export_rate = zeros(1,numcells);
                    MCDS.discrete_cells.phenotype.transport_processes.variable(j).import_rate = zeros(1,numcells);
                    MCDS.discrete_cells.phenotype.transport_processes.variable(j).saturation_density = zeros(1,numcells);
                end

                MCDS.discrete_cells.state.position = zeros( numcells , 3 );

                % fill out the cells
                MCDS.discrete_cells.state.position(:,1:3) = MAT(2:4,:)';
                MCDS.discrete_cells.phenotype.geometrical_properties.volumes.total_volume = MAT(5,:)';
                my_constant = 3.0 / 4.0 / pi;
                MCDS.discrete_cells.phenotype.geometrical_properties.lengths.radius = ( my_constant * MCDS.discrete_cells.phenotype.geometrical_properties.volumes.total_volume ).^(1/3);

                num_substrates = length( MCDS.continuum_variables );

                for j=1:num_substrates
                    ind = 6+3*(j-1);
                    MCDS.discrete_cells.phenotype.transport_processes.variable(j).export_rate = MAT(ind,:)';
                    MCDS.discrete_cells.phenotype.transport_processes.variable(j).import_rate = MAT(ind+1,:)';
                    MCDS.discrete_cells.phenotype.transport_processes.variable(j).saturation_density = MAT(ind+2,:)';
                end

                % <label index="0" size="1">ID</label> % 1
                % <label index="1" size="3">position</label> % 2:4
                % <label index="4" size="1">total_volume</label> % 5

                % <label index="5" size="1">cell_type</label> % 6
                MCDS.discrete_cells.metadata.type = int8( MAT(6,:) );

                % <label index="6" size="1">cycle_model</label>
                MCDS.discrete_cells.phenotype.cycle.cycle_model = int8( MAT(7,:) );
                % <label index="7" size="1">current_phase</label>
                MCDS.discrete_cells.phenotype.cycle.current_phase = int8( MAT(8,:) );
                % <label index="8" size="1">elapsed_time_in_phase</label>
                MCDS.discrete_cells.phenotype.cycle.elapsed_time_in_phase = ( MAT(9,:) );

                % <label index="9" size="1">nuclear_volume</label>
                MCDS.discrete_cells.phenotype.geometrical_properties.volumes.nuclear = ( MAT(10,:) );
                % <label index="10" size="1">cytoplasmic_volume</label>
                MCDS.discrete_cells.phenotype.geometrical_properties.volumes.cytoplasmic = ( MAT(11,:) );
                % <label index="11" size="1">fluid_fraction</label>
                MCDS.discrete_cells.phenotype.geometrical_properties.volumes.fluid_fraction = ( MAT(12,:) );
                % <label index="12" size="1">calcified_fraction</label>
                MCDS.discrete_cells.phenotype.geometrical_properties.volumes.calcified_fraction = ( MAT(13,:) );

                % <label index="13" size="3">orientation</label>
                MCDS.discrete_cells.state.orientation = MAT(14:16,:)';
                % <label index="16" size="1">polarity</label>
                MCDS.discrete_cells.state.polarity = MAT(17,:);

                % <label index="17" size="1">migration_speed</label>
                MCDS.discrete_cells.phenotype.motility.migration_speed = MAT(18,:);
                % <label index="18" size="3">motility_vector</label>
                MCDS.discrete_cells.phenotype.motility.motility_vector = MAT(19:21,:)';
                % <label index="21" size="1">migration_bias</label>
                MCDS.discrete_cells.phenotype.motility.migration_bias = MAT(22,:);
                % <label index="22" size="3">motility_bias_direction</label>
                MCDS.discrete_cells.phenotype.motility.motility_bias_direction = MAT(23:25,:)';
                % <label index="25" size="1">persistence_time</label>
                MCDS.discrete_cells.phenotype.motility.persistence_time = MAT(26,:);
                % <label index="26" size="1">motility_reserved</label>

                % now get custom variables

                % simplified_data_node1 = custom_node.getElementsByTagName( 'simplified_data' ).item(1);
                labels_node = simplified_data_tags.item(0).getElementsByTagName( 'labels' ).item(0);

                labels = labels_node.getElementsByTagName( 'label' ) ;

                max_label_index = labels.getLength-1;

                custom = [];

                for label_ind = 19:max_label_index
                    mysize = int8( str2double( labels.item(label_ind).getAttribute('size') ) );
                    myindex = int8( str2double( labels.item(label_ind).getAttribute('index')) )+1;

                    newname = char(labels.item(label_ind).getTextContent);
                    newname = strrep( newname, ' ', '_' ); % replace( newname, ' ', '_' );

                    newdata = MAT(myindex:myindex+mysize-1,:);
                    if( mysize > 1 )
                        newdata = newdata';
                    end
                    custom.(newname) = newdata;
                end
                MCDS.discrete_cells.custom = custom;
            end


            % end

        end

    end

else
    MCDS.discrete_cells = [];
end

% figure out which cells are dead or live

MCDS.discrete_cells.dead_cells = find( MCDS.discrete_cells.phenotype.cycle.cycle_model == MCDS.constants.apoptosis_death_model | ...
    MCDS.discrete_cells.phenotype.cycle.cycle_model == MCDS.constants.necrosis_death_model );
MCDS.discrete_cells.live_cells = find( MCDS.discrete_cells.phenotype.cycle.cycle_model == MCDS.constants.advanced_Ki67_cycle_model | ...
    MCDS.discrete_cells.phenotype.cycle.cycle_model == MCDS.constants.basic_Ki67_cycle_model | ...
    MCDS.discrete_cells.phenotype.cycle.cycle_model == MCDS.constants.flow_cytometry_cycle_model | ...
    MCDS.discrete_cells.phenotype.cycle.cycle_model == MCDS.constants.flow_cytometry_separated_cycle_model | ...
    MCDS.discrete_cells.phenotype.cycle.cycle_model == MCDS.constants.live_apoptotic_cycle_model | ...
    MCDS.discrete_cells.phenotype.cycle.cycle_model == MCDS.constants.total_cells_cycle_model | ...
    MCDS.discrete_cells.phenotype.cycle.cycle_model == MCDS.constants.live_cells_cycle_model );

end
