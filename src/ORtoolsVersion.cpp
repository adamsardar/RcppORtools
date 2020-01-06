#include <RcppORtools.h>
#include <Rcpp.h>
#include <assert.h> 
using namespace Rcpp;

#include "ortools/base/version.h"

//' @export
// [[Rcpp::export]]
IntegerVector ORtoolsMinorVersion(){
  
  return operations_research::OrToolsMinorVersion();
}

//' @export
// [[Rcpp::export]]
IntegerVector ORtoolsMajorVersion(){
  
  return operations_research::OrToolsMajorVersion();
}