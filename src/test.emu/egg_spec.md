

{
    "emu" {
        "provenance" : {
            "version"     : "1.0",
            "design-file" : "full path to file",
            "sim-file"    : "full path to file",
            "samples"     : "number of samples",
            "options"     : "other options",
            "date"        : "date generated",
        },
        "inputs" {
            "0" : {
                "name" : "temperature",
                "desc" : "Initial temperature of the material",
                "type" : "double",
                "type" : "continuous"
            }
        },
        "output" {
            "type" : "function",
            "index" : ["1.0", 42.0, 157. ... ]
        }
    }
}
