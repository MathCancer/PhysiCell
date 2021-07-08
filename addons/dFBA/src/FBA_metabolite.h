/*
 * Metabolite.h
 *
 *  Created on: 13 jun. 2019
 *      Author: mponce
 */

#ifndef SRC_METABOLITE_H_
#define SRC_METABOLITE_H_

#include <iostream>
#include <vector>
#include <map>

class FBA_metabolite
{
	private:
		std::string id;
		std::string name;

	public:
		FBA_metabolite(std::string id);
		~FBA_metabolite();

		const std::string& getId() const;

		void setName(std::string value);
		const std::string& getName() const;

};



#endif /* SRC_METABOLITE_H_ */
