#ifndef RcppORtools__RcppORtools__h
#define RcppORtools__RcppORtools__h

// Use GLOP in the package
// https://github.com/google/or-tools/issues/1214
#define USE_GLOP

// Looks like ERROR macro is already defined elsehwere in R
#define GLOG_NO_ABBREVIATED_SEVERITIES

// R headers define macros with very generic names, such as Free for S-PLUS back compatibility. STRICT_R_HEADERS disables this.
// https://github.com/dahtah/imager/issues/7
#define STRICT_R_HEADERS

#endif