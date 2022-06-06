#ifndef _PhysiBoSS_utils_h_
#define _PhysiBoSS_utils_h_
#include "maboss_network.h"
class MaBoSSInput
{
    static const int NODE = 0;
    static const int PARAMETER = 1;

public:
    std::string physicell_name;
    int type;
    std::string intracellular_name;
    std::string intracellular_parameter;
    std::string action;
    double threshold;
    double inact_threshold;
    double scaling;
    int smoothing;
    double smoothed_value;
    MaBoSSInput(std::string physicell_name, std::string intracellular_name, std::string action, double threshold, double inact_threshold, int smoothing) : physicell_name(physicell_name), intracellular_name(intracellular_name), action(action), threshold(threshold), inact_threshold(inact_threshold), smoothing(smoothing) {
        type = NODE;
        smoothed_value = 0;
    }

    MaBoSSInput(std::string physicell_name, std::string intracellular_parameter, double scaling, int smoothing) : physicell_name(physicell_name), intracellular_parameter(intracellular_parameter), scaling(scaling), smoothing(smoothing) {
        type = PARAMETER;
        smoothed_value = 0;
    }

    bool isNode() { return type == NODE; }
    bool isParameter() { return type == PARAMETER; }

};

class MaBoSSOutput
{
public:
    std::string physicell_name;
    std::string intracellular_name;
    std::string action;
    double value;
    double base_value;
    int smoothing;
    double smoothed_value;

    MaBoSSOutput(std::string physicell_name, std::string intracellular_name, std::string action, double value, double base_value, int smoothing) : physicell_name(physicell_name), intracellular_name(intracellular_name), action(action), value(value), base_value(base_value), smoothing(smoothing) {
        smoothed_value = base_value;
    }

};
#endif