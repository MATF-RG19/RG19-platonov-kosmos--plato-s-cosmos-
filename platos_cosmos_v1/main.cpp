/* 	Racunarska grafika  
	Projekat: Platonov kosmos
	Student: Miloš Krsmanović, 263/2015
*/
#include <iostream>
#include <cstdlib>

#include <functional>
#include <memory>
#include <GL/glut.h>

#include <time.h>
#include <math.h>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include "classes.hpp"

rg::_Data global {
    .body_translate_parameter = 0.05,
    .game_running = true,
    .timer_id = 0,
    .timer_interval = 10,
    .body_spawn_point = glm::vec3(0,0,-10),
    .body_generation_cooldown = 0,
    .body_manager = rg::BodyManager(),
    .projection_matrix = glm::mat4(1.0f),
    .view_matrix = glm::mat4(1.0f),
    .viewport = glm::vec4(0,0,0,0),
    .mouse = rg::Mouse(),
    .keyboard = rg::Keyboard()
};

// Zbog bolje citljivosti, ovde cu definisati sve 
// konstante koje su u upotrebi

#define esc 27
#define sirina_prozora 1000
#define duzina_prozora 500
#define NazivAplikacije "Platonov kosmos"

void on_mouse_click(int button, int state, int x, int y);
void on_mouse_move(int x, int y);
static void on_keyboard(unsigned char key, int x1, int y1);
static void on_keyboard_release(unsigned char key, int x1, int y1);
void on_display(void);
void on_reshape(int width, int height);
static void on_timer(int value);

int main(int argc, char** argv) {
	//Inicijalizacija gluta
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	
	//Kreiranje prozora
	glutInitWindowSize(sirina_prozora, duzina_prozora);			
	glutInitWindowPosition(200, 100); //Srediti
	glutCreateWindow(NazivAplikacije);
	
	//Inicijalizacija on display funkcija
	glutDisplayFunc(on_display);
	glutReshapeFunc(on_reshape);

	glutKeyboardFunc(on_keyboard);
    glutKeyboardUpFunc(on_keyboard_release);
    glutMouseFunc(on_mouse_click);
    glutMotionFunc(on_mouse_move);
    glutPassiveMotionFunc(on_mouse_move);

    // OpenGL inicijalizacija 
    glClearColor(0.1, 0.1, 0.1, 1);


    glutTimerFunc(global.timer_interval, on_timer, global.timer_id);
    global.body_manager.send_request(std::make_unique<rg::BodyManager::GenerateBodyRequest>
            (std::make_unique<rg::Hexahedron>(global.body_spawn_point, glm::vec2(0,0))));

	//Glavna petlja
	glutMainLoop();

	return 0;
}

void on_mouse_click(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        global.mouse.press_left_click();

        if (global.keyboard.is_pressed('h') || global.keyboard.is_pressed('H')) {
            global.body_manager.send_request(std::make_unique<rg::BodyManager::RemoveBodyRequest>
                    (rg::BodyType::HEXAHEDRON, global.mouse.position(), global.viewport, global.projection_matrix));
        } else {
            global.body_manager.send_request(std::make_unique<rg::BodyManager::RemoveBodyRequest>
                    (rg::BodyType::NON_PLATONIC, global.mouse.position(), global.viewport, global.projection_matrix));        
        }
        // TODO uraditi za ostale objekte
    }
}

void on_mouse_move(int x, int y) {
    global.mouse.set_position({x,y});
}

void on_keyboard(unsigned char key, int x1, int y1) {
	
	switch (key){
        //Ako je pritisnut 'esc' izlazimo iz programa
        case esc:
            exit(EXIT_SUCCESS);
            break;

        default: global.keyboard.press_key(key);
  }
}

static void on_keyboard_release(unsigned char key, int x1, int y1) { 
    global.keyboard.release_key(key);
}

void on_display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

    // Ucitamo pogled (kameru)
    glLoadMatrixf(glm::value_ptr(global.view_matrix)); 	

    // Iscrtamo tela koje je generisao body generator
    for (const auto &body : global.body_manager.bodies()) {
        glColor3f(1,0,0);
        glMultMatrixf(body->raw_transformation_matrix());

        body->draw();
    }

	glutSwapBuffers();
}


void on_reshape(int width, int height)
{
    global.viewport = glm::vec4(0.0f, 0.0f, width, height);
	glViewport(0.0f,0.0f,width,height);
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    global.projection_matrix = glm::perspective(45.0f, static_cast<float>(width)/height, 0.01f, 100.0f);
    glMultMatrixf(glm::value_ptr(global.projection_matrix));
}

static void on_timer(int value)
{
	if (value != global.timer_id || !global.game_running)
		return;

    // FOR DEBUGING
    //for (const auto &b : global.body_manager.bodies()) {
    //    std::cout << glm::to_string(b->transformation_matrix()) << std::endl;
    //}

    if (auto opt_error = global.body_manager.update(global.body_translate_parameter); opt_error.has_value()) {
        std::cerr << opt_error.value() << std::endl;
        exit(EXIT_FAILURE);
    }

	glutPostRedisplay();
	glutTimerFunc(global.timer_interval, on_timer, global.timer_id);
}
