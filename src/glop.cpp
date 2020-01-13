#include <RcppORtools.h>
#include <Rcpp.h>
#include <assert.h> 
using namespace Rcpp;

#include <ortools/linear_solver/linear_solver.h>
#include <ortools/linear_solver/linear_solver.pb.h>

//' R binding to the powerful GLOP linear solver
//' 
//' The primary OR-Tools linear optimization solver is Glop, Google's linear programming system. It's fast, memory efficient, and numerically stable.
//' 
//' This function provides a binding via Rcpp to the linear solver.
//' 
//' @param objectiveCoefs A *named* numeric vector representing the objective coefficients (the c vector). The names are essential as GLOP insists on named variables.
//' @param constraintMat  A numeric matrix of constraint coefficients. (the A matrix)
//' @param constraintRHS A vector of inequality right-hand sides (the b vector)
//' @param maximise Logical giving the direction of the optimization. TRUE means that the objective is to maximize the objective function, FALSE (default) means to minimize it.
//' 
//' @return solution A named list of:
//'            'optimum' - value of the objective funtion
//'            'solution' - a vector of equal length to objectiveCoefs, detailing the solution
//'            'wall_time_ms' - time taken by solved
//'            'is_optimal -  status flag provided by the solver
//'            'is_feasible' -  status flag provided by the solver (if this is false, then the solver might not be able to solve the problem at all!)
//' 
//' @references \url{https://developers.google.com/optimization/lp/glop}
//' @references \url{https://en.wikipedia.org/wiki/Linear_programming}
//' 
//' @examples
//' library(data.table)
//' data(StiglerDiet)
//' 
//' # everything will be per dollar spent, so the cvec is all 1's
//' costVec <- rep(1, length(StiglerDiet$foodAvailable$Commodity)) 
//' names(costVec) <- StiglerDiet$foodAvailable$Commodity # Name the variables
//' 
//' #Nutrient data is in amount per dollar
//' constraintMat <- StiglerDiet$foodAvailable[,as.matrix(.SD) ,
//'                     .SDcols = !c("Commodity","Price1939","Weight","Weight_unit","Weight_value")]
//'                                    
//' row.names(constraintMat) <- StiglerDiet$foodAvailable$Commodity
//' 
//' objectiveVec <- StiglerDiet$nutrientRDA$RDI_value
//' names(objectiveVec) <- StiglerDiet$nutrientRDA$Nutrient
//' 
//' glopt <- glop_lp(
//'             objectiveCoefs = costVec,
//'             constraintMat = t(constraintMat[names(costVec),]),
//'             constraintRHS = objectiveVec[colnames(constraintMat)])
//' 
//' 
//' glopt$solution[glopt$solution > 0]
//' 
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
  solution.names() = objectiveNames;
  
  for (auto optimisedResultIt = solverObjectives.begin(); optimisedResultIt != solverObjectives.end(); ++optimisedResultIt ) {
    
    if ( (**optimisedResultIt).solution_value() > 0.0) {
      
      solution[(**optimisedResultIt).index()] = (**optimisedResultIt).solution_value();
    }
  }
  
  return Rcpp::List::create(Rcpp::Named("optimum") = objective->Value(),
                            Rcpp::Named("solution")  = solution,
                            Rcpp::Named("wall_time_ms")  = solver.wall_time(),
                            Rcpp::Named("is_optimal") = (result_status == operations_research::MPSolver::OPTIMAL),
                            Rcpp::Named("is_feasible") = (result_status == operations_research::MPSolver::FEASIBLE || result_status == operations_research::MPSolver::OPTIMAL) );
}
