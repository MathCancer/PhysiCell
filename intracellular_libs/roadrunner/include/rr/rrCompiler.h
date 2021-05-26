/*
 * rrCompiler.h
 *
 *  Author: Andy Somogyi
 *  Created on: Jul 15, 2013
 */

#ifndef rrCompiler_H
#define rrCompiler_H

#include "rrOSSpecifics.h"
#include <string>

namespace rr
{

/**
 * interface to manipulate 'compiler' settings.
 *
 * In the C generating model sytem, an external C compiler is used to compile
 * generated C code. However, in the LLVM system, there is no code generation
 * nor compilation, so this interface is not really usefull, but is implemented
 * for compatiblity with the C system.
 */
class RR_DECLSPEC Compiler
{
public:
    virtual std::string getCompiler() const = 0;
    virtual bool setCompiler(const std::string& compiler) = 0;

    virtual std::string getCompilerLocation() const = 0;
    virtual bool setCompilerLocation(const std::string& path) = 0;

    virtual std::string getSupportCodeFolder() const = 0;
    virtual bool setSupportCodeFolder(const std::string& path) = 0;


    /**
     * getDefaultTargetTriple() - Return the default target triple the compiler
     * has been configured to produce code for.
     *
     * The target triple is a string in the format of:
     *   CPU_TYPE-VENDOR-OPERATING_SYSTEM
     * or
     *   CPU_TYPE-VENDOR-KERNEL-OPERATING_SYSTEM
     */
    virtual std::string getDefaultTargetTriple() = 0;

    /**
     * getProcessTriple() - Return an appropriate target triple for generating
     * code to be loaded into the current process, e.g. when using the JIT.
     */
    virtual std::string getProcessTriple() = 0;

    /**
     * getHostCPUName - Get the LLVM name for the host CPU. The particular format
     * of the name is target dependent, and suitable for passing as -mcpu to the
     * target which matches the host.
     *
     * \return - The host CPU name, or empty if the CPU could not be determined.
     */
    virtual std::string getHostCPUName() = 0;

    /**
     * get the version of the compiler.
     */
    virtual std::string getVersion() = 0;

    static std::string getDefaultCompiler();

    static Compiler* New();

    virtual ~Compiler() {};
};

} /* namespace rr */
#endif /* rrCompiler_H */
