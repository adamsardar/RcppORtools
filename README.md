# RcppORtools: Exploring integration of google's c++ ORtools library and R via Rcpp

[Google OR's recent CP-SAT solver](https://developers.google.com/optimization/) has been made open source (Apache 2.0) under the GoogleAI initiative. I thought that it'd be interesting to explore its use via Rcpp.

At current, this package is very much in BETA. The aim is to smooth down some of the rough edges involved in mapping
R objects to the C++ objects used by the library and also to export a handful of useful operations research solvers into R.

Install the development version using:

```
devtools::install_github("adamsardar/RcppORtools")
```

Your system will need a copy of OR tools to link against. [BUILD DETAILS TO FOLLOW]
 
## Examples

### Knapsack Problem
 
A (single) knapsack problem [taken from the ORtools documentation](https://developers.google.com/optimization/bin/knapsack):
 
```
> library(RcppORtools)

 
> SKPProfits <- c(360, 83, 59,  130, 431, 67, 230, 52,  93,  125, 670, 892, 600,
                 38,  48, 147, 78,  256, 63, 17,  120, 164, 432, 35,  92,  110,
                 22,  42, 50,  323, 514, 28, 87,  73,  78,  15,  26,  78,  210,
                 36,  85, 189, 274, 43,  33, 10,  19,  389, 276, 312)
                         
> SKPWeights <- matrix(nrow = 1,
                       c(7,  0,  30, 22, 80, 94, 11, 81, 70, 64, 59, 18, 0,  36, 3,  8,  15,
                         42, 9,  0,  42, 47, 52, 32, 26, 48, 55, 6,  29, 84, 2,  4,  18, 56,
                         7,  29, 93, 44, 71, 3,  86, 66, 31, 65, 0,  79, 20, 65, 52, 13) )
                         
                         
> SKPCapacities <- c(850)
     
      
> SKPsolution <- solveMultiKnapsackProblem(SKPProfits, SKPWeights, SKPCapacities)
     
> SKPsolution
$packed_items
 [1]  1  2  4  5  7 11 12 13 15 16 17 18 19 20 22 23 25 28 29 30 31 32 33 35 39
[26] 40 42 43 45 48 49 50

$total_profit
[1] 7534

$is_optimal
[1] TRUE

     
sum(SKPProfits[SKPsolution$packed_items]) # same as 'total_profit'
[1] 7534

SKPWeights[SKPsolution$packed_items] # Items packed
[1]  7  0 22 80 11 59 18  0  3  8 15 42  9  0 47 52 26  6 29 84  2  4 18  7 71
[26]  3 66 31  0 65 52 13

sum(SKPWeights[SKPsolution$packed_items]) # capacity used
[1] 850
```