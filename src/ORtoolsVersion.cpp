#include <RcppORtools.h>
#include <Rcpp.h>
#include <assert.h> 
using namespace Rcpp;

#include "ortools/base/version.h"

//' @export
// [[Rcpp::export]]
IntegerVector orToolsVersion() {

  return IntegerVector::create(_["major"] =  operations_research::OrToolsMajorVersion(),
                               _["minor"] = operations_research::OrToolsMinorVersion() 
                              );
}