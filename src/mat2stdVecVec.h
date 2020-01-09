#ifndef MAT2STDVECVEC_H
#define MAT2STDVECVEC_H

#include <RcppORtools.h>
#include <Rcpp.h> 
using namespace Rcpp;

#include <ortools/base/integral_types.h>

// The std::vector< std::vector<int64>> structure is used alot throughout ORtools.
// I did consider using a templated function, which necessitated RcppArmadillo, but integer types are used throughout ORtools.
std::vector<std::vector<int64>> matrixToStdVecVec(IntegerMatrix mat2convert){
  
  std::vector< std::vector<int64>> vecvec(mat2convert.nrow());
  
  //Alot of copying going on. This is probably fine, because the branch & bound process will dominate time.
  for(size_t i = 0; i < mat2convert.nrow(); ++i){
    
    IntegerVector intMatRow = mat2convert.row(i);
    vecvec[i] = as<std::vector<int64>>(intMatRow);
  }
  
  return vecvec;
}

// This should probably be moved in to the includes at some stage. Think about Rcpp Honey to seemlessly convet from R types to int64
// [[Rcpp::export]]
List reflectMatrixToVecVec(IntegerMatrix mat2convert){
  
  std::vector< std::vector<int64>> matAsVecVec = matrixToStdVecVec(mat2convert);
  
  List list( matAsVecVec.size() ) ;
  for( int j=0; j<  matAsVecVec.size(); j++) list[j] = wrap( matAsVecVec[j].begin(), matAsVecVec[j].end() ) ;
  
  return wrap(list);
}

#endif