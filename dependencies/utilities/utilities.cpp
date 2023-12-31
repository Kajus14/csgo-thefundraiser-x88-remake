#include "utilities.hpp"
#include "../utilities/csgo.hpp"
#include <psapi.h>

std::uint8_t* utilities::pattern_scan(const char* module_name, const char* signature) noexcept {
    const auto module_handle = GetModuleHandleA(module_name);

    if (!module_handle)
        return nullptr;

    static auto pattern_to_byte = [](const char* pattern) {
        auto bytes = std::vector<int>{};
        auto start = const_cast<char*>(pattern);
        auto end = const_cast<char*>(pattern) + std::strlen(pattern);

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;

                if (*current == '?')
                    ++current;

                bytes.push_back(-1);
            }
            else {
                bytes.push_back(std::strtoul(current, &current, 16));
            }
        }
        return bytes;
    };

    auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
    auto nt_headers =
        reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

    auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
    auto pattern_bytes = pattern_to_byte(signature);
    auto scan_bytes = reinterpret_cast<std::uint8_t*>(module_handle);

    auto s = pattern_bytes.size();
    auto d = pattern_bytes.data();

    for (auto i = 0ul; i < size_of_image - s; ++i) {
        bool found = true;

        for (auto j = 0ul; j < s; ++j) {
            if (scan_bytes[i + j] != d[j] && d[j] != -1) {
                found = false;
                break;
            }
        }
        if (found)
            return &scan_bytes[i];
    }

    throw std::runtime_error(std::string("Wrong signature: ") + signature);
}

bool utilities::is_behind_smoke(vec3_t start_pos, vec3_t end_pos) {
    typedef bool(__cdecl* line_goes_through_smoke) (vec3_t, vec3_t);
    static line_goes_through_smoke line_goes_through_smoke_fn = 0;

    if (!line_goes_through_smoke_fn)
        line_goes_through_smoke_fn = reinterpret_cast<line_goes_through_smoke>(utilities::pattern_scan("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0"));

    if (line_goes_through_smoke_fn)
        return line_goes_through_smoke_fn(start_pos, end_pos);
    return false;
}

int utilities::get_fps() noexcept {
    static float frame_rate = 0;
    frame_rate = 0.9f * frame_rate + (1.f - 0.9f) * interfaces::globals->absolute_frametime;
    return int(1.f / frame_rate);
}
