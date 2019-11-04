#pragma once

#include "models.hpp"

#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>
#include <unordered_set>
#include <unordered_map>

namespace geometry {

// Данной погрешности вычислений обычно хватает, но в теории могут возникать проблемы
const auto eps = 1e-9;

class DelaunayBuilder {

 public:
	DelaunayBuilder(const DelaunayBuilder&) = delete;
	const DelaunayBuilder& operator=(const DelaunayBuilder&) = delete;
	DelaunayBuilder(DelaunayBuilder&&) = default;
	DelaunayBuilder& operator=(DelaunayBuilder&&) = default;

	inline const DelaunayTriangulation& Get() const { return triangulation_; }

	static std::unique_ptr<DelaunayBuilder> Create(std::vector<Vector2D> points);

 private:
	explicit DelaunayBuilder(std::vector<Vector2D> points) noexcept {
		triangulation_.points = std::move(points);
	}

	// Проверяет, нужно ли перестраивать четырехугольник, заданный индексами точек в аргументах
	bool CheckDelaunayCondition(int left, int right, int outer, int inner) const;

	// Строит триангуляцию
	void Build();

	void AddPointToTriangulation(int i /*index*/);

	// Функция, которая рекурсивно перестраивает триангуляцию (шаг 4)
	void FixTriangulation(int left, int right, int outer);

	// Моделируем рекурсию с помощью стека, чтобы не было переполнения
	std::vector<Edge> recursion_stack_{};

	// Структура для хранения триангуляции: ребро -> пара вершин + точки
	DelaunayTriangulation triangulation_{};

	List convex_hull_{};
};

// Возвращает выпуклую оболочку как номера индексов вершин points в ней
std::unordered_set<int> BuildConvexHull(const Triangulation& triangulation);

}  // namespace geometry
