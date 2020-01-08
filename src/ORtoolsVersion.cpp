#include <RcppORtools.h>
#include <Rcpp.h>
#include <assert.h> 
using namespace Rcpp;

#include "ortools/base/version.h"

//' @export
// [[Rcpp::export]]
IntegerVector orToolsVersion() {

  return IntegerVector::create(_["MAJOR"] =  operations_research::OrToolsMajorVersion(),
                               _["MINOR"] = operations_research::OrToolsMinorVersion() 
                              );
}