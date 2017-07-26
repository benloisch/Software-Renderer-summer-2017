#ifndef COLOR_RGB_H_
#define COLOR_RGB_H_

class Color_RGB {
public:
	float r;
	float g;
	float b;

	Color_RGB();
	Color_RGB(float r, float g, float b);
	inline Color_RGB Color_RGB::operator*(Color_RGB &rhs) {
		Color_RGB rgb;
		rgb.r = this->r * rhs.r;
		rgb.g = this->g * rhs.g;
		rgb.b = this->b * rhs.b;
		return rgb;
	}

	inline Color_RGB Color_RGB::operator+(Color_RGB &rhs) {
		Color_RGB rgb;
		rgb.r = this->r + rhs.r;
		rgb.g = this->g + rhs.g;
		rgb.b = this->b + rhs.b;
		return rgb;
	}

	inline Color_RGB Color_RGB::operator*(float rhs) {
		Color_RGB rgb;
		rgb.r = r * rhs;
		rgb.g = g * rhs;
		rgb.b = b * rhs;
		return rgb;
	}

	inline Color_RGB Color_RGB::operator/(float rhs) {
		Color_RGB rgb;
		rgb.r = r / rhs;
		rgb.g = g / rhs;
		rgb.b = b / rhs;
		return rgb;
	}

	inline Color_RGB& Color_RGB::operator=(const Color_RGB &rhs) {
		this->r = rhs.r;
		this->g = rhs.g;
		this->b = rhs.b;
		return *this;
	}

	inline Color_RGB& Color_RGB::operator+=(const Color_RGB &rhs) {
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		return *this;
	}

	inline Color_RGB& Color_RGB::operator/=(const float rhs) {
		r = r / rhs;
		g = g / rhs;
		b = b / rhs;
		return *this;
	}
};

#endif