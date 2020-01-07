.onAttach <- function(...) {

  packageStartupMessage("Linked against OR-tools version ", 
                        paste0(orToolsVersion(), collapse = "."))
}