#include "gtest/gtest.h"

#define private public  // never do this
#include "../src/scanline_triangulation.hpp"

#include <fstream>

auto GetPointsFromStream(std::ifstream& in) {
	std::vector<geometry::Vector2D> points;
	double x, y;
// Считывание пар точек до EOF
	while ((in >> x) && (in >> y)) {
		points.emplace_back(x, y);
	}
	return points;
}

auto FileTestBody(std::ifstream&& in) {
	auto points = GetPointsFromStream(in);
	auto builder = geometry::DelaunayBuilder::Create(std::move(points));
	const auto& [triangulation, _] = builder->Get();
	for (const auto& [edge, vertices] : triangulation) {
		if (vertices.size() == 2) {
			builder->CheckDelaunayCondition(edge.v1,
																			edge.v2,
																			vertices.GetMin(),
																			vertices.GetMax());
		}
	}
}

TEST(Triangulation, Simple) {
	std::vector<geometry::Vector2D> points = {{0, 0}, {1, 0}, {0, 1}};

	auto builder = geometry::DelaunayBuilder::Create(std::move(points));
	const auto& [triangulation, _] = builder->Get();
	ASSERT_EQ(triangulation.size(), 3);

	auto outer = triangulation.at(geometry::Edge{0, 1});
	ASSERT_EQ(outer.GetMin(), 2);
	ASSERT_EQ(outer.GetMax(), 2);

	outer = triangulation.at(geometry::Edge{0, 2});
	ASSERT_EQ(outer.GetMin(), 1);
	ASSERT_EQ(outer.GetMax(), 1);

	outer = triangulation.at(geometry::Edge{1, 2});
	ASSERT_EQ(outer.GetMin(), 0);
	ASSERT_EQ(outer.GetMax(), 0);
}

TEST(Triangulation, FileInput1) {
	std::ifstream in;
	in.open("../test/tests/001");
	FileTestBody(std::move(in));
}

TEST(Triangulation, FileInput2) {
	std::ifstream in;
	in.open("../test/tests/002");
	FileTestBody(std::move(in));
}

TEST(Triangulation, FileInput3) {
	std::ifstream in;
	in.open("../test/tests/003");
	FileTestBody(std::move(in));
}

TEST(Triangulation, FileInput4) {
	std::ifstream in;
	in.open("../test/tests/004");
	FileTestBody(std::move(in));
}

TEST(Triangulation, FileInput5) {
	std::ifstream in;
	in.open("../test/tests/005");
	FileTestBody(std::move(in));
}

int main(int argc, char *argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
