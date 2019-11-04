#include "models.hpp"

namespace geometry {

Vector2D Vector2D::operator+(const Vector2D& rhs) const {
	return {x + rhs.x, y + rhs.y};
}

Vector2D Vector2D::operator-(const Vector2D& rhs) const {
	return {x - rhs.x, y - rhs.y};
}

Vector2D Vector2D::operator-() const {
	return {-x, -y};
}

}  // namespace geometry
