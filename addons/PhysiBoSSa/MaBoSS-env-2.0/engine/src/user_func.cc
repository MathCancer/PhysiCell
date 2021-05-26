
//
// How to add a user_func example:
//  create a C++ module <module_name> similar to this one
//  make FUNC_MODULE=<module_name> <module_name>
//  for instance:
//  make FUNC_MODULE=user_func user_func
//
//

#include "BooleanNetwork.h"

class TestFunction : public Function {

public:
  TestFunction() : Function("test", 1, 2) { }

  double eval(const Node* this_node, const NetworkState& network_state, ArgumentList* arg_list) {
    const std::vector<Expression*>& expr_v = arg_list->getExpressionList();
    std::vector<Expression*>::const_iterator iter = expr_v.begin();
    double val = (*iter)->eval(this_node, network_state);
    if (expr_v.size() == 1) {
      std::cout << "test function(" << val << ")\n";
      return log(val+10);
    }

    iter++;
    double base = (*iter)->eval(this_node, network_state);
    return log(val) / log(base);
  }

  std::string getDescription() const {
    return "double test(double VALUE[, double BASE=e])\n  computes the value of the natural testarithm of VALUE; uses BASE if set";
  }
};

extern "C" void maboss_user_func_init(std::map<std::string, Function*>* func_map)
{
  Function::setFuncMap(func_map);
  new TestFunction();
}
