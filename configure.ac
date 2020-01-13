## Process this file with autoconf to produce a configure script.
##
## Configure.ac for RcppORtools
##
## Copyright (C) 2019 Adam Sardar
##
## Licensed under MPL-2.0

# The version set here will propagate to other files from here
AC_INIT(RcppORtools, m4_esyscmd_s([awk -e '/^Version:/ {print $2}' DESCRIPTION]))

## We are using C++
AC_LANG(C++)
AC_REQUIRE_CPP

# Build up a location for OR tools, depending on 
if [ -z "$ORtoolsDIR" ]; then

  if [ -z "$ORtoolsInstallLoc" ]; then
    
     ORtoolsInstallLoc = "/opt/"
  fi
  
  ORtoolsDIR = "$ORtoolsInstallLoc/ORtools"
fi

ORtools_LIBLOC="$ORtoolsDIR/lib/"
ORtools_INCLOC="$ORtoolsDIR/include/"

# Now substitute these variables in src/Makevars.in to create src/Makevars
AC_SUBST(ORtools_LIBLOC)
AC_SUBST(ORtools_INCLOC)

AC_OUTPUT(src/Makevars)