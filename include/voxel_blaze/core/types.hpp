#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>

namespace voxel_blaze
{
    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

    using i8 = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;

    using f32 = float;
    using f64 = double;

    using String = std::string;

    using RuntimeError = std::runtime_error;

    template <typename T>
    using Vec = std::vector<T>;

    template <typename T>
    using Shared = std::shared_ptr<T>;
}
