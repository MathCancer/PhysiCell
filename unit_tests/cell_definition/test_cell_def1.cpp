/*=============================================================================
# Copyright (c) 2015-2018, Paul Macklin and the PhysiCell Project
# All rights reserved.
#
# For citation details, see https://github.com/MathCancer/PhysiCell/blob/master/CITATION.txt
#
# For license details, see https://github.com/MathCancer/PhysiCell/tree/master/licenses 
#
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
# POSSIBILITY OF SUCH DAMAGE. 
==============================================================================*/

#include<stdio.h>
#include "../../core/PhysiCell_cell.h"

using namespace PhysiCell;

Cell_Definition celltype1;  // declare, but don't define

int main(int argc, char** argv)
{
    std::cout << "--------  " << argv[0] << "  --------"  << std::endl;
    // build_cell_definitions_maps(); 
    display_cell_definitions( std::cout ); 
    std::cout << std::endl;
}
