namespace rr {

/** 
@mainpage The LibRoadRunner C++ Library

@author Andy Somogyi (andy.somogyi@gmail.com, somogyie@indiana.edu)
@author Totte Karlsson (totte@dunescientific.com)
@author Herbert M. Sauro  (hsauro@u.washington.edu)


@section sec_intro Introduction
 


This document describes the application programming interface (API) of RoadRunner, 
an open source (BSD) library for computing structural characteristics of cellular networks.


 

@section sec_install Instalation

 \par

 \par

 \par Dependencies
 The RoadRunner library depend on several third-party libraries, CLapack, libSBML (libxml2), Sundials, NLEQ, and Poco. These are provided with the binary installation where necessary.
 \par


 \par
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 \li Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

 \li Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

 \li Neither the name of University of Washington nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

 \par


@section sec_new_integrator Creating a new Integrator

One of the key design goals of the LibRoadRunner library is extensibility. This means that the 
library is designed with the idea that most internal components are loosely coupled and that
it is simple to add new solvers such as new steady state solvers or integrators. This section
will give a tutorial on creating a new integrator using the EulerIntegrator as an example. 

At its simplest, an Integrator is a class which implements the Integrator interface and
is responsible for advanding a model (an object which implements the ExecutableModel interface)
forward in time. 

All Integrators are created by the IntegratorFactory class, this is the only class that knows
about the existence of any Integrator objects. All integrators that the IntegratorFactory knows about
are automatically available to any code (including any Python code) that needs to create one. Each time 
the RoadRunner::simulate method is called, a different integrator may be specified. In Python, this is
done with the <TT>integrator</TT> keyword, i.e.

@code 
r.simulate(0, 10, 100, integrator="MyIntegratorName")
@endcode

Or, in C++:

@code
BasicDictionary d;
d.setItem("integrator", "MyIntegratorName")
r.simulate(&d);
@endcode

To create a new integrator, one first needs to create an object that implments the Integrator interface,
tell RoadRunner about it. 

@subsection sec_int_impl Implementing the Integrator interface

The IntegratorFactory is the ONLY object that creates integrators.

Integrators are created when the IntegratorFactory::New method is called,
typically by the top level RoadRunner object. New Integrators are given a pointer to an 
existing ExecutableModel object which the Integrator is responsible for advancing forward in time, 
and pointer to a SimulateOptions object which contains the initial set of parameters that the 
Integrator may configure itself with. 

The integrator will hold onto the ExecutableModel pointer, m, and when the
Integrator::integrate method is called, will advance the model object forward in time.


There are three key members of the Integrator interface that an integrator needs be implement: 

@li Integrator::setSimulateOptions
This method is called whenever the simulation parameters are changed via calls to the
RoadRunner::simulate method, the setSimulateOptions method is
called by RoadRunner object to inform the Integrator of any new parameter changes. 
This method should be used to read any updated tuning parameters.

@li Integrator::integrate This is the actual method that should perform the time integration. 

@li Integrator::restart This method is called whenever the RoadRunner object is re-started. This gives the 
integrator an option to reload any internal solvers. Simple integrators like the EulerIntegrator does not 
really do anything in the EulerIntegrator::restart method, but more sophisticated ones like the CVODE integrator
perform a number of tasks such as re-calclating the tollerances and so forth. 

The other key thing that an Integrator needs to do is provide a 
@code 
static const Dictionary* getIntegratorOptions();
@endcode 
method, as in the EulerIntegrator::getIntegratorOptions. This method is used by the IntegratorFactory
to build a list of all the available tuning parameters that any integrator supports. The returned
Dictionary pointer should be statically created inside the implemtation file, and should contain the 
following keys / values


<table>
  <tr>
  <td>Key</td>
  <td>Value</td>
  <tr>
    <td>integrator</td>
    <td>The name of your new integrator</td> 
  </tr>
  <tr>
    <td>integrator.description</td>
    <td>A description of your new integrator</td> 
  </tr>
  <tr>
    <td>integrator.hint</td>
    <td>A short hint for your new integrator</td> 
  </tr>
</table> 


Any additional tuning parameters should be listed in this dictionary, where each tuning parameter
should have three key/value pairs. 
 
<table>
  <tr>
  <td>Key</td>
  <td>Value</td>
  <tr>
    <td>parameterName</td>
    <td>The default value of this tuning parmeter</td>
  </tr>
  <tr>
    <td>parameterName.description</td>
    <td>A description of this tuning parameter</td> 
  </tr>
  <tr>
    <td>integrator.hint</td>
    <td>A short hint for this tuning parameter</td> 
  </tr>
</table> 


When the Integrator::setSimulateOptions method is called, the integrator should read any
parameters it expects out of the given dictionary. 


@subsection sec_tellrr Telling RoadRunner about the new Integrator

In order for the RoadRunner::simulate method use the new integrator, it needs to know about it. 
The IntegratorFactory is the only object that knows about all the integrators, and a few lines of 
code need to be added in the implementation of this object so that it can construct one. 


First, a new enum value needs to be added to the Integrator::IntegratorId enum. This is numeric
index of the integrator. Then the textual name of the integrator needs to be added to the 
<tt>integratorNames</tt> static array in the Integrator.cpp file. Then a line needs to be added inside
the IntegratorFactory::New method which will create the new integrator, e.g. this is a series
of <tt>if</tt> statements, and a new statment needs to be created which creates an instance of the 
new kind of integrator:

@code
if (opt->integrator == Integrator::GILLESPIE)
{
    result = new GillespieIntegrator(m, opt);
}
else if(opt->integrator == Integrator::RK4)
{
    result = new RK4Integrator(m, opt);
}
else if(opt->integrator == Integrator::EULER)
{
    result = new EulerIntegrator(m, opt);
}
else
{
    result = new CVODEIntegrator(m, opt);
}
@endcode

Finally, the IntegratorFactory::getIntegratorOptions() method needs to be updated to also return
the dictionary that was created in the getIntegratorOptions method, e.g.

@code
const Dictionary* options[] = {
        CVODEIntegrator::getIntegratorOptions(),
        GillespieIntegrator::getIntegratorOptions(),
        RK4Integrator::getIntegratorOptions(),
        EulerIntegrator::getIntegratorOptions()
};
@endcode

Once the IntegratorFactory is made aware of your new integrator, it is available for full introspection and
can be used by just adding the <tt>integrator="myNewIntegrator"</tt> argument whenever the 
RoadRunner.simulate method is called. The EulerIntegrator was created as an example of how to create and add
a new integrator, have a look at it. 


@subsection sec_euler A complete example of creating an integrator.
This section includes the complete Euler integrator implemented in EulerIntegrator.h as an example of 
creating a new integrator. 
This class has two demo paramters which may be set via the keyword arguments to RoadRunner.simulate
in Python, or set via the Dictionary::setItem method on the dictionary that is given to the 
RoadRunner::simulate method in C++. In Python, this would be:

@code
r.simulate(integrator='euler', exampleParameter1=123456, exampleParameter2='some value');
print(r.integrator)

< roadrunner.EulerIntegrator() { 
  'this' : 0x101f28350
  'exampleParameter1' : 123456
  'exampleParameter2' : some value
}>
@endcode

In Python, all the keyword arguments to the simulate method are packaged up and added to the 
dictionary which is passed into the RoadRunner::simulate method. In C++, the equivalent code 
would be:
@code
SimulateOptions& opt = r.getSimulateOptions();
opt.setItem("integrator", "euler")
opt.setItem("exampleParameter1", 123456);
opt.setItem("exampleParameter2", "some value");
r.simulate();

cout << r.getIntegrator()->toString() << endl;
@endcode

The EulerIntegrator.h file serves as a complete example of creating an new integrator. 
This example was written entierly in the header file for clarity, but a real integrator 
should separate the code and header files. 


*/
} // namespace rr

