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
std::optional<fsys::path>
get_path_to_project_subdir(std::string_view project_dir_name,
						   std::string_view subdir_name);

} // namespace reassembler_impl


namespace reassembler
{
std::filesystem::path	   get_path_to_input_file(std::string_view file_name);
std::optional<std::string> reassemble_data(std::istream& input_stream);
} // namespace reassembler

#endif // REASSEMBLER_HPP