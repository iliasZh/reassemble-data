#pragma once
#ifndef REASSEMBLER_HPP
	#define REASSEMBLER_HPP

	#include <algorithm>
	#include <filesystem>
	#include <fstream>
	#include <iostream>
	#include <numeric> // std::accumulate
	#include <optional>
	#include <sstream>
	#include <stdexcept>
	#include <string>
	#include <string_view>
	#include <vector>

namespace reassembler_impl
{
struct data_segment;
std::vector<data_segment> read_packet(std::istream& input_stream);
void filter_out_unneeded_data_types(std::vector<data_segment>& packet,
									short target_data_type);
void stable_sort_by_index(std::vector<data_segment>& packet);
void remove_duplicates(std::vector<data_segment>& packet);
bool can_reassemble(const std::vector<data_segment>& p);
std::optional<std::string> reassembly(const std::vector<data_segment>& packet);

namespace fsys = std::filesystem;
fsys::path get_path_to_folder(std::string_view name);

} // namespace reassembler_impl


namespace reassembler
{
/// returns a full path to input.txt in /src or /build directories
/// if there's no input.txt, throws
std::filesystem::path get_path_to_input();

/// reassembles the message from the packet described in input_stream
std::optional<std::string> reassemble_data(std::istream& input_stream);
} // namespace reassembler

#endif // REASSEMBLER_HPP