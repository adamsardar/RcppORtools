#include <RcppORtools.h>
#include <Rcpp.h>
#include <assert.h> 
using namespace Rcpp;

#include <ortools/linear_solver/linear_solver.h>
#include <ortools/linear_solver/linear_solver.pb.h>

// Argument names taken from Rglpk_solve_LP for consistency
//' @export
// [[Rcpp::export]]
List glop_lp(NumericVector objectiveCoefs,
             NumericMatrix constraintMat,
             NumericVector rhs,
             bool max = false){
  
  operations_research::MPSolver solver("GoogleGLOPsolver", 
                                       operations_research::MPSolver::GLOP_LINEAR_PROGRAMMING);
  
  std::vector<operations_research::MPVariable*> solverObjectives;
  std::vector<operations_research::MPConstraint*> solverConstraints;

  CharacterVector objectiveNames = objectiveCoefs.names();   
  // TODO check for names on obj
    
  operations_research::MPObjective* const objective = solver.MutableObjective();

  const double infty = solver.infinity();

  ///variables
  for (std::pair<NumericVector::iterator, 
                 CharacterVector::iterator> it(objectiveCoefs.begin(), objectiveNames.begin());
       it.first != objectiveCoefs.end();
       ++it.first, ++it.second){

    solverObjectives.push_back( solver.MakeNumVar(0.0, infty, as<std::string>(*it.second)) ) ;
    // TODO - support bounds on objectives
    // TODO - support other variable types?
    
    objective->SetCoefficient(solverObjectives.back(), *it.first);
  }
  
  // check that rhs is the same length as nrows
  // check that ncol is the same as obj coefs
  
  // constraints constraintMat
  for (int i=0; i < constraintMat.rows(); ++i){
    
      solverConstraints.push_back( solver.MakeRowConstraint(rhs[i], infty) ); //RHS of row constraints.
      // Only supporting continuous bounds at current
      // TODO support constraints other than GTE
      
      // TODO - use a sparse matrix form and only iterate over non-zeros: https://gallery.rcpp.org/articles/sparse-iterators/
      for (int j=0; j < constraintMat.cols(); ++j){
        
        solverConstraints.back()->SetCoefficient(solverObjectives[j], constraintMat(i,j) ) ;
      }
  }
  
  const operations_research::MPSolver::ResultStatus result_status = solver.Solve();
  
  // Check that the problem has an optimal solution.
  if (result_status != operations_research::MPSolver::OPTIMAL) {
    
    Rcout << "The problem does not have an optimal solution!" << std::endl;
    if (result_status == operations_research::MPSolver::FEASIBLE) {
      
      Rcout << "A potentially suboptimal solution was found" << std::endl;
    } else {
      
      Rcout << "The solver could not solve the problem." << std::endl;
    }
  }
  
  NumericVector solution(objectiveCoefs.size());
  
  for (auto optimisedResultIt = solverObjectives.begin(); optimisedResultIt != solverObjectives.end(); ++optimisedResultIt ) {
    
    if ( (**optimisedResultIt).solution_value() > 0.0) {
      
      solution[(**optimisedResultIt).index()] = (**optimisedResultIt).solution_value();
    }
  }
  
  return Rcpp::List::create(Rcpp::Named("is_optimal") = 1,
                            Rcpp::Named("solution")  = solution);
}
