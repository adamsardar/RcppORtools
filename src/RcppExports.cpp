// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include "../inst/include/RcppORtools.h"
#include <Rcpp.h>

using namespace Rcpp;

// orToolsVersion
IntegerVector orToolsVersion();
RcppExport SEXP _RcppORtools_orToolsVersion() {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(orToolsVersion());
    return rcpp_result_gen;
END_RCPP
}
// glop_lp
List glop_lp(NumericVector objectiveCoefs, NumericMatrix constraintMat, NumericVector constraintRHS, bool maximise);
RcppExport SEXP _RcppORtools_glop_lp(SEXP objectiveCoefsSEXP, SEXP constraintMatSEXP, SEXP constraintRHSSEXP, SEXP maximiseSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< NumericVector >::type objectiveCoefs(objectiveCoefsSEXP);
    Rcpp::traits::input_parameter< NumericMatrix >::type constraintMat(constraintMatSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type constraintRHS(constraintRHSSEXP);
    Rcpp::traits::input_parameter< bool >::type maximise(maximiseSEXP);
    rcpp_result_gen = Rcpp::wrap(glop_lp(objectiveCoefs, constraintMat, constraintRHS, maximise));
    return rcpp_result_gen;
END_RCPP
}
// reflectMatrixToVecVec
List reflectMatrixToVecVec(IntegerMatrix mat2convert);
RcppExport SEXP _RcppORtools_reflectMatrixToVecVec(SEXP mat2convertSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< IntegerMatrix >::type mat2convert(mat2convertSEXP);
    rcpp_result_gen = Rcpp::wrap(reflectMatrixToVecVec(mat2convert));
    return rcpp_result_gen;
END_RCPP
}
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
    {"_RcppORtools_orToolsVersion", (DL_FUNC) &_RcppORtools_orToolsVersion, 0},
    {"_RcppORtools_glop_lp", (DL_FUNC) &_RcppORtools_glop_lp, 4},
    {"_RcppORtools_reflectMatrixToVecVec", (DL_FUNC) &_RcppORtools_reflectMatrixToVecVec, 1},
    {"_RcppORtools_singleVisitorTSP", (DL_FUNC) &_RcppORtools_singleVisitorTSP, 2},
    {"_RcppORtools_solveMultiKnapsackProblem", (DL_FUNC) &_RcppORtools_solveMultiKnapsackProblem, 3},
    {NULL, NULL, 0}
};

RcppExport void R_init_RcppORtools(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
