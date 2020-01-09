#include <RcppORtools.h>
#include <Rcpp.h>
#include <assert.h> 
using namespace Rcpp;

#include "ortools/base/version.h"

//' Display the version of or-tools that the package links to
//' 
//' Useful for data analysis provenance.
//' 
//' @export
// [[Rcpp::export]]
IntegerVector orToolsVersion() {

  return IntegerVector::create(_["MAJOR"] =  operations_research::OrToolsMajorVersion(),
                               _["MINOR"] = operations_research::OrToolsMinorVersion() 
                              );
}