#include "reassembler.hpp"
#include <filesystem>

namespace reassembler_impl
{
struct data_segment
{
	int			index		   = -1;
	short		data_type	   = -1;
	short		end_flag	   = -1;
	short		content_length = -1;
	std::string content{};
};

std::vector<data_segment> read_packet(std::istream& input_stream)
{
	size_t num_of_segments = 0U;
	input_stream >> num_of_segments;

	std::vector<data_segment> packet(num_of_segments);
	for (auto& segment : packet) {
		input_stream >> segment.data_type >> segment.index >>
			segment.end_flag >> segment.content_length >> segment.content;
	}
	return packet;
}

void filter_out_unneeded_data_types(std::vector<data_segment>& packet,
									short					   target_data_type)
{
	auto data_type_filter = [target_data_type](const data_segment& ds) {
		return ds.data_type == target_data_type;
	};
	auto packet_new_end =
		std::partition(packet.begin(), packet.end(), data_type_filter);

	packet.erase(packet_new_end, packet.end());
	packet.shrink_to_fit();
}

void stable_sort_by_index(std::vector<data_segment>& packet)
{
	auto comp = [](const data_segment& lhs, const data_segment& rhs) {
		return lhs.index < rhs.index;
	};
	std::stable_sort(packet.begin(), packet.end(), comp);
}

void remove_duplicates(std::vector<data_segment>& packet)
{
	auto equal_indexes = [](const data_segment& lhs, const data_segment& rhs) {
		return lhs.index == rhs.index;
	};
	auto non_unique_begin =
		std::unique(packet.begin(), packet.end(), equal_indexes);
	packet.erase(non_unique_begin, packet.end());
}

bool can_reassemble(const std::vector<data_segment>& p)
{
	for (auto it = p.cbegin(); it != p.cend() - 1; ++it) {
		if (it->end_flag != 0 ||
			(size_t)it->content_length != it->content.length()) {
			return false;
		}
	}
	auto it = p.cend() - 1;
	return it->end_flag == 1 &&
		   (size_t)it->content_length == it->content.length();
}

std::optional<std::string> reassembly(const std::vector<data_segment>& packet)
{
	if (can_reassemble(packet)) {
		auto accumulator = [](const std::string& str, const data_segment& ds) {
			return str + ds.content + ' ';
		};
		std::string result = std::accumulate(packet.begin(), packet.end(),
											 std::string{}, accumulator);
		result.pop_back();
		return result;
	}
	return {};
}

namespace fsys = std::filesystem;
std::optional<fsys::path>
get_path_to_project_subdir(std::string_view project_dir_name,
						   std::string_view subdir_name)
{
	for (auto path						  = fsys::current_path().parent_path();
		 path != path.parent_path(); path = path.parent_path())
	{
		if (auto candidate = path / project_dir_name / subdir_name;
			fsys::exists(candidate))
		{
			return candidate;
		}
	}

	return {};
}

} // namespace reassembler_impl


namespace reassembler
{
/// returns a full path to input.txt in "/src" or executable's directories
/// if there's no input.txt, throws
std::filesystem::path get_path_to_input_file(std::string_view file_name)
{
	namespace fsys = std::filesystem;
	namespace impl = reassembler_impl;

	// prepare error message
	std::stringstream exception_msg{};
	exception_msg << "fatal: failed to find \"" << file_name << "\"\n"
				  << "search paths are:\n";

	// look in the "src/" dir
	if (auto src_dir_opt =
			impl::get_path_to_project_subdir("reassemble-data", "src");
		src_dir_opt.has_value())
	{
		auto src_dir_file = *src_dir_opt / file_name;
		if (fsys::exists(src_dir_file)) {
			return src_dir_file;
		}
		exception_msg << src_dir_file << '\n';
	}

	// look in the current dir
	auto curr_dir_file = fsys::current_path() / file_name;
	if (fsys::exists(curr_dir_file)) {
		return curr_dir_file;
	}

	// throw if file was not found
	exception_msg << curr_dir_file << '\n';
	throw std::runtime_error(exception_msg.str());
}

/// reassembles the message from the packet described in input_stream
std::optional<std::string> reassemble_data(std::istream& input_stream)
{
	namespace impl = reassembler_impl;

	auto packet = impl::read_packet(input_stream);

	short target_data_type = 0;
	input_stream >> target_data_type;

	impl::filter_out_unneeded_data_types(packet, target_data_type);

	impl::stable_sort_by_index(packet);

	impl::remove_duplicates(packet);

	return impl::reassembly(packet);
}

} // namespace reassembler