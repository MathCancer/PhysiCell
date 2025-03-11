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
    bool use_for_dead;

    MaBoSSInput() 
        : physicell_name(""),
          type(NODE),
          intracellular_name(""),
          intracellular_parameter(""),
          action(""),
          threshold(0.0),
          inact_threshold(0.0),
          scaling(1.0),
          smoothing(1),
          smoothed_value(0.0),
          use_for_dead(false) {}

    void save_maboss_input(std::ostream& out_stream) const {
        out_stream << "MaBoSSInput: " << std::endl;
        out_stream << this->physicell_name << std::endl;
        out_stream << "type: " << this->type << std::endl;
        out_stream << this->intracellular_name << std::endl;
        out_stream << this->intracellular_parameter << std::endl;
        out_stream << this->action << std::endl;
        out_stream << "threshold: " << this->threshold << std::endl;
        out_stream << "inact_threshold: " << this->inact_threshold << std::endl;
        out_stream << "scaling: " << this->scaling << std::endl;
        out_stream << "smoothing: " << this->smoothing << std::endl;
        out_stream << "smoothed_value: " << this->smoothed_value << std::endl;
        out_stream << "use_for_dead: " << (this->use_for_dead ? "true" : "false") << std::endl;
    }

    void read_maboss_input(std::ifstream& in_stream) {
        std::string dummy;
        std::getline(in_stream, dummy);
        
        std::getline(in_stream, dummy);
        physicell_name = dummy;

        std::getline(in_stream, dummy);
        type = read_number_in_line_int(dummy);

        std::getline(in_stream, dummy);
        intracellular_name = dummy;

        std::getline(in_stream, dummy);
        intracellular_parameter = dummy;

        std::getline(in_stream, dummy);
        action = dummy;

        std::getline(in_stream, dummy);
        threshold = read_number_in_line(dummy);

        std::getline(in_stream, dummy);
        inact_threshold = read_number_in_line(dummy);

        std::getline(in_stream, dummy);
        //std::cout << "dummy: " << dummy << std::endl;   
        //std::cout << "scaling pre: " << scaling << std::endl;
        scaling = read_number_in_line(dummy);
        //std::cout << "scaling post: " << scaling << std::endl;

        std::getline(in_stream, dummy);
        smoothing = read_number_in_line_int(dummy);

        std::getline(in_stream, dummy);
        smoothed_value = read_number_in_line(dummy);

        std::getline(in_stream, dummy);
        use_for_dead = read_number_in_line_bool(dummy);
    }
    
    MaBoSSInput(std::string physicell_name, std::string intracellular_name, std::string action, double threshold, double inact_threshold, int smoothing, bool use_for_dead) : physicell_name(physicell_name), intracellular_name(intracellular_name), action(action), threshold(threshold), inact_threshold(inact_threshold), smoothing(smoothing), use_for_dead(use_for_dead){
        type = NODE;
        smoothed_value = 0;
    }

    MaBoSSInput(std::string physicell_name, std::string intracellular_parameter, double scaling, int smoothing, bool use_for_dead) : physicell_name(physicell_name), intracellular_parameter(intracellular_parameter), scaling(scaling), smoothing(smoothing), use_for_dead(use_for_dead) {
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
    double probability;
    bool initialized = false;
    int steepness;
    bool use_for_dead;

    MaBoSSOutput() 
    : physicell_name(""),
      intracellular_name(""),
      action(""),
      value(0.0),
      base_value(0.0),
      smoothing(1),
      probability(0.5),
      initialized(false),
      steepness(1),
      use_for_dead(false) {}

      void save_maboss_output(std::ostream& out_stream) const {
        out_stream << "MaBoSSOutput: " << std::endl;
        out_stream << this->physicell_name << std::endl;
        out_stream << this->intracellular_name << std::endl;
        out_stream << this->action << std::endl;
        out_stream << "value: " << this->value << std::endl;
        out_stream << "base_value: " << this->base_value << std::endl;
        out_stream << "smoothing: " << this->smoothing << std::endl;
        out_stream << "probability: " << this->probability << std::endl;
        out_stream << "initialized: " << (this->initialized ? "true" : "false") << std::endl;
        out_stream << "steepness: " << this->steepness << std::endl;
        out_stream << "use_for_dead: " << (this->use_for_dead ? "true" : "false") << std::endl;
    }

    void read_maboss_output(std::ifstream& in_stream) {
        std::string dummy;
        std::getline(in_stream, dummy); // Legge l'intestazione (da ignorare)

        std::getline(in_stream, dummy);
        physicell_name = dummy;

        std::getline(in_stream, dummy);
        intracellular_name = dummy;

        std::getline(in_stream, dummy);
        action = dummy;

        std::getline(in_stream, dummy);
        value = read_number_in_line(dummy);

        std::getline(in_stream, dummy);
        base_value = read_number_in_line(dummy);

        std::getline(in_stream, dummy);
        smoothing = read_number_in_line_int(dummy);

        std::getline(in_stream, dummy);
        probability = read_number_in_line(dummy);

        std::getline(in_stream, dummy);
        initialized = read_number_in_line_bool(dummy);

        std::getline(in_stream, dummy);
        steepness = read_number_in_line_int(dummy);

        std::getline(in_stream, dummy);
        use_for_dead = read_number_in_line_bool(dummy);
    }
    
    MaBoSSOutput(std::string physicell_name, std::string intracellular_name,
                std::string action, double value, double base_value,
                int smoothing, int steepness, bool use_for_dead)
        : physicell_name(physicell_name), intracellular_name(intracellular_name),
        action(action), value(value), base_value(base_value),
        smoothing(smoothing), steepness(steepness), use_for_dead(use_for_dead) {
    probability = 0.5;
    }

    double update_probability(bool test) {
    if (!initialized) {
        probability = test;
        initialized = true;
    } else
        probability =
            (probability * smoothing + (test ? 1.0 : 0.0)) / (smoothing + 1.0);

    return probability;
    }

    double update(bool test) {

    double hill_input;

    if (smoothing == 0) {
        hill_input = test ? 1.0 : 0.0;
    } else {
        hill_input = update_probability(test);
    }

    if (action == "activation") {
        double hill = PhysiCell::Hill_response_function(hill_input * 2, 1, steepness);
        return (value - base_value) * hill + base_value;
    } else if (action == "inhibition") {
        double hill = PhysiCell::Hill_response_function(hill_input * 2, 1, steepness);
        return ((value - base_value) * (1 - hill)) + base_value;
    }

    return base_value;
    }
};
#endif