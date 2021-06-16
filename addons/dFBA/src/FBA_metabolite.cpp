/*
 * Metabolite.cpp
 *
 *  Created on: 13 jun. 2019
 *      Author: mponce
 */


#include "FBA_metabolite.h"

#include <iostream>
#include <vector>
#include <map>



FBA_metabolite::FBA_metabolite(std::string id) {
   this->id = id;
}

FBA_metabolite::~FBA_metabolite() {

}

const std::string& FBA_metabolite::getId() const {
  return this->id;
}

void FBA_metabolite::setName(std::string value) {
  this->name = value;
}

const std::string& FBA_metabolite::getName() const {
  return this->name;
}


