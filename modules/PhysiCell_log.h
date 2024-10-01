#include <fstream>
#include <iostream>

namespace PhysiCell
{
    extern std::ofstream log_file;

    void open_log_file(std::string filename);
    void close_log_file(void);
    void flush_log_file(void);
    void log_output(std::string message, bool also_write_to_cout = false, bool add_newline_at_end = true);
    void log_output(std::ostream &(*f)(std::ostream &), bool also_write_to_cout = false, bool add_newline_at_end = true);
    void log_warning(std::string message, std::string filename = "");
    void log_error(std::string message, std::string filename = "");
}