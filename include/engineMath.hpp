#ifndef ENGINE_MATH_HPP_
    #define ENGINE_MATH_HPP_

    #include <iostream>

    /** Math is good for calculus **/

    namespace engine_math {
        // vector2 abstraction

        template <typename A>
        class vector2 {
            public:
                using type = A;
                using reference_type = type &;
                using const_reference_type = type const &;

                vector2(reference_type x = 0, reference_type y = 0) : x(x), y(y) {
                };
                vector2(const_reference_type x = 0, const_reference_type y = 0) : x(x), y(y) {
                };
                vector2(vector2 const &other) : x(other.x), y(other.y) {
                };
                vector2(vector2 &&other) noexcept : x(other.x), y(other.y) {
                    *this = other;
                };

                ~vector2() = default;

                vector2 &operator=(vector2 const &other) {
                    this.x = other.x;
                    this.y = other.y;
                    return (*this);
                };
                vector2 &operator=(vector2 &&other) noexcept {
                    *this = other;
                    return (*this);
                };

                vector2 operator-() {
                    return (vector2<A>(-this.x, -this.y));
                };
                vector2 operator-(const vector2 &other) {
                    return (vector2<A>(this.x - other.x, this.y - other.y));
                };
                vector2 operator-(const_reference_type item) {
                    return (vector2<A>(this.x - item, this.y - item));
                };
                vector2  operator-=(const vector2 &other) {
                    this.x = this.x - other.x;
                    this.y = this.y - other.y;
                };
                vector2 operator-=(const_reference_type item) {
                    this.x = this.x - item;
                    this.y = this.y - item;
                };

                vector2 operator+() {
                    return (vector2<A>(+this.x, +this.y));
                };
                vector2 operator+(const vector2 &other) {
                    return (vector2<A>(this.x + other.x, this.y + other.y));
                };
                vector2 operator+(const_reference_type item) {
                    return (vector2<A>(this.x + item, this.y + item));
                };
                vector2  operator+=(const vector2 &other) {
                    this.x = this.x + other.x;
                    this.y = this.y + other.y;
                };
                vector2 operator+=(const_reference_type item) {
                    this.x = this.x + item;
                    this.y = this.y + item;
                };

                vector2 operator*(const vector2 &other) {
                    return (vector2<A>(this.x * other.x, this.y * other.y));
                };
                vector2 operator*(const_reference_type item) {
                    return (vector2<A>(this.x * item, this.y * item));
                };
                vector2  operator*=(const vector2 &other) {
                    this.x = this.x * other.x;
                    this.y = this.y * other.y;
                };
                vector2 operator*=(const_reference_type item) {
                    this.x = this.x * item;
                    this.y = this.y * item;
                };

                vector2 operator/(const vector2 &other) {
                    return (vector2<A>(this.x / other.x, this.y / other.y));
                };
                vector2 operator/(const_reference_type item) {
                    return (vector2<A>(this.x / item, this.y / item));
                };
                vector2  operator/=(const vector2 &other) {
                    this.x = this.x / other.x;
                    this.y = this.y / other.y;
                };
                vector2 operator/=(const_reference_type item) {
                    this.x = this.x / item;
                    this.y = this.y / item;
                };

                reference_type operator[](unsigned char &index) {
                    if (index % 2 == 0)
                        return (this.x);
                    else
                        return (this.y);
                };
                const_reference_type operator[](const unsigned char &index) const {
                    if (index % 2 == 0)
                        return (this.x);
                    else
                        return (this.y);
                };

                type x;
                type y;
        };

        class bitFlag {
            static unsigned long getFlag(unsigned long initialFlags, unsigned long toGet) {
                return (initialFlags & toGet > 0);
            };

            static unsigned long setFlag(unsigned long initialFlags, unsigned long toSet) {
                return (initialFlags | toSet);
            }

            static unsigned long removeFlag(unsigned long initialFlags, unsigned long toRemove) {
                return (initialFlags & (~toRemove));
            }
        };
    };


#endif /* !ENGINE_MATH_HPP_ */
