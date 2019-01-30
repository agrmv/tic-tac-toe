#ifndef PROJECT_RESOURCES_HPP
#define PROJECT_RESOURCES_HPP

#define resource_file(filename) ("resources/" filename)

namespace resources {
    namespace textures {
        namespace cell {
            const char *const Empty = resource_file("empty.png");
            const char *const X = resource_file("x.png");
            const char *const O = resource_file("o.png");
        }

        const char *const Icon = resource_file("icon.png");
        const char *const Background = resource_file("background2.jpg");
        const char *const Background2  = resource_file("background.png");
    }

    namespace fonts {
        const char *const Default = resource_file("arial.ttf");
    }
}

#endif //PROJECT_RESOURCES_HPP