#include <chrono>
#include <voxel-blaze/consts.hpp>
#include <voxel-blaze/graphics/model.hpp>
#include <voxel-blaze/graphics/renderer.hpp>
#include <voxel-blaze/graphics/shader.hpp>
#include <voxel-blaze/graphics/window.hpp>
#include <voxel-blaze/parsers/vox_parser.hpp>
#include <voxel-blaze/voxels/array_voxel_grid.hpp>

// TODO Handle VOX file with multiple "frames".
// TODO Handle alpha transparency.
// TODO Implement orbit camera.

const unsigned cubic_size = 64;
const auto locale = std::locale("");

struct Timer
{
    std::chrono::high_resolution_clock::time_point start_point = std::chrono::high_resolution_clock::now();

    inline void start()
    {
        start_point = std::chrono::high_resolution_clock::now();
    }

    inline std::chrono::nanoseconds round() const
    {
        auto end_point = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_point - start_point);
        return duration;
    }

    static std::string format_duration(std::chrono::nanoseconds duration)
    {
        using namespace std::chrono;

        auto ns_count = duration.count();

        if (ns_count < 1000)
        {
            return std::to_string(ns_count) + "ns";
        }
        else if (ns_count < 1000 * 1000)
        {
            double us = ns_count / 1000.0;
            std::stringstream ss;
            ss << std::fixed << std::setprecision(3) << us << "µs";
            return ss.str();
        }
        else if (ns_count < 1000 * 1000 * 1000)
        {
            double ms = ns_count / (1000.0 * 1000.0);
            std::stringstream ss;
            ss << std::fixed << std::setprecision(3) << ms << "ms";
            return ss.str();
        }
        else
        {
            double sec = ns_count / (1000.0 * 1000.0 * 1000.0);
            std::stringstream ss;
            ss << std::fixed << std::setprecision(3) << sec << "s";
            return ss.str();
        }
    }
};

struct Result
{
    std::string name;
    unsigned size;
    size_t vertex_count;
    size_t face_count;
    std::chrono::nanoseconds duration;

    inline void print(std::ostream &ostream, unsigned col_width, char delim) const
    {
        ostream << std::setw(col_width * 3) << name << delim;
        ostream << std::setw(col_width) << size << delim;
        ostream << std::setw(col_width) << vertex_count << delim;
        ostream << std::setw(col_width) << face_count << delim;
        ostream << std::setw(col_width) << Timer::format_duration(duration) << delim;
        ostream << "\n";
    }

    inline void print_compare(std::ostream &ostream, unsigned col_width, char delim, const Result &other) const
    {
        auto ratio_vertex_count = (double)vertex_count / (double)other.vertex_count;
        auto ratio_face_count = (double)face_count / (double)other.face_count;
        auto ratio_duration = (double)duration.count() / (double)other.duration.count();

        ratio_vertex_count = (ratio_vertex_count - 1.0) * 100.0;
        ratio_face_count = (ratio_face_count - 1.0) * 100.0;
        ratio_duration = (ratio_duration - 1.0) * 100.0;

        ostream << std::setw(col_width * 3) << (name + " comp to " + other.name) << delim;
        ostream << std::setw(col_width) << size << delim;
        ostream << std::setw(col_width) << std::fixed << std::setprecision(2) << (ratio_vertex_count > 0 ? "+" : "")
                << ratio_vertex_count << "%" << delim;
        ostream << std::setw(col_width) << std::fixed << std::setprecision(2) << (ratio_face_count > 0 ? "+" : "")
                << ratio_face_count << "%" << delim;
        ostream << std::setw(col_width) << std::fixed << std::setprecision(2) << (ratio_duration > 0 ? "+" : "")
                << ratio_duration << "%" << delim;
        ostream << "\n";
    }

    static inline void print_sep(std::ostream &ostream)
    {
        ostream << "\n";
    }

    static inline void print_sec(std::ostream &ostream)
    {
        ostream << "\n\n";
    }
};

void test_suite()
{

    std::vector<Result> results_direct;
    std::vector<Result> results_culled;
    std::vector<Result> results_greedy;
    std::vector<ArrayVoxelGrid> cuboids;
    std::vector<ArrayVoxelGrid> ellipsoids;
    std::vector<ArrayVoxelGrid> noises;

    for (unsigned i = 1; i <= 8; i++)
    {
        auto size = glm::pow(2, i);
        ArrayVoxelGrid cuboid = ArrayVoxelGrid(size, size, size);
        ArrayVoxelGrid ellipsoid = ArrayVoxelGrid(size, size, size);
        ArrayVoxelGrid noise = ArrayVoxelGrid(size, size, size);
        cuboid.fill_cuboid(Voxel{1.0, 1.0, 1.0});
        ellipsoid.fill_ellipsoid(Voxel{1.0, 1.0, 1.0});
        noise.fill_perlin_noise(Voxel{1.0, 1.0, 1.0}, 0.05);
        cuboids.push_back(cuboid);
        ellipsoids.push_back(ellipsoid);
        noises.push_back(noise);
    }

    // Direct
    Timer timer;

    for (const auto &v : cuboids)
    {
        timer.start();
        const auto mesh = v.meshify_direct();
        results_direct.push_back(
            {"direct cuboid", v.max_size(), mesh.vertices.size(), mesh.indices.size() / 3, timer.round()});
    }

    for (const auto &v : ellipsoids)
    {
        timer.start();
        const auto mesh = v.meshify_direct();
        results_direct.push_back(
            {"direct ellipsoid", v.max_size(), mesh.vertices.size(), mesh.indices.size() / 3, timer.round()});
    }

    for (const auto &v : noises)
    {
        timer.start();
        const auto mesh = v.meshify_direct();
        results_direct.push_back(
            {"direct noise", v.max_size(), mesh.vertices.size(), mesh.indices.size() / 3, timer.round()});
    }

    // Culled

    for (const auto &v : cuboids)
    {
        timer.start();
        const auto mesh = v.meshify_culled();
        results_culled.push_back(
            {"culled cuboid", v.max_size(), mesh.vertices.size(), mesh.indices.size() / 3, timer.round()});
    }

    for (const auto &v : ellipsoids)
    {
        timer.start();
        const auto mesh = v.meshify_culled();
        results_culled.push_back(
            {"culled ellipsoid", v.max_size(), mesh.vertices.size(), mesh.indices.size() / 3, timer.round()});
    }

    for (const auto &v : noises)
    {
        timer.start();
        const auto mesh = v.meshify_culled();
        results_culled.push_back(
            {"culled noise", v.max_size(), mesh.vertices.size(), mesh.indices.size() / 3, timer.round()});
    }

    // Greedy

    for (const auto &v : cuboids)
    {
        timer.start();
        const auto mesh = v.meshify_greedy();
        results_greedy.push_back(
            {"greedy cuboid", v.max_size(), mesh.vertices.size(), mesh.indices.size() / 3, timer.round()});
    }

    for (const auto &v : ellipsoids)
    {
        timer.start();
        const auto mesh = v.meshify_greedy();
        results_greedy.push_back(
            {"greedy ellipsoid", v.max_size(), mesh.vertices.size(), mesh.indices.size() / 3, timer.round()});
    }

    for (const auto &v : noises)
    {
        timer.start();
        const auto mesh = v.meshify_greedy();
        results_greedy.push_back(
            {"greedy noise", v.max_size(), mesh.vertices.size(), mesh.indices.size() / 3, timer.round()});
    }

    // Write results to file

    const auto col_width = 0;

    std::locale comma_locale(std::locale(), new std::numpunct<char>(' ')); // You can use ',' instead of ' ' for comma
    std::ofstream file("testresults.csv");
    file.imbue(locale);

    for (const auto &result : results_direct)
    {
        result.print(file, col_width, '\t');
    }

    Result::print_sep(file);

    for (const auto &result : results_culled)
    {
        result.print(file, col_width, '\t');
    }

    Result::print_sep(file);

    for (const auto &result : results_greedy)
    {
        result.print(file, col_width, '\t');
    }

    Result::print_sep(file);

    // Compare

    for (unsigned i = 0; i < results_culled.size(); i++)
    {
        const auto result_direct = results_direct[i];
        const auto result_culled = results_culled[i];
        result_culled.print_compare(file, col_width, '\t', result_direct);
    }

    Result::print_sep(file);

    for (unsigned i = 0; i < results_greedy.size(); i++)
    {
        const auto result_direct = results_direct[i];
        const auto result_greedy = results_greedy[i];
        result_greedy.print_compare(file, col_width, '\t', result_direct);
    }

    Result::print_sep(file);

    for (unsigned i = 0; i < results_greedy.size(); i++)
    {
        const auto result_culled = results_culled[i];
        const auto result_greedy = results_greedy[i];
        result_greedy.print_compare(file, col_width, '\t', result_culled);
    }

    file.close();
}

int main()
{
    test_suite();
    return 0;

    spdlog::set_level(spdlog::level::info);
    Window window(1280, 1280);
    Shader shader(consts::vertex_shader_source, consts::fragment_shader_source);
    Renderer renderer(std::move(shader));

    Camera camera;

    VoxParser parser("resources/teapot.vox");

    ArrayVoxelGrid filled = ArrayVoxelGrid(cubic_size, cubic_size, cubic_size);
    filled.fill_perlin_noise(Voxel{1.0f, 1.0f, 1.0f}, 0.05);
    const auto voxel_grid = parser.get_voxel_grid();
    // Model model = voxel_grid->meshify_culled();

    Mesh mesh = filled.meshify_greedy();

    mesh.save_obj("test.obj");
    Model model = Model(mesh);
    model.rotate(glm::vec3(-glm::pi<float>() / 2.0f, 0.0f, 0.0f));

    // auto voxel_grid = ArrayVoxelGrid(cubic_size, cubic_size, cubic_size);
    // voxel_grid.fill_ellipsoid(Voxel{1.0f, 1.0f, 1.0f});
    // // voxel_grid.set_voxel(0, 0, 0, std::nullopt);
    // // voxel_grid.set_voxel(0, 1, 0, std::nullopt);
    // // voxel_grid.set_voxel(0, cubic_size / 2, cubic_size / 2, std::nullopt);
    // auto model = Model(voxel_grid.meshify_greedy());
    // //model.translate(glm::vec3(cubic_size / 2, cubic_size / 2, cubic_size / 2));

    camera.look_at(glm::vec3(cubic_size / 2, cubic_size / 2, voxel_grid->max_size() * 1.5f), glm::vec3(0));

    while (window.opened())
    {
        const auto delta_time = renderer.draw(camera, model);
        const auto speed = delta_time * 0.05f;

        if (window.key_down(GLFW_KEY_Q))
        {
            if (window.key_down(GLFW_KEY_LEFT_SHIFT))
            {
                model.rotate(glm::vec3(-speed, 0.0f, 0.0f));
            }
            else
            {
                model.rotate(glm::vec3(speed, 0.0f, 0.0f));
            }
        }
        else if (window.key_down(GLFW_KEY_W))
        {
            if (window.key_down(GLFW_KEY_LEFT_SHIFT))
            {
                model.rotate(glm::vec3(0.0f, -speed, 0.0f));
            }
            else
            {
                model.rotate(glm::vec3(0.0f, speed, 0.0f));
            }
        }
        else if (window.key_down(GLFW_KEY_E))
        {
            if (window.key_down(GLFW_KEY_LEFT_SHIFT))
            {
                model.rotate(glm::vec3(0.0f, 0.0f, -speed));
            }
            else
            {
                model.rotate(glm::vec3(0.0f, 0.0f, speed));
            }
        }
    }
}
