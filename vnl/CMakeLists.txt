project("pyvxl-vnl")

# add as pyvxl module
pyvxl_add_module(vnl)

# additional linking
target_link_libraries(pyvnl PRIVATE vnl_io)
