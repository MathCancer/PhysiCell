import xml.etree.ElementTree as ET
import numpy as np
import pandas as pd
import scipy.io as sio
import sys
import warnings
from pathlib import Path

class pyMCDS:
    """
    This class contains a dictionary of dictionaries that contains all of the 
    output from a single time step of a PhysiCell Model. This class assumes that
    all output files are stored in the same directory. Data is loaded by reading
    the .xml file for a particular timestep.
    
    Parameters
    ----------
    xml_name: str
        String containing the name of the xml file without the path
    output_path: str, optional
        String containing the path (relative or absolute) to the directory
        where PhysiCell output files are stored (default= ".")

    Attributes
    ----------
    data : dict
        Hierarchical container for all of the data retrieved by parsing the xml
        file and the files referenced therein.
    """
    def __init__(self, xml_file, output_path='.'):
        self.data = self._read_xml(xml_file, output_path)

    ## METADATA RELATED FUNCTIONS

    def get_time(self):
        return self.data['metadata']['current_time']

    ## MESH RELATED FUNCTIONS

    def get_mesh(self, flat=False):
        """
        Return a meshgrid of the computational domain. Can return either full
        3D or a 2D plane for contour plots.

        Parameters
        ----------
        flat : bool
            If flat is set to true, we return only the x and y meshgrid.
            Otherwise we return x, y, and z

        Returns
        -------
        splitting : list length=2 if flat=True, else length=3
            Contains arrays of voxel center coordinates as meshgrid with shape 
            [nx_voxel, ny_voxel, nz_voxel] or [nx_voxel, ny_voxel] if flat=True.
        """
        if flat == True:
            xx = self.data['mesh']['x_coordinates'][:, :, 0]
            yy = self.data['mesh']['y_coordinates'][:, :, 0]

            return [xx, yy]

        # if we dont want a plane just return appropriate values
        else:
            xx = self.data['mesh']['x_coordinates']
            yy = self.data['mesh']['y_coordinates']
            zz = self.data['mesh']['z_coordinates']

            return [xx, yy, zz]

    def get_2D_mesh(self):
        """
        This function returns the x, y meshgrid as two numpy arrays. It is 
        identical to get_mesh with the option flat=True

        Returns
        -------
        splitting : list length=2
            Contains arrays of voxel center coordinates in x and y dimensions 
            as meshgrid with shape [nx_voxel, ny_voxel]
        """
        xx = self.data['mesh']['x_coordinates'][:, :, 0]
        yy = self.data['mesh']['y_coordinates'][:, :, 0]

        return [xx, yy]

    def get_linear_voxels(self):
        """
        Helper function to quickly grab voxel centers array stored linearly as
        opposed to meshgrid-style.
        """
        return self.data['mesh']['voxels']['centers']

    def get_mesh_spacing(self):
        """
        Returns the space in between voxel centers for the mesh in terms of the
        mesh's spatial units. Assumes that voxel centers fall on integer values.

        Returns
        -------
        dx : float
            Distance between voxel centers in the same units as the other 
            spatial measurements
        """
        centers = self.get_linear_voxels()
        X = np.unique(centers[0, :])
        Y = np.unique(centers[1, :])
        Z = np.unique(centers[2, :])

        dx = (X.max() - X.min()) / X.shape[0]
        dy = (Y.max() - Y.min()) / Y.shape[0]
        dz = (Z.max() - Z.min()) / Z.shape[0]
        
        if np.abs(dx - dy) > 1e-10 or np.abs(dy - dz) > 1e-10 \
            or np.abs(dx - dz) > 1e-10:
            print('Warning: grid spacing may be axis dependent.')
        
        return round(dx)

    def get_containing_voxel_ijk(self, x, y, z):
        """
        Internal function to get the meshgrid indices for the center of a voxel
        that contains the given position. 
        
        Note that pyMCDS stores meshgrids as 'cartesian' 
        (indexing='xy' in np.meshgrid) which means that we will have
        to use these indices as [j, i, k] on the actual meshgrid objects

        Parameters
        ----------
        x : float
            x-coordinate for the position
        y : float
            y-coordinate for the position
        z : float
            z-coordinate for the position

        Returns
        -------
        ijk : list length=3
            contains the i, j, and k indices for the containing voxel's center
        """
        xx, yy, zz = self.get_mesh()
        ds = self.get_mesh_spacing()

        if x > xx.max():
            warnings.warn('Position out of bounds: x out of bounds in pyMCDS._get_voxel_idx({0}, {1}, {2}). Setting x = x_max!'.format(x, y, z))
            x = xx.max()
        elif x < xx.min():
            warnings.warn('Position out of bounds: x out of bounds in pyMCDS._get_voxel_idx({0}, {1}, {2}). Setting x = x_min!'.format(x, y, z))
            x = xx.min()
        elif y > yy.max():
            warnings.warn('Position out of bounds: y out of bounds in pyMCDS._get_voxel_idx({0}, {1}, {2}). Setting y = y_max!'.format(x, y, z))
            y = yy.max()
        elif y < yy.min():
            warnings.warn('Position out of bounds: y out of bounds in pyMCDS._get_voxel_idx({0}, {1}, {2}). Setting y = y_min!'.format(x, y, z))
            y = yy.min()
        elif z > zz.max():
            warnings.warn('Position out of bounds: z out of bounds in pyMCDS._get_voxel_idx({0}, {1}, {2}). Setting z = z_max!'.format(x, y, z))
            z = zz.max()
        elif z < zz.min():
            warnings.warn('Position out of bounds: z out of bounds in pyMCDS._get_voxel_idx({0}, {1}, {2}). Setting z = z_min!'.format(x, y, z))
            z = zz.min()
        
        i = np.round((x - xx.min()) / ds)
        j = np.round((y - yy.min()) / ds)
        k = np.round((z - zz.min()) / ds)

        ii, jj, kk = int(i), int(j), int(k)

        return [ii, jj, kk]

    ## MICROENVIRONMENT RELATED FUNCTIONS

    def get_substrate_names(self):
        """
        Returns list of chemical species in microenvironment

        Returns
        -------
        species_list : array (str), shape=[n_species,]
            Contains names of chemical species in microenvironment
        """
        species_list = []
        for name in self.data['continuum_variables']:
            species_list.append(name)

        return species_list
    
    def get_concentrations(self, species_name, z_slice=None):
        """
        Returns the concentration array for the specified chemical species
        in the microenvironment. Can return either the whole 3D picture, or
        a 2D plane of concentrations.

        Parameters
        ----------
        species_name : str
            Name of the chemical species for which to get concentrations
        
        z_slice : float
            z-axis position to use as plane for 2D output. This value must match
            a plane of voxel centers in the z-axis.
        Returns
        -------
        conc_arr : array (np.float) shape=[nx_voxels, ny_voxels, nz_voxels]
            Contains the concentration of the specified chemical in each voxel.
            The array spatially maps to a meshgrid of the voxel centers.
        """
        if z_slice is not None:
            # check to see that z_slice is a valid plane
            zz = self.data['mesh']['z_coordinates']
            assert z_slice in zz, 'Specified z_slice {} not in z_coordinates'.format(z_slice)

            # do the processing if its ok
            mask = zz == z_slice
            full_conc = self.data['continuum_variables'][species_name]['data']
            conc_arr = full_conc[mask].reshape((zz.shape[0], zz.shape[1]))
        else:
            conc_arr = self.data['continuum_variables'][species_name]['data']

        return conc_arr

    def get_concentrations_at(self, x, y, z):
        """
        Return concentrations of each chemical species inside a particular voxel
        that contains the point described in the arguments.
        
        Parameters
        ----------
        x : float
            x-position for the point of interest
        y : float
            y_position for the point of interest
        z : float
            z_position for the point of interest
        
        Returns
        -------
        concs : array, shape=[n_substrates,]
            array of concentrations in the order given by get_substrate_names()
        """
        i, j, k = self.get_containing_voxel_ijk(x, y, z)
        sub_name_list = self.get_substrate_names()
        concs = np.zeros(len(sub_name_list))

        for ix in range(len(sub_name_list)):
            concs[ix] = self.get_concentrations(sub_name_list[ix])[j, i, k]
        
        return concs


    ## CELL RELATED FUNCTIONS

    def get_cell_df(self):
        """
        Builds DataFrame from data['discrete_cells']

        Returns
        -------
        cells_df : pd.Dataframe, shape=[n_cells, n_variables]
            Dataframe containing the cell data for all cells at this time step
        """
        cells_df = pd.DataFrame(self.data['discrete_cells'])
        return cells_df
    
    def get_cell_variables(self):
        """
        Returns the names of all of the cell variables tracked in ['discrete cells']
        dictionary

        Returns
        -------
        var_list : list, shape=[n_variables]
            Contains the names of the cell variables
        """
        var_list = []
        for name in self.data['discrete_cells']:
            var_list.append(name)
        return var_list

    def get_cell_df_at(self, x, y, z):
        """
        Returns a dataframe for cells in the same voxel as the position given by
        x, y, and z.

        Parameters
        ----------
        x : float
            x-position for the point of interest
        y : float
            y_position for the point of interest
        z : float
            z_position for the point of interest

        Returns
        -------
        vox_df : pd.DataFrame, shape=[n_cell_in_voxel, n_variables]
            cell dataframe containing only cells in the same voxel as the point 
            specified by x, y, and z.
        """
        ds = self.get_mesh_spacing()
        xx, yy, zz = self.get_mesh()
        i, j, k = self.get_containing_voxel_ijk(x, y, z)
        x_vox = xx[j, i, k]
        y_vox = yy[j, i, k]
        z_vox = zz[j, i, k]

        cell_df = self.get_cell_df()
        inside_voxel = ( (cell_df['position_x'] < x_vox + ds/2.) &
                         (cell_df['position_x'] > x_vox - ds/2.) &
                         (cell_df['position_y'] < y_vox + ds/2.) &
                         (cell_df['position_y'] > y_vox - ds/2.) &
                         (cell_df['position_z'] < z_vox + ds/2.) &
                         (cell_df['position_z'] > z_vox - ds/2.) )
        vox_df = cell_df[inside_voxel]
        return vox_df

    def _read_xml(self, xml_file, output_path='.'):
        """
        Does the actual work of initializing MultiCellDS by parsing the xml
        """

        output_path = Path(output_path)
        xml_file = output_path / xml_file
        tree = ET.parse(xml_file)

        print('Reading {}'.format(xml_file))

        root = tree.getroot()
        MCDS = {}

        # Get current simulated time
        metadata_node = root.find('metadata')
        time_node = metadata_node.find('current_time')
        MCDS['metadata'] = {}
        MCDS['metadata']['current_time'] = float(time_node.text)
        MCDS['metadata']['time_units'] = time_node.get('units')

        # Get current runtime
        time_node = metadata_node.find('current_runtime')
        MCDS['metadata']['current_runtime'] = float(time_node.text)
        MCDS['metadata']['runtime_units'] = time_node.get('units')

        # find the microenvironment node
        me_node = root.find('microenvironment')
        me_node = me_node.find('domain')

        # find the mesh node
        mesh_node = me_node.find('mesh')
        MCDS['metadata']['spatial_units'] = mesh_node.get('units')
        MCDS['mesh'] = {}

        # while we're at it, find the mesh
        coord_str = mesh_node.find('x_coordinates').text
        delimiter = mesh_node.find('x_coordinates').get('delimiter')
        x_coords = np.array(coord_str.split(delimiter), dtype=np.float)

        coord_str = mesh_node.find('y_coordinates').text
        delimiter = mesh_node.find('y_coordinates').get('delimiter')
        y_coords = np.array(coord_str.split(delimiter), dtype=np.float)

        coord_str = mesh_node.find('z_coordinates').text
        delimiter = mesh_node.find('z_coordinates').get('delimiter')
        z_coords = np.array(coord_str.split(delimiter), dtype=np.float)

        # reshape into a mesh grid
        xx, yy, zz = np.meshgrid(x_coords, y_coords, z_coords)

        MCDS['mesh']['x_coordinates'] = xx
        MCDS['mesh']['y_coordinates'] = yy
        MCDS['mesh']['z_coordinates'] = zz

        # Voxel data must be loaded from .mat file
        voxel_file = mesh_node.find('voxels').find('filename').text
        voxel_path = output_path / voxel_file
        try:
            initial_mesh = sio.loadmat(voxel_path)['mesh']
        except:
            raise FileNotFoundError(
                "No such file or directory:\n'{}' referenced in '{}'".format(voxel_path, xml_file))
            sys.exit(1)

        print('Reading {}'.format(voxel_path))

        # center of voxel specified by first three rows [ x, y, z ]
        # volume specified by fourth row
        MCDS['mesh']['voxels'] = {}
        MCDS['mesh']['voxels']['centers'] = initial_mesh[:3, :]
        MCDS['mesh']['voxels']['volumes'] = initial_mesh[3, :]

        # Continuum_variables, unlike in the matlab version the individual chemical
        # species will be primarily accessed through their names e.g.
        # MCDS['continuum_variables']['oxygen']['units']
        # MCDS['continuum_variables']['glucose']['data']
        MCDS['continuum_variables'] = {}
        variables_node = me_node.find('variables')
        file_node = me_node.find('data').find('filename')

        # micro environment data is shape [4+n, len(voxels)] where n is the number
        # of species being tracked. the first 3 rows represent (x, y, z) of voxel
        # centers. The fourth row contains the voxel volume. The 5th row and up will
        # contain values for that species in that voxel.
        me_file = file_node.text
        me_path = output_path / me_file
        # Changes here
        try:
            me_data = sio.loadmat(me_path)['multiscale_microenvironment']
        except:
            raise FileNotFoundError(
                "No such file or directory:\n'{}' referenced in '{}'".format(me_path, xml_file))
            sys.exit(1)

        print('Reading {}'.format(me_path))

        var_children = variables_node.findall('variable')

        # we're going to need the linear x, y, and z coordinates later
        # but we dont need to get them in the loop
        X, Y, Z = np.unique(xx), np.unique(yy), np.unique(zz)

        for si, species in enumerate(var_children):
            species_name = species.get('name')
            MCDS['continuum_variables'][species_name] = {}
            MCDS['continuum_variables'][species_name]['units'] = species.get(
                'units')

            print('Parsing {:s} data'.format(species_name))

            # initialize array for concentration data
            MCDS['continuum_variables'][species_name]['data'] = np.zeros(xx.shape)

            # travel down one level on tree
            species = species.find('physical_parameter_set')

            # diffusion data for each species
            MCDS['continuum_variables'][species_name]['diffusion_coefficient'] = {}
            MCDS['continuum_variables'][species_name]['diffusion_coefficient']['value'] \
                = float(species.find('diffusion_coefficient').text)
            MCDS['continuum_variables'][species_name]['diffusion_coefficient']['units'] \
                = species.find('diffusion_coefficient').get('units')

            # decay data for each species
            MCDS['continuum_variables'][species_name]['decay_rate'] = {}
            MCDS['continuum_variables'][species_name]['decay_rate']['value'] \
                = float(species.find('decay_rate').text)
            MCDS['continuum_variables'][species_name]['decay_rate']['units'] \
                = species.find('decay_rate').get('units')

            # store data from microenvironment file as numpy array            
            # iterate over each voxel
            for vox_idx in range(MCDS['mesh']['voxels']['centers'].shape[1]):
                # find the center
                center = MCDS['mesh']['voxels']['centers'][:, vox_idx]

                i = np.where(np.abs(center[0] - X) < 1e-10)[0][0]
                j = np.where(np.abs(center[1] - Y) < 1e-10)[0][0]
                k = np.where(np.abs(center[2] - Z) < 1e-10)[0][0]

                MCDS['continuum_variables'][species_name]['data'][j, i, k] \
                    = me_data[4+si, vox_idx]

        # in order to get to the good stuff we have to pass through a few different
        # hierarchal levels
        cell_node = root.find('cellular_information')
        cell_node = cell_node.find('cell_populations')
        cell_node = cell_node.find('cell_population')
        cell_node = cell_node.find('custom')
        # we want the PhysiCell data, there is more of it
        for child in cell_node.findall('simplified_data'):
            if child.get('source') == 'PhysiCell':
                cell_node = child
                break

        MCDS['discrete_cells'] = {}
        data_labels = []
        # iterate over 'label's which are children of 'labels' these will be used to
        # label data arrays
        for label in cell_node.find('labels').findall('label'):
            # I don't like spaces in my dictionary keys
            fixed_label = label.text.replace(' ', '_')
            if int(label.get('size')) > 1:
                # tags to differentiate repeated labels (usually space related)
                dir_label = ['_x', '_y', '_z']
                for i in range(int(label.get('size'))):
                    data_labels.append(fixed_label + dir_label[i])
            else:
                data_labels.append(fixed_label)

        # load the file
        cell_file = cell_node.find('filename').text
        cell_path = output_path / cell_file
        try:
            cell_data = sio.loadmat(cell_path)['cells']
        except:
            raise FileNotFoundError(
                "No such file or directory:\n'{}' referenced in '{}'".format(cell_path, xml_file))
            sys.exit(1)

        print('Reading {}'.format(cell_path))

        for col in range(len(data_labels)):
            MCDS['discrete_cells'][data_labels[col]] = cell_data[col, :]

        return MCDS
