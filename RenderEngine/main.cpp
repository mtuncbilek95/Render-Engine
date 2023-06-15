#include <Application/Application.hpp>

int main(int argc, char** argv) {
    Application app({ 1280, 720 });
    app.Run();

    return EXIT_SUCCESS;
}
