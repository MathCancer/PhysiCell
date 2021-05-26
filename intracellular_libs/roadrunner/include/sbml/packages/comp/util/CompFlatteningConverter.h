/**
 * @file    CompFlatteningConverter.h
 * @brief   Definition of a first flattening converter.
 * @author  Frank T. Bergmann
 * 
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2013-2016 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *     3. University of Heidelberg, Heidelberg, Germany
 * 
 * Copyright 2011-2012 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ---------------------------------------------------------------------- -->
 *
 * @class CompFlatteningConverter
 * @sbmlbrief{comp} "Flattens" a model, removing composition.
 *
 * @htmlinclude libsbml-facility-only-warning.html
 *
 * This converter translates a hierarchical model defined with the SBML
 * Level&nbsp;3 Hierarchical %Model Composition package to a so-called
 * "flattened" version of the same model.  A "flattened" model is one that
 * results from interpreting the hierarchical structure and creating an
 * equivalent model that does not use any Hierarchical %Model Composition
 * package constructs; all of the mathematics of the model remain as
 * intended, but the hierarchical structure is removed.
 * CompFlatteningConverter achieves this by performing the following actions
 * on a model:
 * <ol>
 * <li> Each submodel is instantiated; that is, a copy of every Model object
 * referenced by every Submodel object is created.  This is a recursive
 * process: if the instantiated Model itself has Submodel children, they are
 * also instantiated.
 *
 * <li> The identifier of every component object in every Model that is
 * instantiated is modified by prepending a prefix to it, to make the
 * identifier unique in the overall flattened SBML model.  The prefix
 * consists of the Submodel object identifier, plus a small integer if
 * necessary to ensure uniqueness, plus two underscore characters
 * (<code>"__"</code>).  Typically, this results in a final identifier of the
 * form <span
 * class="code-placeholder">%SubmodelIdentifier__</span><span
 * class="code-placeholder2-nospace">ObjectIdentifier</span>, where <span
 * class="code-placeholder2">ObjectIdentifier</span> is the object's original
 * identifier and <span
 * class="code-placeholder">%SubmodelIdentifier__</span> is
 * the prefix.  The same process is applied to @em metaid (meta-identifier)
 * values of every component object as well.
 *
 * <li> All deleted elements are removed from the model and all instantiated
 * submodels.
 *
 * <li> All replaced elements are removed from the model and all instantiated
 * submodels.
 *
 * <li> All references to replaced elements are changed to point to the
 * replacement element instead.
 *
 * <li> All remaining elements are placed in a single Model object; this Model
 * object is made the new child of the SBMLDocument container.  The original
 * Model, ModelDefinition, and ExternalModelDefinition objects are all
 * deleted.
 *
 * </ol>
 *
 * Note an implication of this process is that if conversion is successful,
 * all old pointers to the SBMLDocument object's Model object as well as all
 * of its children, will be rendered obsolete, and will no longer work.
 *
 * @section compflatteningconverter-options-summary Summary of options for CompFlatteningConverter
 *
 * @copydetails doc_compflatteningconverter_options
 *
 * @section compflatteningconverter-options-details Detailed explanation of available options
 *
 * The following sections examine in detail the options understood by
 * CompFlatteningConverter and their effects on the flattening process.
 *
 * @subsection behavior-if-unflattenable Options for handling Level&nbsp;3 packages
 *
 * If other SBML Level&nbsp;3 packages are used in the SBMLDocument, the same
 * rules apply to each package's constructs if an implementation of the
 * flattener exists for that Level&nbsp;3 package.  If no implementation
 * exists, the behavior of this CompFlatteningConverter depends on the values
 * of the options @em "abortIfUnflattenable" and @em
 * "stripUnflattenablePackages".  Specifically:
 * <ul>
 *
 * <li> The option @em "abortIfUnflattenable" controls whether the flattening
 * process is abandoned.  The following are the possible values for this
 * option and their meanings:
 * <ul style="list-style-type: circle">
 *
 * <li> If the @em "abortIfUnflattenable" option is set to @c "all", then if any
 *     construct is found for a package that has no flattening algorithm
 *     available, the converter will abort, return failure, and avoid
 *     changing the original SBMLDocument object.
 *
 * <li> If @em "abortIfUnflattenable" is set to @c "requiredOnly" (the default),
 *     then if any SBML Level&nbsp;3 package construct is found for a package
 *     that is labeled @c "required=true" and for which no flattening
 *     algorithm is available, the converter will abort, return failure, and
 *     avoid changing the original SBMLDocument object.  SBML Level&nbsp;3
 *     package constructs from packages set @c "required=false" for which no
 *     flattening algorithm is implemented will be ignored and the constructs
 *     will be kept or removed depending on the value of the @em
 *     "stripUnflattenablePackages" option described below.
 * 
 * <li> If @em "abortIfUnflattenable" is set to @c 'none', constructs from all
 *     unflattenable SBML Level&nbsp;3 packages will be ignored, and their
 *     constructs will be kept or removed depending on the value of the @em
 *     "stripUnflattenablePackages" option.
 *
 * </ul>
 *
 * <li> The option @em "stripUnflattenablePackages" controls whether SBML
 * Level&nbsp;3 package constructs are removed from the final flattened
 * model.  The following are the possible values for this option and their
 * meanings:
 * <ul style="list-style-type: circle">
 *
 * <li> If the option @em "stripUnflattenablePackages" is set to @c "true", and
 *      the option @em "abortIfUnflattenable" has the value @c "requiredOnly"
 *      or @c "none", then CompFlatteningConverter will remove all constructs
 *      belonging to packages that lack a flattening converter.  The XML
 *      namespace declaration for unflattenable SBML Level&nbsp;3 packages
 *      will also be removed.
 *
 * <li> If the option @em "stripUnflattenablePackages" is set to @c "false"
 *     (the default), CompFlatteningConverter will leave any package
 *     constructs that cannot be flattened due to the lack of a flattening
 *     implementation.  These Level&nbsp;3 package constructs will remain in
 *     the final model.  Note that any package constructs from an
 *     instantiated Submodel object that was @em not a child of a retained
 *     component will be lost (because the SBML component where it was
 *     located was removed from the final, flattened model).
 *
 * </ul>
 *
 * <li> The option @em "stripPackages" controls whether SBML Level&nbsp;3
 * package information is stripped from a model before conversion.
 * Regardless of the above options, if an SBML package is listed in this
 * comma-separated list, all constructs from that package will be removed
 * from the model before the flattening algorithm is applied.  This happens
 * whether a flattening algorithm exists for that package or not, and applies
 * to all referenced models, both local and external.  In addition, the
 * removal takes place before any validation is performed by libSBML.
 *
 * </ul>
 *
 * @subsection general-flattener-options Additional general conversion options for CompFlatteningConverter
 *
 * The following are other, general options offered by CompFlatteningConverter:
 *
 * @li @em "basePath": if there are ExternalModelDefinition objects that are to
 *     be instantiated in a flattened Submodel, the @em "basePath" option may
 *     be set to a location where those external models may be found.  The
 *     default is the working directory (@c ".").
 *
 * @li @em "leavePorts": if this option is set to @c "false" (the default), the
 *     flattened model will have no Port objects in it.  Conversely, if @em
 *     "leavePorts" is set to @c "true", any Port objects not referenced by any
 *     Replacement or Deletion will be left in the resulting flattened Model.
 *
 * @li @em "listModelDefinitions": If this option is set to @c "false" (the
 *     default), no ModelDefinition or ExternalModelDefinition objects will
 *     be present in the flattened SBMLDocument.  If @em "listModelDefinitions"
 *     is set to @c "true", they will remain, although they will no longer be
 *     referenced by any Submodel in the flattened Model child of the
 *     SBMLDocument.
 *
 * @li @em "performValidation": if this option is set to @c "true" (the
 *     default), the SBMLDocument will first be validated before flattening
 *     is attempted.  If there are any validation errors, those errors will
 *     be logged with the SBMLDocument, which will remain otherwise
 *     unchanged, and the conversion process will return a failure code.
 *     Similarly, if the flattened Model is not valid, those validation
 *     errors will be added to the SBMLDocument, which will remain otherwise
 *     unchanged, and the conversion attempt will return failure.
 *     Conversely, if the option @em "performValidation" is set to @c
 *     "false", the SBMLDocument will be flattened regardless of any
 *     validation errors.  The conversion may yet fail if insurmountable
 *     errors are encountered in the course of trying to flatten the model
 *     (for instance, if an element is replaced by something that does not
 *     exist), but no separate validation steps are performed.
 *
 * Note that if both the option @em "leavePorts" and @em "listModelDefinitions"
 * are set to @c "false" (which they are by default), the Hierarchical %Model
 * Composition namespace will be removed from the resulting SBMLDocument.
 *
 * @section comp-flattening-example Complete example of invoking CompFlatteningConverter
 *
 * In this section, we present a complete example of a program that can take
 * an SBML model containing Level&nbsp;3 Hierarchical %Model Composition
 * constructs and flatten it to a plain SBML Level&nbsp;3 model.
 * A version of this program is available in the libSBML distribution's
 * @if cpp <code>examples/c++/comp</code>@endif@if python
 * <code>examples/python/comp</code>@endif@if java
 * <code>examples/java/comp</code>@endif@if JavaScript
 * <code>examples/javascript/comp</code>@endif@if csharp
 * <code>examples/csharp/comp</code>@endif@if ruby
 * <code>examples/ruby/comp</code>@endif@if perl
 * <code>examples/perl/comp</code>@endif@~ directory as the
 * program named
 * @if cpp @ref flattenModel.cpp "flattenModel.cpp"@endif@if python
 * @ref flattenModel.py "flattenModel.py"@endif@if java
 * <code>"flattenModel.java"</code>@endif@if JavaScript
 * @ref flattenModel.js "flattenModel.js"@endif@if csharp
 * @ref FlattenModel.cs "FlattenModel.cs"@endif@if ruby
 * @ref flattenModel.rb "flattenModel.rb"@endif@if perl
 * @ref flattenModel.pl "flattenModel.pl"@endif.  The example
 * XML models shown below are the same models as given in sections
 * 4.1&ndash;4.2 in the specification document for SBML Level&nbsp;3
 * Hierarchical %Model Composition.
 *
 * @subsection comp-flattening-example-program Example program
 *
 * For brevity, we do not give the general scaffolding that a real program
 * would have (such as inclusion of header files, command-line argument
 * checks, additional error checks throughout the code, and so on), and focus
 * instead on the parts relevant to an application using the libSBML "comp"
 * extension.
 *
 * First, our program checks that this copy of libSBML has the "comp"
 * extension available.  The process for doing that simply involves a call to
 * the extension registry in libSBML:
@if cpp
@code{.cpp}
if (SBMLExtensionRegistry::isPackageEnabled("comp") == false)
{
    cerr << "The version of libsbml being used does not have the comp"
         << " package extension enabled" << endl;
    return 1;
}
@endcode
@endif
@if python
@code{.py}
if not SBMLExtensionRegistry.isPackageEnabled("comp"):
    err_msg = 'This copy of libSBML does not contain the "comp" extension.'
    raise SystemExit(err_msg)
@endcode
@endif
@if java
@code{.java}
if (! SBMLExtensionRegistry.isPackageEnabled("comp"))
{
    System.out.println("This copy of libSBML does not contain the 'comp' extension");
    System.out.println("Unable to proceed with flattening the model.");
    System.exit(1);
}
@endcode
@endif
 *
 * Next, we read the SBML input file.  For this example, we simply
 * assume that the path to the file is given as the first argument
 * to the program; a real program would perform more sophisticated
 * command-line argument processing and error checking.
@if cpp
@code{.cpp}
const char* inputFile  = argv[1];
SBMLDocument* document = readSBML(inputFile);

if (document->getNumErrors() > 0)
{
    cerr << "Encountered the following SBML errors:" << endl;
    document->printErrors(cerr);
    return 1;
}
@endcode
@endif
@if python
@code{.py}
reader  = SBMLReader()
sbmldoc = reader.readSBML(args[1])
if sbmldoc.getNumErrors() > 0:
    sbmldoc.printErrors()
    raise SystemExit(2)
@endcode
@endif
@if java
@code{.java}
SBMLReader reader = new SBMLReader();
SBMLDocument doc  = reader.readSBML(args[0]);
if (doc.getErrorLog().getNumFailsWithSeverity(libsbml.LIBSBML_SEV_ERROR) > 0)
{
    doc.printErrors();
    System.exit(1);
}
@endcode
@endif
 * Continuing, we set up options for the call to the converter.
 * The use of ConversionProperties and the general scheme behind
 * conversion options is explained further below; for the purposes
 * of this example, it is enough to know that the following are the
 * basic lines of code needed to obtain a copy of a libSBML
 * converter object that will invoke CompFlatteningConverter:
@if cpp
@code{.cpp}
ConversionProperties props;
props->addOption("flatten comp");
SBMLConverter* converter =
    SBMLConverterRegistry::getInstance().getConverterFor(props);
@endcode
@endif
@if python
@code{.py}
props = ConversionProperties()
props.addOption("flatten comp", True)       # Invokes CompFlatteningConverter
@endcode
@endif
@if java
@code{.java}
ConversionProperties props = new ConversionProperties();
props.addOption("flatten comp", true);
@endcode
@endif
 * Now comes the actual invocation of CompFlatteningConverter.
 * As always, it is critical to check for possible errors by
 * checking the status code returned by the call; we do this
 * in the code below too.
@if cpp
@code{.cpp}
converter->setDocument(document);
int result = converter->convert();

if (result != LIBSBML_OPERATION_SUCCESS)
{
    cerr << "Conversion failed\n";
    document->printErrors();
}
@endcode
@endif
@if python
@code{.py}
result = sbmldoc.convert(props)
if (result != LIBSBML_OPERATION_SUCCESS):
    sbmldoc.printErrors()
    raise SystemExit("Conversion failed... ("+ str(result) + ")")
@endcode
@endif
@if java
@code{.java}
if (doc.convert(props) != libsbml.LIBSBML_OPERATION_SUCCESS)
{
    doc.printErrors();
    System.exit(1);
}
@endcode
@endif
 * If successful, we simply write out the resulting flattened model
 * to an output file which, for the purposes of this simple example,
 * we assume is indicated by the second argument handed to the program
 * on the command line by the user.  @if cpp We also clean up the objects
 * we allocated, to avoid leaking memory.@endif@~
@if cpp
@code{.cpp}
const char* outputFile  = argv[2];
writeSBML(document, outputFile);

delete converter;
delete document;
@endcode
@endif
@if python
@code{.py}
writer  = SBMLWriter()
check(writer, 'create an SBMLWriter object.')
writer.writeSBML(sbmldoc, output_file)
print("Flattened model written to %s" % (output_file))
@endcode
@endif
@if java
@code{.java}
SBMLWriter writer = new SBMLWriter();
writer.writeSBML(doc, args[1]);
@endcode
@endif
 *
 * @subsection comp-flattening-example-use Example use of the program
 *
 * What is the result of the above on an actual model?  Suppose we
 * have the following SBML Level&nbsp;3 model stored in a file named
 * @c "enzyme_model.xml":
 *
 * @htmlinclude example-sbml-enzyme-model.html
 *
 * Also suppose we have the following SBML Level&nbsp;3 model stored
 * in a file called @c "main.xml" in the same directory as the
 * file above.  The model below imports the model @c "enzyme" from
 * the model file above twice; this is used to create two submodels
 * whose identifiers are @c "A" and @c "B".
 *
 * @htmlinclude example-sbml-main.html
 *
 * Finally, invoking the example program developed the previous section on the
 * file @c "main.xml", we would obtain the following XML output as result a
 * result
 *
 * @htmlinclude example-sbml-flattened-output.html
 *
 * The final model is a concrete realization of the effective
 * model defined by @c "main.xml", with a single compartment and two
 * reactions; the species @c "S" can either bind with enzyme @c "E" (from
 * submodel @c "A") to form @c "D" (from submodel @c "A"), or with enzyme
 * @c "E" (from submodel @c "B") to form @c "D" (from submodel @c "B").
 * In the flattened model above, note how the identifiers of components
 * inside the ModelDefinition objects @c "A" and @c "B" of our file @c
 * "enzyme_model.xml" have been rewritten as (for example) @c "A__E" @c
 * "B__E", etc.
 *
 * @copydetails doc_section_using_sbml_converters
 */
/**
 * <!-- ~ ~ ~ ~ ~ Start of common documentation strings ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
 * The following text is used as common documentation blocks copied multiple
 * times elsewhere in this file.  The use of @class is a hack needed because
 * Doxygen's @copydetails command has limited functionality.  Symbols
 * beginning with "doc_" are marked as ignored in our Doxygen configuration.
 * ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  -->
 *
 * @class doc_compflatteningconverter_options
 *
 * @par
 * CompFlatteningConverter is enabled by creating a ConversionProperties
 * object with the option @em "flatten comp", and passing this
 * ConversionProperties object to SBMLDocument::convert(@if java
 * ConversionProperties@endif).  The CompFlatteningConverter converter
 * accepts numerous options influencing its behavior.  The following list
 * briefly summarizes the options:
 * <ul>
 * <li> <em>"flatten comp"</em>: Possible values are @c "true" or @c "false".
 * Setting the option to @c true (the default) means enable the flattening
 * converter.
 *
 * <li> @em "abortIfUnflattenable": Possible values are @c "all", @c
 * "requiredOnly" (the default), or @c "none".  Controls what happens upon
 * encountering an SBML Level&nbsp;3 package with no flattener
 * implementation.
 *
 * <li> @em "stripUnflattenablePackages": Possible values are @c "true" or @c
 * "false" (the default).  Controls whether the constructs of Level&nbsp;3
 * packages with no flattener implementation are stripped from the output.
 *
 * <li> @em "stripPackages": The value must be a string representing a
 * comma-separated list of SBML Level&nbsp;3 packages to be stripped before
 * conversion.  (Default value: empty string, meaning, no packages.)
 *
 * <li> @em "basePath": The value must be a string representing the path where
 * the converter should search for any ExternalModelDefinition objects.
 * (Default value: ".", meaning, the current directory.)
 *
 * <li> @em "leavePorts": Possible values are @c "true" or @c "false" (the
 * default).  Controls what happens to Port constructs in the output.
 *
 * <li> @em "listModelDefinitions": Possible values are @c "true" or @c
 * "false" (the default).  Controls what happens to ModelDefinition and
 * ExternalModelDefinition objects in the final output.
 *
 * <li> @em "performValidation": Possible values are @c "true" (the default)
 * or @c "false".  Controls whether whether libSBML validates the model
 * before attempting to flatten it.
 * </ul>
 */



#ifndef CompFlatteningConverter_h
#define CompFlatteningConverter_h

#include <sbml/SBMLNamespaces.h>
#include <sbml/packages/comp/extension/CompModelPlugin.h>
#include <sbml/conversion/SBMLConverter.h>
#include <sbml/conversion/SBMLConverterRegister.h>


#ifdef __cplusplus


LIBSBML_CPP_NAMESPACE_BEGIN


class LIBSBML_EXTERN CompFlatteningConverter : public SBMLConverter
{
public:

  /** @cond doxygenLibsbmlInternal */
  /**
   * Register with the ConversionRegistry.
   */
  static void init();

  /** @endcond */


  /**
   * Creates a new CompFlatteningConverter object.
   */
  CompFlatteningConverter();


  /**
   * Copy constructor.
   *
   * This creates a copy of a CompFlatteningConverter object.
   *
   * @param orig the CompFlatteningConverter instance to copy.
   */
  CompFlatteningConverter(const CompFlatteningConverter& orig);


  /**
   * Creates and returns a deep copy of this CompFlatteningConverter.
   *
   * @return a (deep) copy of this CompFlatteningConverter.
   */
  virtual CompFlatteningConverter* clone() const;


  /**
   * Destroy this CompFlatteningConverter object.
   */
  virtual ~CompFlatteningConverter ();


  /**
   * Returns @c true if this converter matches the given properties.
   *
   * Given a ConversionProperties object @p props, this method checks that @p
   * props possesses an option value to enable the CompFlatteningConverter.  If
   * it does, this method returns @c true.
   *
   * @param props the properties to match.
   *
   * @return @c true if the properties @p props would match the necessary
   * properties for CompFlatteningConverter type of converter, @c false
   * otherwise.
   */
  virtual bool matchesProperties(const ConversionProperties &props) const;


  /**
   * Performs the conversion.
   *
   * This method causes CompFlatteningConverter to do the actual conversion
   * work, that is, to convert the SBMLDocument object set by
   * SBMLConverter::setDocument(@if java const SBMLDocument@endif) and with
   * the configuration options set by SBMLConverter::setProperties(@if java
   * const ConversionProperties@endif).
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   */
  virtual int convert();


  /** @cond doxygenLibsbmlInternal */
  /**
   * Performs the conversion.
   *
   * This method causes CompFlatteningConverter to do the actual conversion
   * work, that is, to convert the SBMLDocument object set by
   * SBMLConverter::setDocument(@if java const SBMLDocument@endif) and with
   * the configuration options set by SBMLConverter::setProperties(@if java
   * const ConversionProperties@endif).
   *
   * @copydetails doc_returns_success_code
   * @li @sbmlconstant{LIBSBML_OPERATION_SUCCESS, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_OPERATION_FAILED, OperationReturnValues_t}
   * @li @sbmlconstant{LIBSBML_INVALID_OBJECT, OperationReturnValues_t}
   */
  virtual int performConversion();
  /** @endcond */


  /**
   * Returns the default properties of this converter.
   *
   * A given converter exposes one or more properties that can be adjusted in
   * order to influence the behavior of the converter.  This method returns
   * the @em default property settings for CompFlatteningConverter.  It is
   * meant to be called in order to be able to programmatically discover all
   * the settings for the converter object.
   *
   * @copydetails doc_compflatteningconverter_options
   *
   * @return the ConversionProperties object describing the default properties
   * for this converter.
   *
   * @note Previously, CompFlatteningConverter also offered an @em
   * "ignorePackages" option, whose name proved to be confusing.  This option
   * has been deprecated and replaced by the @em "stripUnflattenablePackages"
   * option.
   */
  virtual ConversionProperties getDefaultProperties() const;


private:

  /** @cond doxygenLibsbmlInternal */
  int validateOriginalDocument();

  int validateFlatDocument(Model* flatmodel, unsigned int pkgVersion,
                           unsigned int level, unsigned int version);

  int stripPackages();

  int reconstructDocument(Model* flatmodel);

  int reconstructDocument(Model* flatmodel,
                          SBMLDocument &dummyDoc,  bool dummyRecon = false);

  void stripUnflattenablePackages();

  bool getLeavePorts() const;

  bool getLeaveDefinitions() const;

  bool getIgnorePackages() const;

  bool getStripUnflattenablePackages() const;

  bool getPerformValidation() const;

  bool getAbortForAll() const;

  bool getAbortForRequired() const;

  bool getAbortForNone() const;

  const std::string& getPackagesToStrip() const;

  bool canBeFlattened();

  void restoreNamespaces();

  std::set<std::pair<std::string, std::string> > mDisabledPackages;

#ifndef SWIG
  typedef std::vector<bool>                     ValueSet;
  typedef std::map<const std::string, ValueSet> PackageValueMap;
  typedef PackageValueMap::iterator             PackageValueIter;
#endif

  PackageValueMap mPackageValues;
  IdList * mPkgsToStrip;

  void analyseDocument();

  bool getRequiredStatus(const std::string & package);

  bool getKnownStatus(const std::string& package);

  bool getFlattenableStatus(const std::string& package);

  bool haveUnknownRequiredPackages();

  bool haveUnknownUnrequiredPackages();

  bool haveUnflattenableRequiredPackages();

  bool haveUnflattenableUnrequiredPackages();

  /** @endcond */

};

LIBSBML_CPP_NAMESPACE_END

#endif  /* __cplusplus */


#ifndef SWIG

LIBSBML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


END_C_DECLS
LIBSBML_CPP_NAMESPACE_END

#endif  /* !SWIG */
#endif  /* CompFlatteningConverter_h*/

