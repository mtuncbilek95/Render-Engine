#include "ApplicationModule/Application.hpp"

int main(int argc, char** argv) {
    Application app({ 1600, 900 });
    app.Run();

    return EXIT_SUCCESS;
}
