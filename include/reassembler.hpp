#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric> // std::accumulate
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace reassembler_impl
{
/// Reassembles the message from the packet described in input_stream
std::optional<std::string> reassemble_data(std::istream& input_stream);

/// Returns a full path to input.txt in /src or /build directories.
/// If there's no input.txt, throws
std::filesystem::path get_path_to_input();
} // namespace reassembler_impl

namespace reassembler
{
using reassembler_impl::get_path_to_input;
using reassembler_impl::reassemble_data;
} // namespace reassembler
