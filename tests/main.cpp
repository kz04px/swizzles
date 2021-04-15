#define CATCH_CONFIG_RUNNER
#include "../src/globals.hpp"
#include "catch.hpp"

int main(int argc, char* argv[]) {
    tt.resize(4);

    const int result = Catch::Session().run(argc, argv);

    return result;
}
