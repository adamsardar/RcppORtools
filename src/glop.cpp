#include <RcppORtools.h>
#include <Rcpp.h>
#include <assert.h> 
using namespace Rcpp;

#include <ortools/linear_solver/linear_solver.h>
#include <ortools/linear_solver/linear_solver.pb.h>

//' @export
// [[Rcpp::export]]
List glop_lp(NumericVector objectiveCoefs,
             NumericMatrix constraintMat,
             NumericVector constraintRHS,
             bool maximise = false){
  
  operations_research::MPSolver solver("GoogleGLOPsolver", 
                                       operations_research::MPSolver::GLOP_LINEAR_PROGRAMMING);
  
  std::vector<operations_research::MPVariable*> solverObjectives;
  std::vector<operations_research::MPConstraint*> solverConstraints;

  CharacterVector objectiveNames = objectiveCoefs.names();   

  //checking for conformable parameters
  if(Rf_isNull(objectiveNames)){ throw std::invalid_argument("objectiveCoefs must be a named vector!"); }
  if(constraintMat.nrow() != constraintRHS.length() ){ throw std::invalid_argument("constraintRHS is not the same size as the number of constraints in constraintMat!"); }
  if(constraintMat.ncol() != objectiveCoefs.length() ){ throw std::invalid_argument("objectiveCoefs is not the same size as the number of variables in constraintMat!"); }

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
  

  
  // constraints constraintMat
  for (int i=0; i < constraintMat.rows(); ++i){
    
      solverConstraints.push_back( solver.MakeRowConstraint(constraintRHS[i], infty) ); //RHS of row constraints.
      // Only supporting continuous bounds at current
      // TODO support constraints other than GTE
      
      // TODO - use a sparse matrix form and only iterate over non-zeros: https://gallery.rcpp.org/articles/sparse-iterators/
      for (int j=0; j < constraintMat.cols(); ++j){
        
        solverConstraints.back()->SetCoefficient(solverObjectives[j], constraintMat(i,j) ) ;
      }
  }
  
  objective->SetOptimizationDirection(maximise);
  
  // SOLVE
  const operations_research::MPSolver::ResultStatus result_status = solver.Solve();
  
  NumericVector solution(objectiveCoefs.size());
  
  for (auto optimisedResultIt = solverObjectives.begin(); optimisedResultIt != solverObjectives.end(); ++optimisedResultIt ) {
    
    if ( (**optimisedResultIt).solution_value() > 0.0) {
      
      solution[(**optimisedResultIt).index()] = (**optimisedResultIt).solution_value();
    }
  }
  
  return Rcpp::List::create(Rcpp::Named("optimum") = objective->Value(),
                            Rcpp::Named("solution")  = solution,
                            Rcpp::Named("walltime.ms")  = solver.wall_time(),
                            Rcpp::Named("is_optimal") = (result_status == operations_research::MPSolver::OPTIMAL),
                            Rcpp::Named("is_feasible") = (result_status == operations_research::MPSolver::FEASIBLE || result_status == operations_research::MPSolver::OPTIMAL) );
}
