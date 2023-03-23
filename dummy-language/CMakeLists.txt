# CMakeList.txt: proyecto de CMake para dummy-language, incluya el origen y defina
# la lógica específica del proyecto aquí.
#

# Agregue un origen al ejecutable de este proyecto.
add_executable (dummy-language "dummy-language.cpp"  "token.h" "enum_utility.h" "lexer.h" "expressions.h" "statements.h" "checkers.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET dummy-language PROPERTY CXX_STANDARD 20)
endif()

# TODO: Agregue pruebas y destinos de instalación si es necesario.
