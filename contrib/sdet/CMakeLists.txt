project("pyvxl-contrib-sdet")

# add as pyvxl module
pyvxl_add_module(sdet)

# Recurse
add_subdirectory("algo" "algo-build")
