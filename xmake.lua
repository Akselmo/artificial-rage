add_rules("mode.debug", "mode.release")
add_requires("raylib 4.2.0", {system = false})
set_defaultmode("debug")

target("artificial-rage")

    set_kind("binary")
    add_files("src/*.c")
    add_packages("raylib")
    set_languages("c99", "c++11")
    local modefolder = "debug"

    if is_mode("release") then
        modefolder = "release"
    end

    after_build(function (target)
        print("Copying assets folder where build is...")
        os.cp("$(projectdir)/src/assets/*.*", "$(buildir)/$(os)/".. os.arch() .. "/" .. modefolder .. "/assets/")
    end)