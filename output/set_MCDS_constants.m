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
% MCDS_constants = set_MCDS_constants( )
%
% This function sets numerous MultiCellDS constants, to help identify cycle 
% models, phases, and other important types in MultiCellDS data. 
%
%
% Copyright 2016-2017 Paul Macklin / PhysiCell project
% Licensed under 3-Clause BSD

function MCDS_constants = set_MCDS_constants( )

MCDS_constants = [];

% currently recognized cell cycle models 
MCDS_constants.advanced_Ki67_cycle_model= 0;
MCDS_constants.basic_Ki67_cycle_model=1;
MCDS_constants.flow_cytometry_cycle_model=2;
MCDS_constants.live_apoptotic_cycle_model=3;
MCDS_constants.total_cells_cycle_model=4;
MCDS_constants.live_cells_cycle_model = 5; 

MCDS_constants.flow_cytometry_separated_cycle_model  = 6; 
	
% currently recognized death models 
MCDS_constants.apoptosis_death_model = 100; 
MCDS_constants.necrosis_death_model = 101; 
MCDS_constants.autophagy_death_model = 102; 
	
MCDS_constants.custom_cycle_model=9999; 
	
% currently recognized cell cycle and death phases 
% cycle phases
MCDS_constants.Ki67_positive_premitotic=0; 
MCDS_constants.Ki67_positive_postmitotic=1; 
MCDS_constants.Ki67_positive=2; 
MCDS_constants.Ki67_negative=3; 
MCDS_constants.G0G1_phase=4;
MCDS_constants.G0_phase=5;
MCDS_constants.G1_phase=6; 
MCDS_constants.G1a_phase=7; 
MCDS_constants.G1b_phase=8;
MCDS_constants.G1c_phase=9;
MCDS_constants.S_phase=10;
MCDS_constants.G2M_phase=11;
MCDS_constants.G2_phase=12;
MCDS_constants.M_phase=13;
MCDS_constants.live=14;

MCDS_constants.G1pm_phase = 15;
MCDS_constants.G1ps_phase = 16;

	
MCDS_constants.custom_phase = 9999;
% death phases
MCDS_constants.apoptotic=100;
MCDS_constants.necrotic_swelling=101;
MCDS_constants.necrotic_lysed=102;
MCDS_constants.necrotic=103; 
MCDS_constants.debris=104; 

return; 

