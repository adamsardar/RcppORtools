test_that("Inspect mapping from and to integer matricies", {
  
  for(i in 1:20){

    nRow <- rpois(1,10)+1
    normMat <- matrix(rnorm(nRow^2), ncol =nRow, nrow=nRow)
    
    expect_equivalent(
      Reduce(rbind, reflectMatrixToVecVec(50*normMat)), # Convert to and from matrix
      matrix(as.integer(50*normMat), ncol =nRow, nrow=nRow) )    
  }
  
})
