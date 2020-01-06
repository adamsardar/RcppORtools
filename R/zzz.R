.onAttach <- function(...) {

  packageStartupMessage("Linked against OR-tools version ", 
                        ORtoolsMajorVersion(), 
                        ".", 
                        ORtoolsMinorVersion())
}