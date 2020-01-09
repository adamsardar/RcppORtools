test_that("Testing GLOP with the Stigler diet, for which we know the answer", {

  # Taken from https://developers.google.com/optimization/lp/glop
  refSol <-c(
      `Wheat Flour (Enriched)` = 0.029519,
      `Liver (Beef)` = 0.001893,
      Cabbage = 0.011214,
      Spinach = 0.005008,
      `Navy Beans, Dried` = 0.061029)
  
  data(StiglerDiet)
  
  costVec <- rep(1, length(menuChoicesTable$Commodity)) # everything will be per dollar spent, so the cvec is all 1's
  names(costVec) <- menuChoicesTable$Commodity # Name the variables
  
  #Nutrient data is in amount per dollar
  constraintMat <- menuChoicesTable[,as.matrix(.SD) ,
                                    .SDcols = !c("Commodity","Price1939","Weight","Weight_unit","Weight_value")]
  
  row.names(constraintMat) <- menuChoicesTable$Commodity
  
  objectiveVec <- nutrientConstraintsTable$RDI_value
  names(objectiveVec) <- nutrientConstraintsTable$Nutrient
  
  
  expect_error(  glop_lp(
    objectiveCoefs = c(costVec,0),
    constraintMat = t(constraintMat),
    constraintRHS = objectiveVec),
    regexp = "objectiveCoefs is not the same size",
    class = "std::invalid_argument")
  
  
  expect_error(  glop_lp(
    objectiveCoefs = costVec,
    constraintMat = t(constraintMat),
    constraintRHS = c(objectiveVec,0)),
    regexp = "constraintRHS is not the same size",
    class = "std::invalid_argument")
  
  
  glopt <- glop_lp(
    objectiveCoefs = costVec,
    constraintMat = t(constraintMat),
    constraintRHS = objectiveVec)
  
  expect_true(glopt$is_optimal)
  expect_true(glopt$is_feasible)
  
  expect_equivalent( glopt$optimum, sum(refSol), tolerance = 1e-6)

  expect_equivalent( sort(glopt$solution[glopt$solution>0]), sort(refSol), tolerance = 1e-6)  
})
