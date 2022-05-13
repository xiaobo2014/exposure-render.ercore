
// Ignore common warnings:
// 302 : Redefinition of Macro, usually not a problem
// 362 : Operator= is ignored. Can't help it.
// 503 : Can't wrap operator of type "*" unless renamed to a valid identifier, no problem as operator not needed.
// 509 : Overloaded function ignored. Usually not a problem, as overloaded functions shouldn't give different results.
// 511 : Can't use keyword arguments with overloaded functions
#pragma SWIG nowarn=302,362,503,509,511

// Splitted the information about the addition files into sub-files:

%naturalvar;

// Includes of STD-Files goes in here
%include <rx_swig_std.i>

// Include c++ code in this file. It is basically a c++-header wrapped in the commands so it is included in the std-file
%include <rx_swig_cpp_include.i>

// information about classes that are going to be wrapped are in here:
%include <rx_swig_classes.i>

//
// How to wrap a new class:
// ------------------------------------
// (1. Add the c++ include file to rx_swig_cpp_include.i
//    If the class is in a new module, make sure that this module is added as dependency in cmake )
//    This step should no longer be necessary as the corresponding header is now included by
//    the SWIG_ADD_RX_CLASS macro.
// 2. Add the class definition in rx_swig_classes.i
//    If the definition of the class needs new macros, for example because it is not in the core
//    and has a new Export-Macro, be sure to define this macro first.
