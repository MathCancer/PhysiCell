/*
 * ExecutableModelFactory.h
 *
 *  Created on: Dec 11, 2014
 *      Author: andy
 */

#ifndef EXECUTABLEMODELFACTORY_H_
#define EXECUTABLEMODELFACTORY_H_

#include "rrExecutableModel.h"
#include "Dictionary.h"
#include <string>


namespace rr
{

/**
 * A factory class to create ExecutableModel objects.
 */
class RR_DECLSPEC ExecutableModelFactory
{
public:
    /**
     * creates a NEW object which must be deleted by the caller.
     *
     * @param sbml: an sbml string
     * @param dict: a dictionary of options, this is typcally a LoadSBMLOptions object,
     * but it may be any dictionary.
     */
    static ExecutableModel *createModel(const std::string& sbml, const Dictionary* dict = 0);
};

} /* namespace rr */

#endif /* EXECUTABLEMODELFACTORY_H_ */
