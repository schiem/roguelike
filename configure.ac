AC_INIT([roguelike], [0.1], [email@address.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_PROG_CXX

AC_CHECK_LIB([SDL],[SDL_DisplayFormat], ,[AC_MSG_ERROR(Could not find the SDL\
 library on your system. Make sure that it is installed properly!)])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
