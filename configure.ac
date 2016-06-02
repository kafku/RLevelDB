#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.63])
AC_INIT([rleveldb], [0.0.1])

: ${R_HOME=`R RHOME`}
if test -z "${R_HOME}" ; then
	echo "could not determin R_HOME"
	exit 1
fi

AC_LANG([C++])

CXX=`${R_HOME}/bin/R CMD config CXX`
CXXFLAGS=`${R_HOME}/bin/R CMD config CXXFLAGS`
LDFLAGS=`${R_HOME}/bin/R CMD config LDFLAGS`


dnl  TODO : windows support
dnl AC_CANONICAL_HOST


# check complier version
AC_CHECK_PROG(CXX_CHECK, [${CXX}], found, missing)
if test "${CXX_CHECK}" = "missing" ; then
	AC_MSG_ERROR([unable to find ${CXX}])
	exit 1
fi

case "${CXX}" in
	g++)
		GCC_VERSION=`g++ -dumpversion \
			| sed -e 's/\.\(@<:@0-9@:>@@<:@0-9@:>@\)/\1/g' -e 's/\.\(@<:@0-9@:>@\)/0\1/g' -e 's/^@<:@0-9@:>@\{3,4\}$/&00/'`
		AC_MSG_CHECKING([for GCC version >= 4.4])
		if test ${GCC_VERSION} -gt 40400 ; then
			AC_MSG_RESULT(yes)
		else
			AC_MSG_RESULT(no)
			AC_MSG_ERROR([GCC is too old])
		fi
		;;
	*)
		AC_MSG_ERROR([unsupported C++ complier])
		exit 1
		;;
esac

# set path for leveldb
AC_ARG_WITH([leveldb_headers], 
			AC_HELP_STRING([--with-leveldb-headers=PREFIX], 
						   [path to where the headers of leveldb are installed (default=/usr/include)]),
			[],
			[with_leveldb_headers="/usr/include"])

if test -e ${with_leveldb_headers} ; then
	CXXFLAGS="${CXXFLAGS} -I${with_leveldb_headers}"
else
	AC_MSG_ERROR([Unable to find ${with_leveldb_headers}])
	exit 1
fi        

AC_ARG_WITH([leveldb_libs], 
			AC_HELP_STRING([--with-leveldb-libs=PREFIX], 
						   [path to where the libs of leveldb are installed (default=/usr/lib64)]),
			[],
			[with_leveldb_libs="/usr/lib64"])

if test -e ${with_leveldb_headers} ; then
	LDFLAGS="${LDFLAGS} -L${with_leveldb_libs}"
else
	AC_MSG_ERROR([Unable to find ${with_leveldb_libs}])
	exit 1
fi

# check for leveldb header
AC_CHECK_HEADER([leveldb/db.h],
				[],
				[
				 AC_MSG_ERROR([unable to find leveldb/db.h])
				 exit 1
				])

AC_CHECK_HEADER([leveldb/write_batch.h],
				[],
				[
				 AC_MSG_ERROR([unable to find leveldb/write_batch.h])
				 exit 1
				])

# check for leveldb lib
AC_MSG_CHECKING([for libleveldb.so])
LDFLAGS="${LDFLAGS} -lleveldb"
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <leveldb/db.h>
#include <leveldb/write_batch.h>
leveldb::DB *d;
leveldb::WriteBatch batch;]])],
			   [AC_MSG_RESULT(yes)],
			   [
				AC_MSG_RESULT(no)
				AC_MSG_ERROR([unable to find libleveldb.so])
				exit 1
			   ])


AC_SUBST([LEVELDB_INCLUDE], ["-I${with_leveldb_headers}"])
AC_SUBST([LEVELDB_LIBS], ["-L${with_leveldb_libs} -lleveldb"])

AC_CONFIG_FILES([src/Makevars])
AC_OUTPUT
