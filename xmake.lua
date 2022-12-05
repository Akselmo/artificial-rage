add_rules("mode.debug")

add_requires("raylib master", {system = false})
set_defaultmode("debug")

set_policy("build.warning", true)
set_warnings("all", "conversion", "extra", "shadow", "pedantic")

target("artificial-rage")

    set_kind("binary")
    add_files("src/*.c")
    add_packages("raylib")
    set_languages("c99", "c++11")

    after_build(function (target)
        print("Copying assets folder where build is...")
        os.cp("$(projectdir)/src/assets/*", "$(buildir)/$(plat)/$(arch)/$(mode)/assets/")
    end)
