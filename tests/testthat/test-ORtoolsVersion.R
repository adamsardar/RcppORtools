test_that("Tool version as expected", {
  
  expect_true(is.integer(orToolsVersion()))
  expect_true( all(c("MAJOR", "MINOR") %in% names(orToolsVersion())) )
})
