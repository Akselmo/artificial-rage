target_sources(
    ${PROJECT_NAME}
    PUBLIC
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
    PUBLIC FILE_SET CXX_MODULES FILES
        game.cpp
)
