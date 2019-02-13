#include <RcppORtools.h>
#include <Rcpp.h>
#include <assert.h> 
using namespace Rcpp;

#include <ortools/algorithms/knapsack_solver.h>

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
// This should probably be moved in to the includes at some stage. Think about Rcpp Honey


//' A simple knapsack solver
//' 
//' Solve knapsack proble via google's brilliant OR tools lbirary.
//' 
//' @param profits A vector of integer item profits (must be equal in length to ncol(weights))
//' @param weights A matrix (which can have a single row if one is only interested in the standard knapsack problem) of item weights
//' @param capacities A vector of length nrow(weights) of knapsack capacities
//' 
//' @return solution A named list of:
//'            'packed_items' - the 1-based indicies of items selected for packing
//'            'total_profit' - the optimised sum of profits
//'            'is_optimal' - a status flag provided by the solver
//' @references \url{https://developers.google.com/optimization/reference/algorithms/knapsack_solver/}
//' @references \url{https://en.wikipedia.org/wiki/Knapsack_problem}
//' 
//' @examples
//' # Taken from the OR-tools documentation examples:
//' 
//' SKPProfits <- c(360, 83, 59,  130, 431, 67, 230, 52,  93,  125, 670, 892, 600,
//'                     38,  48, 147, 78,  256, 63, 17,  120, 164, 432, 35,  92,  110,
//'                     22,  42, 50,  323, 514, 28, 87,  73,  78,  15,  26,  78,  210,
//'                     36,  85, 189, 274, 43,  33, 10,  19,  389, 276, 312)
//'                     
//' SKPWeights <- matrix(nrow = 1,
//'                   c(7,  0,  30, 22, 80, 94, 11, 81, 70, 64, 59, 18, 0,  36, 3,  8,  15,
//'                     42, 9,  0,  42, 47, 52, 32, 26, 48, 55, 6,  29, 84, 2,  4,  18, 56,
//'                     7,  29, 93, 44, 71, 3,  86, 66, 31, 65, 0,  79, 20, 65, 52, 13) )
//'                     
//'                     
//' SKPCapacities <- c(850)
//' 
//'  
//' SKPsolution <- solveMultiKnapsackProblem(SKPProfits, SKPWeights, SKPCapacities)
//' 
//' sum(SKPProfits[SKPsolution$packed_items]) # same as 'total_profit'
//' SKPWeights[SKPsolution$packed_items] # Items packed
//' sum(SKPWeights[SKPsolution$packed_items]) # Capacities used
//' 
//' 
//' #Example taken from ORtools docs
//' MKPprofits <- c(1, 2, 3, 4, 5, 6, 7, 8, 9)
//' MKPweights <- matrix(c(c(1, 2, 3, 4, 5, 6, 7, 8, 9),
//'                    c(1, 1, 1, 1, 1, 1, 1, 1, 1)),
//'                    nrow = 2, byrow = TRUE)
//' 
//' MKPcapacities <- c(34,4)
//' 
//' 
//' MKPsoolution <- solveMultiKnapsackProblem(MKPprofits, MKPweights, MKPcapacities)
//' MKPsoolution
//' 
//' sum(MKPprofits[MKPsoolution$packed_items]) # same as 'total_profit'
//' MKPweights[,MKPsoolution$packed_items] # Items packed
//' rowSums(MKPweights[,MKPsoolution$packed_items]) # Capacities used
//' 
//' @export
// [[Rcpp::export]]
List solveMultiKnapsackProblem(IntegerVector profits, IntegerMatrix weights, IntegerVector capacities){

  assert(profits.length() == weights.ncol() && "The number of profit items must equal the number of weight items (columns).");
  assert(capacities.length() == weights.nrow() && "The number of capacities must be equal to the number of per-item weights (rows)!");
  
  operations_research::KnapsackSolver solver(
      operations_research::KnapsackSolver::KNAPSACK_MULTIDIMENSION_BRANCH_AND_BOUND_SOLVER,
      "Multi-dimensional KnapsackProblem");

  solver.Init(as<std::vector<int64>>(profits),
              matrixToStdVecVec(weights),
              as<std::vector<int64>>(capacities));
  
  int total_profit = solver.Solve();

  IntegerVector packedItems;
  
  //Solver only provides solution presence/absence on a one-by-one basis
  for(int potentialItemIndex = 0; potentialItemIndex < profits.length(); ++potentialItemIndex){
    
    if(solver.BestSolutionContains(potentialItemIndex)){
      packedItems.push_back(potentialItemIndex + 1); //C++(0-based) to R(1-based) index addition
    }
  }
  
  return Rcpp::List::create(Rcpp::Named("packed_items") = packedItems,
                            Rcpp::Named("total_profit") = total_profit,
                            Rcpp::Named("is_optimal") = solver.IsSolutionOptimal() );
}