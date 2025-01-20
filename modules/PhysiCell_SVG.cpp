/*
###############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the version #
# number, such as below:                                                      #
#                                                                             #
# We implemented and solved the model using PhysiCell (Version x.y.z) [1].    #
#                                                                             #
# [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  #
#     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   #
#     DOI: 10.1371/journal.pcbi.1005991                                       #
#                                                                             #
# See VERSION.txt or call get_PhysiCell_version() to get the current version  #
#     x.y.z. Call display_citations() to get detailed information on all cite-#
#     able software used in your PhysiCell application.                       #
#                                                                             #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite BioFVM  #
#     as below:                                                               #
#                                                                             #
# We implemented and solved the model using PhysiCell (Version x.y.z) [1],    #
# with BioFVM [2] to solve the transport equations.                           #
#                                                                             #
# [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  #
#     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   #
#     DOI: 10.1371/journal.pcbi.1005991                                       #
#                                                                             #
# [2] A Ghaffarizadeh, SH Friedman, and P Macklin, BioFVM: an efficient para- #
#     llelized diffusive transport solver for 3-D biological simulations,     #
#     Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730  #
#                                                                             #
###############################################################################
#                                                                             #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)     #
#                                                                             #
# Copyright (c) 2015-2025, Paul Macklin and the PhysiCell Project             #
# All rights reserved.                                                        #
#                                                                             #
# Redistribution and use in source and binary forms, with or without          #
# modification, are permitted provided that the following conditions are met: #
#                                                                             #
# 1. Redistributions of source code must retain the above copyright notice,   #
# this list of conditions and the following disclaimer.                       #
#                                                                             #
# 2. Redistributions in binary form must reproduce the above copyright        #
# notice, this list of conditions and the following disclaimer in the         #
# documentation and/or other materials provided with the distribution.        #
#                                                                             #
# 3. Neither the name of the copyright holder nor the names of its            #
# contributors may be used to endorse or promote products derived from this   #
# software without specific prior written permission.                         #
#                                                                             #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" #
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   #
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  #
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   #
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         #
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF        #
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    #
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN     #
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)     #
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE  #
# POSSIBILITY OF SUCH DAMAGE.                                                 #
#                                                                             #
###############################################################################
*/

#include "./PhysiCell_SVG.h"

bool Write_SVG_start( std::ostream& os, double width, double height )
{
 os << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" << std::endl 
    << "<!-- Created with PhysiCell (http://PhysiCell.MathCancer.org/) -->" << std::endl; 

 os << "<svg " << std::endl
    << " xmlns:dc=\"http://purl.org/dc/elements/1.1/\" " << std::endl
    << " xmlns:cc=\"http://creativecommons.org/ns#\" " << std::endl
    << " xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\" " << std::endl
    << " xmlns:svg=\"http://www.w3.org/2000/svg\" " << std::endl
    << " xmlns=\"http://www.w3.org/2000/svg\" " << std::endl
    << " version=\"1.1\" " << std::endl
    << " width=\"" << width << "\" " << std::endl
    << " height=\"" << height << "\" " << std::endl
    << " id=\"svg2\">" << std::endl;
	
	return true; 
}

bool Write_SVG_end( std::ostream& os )
{
 os << "</svg>" << std::endl;
 return true; 
}

bool Write_SVG_text( std::ostream& os, const char* str , double position_x, double position_y, double font_size , const char* color , const char* font)
{
 os << "  <text x=\"" << position_x << "\" y=\""  << position_y << "\"" << std::endl
    << "   font-family=\"" << font << "\" font-size=\"" << font_size << "\" fill=\"" << color << "\" >" << std::endl
    << "   " << str << std::endl << "  </text>" << std::endl; 
  return true; 
}

void Write_SVG_text(std::ostream& os, const char* str , double position_x, double position_y, double font_size , const char* color , const char* font, double rotation)
{
    double text_width = font_size * strlen(str) / 2.0;  // estimate the width of the text
    double text_height = font_size / 2.0;  // estimate the height of the text

    double center_x = position_x + text_width / 2.0;
    double center_y = position_y + text_height / 2.0;

    os << "<text x=\"" << position_x << "\" y=\"" << position_y << "\" font-size=\"" << font_size << "\" fill=\"" << color << "\" font-family=\"" << font << "\" transform=\"rotate(" << rotation << " " << center_x << " " << center_y << ")\">" << str << "</text>\n";
}

bool Write_SVG_circle( std::ostream& os, double center_x, double center_y, double radius, double stroke_size, 
                       std::string stroke_color , std::string fill_color )
{
 os << "  <circle cx=\"" << center_x << "\" cy=\"" << center_y << "\" r=\"" << radius << "\" stroke-width=\"" << stroke_size 
    << "\" stroke=\"" << stroke_color << "\" fill=\"" << fill_color << "\"/>" << std::endl; 
 return true; 
}


bool Write_SVG_rect( std::ostream& os , double UL_corner_x, double UL_corner_y, double width, double height, 
                     double stroke_size, std::string stroke_color , std::string fill_color )
{
 os << "  <rect x=\"" << UL_corner_x << "\" y=\"" << UL_corner_y << "\" width=\"" << width << "\" height=\"" 
    << height << "\" stroke-width=\"" << stroke_size 
    << "\" stroke=\"" << stroke_color << "\" fill=\"" << fill_color << "\"/>" << std::endl; 
 return true; 
}

bool Write_SVG_line( std::ostream& os , double start_x, double start_y, double end_x , double end_y, double thickness, 
                    std::string stroke_color )
{
 os << "  <line x1=\"" << start_x << "\" y1=\"" << start_y << "\" x2=\"" << end_x << "\" y2=\"" << end_y << "\" "
    << "stroke=\"" << stroke_color << "\" stroke-width=\"" << thickness << "\"/>" << std::endl; 
 return true; 
}
