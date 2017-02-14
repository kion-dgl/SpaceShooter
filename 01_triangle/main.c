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

int init_resources();
void on_display();

GLuint program, vbo_player;
GLint attribute_coord2d, attribute_v_color;

int main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitContextVersion(2, 0);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(640, 480);
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
	glutMainLoop();

	return 0;

}

int init_resources() {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLfloat player_vertices[] = {
		0.0, 0.7, 1.0, 1.0, 1.0,
		-0.7, -0.7, 1.0, 1.0, 1.0,
		0.7, -0.7, 1.0, 1.0, 1.0
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

