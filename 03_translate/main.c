/*
    This file is part of DashGL.com SpaceShooter Tutorial

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

#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "lib/dashgl.h"

#define WIDTH 640
#define HEIGHT 480

int init_resources();
void on_display();
void on_idle();
void keydown_callback(int key, int x, int y);
void keyup_callback(int key, int x, int y);

struct {
	float x;
	float y;
	float dx;
	char left_down;
	char right_down;
} Player;

GLuint program, vbo_player;
GLint attribute_coord2d, attribute_v_color;
GLint uniform_mvp;

int main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitContextVersion(2, 0);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("DashGL - Space Shooter");

	GLenum glew_status = glewInit();
	if(glew_status != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return 1;
	}

	if(!init_resources()) {
		fprintf(stderr, "Error: Could not initialize resources\n");
		return 1;
	}

	glutDisplayFunc(on_display);
	glutIdleFunc(on_idle);
	glutSpecialFunc(keydown_callback);
	glutSpecialUpFunc(keyup_callback);

	glutMainLoop();

	return 0;

}

int init_resources() {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	Player.x = 320.0f;
	Player.y = 32.0f;
	Player.dx = 4.0f;
	Player.left_down = 0;
	Player.right_down = 0;

	GLfloat player_vertices[] = {
		0.0, 18.0, 1.0, 1.0, 1.0,
		-18.0, -18.0, 1.0, 1.0, 1.0,
		18.0, -18.0, 1.0, 1.0, 1.0
	};

	glGenBuffers(1, &vbo_player);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_player);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(player_vertices),
		player_vertices,
		GL_STATIC_DRAW
	);

	program = dash_create_program("shader/vertex.glsl", "shader/fragment.glsl");
	if(!program) {
		return 0;
	}

	const char *attribute_name;

	attribute_name = "coord2d";
	attribute_coord2d = glGetAttribLocation(program, attribute_name);
	if(attribute_coord2d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

	attribute_name = "v_color";
	attribute_v_color = glGetAttribLocation(program, attribute_name);
	if(attribute_v_color == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

	GLint uniform_ortho;
	mat4 ortho;
	mat4_orthographic(0, WIDTH, HEIGHT, 0, ortho);

	const char *uniform_name;
	uniform_name = "ortho";
	uniform_ortho = glGetUniformLocation(program, uniform_name);
	if(uniform_ortho == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}

	uniform_name = "mvp";
	uniform_mvp = glGetUniformLocation(program, uniform_name);
	if(uniform_mvp == -1) {
		fprintf(stderr, "Could not bind uniform%s\n", uniform_name);
		return 0;
	}

	glUseProgram(program);
	glUniformMatrix4fv(uniform_ortho, 1, GL_FALSE, ortho);

	return 1;

}

void on_display() {

	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(program);
	glEnableVertexAttribArray(attribute_coord2d);
	glEnableVertexAttribArray(attribute_v_color);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_player);

	glVertexAttribPointer(
		attribute_coord2d,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 5,
		0
	);

	glVertexAttribPointer(
		attribute_v_color,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 5,
		(void*)(sizeof(float) * 2)
	);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(attribute_coord2d);
	glDisableVertexAttribArray(attribute_v_color);

	glutSwapBuffers();

}

void on_idle() {

	glUseProgram(program);

	if(Player.left_down) {
		Player.x -= Player.dx;
	}

	if(Player.right_down) {
		Player.x += Player.dx;
	}

	if(Player.x < 0.0) {
		Player.x = 0.0;
	} else if(Player.x > 640.0) {
		Player.x = 640.0f;
	}

	mat4 pos;
	vec3 t = { Player.x, Player.y, 0.0 };
	mat4_translate(t, pos);
	
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, pos);
	glutPostRedisplay();

}

void keydown_callback(int key, int x, int y) {

	switch(key) {

		case GLUT_KEY_RIGHT:
			Player.right_down = 1;
		break;

		case GLUT_KEY_LEFT:
			Player.left_down = 1;
		break;

	}

}

void keyup_callback(int key, int x, int y) {

	switch(key) {

		case GLUT_KEY_RIGHT:
			Player.right_down = 0;
		break;

		case GLUT_KEY_LEFT:
			Player.left_down = 0;
		break;

	}
}


