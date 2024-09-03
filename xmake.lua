set_project("SwEngineering")

if is_plat("windows") then
    add_toolchains("msvc")
else
    add_toolchains("gcc")
end

set_languages("cxx23")
add_rules("mode.debug", "mode.release")

includes("src")

add_headerfiles("src/(**.hpp)")