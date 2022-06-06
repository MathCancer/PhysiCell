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

    void update_value(double value) {
        smoothed_value = (smoothed_value * smoothing + value)/(smoothing + 1);
    }

    bool updateNode(bool state, double value) 
    {
        double true_value;
        if (smoothing == 0) {
            true_value = value;
        } else {
            update_value(value);
            true_value = smoothed_value;
        }

        if (state) {
            if (action == "inhibition") {
                return true_value <= inact_threshold; // When the node is active, and this is an activation, the node stays true if the value is below the inact threshold

            } else {
                return true_value >= inact_threshold; // When the node is active, the node stays true if the value is above the inact threshold
            }

        } else {
            if (action == "inhibition") {
                return true_value < threshold;

            } else {
                return true_value > threshold;
            }
        }
    }

    double updateParameter(double value) {
        if (smoothing == 0) {
            return value;
        } else {
            update_value(value);
            return smoothed_value;
        }
    }
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

    void update_value(bool test) {
        smoothed_value = (smoothed_value*smoothing + (test?1.0:0.0)) / (smoothing + 1.0);
    }

    double update(bool test)
    {
        double true_value;
        if (smoothing == 0) {
            true_value = value;
        } else {
            update_value((action == "activation" && test) || (action == "inhibition" and !test));
            true_value = smoothed_value;
        }

        if (action == "activation" && test) {
            double hill = PhysiCell::Hill_response_function( true_value*2 , 1 , 10 ); 
            return (value-base_value)*hill+base_value;
        } else if (action == "inhibition" && !test) {
            double hill = PhysiCell::Hill_response_function( true_value*2 , 1 , 10 ); 
            return value-(value-base_value)*hill;
        }
        return base_value;
    }
};
#endif