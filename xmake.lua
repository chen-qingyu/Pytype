add_rules("mode.debug", "mode.release")
set_languages("cxx17")
add_requires("gtest", {configs = {main = true}})
add_requires("eigen")
if is_plat("windows") then -- for MSVC
    add_cxflags("/utf-8")
    add_ldflags("/subsystem:console")
end

target("tests")
    set_kind("binary")
    add_headerfiles("sources/*.hpp")
    add_headerfiles("tests/*.hpp")
    add_files("tests/*.cpp")
    add_packages("gtest")

target("examples")
    set_kind("binary")
    add_files("examples/*.cpp")
    add_packages("eigen")
