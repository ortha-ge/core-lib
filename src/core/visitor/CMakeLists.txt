
target_sources(core-lib
        PRIVATE
        systems/EntityVisitorSystem.cpp
)

target_sources(core-lib
        PUBLIC FILE_SET modules
        TYPE CXX_MODULES
        FILES
        EntityVisitor.cppm
        services/EntityVisitorSystemService.cppm
        systems/EntityVisitorSystem.cppm
)