// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include "../inst/include/RcppORtools.h"
#include <Rcpp.h>

using namespace Rcpp;

// singleVisitorTSP
List singleVisitorTSP(IntegerMatrix distance_matrix, int startIndex);
RcppExport SEXP _RcppORtools_singleVisitorTSP(SEXP distance_matrixSEXP, SEXP startIndexSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< IntegerMatrix >::type distance_matrix(distance_matrixSEXP);
    Rcpp::traits::input_parameter< int >::type startIndex(startIndexSEXP);
    rcpp_result_gen = Rcpp::wrap(singleVisitorTSP(distance_matrix, startIndex));
    return rcpp_result_gen;
END_RCPP
}
// solveMultiKnapsackProblem
List solveMultiKnapsackProblem(IntegerVector profits, IntegerMatrix weights, IntegerVector capacities);
RcppExport SEXP _RcppORtools_solveMultiKnapsackProblem(SEXP profitsSEXP, SEXP weightsSEXP, SEXP capacitiesSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< IntegerVector >::type profits(profitsSEXP);
    Rcpp::traits::input_parameter< IntegerMatrix >::type weights(weightsSEXP);
    Rcpp::traits::input_parameter< IntegerVector >::type capacities(capacitiesSEXP);
    rcpp_result_gen = Rcpp::wrap(solveMultiKnapsackProblem(profits, weights, capacities));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_RcppORtools_singleVisitorTSP", (DL_FUNC) &_RcppORtools_singleVisitorTSP, 2},
    {"_RcppORtools_solveMultiKnapsackProblem", (DL_FUNC) &_RcppORtools_solveMultiKnapsackProblem, 3},
    {NULL, NULL, 0}
};

RcppExport void R_init_RcppORtools(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
