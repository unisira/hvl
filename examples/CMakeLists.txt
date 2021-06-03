wdk_add_driver(improvisor
    improvisor/entry.c
    )

target_include_directories(improvisor PUBLIC ../include)
target_link_libraries(improvisor hvl)
