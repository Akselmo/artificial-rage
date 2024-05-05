target_sources(
    ArtificialRage
    PRIVATE
        animator.cpp
        entity.cpp
        hud.cpp
        main.cpp
        player.cpp
        projectile.cpp
        scene.cpp
        settings.cpp
        utilities.cpp
        weapon.cpp
    PRIVATE FILE_SET CXX_MODULES FILES
        game.cpp
)
