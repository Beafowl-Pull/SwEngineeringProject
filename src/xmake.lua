-- Add boost packages
add_requires()

-- Create the Stellar-Engine project
target("SwEngineering")

    add_rules("qt.quickapp")

    -- Set the project kind to static
    set_kind("binary")

    -- Add the source files
    add_files("**.cpp")

    -- Add the -std=c++20 flag
    add_cxxflags("-std=c++20", "-lstdc++")

    -- Set the target directory to bin/tests
    set_targetdir("../bin")

    -- Add the boost packages
    add_packages("boost", "spdlog")

    -- Add the include directories
    add_includedirs("../src", {public = true})

    -- Add the system libraries
    if is_plat("windows") then
        add_syslinks("ws2_32", "user32", "gdi32", "winspool", "shell32", "ole32", "oleaut32", "uuid", "comdlg32", "advapi32")
    else
        add_syslinks("pthread")
    end

    -- Set the warnings to all (-Wall)
    set_warnings("allextra", "pedantic", "more")

    -- Set the build modes
    -- If the mode is debug, set the optimization to none and the symbols to debug
    if is_mode("debug") then
        set_optimize("none")
        set_symbols("debug")
    end

    -- If the mode is release, set the optimization to fastest and the symbols to hidden
    if is_mode("release") then
        set_optimize("fastest")
        set_symbols("hidden")
    end

    -- Define the _CRT_SECURE_NO_WARNINGS macro for the MSVC compiler (in case)
    add_defines("_CRT_SECURE_NO_WARNINGS")

    add_frameworks("QtWidgets", "QtGui", "QtCore", "QtQuick", "QtQml", "QtQuickControls2", "QtQuickTemplates2", "QtQuick")