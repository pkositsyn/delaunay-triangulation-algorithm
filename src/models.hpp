#pragma once

#include <algorithm>
#include <cassert>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace geometry {

// Тип ребро, фактически работает как std::pair<int, int> (компаратор не нужен)
struct Edge {
  explicit Edge(int _v1 = -1, int _v2 = -1) : v1(_v1), v2(_v2) {}
  int v1;    // Вершины, соединяющие ребро
  int v2;
  bool operator==(const Edge& other) const {
    return v1 == other.v1 && v2 == other.v2;
  }
};

// Вектор в двумерном пространстве
struct Vector2D {
  Vector2D(double x, double y) : x(x), y(y) {}
  double x, y;

  Vector2D operator+(const Vector2D& rhs) const;
  Vector2D operator-(const Vector2D& rhs) const;
  Vector2D operator-() const;
};

// Модуль векторного произведения со знаком
inline double CrossProduct(const Vector2D& lhs, const Vector2D& rhs) {
  return lhs.x * rhs.y - lhs.y * rhs.x;
}

// Множество из 2 вершин, структура для удобства
struct TwoVertices {
  explicit TwoVertices(int v1 = -1, int v2 = -1) : v1(v1), v2(v2) {}
  int v1;
  int v2;
  inline void Insert(int v) {
    if (v1 == v || v2 == v) return;
    assert(this->Size() < 2);
    (v1 == -1 ? v1 : v2) = v;
  }
  inline void Erase(int v) {
    if (v1 == v) {
      v1 = -1;
    } else if (v2 == v) {
      v2 = -1;
    }
  }
  inline void Replace(int u, int v) {
    if (v1 == u) {
      v1 = v;
    } else if (v2 == u) {
      v2 = v;
    } else {
      Insert(v);
    }
  }
  inline int Max() const {
    assert(v1 != -1 || v2 != -1);
    return std::max(v1, v2);
  }
  inline int Min() const {
    if (v1 != -1 && v2 != -1) {
      return std::min(v1, v2);
    }
    assert(v1 != -1 || v2 != -1);
    return v1 != -1 ? v1 : v2;
  }
  inline int Size() const { return (v1 != -1) + (v2 != -1); }
};

// Определение хэш-функции для ребра как комбинация хэшей вершин
struct EdgeHash {
  std::hash<int> int_hash;
  std::size_t operator()(const Edge& edge) const {
    std::size_t lhs = int_hash(edge.v1);
    std::size_t rhs = int_hash(edge.v2);
    lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
    return lhs;
  }
};

struct ListNode {
  int left;
  int right;
};

using Triangulation = std::unordered_map<Edge, TwoVertices, EdgeHash>;
using List = std::vector<ListNode>;

struct DelaunayTriangulation {
  Triangulation graph{};
  std::vector<Vector2D> points{};
};

}  // namespace geometry
