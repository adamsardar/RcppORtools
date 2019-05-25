#include <RcppORtools.h>
#include <Rcpp.h>
#include <assert.h> 
using namespace Rcpp;

#include <cmath>
#include <vector>
#include <ortools/constraint_solver/routing.h>
#include <ortools/constraint_solver/routing_enums.pb.h>
#include <ortools/constraint_solver/routing_index_manager.h>
#include <ortools/constraint_solver/routing_parameters.h>

//' @export
// [[Rcpp::export]]
List singleVisitorTSP(IntegerMatrix distance_matrix,
                      int startIndex = 1){
  
  // nrow = ncol
  // diag of 0's
  // startIndex < nrow
  
  operations_research::RoutingIndexManager::NodeIndex startDepot{startIndex-1}; //C++(0-based) to R(1-based) index addition
  
  // Create Routing Index Manager
  operations_research::RoutingIndexManager manager(distance_matrix.nrow(),
                              1,
                              startDepot); 
  
  // Create Routing Model.
  operations_research::RoutingModel routing(manager);
  
  // Register a distance callback
  const int transit_callback_index = routing.RegisterTransitCallback(
    [&distance_matrix, &manager](int64 from_index, int64 to_index) -> int64 {
      
      // Convert from routing variable Index to distance matrix NodeIndex.
      int from_node = manager.IndexToNode(from_index).value();
      int to_node = manager.IndexToNode(to_index).value();
      
      return distance_matrix[from_node, to_node];
    });
  
  // Define cost of each arc.
  routing.SetArcCostEvaluatorOfAllVehicles(transit_callback_index);
  
  // Setting first solution heuristic.
  operations_research::RoutingSearchParameters searchParameters = operations_research::DefaultRoutingSearchParameters();
  
  searchParameters.set_first_solution_strategy( operations_research::FirstSolutionStrategy::PATH_CHEAPEST_ARC );
  
  // Solve the problem.
  const operations_research::Assignment* solution = routing.SolveWithParameters(searchParameters);
  
  // Extract solution.
  IntegerVector solutionPath;
  int64 distance{0};

  int64 routeIndex = routing.Start(0);
  solutionPath.push_back( manager.IndexToNode(routeIndex).value()  + 1); //C++(0-based) to R(1-based) index addition
      
  while (routing.IsEnd(routeIndex) == false) {
    
    solutionPath.push_back( manager.IndexToNode(routeIndex).value()  + 1); //C++(0-based) to R(1-based) index addition
  
    int64 previous_index = routeIndex;
    
    routeIndex = solution->Value(routing.NextVar(routeIndex));
    
    distance += const_cast<operations_research::RoutingModel&>(routing).GetArcCostForVehicle(previous_index, routeIndex, int64{0});
  }

  return Rcpp::List::create(Rcpp::Named("route") = solutionPath,
                            Rcpp::Named("route_distance") = distance,
                            Rcpp::Named("wall_time_ms") = routing.solver()->wall_time(),
                            Rcpp::Named("objective_dsitance") = solution->ObjectiveValue() );
  
}