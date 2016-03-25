# Wraps resources in a cpp file
function(wrap_resources resources prefix output_hpp output_cpp)
    # Initialize
    file(WRITE ${output_hpp} "")
    file(WRITE ${output_cpp} "")

    # Content
    foreach(resource ${resources})
        # Filename
        string(REGEX MATCH "([^/]+)$" filename ${resource})
        string(REGEX REPLACE "\\.| |-" "_" filename ${filename})

        # Header
        file(APPEND ${output_cpp} "extern const unsigned char ${filename}[];\n")

        # Source
        file(READ ${resource} filedata HEX)
        string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})
        file(APPEND ${output_cpp} "const unsigned char ${filename}[] = { ${filedata} 0 };\n")
    endforeach()
endfunction()
