project("pyvxl-contrib-bvxm")

# add as pyvxl module
pyvxl_add_module(bvxm)

# Recurse
add_subdirectory("algo" "algo-build")
