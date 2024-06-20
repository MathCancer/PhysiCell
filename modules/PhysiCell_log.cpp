#include "PhysiCell_log.h"

namespace PhysiCell
{
    std::ofstream log_file;

    void open_log_file(std::string filename)
    {
        log_file.open(filename.c_str());
        return;
    }

    void close_log_file(void)
    {
        log_file.close();
        return;
    }

    void flush_log_file(void)
    {
        log_file.flush();
        return;
    }

    void log_output(std::string message, bool also_write_to_cout, bool add_newline_at_end)
    {
        log_file << message;
        if (add_newline_at_end)
        {
            log_file << std::endl;
        }
        if (also_write_to_cout)
        {
            std::cout << message << std::endl;
        }
        return;
    }

    void log_output(std::ostream &(*f)(std::ostream &), bool also_write_to_cout, bool add_newline_at_end)
    {
        log_file << f;
        if (add_newline_at_end)
        {
            log_file << std::endl;
        }
        if (also_write_to_cout)
        {
            std::cout << f << std::endl;
        }
        return;
    }

    void log_warning(std::string message, std::string filename)
    {
        log_file << "Warning: " << message;
        std::cout << "\nWARNING: " << message;
        if (filename != "")
        {
            log_file << " (in " << filename << ")" << std::endl;
            std::cout << " (in " << filename << ")\n" << std::endl;
        }
        return;
    }

    void log_error(std::string message, std::string filename)
    {
        log_file << "Error: " << message;
        std::cout << "ERROR: " << message;
        if (filename != "")
        {
            log_file << " (in " << filename << ")" << std::endl;
            std::cout << " (in " << filename << ")" << std::endl;
        }
        close_log_file();
        exit(-1);
        return;
    }
}
