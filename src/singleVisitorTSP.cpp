#include <RcppORtools.h>
#include <Rcpp.h>
#include <assert.h> 
using namespace Rcpp;

#include <ortools/constraint_solver/routing.h>
#include <ortools/constraint_solver/routing_enums.pb.h>
#include <ortools/constraint_solver/routing_index_manager.h>
#include <ortools/constraint_solver/routing_parameters.h>

//' A simple TSP solver
//'
//' Givent a matrix of pairwise distances, compute a travelling salesman problem solution that visits each
//' location once and only once before returning to the start.
//' 
//' @param distance_matrix All-vs-all distances between cities
//' @param startIndex The integer label of the city in which to start
//' 
//' @return solution A named list of:
//'            'route' - The sequence of cities in which to travel to minimise the distance travelled overall (first and last cities are always the same of course)
//'            'wall_time_ms' - time taken for solver in ms
//'            'objective_dsitance' - total distance travelled
//' 
//' @examples
//' distMatrix <- matrix(c(
//' 0, 2451, 713, 1018, 1631, 1374, 2408, 213, 2571, 875, 1420, 2145, 1972,
//' 2451, 0, 1745, 1524, 831, 1240, 959, 2596, 403, 1589, 1374, 357, 579,
//' 713, 1745, 0, 355, 920, 803, 1737, 851, 1858, 262, 940, 1453, 1260,
//' 1018, 1524, 355, 0, 700, 862, 1395, 1123, 1584, 466, 1056, 1280, 987,
//' 1631, 831, 920, 700, 0, 663, 1021, 1769, 949, 796, 879, 586, 371,
//' 1374, 1240, 803, 862, 663, 0, 1681, 1551, 1765, 547, 225, 887, 999,
//' 2408, 959, 1737, 1395, 1021, 1681, 0, 2493, 678, 1724, 1891, 1114, 701,
//' 213, 2596, 851, 1123, 1769, 1551, 2493, 0, 2699, 1038, 1605, 2300, 2099,
//' 2571, 403, 1858, 1584, 949, 1765, 678, 2699, 0, 1744, 1645, 653, 600,
//' 875, 1589, 262, 466, 796, 547, 1724, 1038, 1744, 0, 679, 1272, 1162,
//' 1420, 1374, 940, 1056, 879, 225, 1891, 1605, 1645, 679, 0, 1017, 1200,
//' 2145, 357, 1453, 1280, 586, 887, 1114, 2300, 653, 1272, 1017, 0, 504,
//' 1972, 579, 1260, 987, 371, 999, 701, 2099, 600, 1162, 1200, 504, 0), nrow = 13, byrow = TRUE)
//' 
//' singleVisitorTSP(distMatrix, 3)
//' 
//' @export
// [[Rcpp::export]]
List singleVisitorTSP(IntegerMatrix distance_matrix,
                      int startIndex = 1){
  
  // nrow = ncol
  // diag of 0's
  // startIndex < nrow
  
  operations_research::RoutingIndexManager::NodeIndex startDepot{startIndex-1}; //C++(0-based) to R(1-based) index addition
  
  int nVisitors = 1;
  
  // Create Routing Index Manager
  operations_research::RoutingIndexManager manager(distance_matrix.nrow(),
                              nVisitors,
                              startDepot); 
  
  // Create Routing Model.
  operations_research::RoutingModel routing(manager);
  
  // Register a distance callback
  const int transit_callback_index = routing.RegisterTransitCallback(
    [&distance_matrix, &manager](int64 from_index, int64 to_index) -> int64 {
      
      // Convert from routing variable Index to distance matrix NodeIndex.
      int from_node = manager.IndexToNode(from_index).value();
      int to_node = manager.IndexToNode(to_index).value();
      
      return distance_matrix(from_node, to_node);
    });
  
  // Define cost of each arc.
  routing.SetArcCostEvaluatorOfAllVehicles(transit_callback_index);
  
  // Setting first solution heuristic.
  operations_research::RoutingSearchParameters searchParameters = operations_research::DefaultRoutingSearchParameters();
  
  searchParameters.set_first_solution_strategy( operations_research::FirstSolutionStrategy_Value_AUTOMATIC );
  
  // Solve the problem.
  const operations_research::Assignment* solution = routing.SolveWithParameters(searchParameters);
  
  // Extract solution.
  IntegerVector solutionPath;
  
  int64 routeIndex = routing.Start(0);
  solutionPath.push_back( manager.IndexToNode(routeIndex).value()  + 1); //C++(0-based) to R(1-based) index addition
      
  while (!routing.IsEnd(routeIndex)) {
    
    int64 previousIndex = routeIndex;
    routeIndex = solution->Value(routing.NextVar(previousIndex));
    
    solutionPath.push_back( manager.IndexToNode(routeIndex).value()  + 1); //C++(0-based) to R(1-based) index addition
  }

  return Rcpp::List::create(Rcpp::Named("route") = solutionPath,
                            Rcpp::Named("wall_time_ms") = routing.solver()->wall_time(),
                            Rcpp::Named("objective_dsitance") = solution->ObjectiveValue() );
  
}
