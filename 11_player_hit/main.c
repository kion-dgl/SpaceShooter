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
#include <math.h>
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

struct Bullet {
	float x;
	float y;
	float dy;
	char active;
};

struct {
	float x;
	float y;
	float dx;
	char left_down;
	char right_down;
	char shoot;
	char max_bullets;
	struct Bullet *bullets;
} Player;

struct Enemy {
	float x;
	float y;
	int cooldown;
	char active;
}; 

#define NB_ENEMIES 3
#define ENEMY_BULLETS 10

char gameover = 0;
float enemy_dx = 3.0f;
struct Enemy enemies[NB_ENEMIES];
struct Bullet enemy_bullets[ENEMY_BULLETS];

GLuint program, vbo_player, vbo_bullet, vbo_enemy, vbo_enemy_bullet;
GLuint tex_program, vbo_gameover, texture_id;
GLint attribute_coord2d, attribute_v_color;
GLint uniform_mvp, uniform_mytexture;
GLint tex_coord, tex_pos;

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
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
	Player.shoot = 0;
	Player.max_bullets = 5;
	Player.bullets = (struct Bullet*)malloc(Player.max_bullets * sizeof(struct Bullet));
	int i;
	for(i = 0; i < Player.max_bullets; i++) {
		Player.bullets[i].active = 0;
		Player.bullets[i].dy = 4.0;
	}


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

	GLfloat bullet_vertices[] = {
		// left
		-5.0, -5.0, 1.0, 1.0, 0.0,
		-5.0, 5.0, 1.0, 1.0, 0.0,
		5.0, 5.0, 1.0, 1.0, 0.0,
		// right
		-5.0, -5.0, 1.0, 1.0, 0.0,
		5.0, 5.0, 1.0, 1.0, 0.0,
		5.0, -5.0, 1.0, 1.0, 0.0
	};

	glGenBuffers(1, &vbo_bullet);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_bullet);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(bullet_vertices),
		bullet_vertices,
		GL_STATIC_DRAW
	);

	GLfloat enemy_vertices[] = {
		// left
		-30.0, -15.0, 0.99, 0.83, 0.98,
		-30.0, 15.0, 0.99, 0.83, 0.98,
		30.0, 15.0, 0.99, 0.83, 0.98,

		// right
		-30.0, -15.0, 0.99, 0.83, 0.98,
		30.0, 15.0,  0.99, 0.83, 0.98,
		30.0, -15.0, 0.99, 0.83, 0.98
	};

	glGenBuffers(1, &vbo_enemy);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_enemy);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(enemy_vertices),
		enemy_vertices,
		GL_STATIC_DRAW
	);

	enemies[0].x = 180;
	enemies[0].y = 420;
	enemies[0].active = 1;
	enemies[0].cooldown = rand() % 120;;

	enemies[1].x = 320;
	enemies[1].y = 420;
	enemies[1].active = 1;
	enemies[1].cooldown = rand() % 120;;

	enemies[2].x = 460;
	enemies[2].y = 420;
	enemies[2].active = 1;
	enemies[2].cooldown = rand() % 120;;
	
	GLfloat enemy_bullet_vertices[] = {
		// left
		-5.0, -5.0, 1.0, 0.0, 0.0,
		-5.0, 5.0, 1.0, 0.0, 0.0,
		5.0, 5.0, 1.0, 0.0, 0.0,
		// right
		-5.0, -5.0, 1.0, 0.0, 0.0,
		5.0, 5.0, 1.0, 0.0, 0.0,
		5.0, -5.0, 1.0, 0.0, 0.0
	};

	glGenBuffers(1, &vbo_enemy_bullet);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_enemy_bullet);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(enemy_bullet_vertices),
		enemy_bullet_vertices,
		GL_STATIC_DRAW
	);

	for(i = 0; i < ENEMY_BULLETS; i++) {
		enemy_bullets[i].active = 0;
	}

	texture_id = dash_texture_load("gameover.png");

	GLfloat gameover_vertices[] = {
		// left
		0.0, 0.0, 0.0, 0.0,
		0.0, 480.0, 0.0, 1.0,
		640.0, 480.0, 1.0, 1.0,
		// right
		0.0, 0.0, 0.0, 0.0,
		640.0, 0.0, 1.0, 0.0,
		640.0, 480.0, 1.0, 1.0
	};

	glGenBuffers(1, &vbo_gameover);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_gameover);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(gameover_vertices),
		gameover_vertices,
		GL_STATIC_DRAW
	);

	program = dash_create_program("shader/vertex.glsl", "shader/fragment.glsl");
	if(!program) {
		return 0;
	}

	tex_program = dash_create_program("shader/display.glsl", "shader/texture.glsl");
	if(!tex_program) {
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

	attribute_name = "coord2d";
	tex_pos = glGetAttribLocation(tex_program, attribute_name);
	if(tex_pos == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

	attribute_name = "texcoord";
	tex_coord = glGetAttribLocation(tex_program, attribute_name);
	if(tex_coord == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

	GLint uniform_ortho, uniform_tex_ortho;
	mat4 ortho;
	mat4_orthographic(0, WIDTH, HEIGHT, 0, ortho);

	const char *uniform_name;
	uniform_name = "ortho";
	uniform_ortho = glGetUniformLocation(program, uniform_name);
	if(uniform_ortho == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}

	uniform_name = "ortho";
	uniform_tex_ortho = glGetUniformLocation(tex_program, uniform_name);
	if(uniform_tex_ortho == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}

	uniform_name = "mvp";
	uniform_mvp = glGetUniformLocation(program, uniform_name);
	if(uniform_mvp == -1) {
		fprintf(stderr, "Could not bind uniform%s\n", uniform_name);
		return 0;
	}

	uniform_name = "mytexture";
	uniform_mytexture = glGetUniformLocation(tex_program, uniform_name);
	if(uniform_mytexture == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}

	glUseProgram(tex_program);
	glUniformMatrix4fv(uniform_ortho, 1, GL_FALSE, ortho);

	glUseProgram(program);
	glUniformMatrix4fv(uniform_ortho, 1, GL_FALSE, ortho);

	return 1;

}

void on_display() {

	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(program);
	glEnableVertexAttribArray(attribute_coord2d);
	glEnableVertexAttribArray(attribute_v_color);
	
	int i;
	mat4 pos;
	vec3 t = { Player.x, Player.y, 0.0 };
	mat4_translate(t, pos);
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, pos);


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
	
	for(i = 0; i < Player.max_bullets; i++) {
	
		if(!Player.bullets[i].active) {
			continue;
		}

		t[0] = Player.bullets[i].x; 
		t[1] = Player.bullets[i].y;
		t[2] = 0.0;
		mat4_translate(t, pos);
		glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, pos);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_bullet);
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
	
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	
	for(i = 0; i < NB_ENEMIES; i++) {
		if(!enemies[i].active) {
			continue;
		}

		t[0] = enemies[i].x; 
		t[1] = enemies[i].y;
		t[2] = 0.0;
		mat4_translate(t, pos);
		glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, pos);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_enemy);
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
	
		glDrawArrays(GL_TRIANGLES, 0, 6);	
	}

	for(i = 0; i < ENEMY_BULLETS; i++) {
		if(!enemy_bullets[i].active) {
			continue;
		}

		t[0] = enemy_bullets[i].x; 
		t[1] = enemy_bullets[i].y;
		t[2] = 0.0;
		mat4_translate(t, pos);
		glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, pos);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_enemy_bullet);
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
	
		glDrawArrays(GL_TRIANGLES, 0, 6);	
	}

	glDisableVertexAttribArray(attribute_coord2d);
	glDisableVertexAttribArray(attribute_v_color);

	if(gameover) {
		
		glUseProgram(tex_program);
		glEnableVertexAttribArray(tex_coord);
		glEnableVertexAttribArray(tex_pos);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glUniform1i(uniform_mytexture, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_gameover);
		glVertexAttribPointer(
			tex_pos,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(float) * 4,
			0
		);

		glVertexAttribPointer(
			tex_coord,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(float) * 4,
			(void*)(sizeof(float) * 2)
		);
		
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(tex_coord);
		glDisableVertexAttribArray(tex_pos);
	}

	glutSwapBuffers();

}

void on_idle() {

	if(gameover) {
		glutPostRedisplay();
		return;
	}
	
	float a, b, c;

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

	int i, k;
	for(i = 0; i < Player.max_bullets; i++){
		if(!Player.bullets[i].active) {
			continue;
		}

		Player.bullets[i].y += Player.bullets[i].dy;

		if(Player.bullets[i].y > 660.0) {
			Player.bullets[i].active = 0;
			continue;
		}

		for(k = 0; k < NB_ENEMIES; k++) {
			if(!enemies[k].active ||!Player.bullets[i].active) {
				continue;
			}

			if(Player.bullets[i].x < enemies[k].x - 30.0f || 
			   Player.bullets[i].x > enemies[k].x + 30.0f) {
				
				continue;

			}

			if(Player.bullets[i].y < enemies[k].y - 15.0f || 
			   Player.bullets[i].y < enemies[k].y + 15.0f) {
				
				continue;

			}
			
			enemies[k].active = 0;
			Player.bullets[i].active = 0;
		}
	}

	char switch_dir = 0;
	for(i = 0; i < NB_ENEMIES; i++) {
		
		if(!enemies[i].active) {
			continue;
		}

		if(enemies[i].cooldown) {
			enemies[i].cooldown--;
		}
		
		if(!enemies[i].cooldown) {
			
			if(rand() % 50 > 20) {
				
				for(k = 0; k < ENEMY_BULLETS; k++) {
					if(enemy_bullets[k].active) {
						continue;
					}

					enemy_bullets[k].active = 1;
					enemy_bullets[k].x = enemies[i].x;
					enemy_bullets[k].y = enemies[i].y;
					enemy_bullets[k].dy = -2.0f;
				
					enemies[i].cooldown = rand() % 50 + 10;
					break;
				}
			
			}
		}

		enemies[i].x += enemy_dx;

		if(enemies[i].x < 0 || enemies[i].x > WIDTH) {
			switch_dir = 1;
		}

	}

	if(switch_dir) {
		
		enemy_dx *= -1.05;

		for(i = 0; i < NB_ENEMIES; i++) {
		
			if(!enemies[i].active) {
				continue;
			}

			enemies[i].y -= 10;
		}

	}

	for(i = 0; i < ENEMY_BULLETS; i++) {
		
		if(!enemy_bullets[i].active) {
			continue;
		}

		enemy_bullets[i].y += enemy_bullets[i].dy;

		a = pow(Player.x - enemy_bullets[i].x, 2);
		b = pow(Player.y - enemy_bullets[i].y, 2);

		c = sqrt(a + b);
		
		if(c < 13.0f) {
			gameover = 1;
		}

		if(enemy_bullets[i].y < -10) {
			enemy_bullets[i].active = 0;
		}
	}

	glutPostRedisplay();

}

void keydown_callback(int key, int x, int y) {
	
	int i;

	switch(key) {

		case GLUT_KEY_RIGHT:
			Player.right_down = 1;
		break;

		case GLUT_KEY_LEFT:
			Player.left_down = 1;
		break;

		case GLUT_KEY_UP:
			
			if(!Player.shoot) {
				Player.shoot = 1;
				
				for(i = 0; i < Player.max_bullets; i++) {
					if(Player.bullets[i].active) {
						continue;
					}

					Player.bullets[i].active = 1;
					Player.bullets[i].x = Player.x;
					Player.bullets[i].y = Player.y + 10.0f;
					break;
				}

			}

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
		
		case GLUT_KEY_UP:
			Player.shoot = 0;
		break;

	}
}


