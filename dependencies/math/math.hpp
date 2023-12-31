#pragma once

namespace math {
	void correct_movement(vec3_t old_angles, c_usercmd* cmd, float old_forwardmove, float old_sidemove);
	vec3_t calculate_angle(vec3_t& a, vec3_t& b);
	void sin_cos(float r, float* s, float* c);
	vec3_t angle_vector(vec3_t angle);
	void transform_vector(vec3_t&, matrix_t&, vec3_t&);
	void vector_angles(vec3_t&, vec3_t&);
	void angle_vectors(vec3_t&, vec3_t*, vec3_t*, vec3_t*);
	vec3_t vector_add(vec3_t&, vec3_t&);
	vec3_t vector_subtract(vec3_t&, vec3_t&);
	vec3_t vector_multiply(vec3_t&, vec3_t&);
	vec3_t vector_divide(vec3_t&, vec3_t&);
	bool screen_transform(const vec3_t& point, vec3_t& screen);
	bool world_to_screen(const vec3_t& origin, vec2_t& screen);
	bool world_to_screen_vec3(const vec3_t& origin, vec3_t& screen);
	vec3_t calculate_angle2(const vec3_t& source, const vec3_t& destination, const vec3_t& viewAngles);
	void angle_vectors(vec3_t& angles, vec3_t& forward);
	float distance_based_fov(float distance, vec3_t angle, c_usercmd* cmd);
	float normalize_yaw(float yaw);
	void angle_vectors_const(const vec3_t& angles, vec3_t& forward);
	float get_fov(const vec3_t& view_angle, const vec3_t& aim_angle);

	template<class T>
	void normalize3(T& vec)
	{
		for (auto i = 0; i < 2; i++) {
			while (vec[i] < -180.0f) vec[i] += 360.0f;
			while (vec[i] > 180.0f) vec[i] -= 360.0f;
		}
		vec[2] = 0.f;
	}
};
