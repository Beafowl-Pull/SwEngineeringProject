-- Create the Stellar-Engine project
target("SwEngineering")

    add_rules("qt.quickapp")

    -- Set the project kind to binary
    set_kind("binary")

    -- Add the source files
    add_files("**.cpp", "**.hpp")

    -- Add the -std=c++20 flag
    add_cxxflags("-std=c++20")

    -- Set the target directory to bin
    set_targetdir("../bin")

    -- Add the include directories
    add_includedirs("../src", {public = true})

    -- Automatically detect and link to the system-installed Qt6
    add_frameworks("QtWidgets", "QtGui", "QtCore", "QtQuick", "QtQml", "QtQuickTemplates2")

    -- Specify the directories where the assets and output files are located
    local asset_dir = "assets"
    local output_dir = "../bin"

    -- Hook into the post-build process to copy files
    after_build(function (target)
        -- Define the source and destination paths
        local src_dir = path.join(os.projectdir(), asset_dir)
        local dest_dir = path.join(target:targetdir(), asset_dir)  -- Copying assets into bin/assets

        -- Debug prints to verify paths
        print("Copying assets from: " .. src_dir)
        print("Copying assets to: " .. dest_dir)

        -- Make sure the destination directory exists
        os.mkdir(dest_dir)

        -- Cross-platform copy using os.cp
        os.cp(path.join(src_dir, "*"), dest_dir)
    end)

    -- Add the system libraries
    if is_plat("windows") then
        add_syslinks("ws2_32", "user32", "gdi32", "winspool", "shell32", "ole32", "oleaut32", "uuid", "comdlg32", "advapi32")
    else
        add_syslinks("pthread")
    end

    -- Set the warnings to all (-Wall)
    set_warnings("allextra", "pedantic", "more")

    -- Set the build modes
    if is_mode("debug") then
        set_optimize("none")
        set_symbols("debug")
    end

    if is_mode("release") then
        set_optimize("fastest")
        set_symbols("hidden")
    end

    -- Define the _CRT_SECURE_NO_WARNINGS macro for the MSVC compiler
    add_defines("_CRT_SECURE_NO_WARNINGS")
