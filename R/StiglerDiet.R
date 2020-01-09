#' Stigler's minimum cost diet problem
#' 
#' From the 1945 paper:
#' 
#' "For a moderately active man weighing 154 pounds, how much of each of 77 foods should be eaten on a daily basis
#'  so that the man’s intake of nine nutrients will be at least equal to the recommended dietary allowances (RDAs) 
#'  suggested by the National Research Council in 1943, with the cost of the diet being minimal?"
#' 
#' This is a classic dataset in the field of linear programming; before the West knew about advances in optimsation theory
#' being made in the USSR, Stigler's team was forced to resort to "trial and error, mathematical insight and agility". Rumour has it that it took 120 man days of nine clerks
#' on desk calculators to arrive at the answer.
#' 
#' This dataset is taken from Stigler's 1945 publication, via Google's OR-tools team who transcribed it. It has been augmented
#' with weights in oz for each foodtype. In cases where this an ambiguous mapping (e.g. 1 bunch of carrots), reference values of calories per 100g
#' were used.
#' 
#' @docType data
#' @usage data(StiglerDiet)
#' 
#' @source \url{https://developers.google.com/optimization/lp/glop#data}
#' @references \url{https://en.wikipedia.org/wiki/Stigler_diet}
#' @references Stigler, George J. “The Cost of Subsistence.” Journal of Farm Economics 1945
#' @format A list of two data.tables:  (nutrientRDA) RDA values of nutrients and (foodAvailable) a menu of food options alongside their nutritional information nd price. Nutrients are in amount per dollar. 
"StiglerDiet.R"