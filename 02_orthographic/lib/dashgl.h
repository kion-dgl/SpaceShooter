/*
    This file is part of Dash Graphics Library

    Copyright (C) 2017 Benjamin Collins

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License version 2
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DASHGL_UTILS
#define DASHGL_UTILS

	/**********************************************************************/
	/** Typedef                                                          **/	
	/**********************************************************************/

	typedef float mat4[16];
	typedef float vec3[3];

	/**********************************************************************/
	/** Constants                                                        **/	
	/**********************************************************************/

	#define M_00 0
	#define M_10 1
	#define M_20 2
	#define M_30 3
	#define M_01 4
	#define M_11 5
	#define M_21 6
	#define M_31 7
	#define M_02 8
	#define M_12 9
	#define M_22 10
	#define M_32 11
	#define M_03 12
	#define M_13 13
	#define M_23 14
	#define M_33 15

	/**********************************************************************/
	/** Shader Utilities                                                 **/	
	/**********************************************************************/

	GLuint dash_create_shader(const char *filename, GLenum type);
	void dash_print_log(GLuint object);
	GLuint dash_create_program(const char *vertex, const char *fragment);
	GLuint dash_texture_load(const char *filename);
	
	/**********************************************************************/
	/** Vector3 Utilities                                                **/	
	/**********************************************************************/

	void vec3_subtract(vec3 a, vec3 b, vec3 v);
	void vec3_cross_multiply(vec3 a, vec3 b, vec3 v);
	void vec3_normalize(vec3 a, vec3 v);
	
	/**********************************************************************/
	/** Matrix Utilities                                                 **/	
	/**********************************************************************/

	void mat4_identity(mat4 m);
	void mat4_copy(mat4 a, mat4 m);
	void mat4_translate(vec3 t, mat4 m);
	void mat4_rotate_x(float x, mat4 m);
	void mat4_rotate_y(float y, mat4 m);
	void mat4_rotate_z(float z, mat4 m);
	void mat4_multiply(mat4 a, mat4 b, mat4 m);
	void mat4_rotate(vec3 r, mat4 m);
	void mat4_look_at(vec3 eye, vec3 center, vec3 up, mat4 m);
	void mat4_perspective(float y_fov, float aspect, float n, float f, mat4 m);
	void mat4_orthographic(int left, int right, int top, int bottom, mat4 m);

#endif
