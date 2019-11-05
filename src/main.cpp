#include "scanline_triangulation.hpp"

#include <iomanip>
#include <iostream>
#include <unordered_set>

auto GetPointsFromStdin() {
  std::vector<geometry::Vector2D> points;

  // Считывание пар точек до EOF
  double x, y;
  while ((std::cin >> x) && (std::cin >> y)) {
    points.emplace_back(x, y);
  }
  return points;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  auto points = GetPointsFromStdin();
  const auto builder = geometry::DelaunayBuilder::Create(std::move(points));
  const auto& triangulation = builder->Get();

  const auto convex_hull = geometry::BuildConvexHull(triangulation.graph);

  return 0;
}
