add_rules("mode.debug")

package("raylib_git")
    add_deps("cmake")
    set_sourcedir("/home/akseli/Repositories/raylib/")
    on_install(function (package)
        local configs = {}
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
        table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))
        import("package.tools.cmake").install(package, configs)
    end)
package_end()

add_requires("raylib_git")
set_defaultmode("debug")
set_warnings("allextra", "conversion")

target("artificial-rage")

    set_kind("binary")
    add_files("src/*.c")
    add_packages("raylib_git")
    set_languages("c99", "c++11")

    after_build(function (target)
        print("Copying assets folder where build is...")
        os.cp("$(projectdir)/src/assets/*.*", "$(buildir)/$(plat)/$(arch)/$(mode)/assets/")
    end)