#include <gtest/gtest.h>

#include "reassembler.hpp"

TEST(GetPathToProjectSubdir, CorrectPath) // NOLINT
{
	namespace impl = reassembler_impl;
	auto path	   = impl::get_path_to_project_subdir("reassemble-data", "src");
	ASSERT_TRUE(path.has_value());
}

TEST(GetPathToProjectSubdir, IncorrectProjectDir) // NOLINT
{
	namespace impl = reassembler_impl;
	auto path = impl::get_path_to_project_subdir("reassemble-dataaaaa", "src");
	ASSERT_FALSE(path.has_value());
}

TEST(GetPathToProjectSubdir, IncorrectSubdir) // NOLINT
{
	namespace impl = reassembler_impl;
	auto path = impl::get_path_to_project_subdir("reassemble-data", "srcccc");
	ASSERT_FALSE(path.has_value());
}

TEST(GetPathToProjectSubdir, IncorrectPath) // NOLINT
{
	namespace impl = reassembler_impl;
	auto path =
		impl::get_path_to_project_subdir("reassemble-dataaaaa", "srcccc");
	ASSERT_FALSE(path.has_value());
}