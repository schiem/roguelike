AC_INIT([roguelike], [0.1], [email@address.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_PROG_CXX

AC_CHECK_LIB([SDL],[SDL_DisplayFormat], ,[AC_MSG_ERROR(Could not find the SDL\
 library on your system. Make sure that it is installed properly!)])
AC_CHECK_LIB([boost_system], [system], , [AC_MSG_ERROR(Could not find the \
 boost_system library on your system. Make sure that it is installed \
properly!)])
AC_CHECK_LIB([boost_filesystem], [main], , [AC_MSG_ERROR(Could not find the \
 boost_filesystem library on your system. Make sure that it is installed \
properly!)])
AC_CHECK_LIB([boost_date_time], [main], , [AC_MSG_ERROR(Could not find the \
 boost_date_time library on your system. Make sure that it is installed \
 properly!)])

AX_BOOST_BASE
AX_BOOST_SYSTEM
AX_BOOST_FILESYSTEM
AX_BOOST_DATE_TIME

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
