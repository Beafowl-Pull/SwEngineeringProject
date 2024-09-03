-- Create the Stellar-Engine-Test project
target("test")

    -- Add the Qt rules
    add_rules("qt.console")

    -- Add the Qt packages
    add_frameworks("QtGui", "QtTest")

    -- Set the project kind to binary
    set_kind("binary")

    -- Set the target directory to bin/tests
    set_targetdir("../bin")

    -- Check if the platform is Windows
    if is_plat("windows") then
        -- Set the C++ standard to C++20 for MSVC
        add_cxxflags("/std:c++20", {force = true})
    else
        -- For other platforms (e.g., Linux)
        add_cxxflags("-std=c++20", "-lstdc++")
    end

    -- Add the source files
    add_files("*.cpp", {rules = "qt.moc"})

    -- Add Stellar-Engine as a dependency
    add_deps("SwEngineering")