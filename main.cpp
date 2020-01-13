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

#include "classes.hpp"

struct Global {
    const float body_translate_parameter = 0.05;
    bool game_running = true;
    const int timer_id = 0; // priblizno 72 puta u sekundi
    const int timer_interval = 14; // priblizno 72 puta u sekundi

    int body_generation_cooldown = 5000; // 5s
    std::shared_ptr<rg::BodyManager> body_manager { nullptr };
    std::shared_ptr<glm::mat4> projection_matrix { nullptr };
    std::shared_ptr<glm::mat4> view_matrix { nullptr };
    std::shared_ptr<glm::vec4> viewport { nullptr };

    std::shared_ptr<rg::Mouse> mouse { nullptr };
    std::shared_ptr<rg::Keyboard> keyboard { nullptr };
} global;

//Zbog bolje citljivosti, ovde cu definisati sve 
//konstante koje su u upotrebi
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

void nacrtajTetredar(void);
void nacrtajHeksaedar(void);

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


    // Inicijalizacija globalnih stanja programa
    global.mouse = std::make_shared<rg::Mouse>();
    global.keyboard = std::make_shared<rg::Keyboard>();

    // Postavljanje pozicije kamere (gledamo u smeru -z)
    global.view_matrix = std::make_shared<glm::mat4>(glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,0,-1), glm::vec3(0,1,0)));
    global.viewport = std::make_shared<glm::vec4>(0,0,sirina_prozora, duzina_prozora);
    global.projection_matrix = std::make_shared<glm::mat4>(1.0f);
    
    // BodyManager(glm::vec3 generation_point, std::shared_ptr<glm::vec4> viewport_ptr, std::shared_ptr<glm::mat4> projection_matrix_ptr);
    global.body_manager = std::make_shared<rg::BodyManager>(global.body_translate_parameter, glm::vec3(0,0,-100), global.viewport, global.projection_matrix);
    glutTimerFunc(global.timer_interval, on_timer, global.timer_id);

    global.body_manager->add_request(std::make_shared<rg::BodyManager::GenerateBodyRequest>(std::make_shared<rg::Hexahedron>(glm::vec2(0,0))));

	//Glavna petlja
	glutMainLoop();
	return 0;
}

void on_mouse_click(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        global.mouse->press_left_click();

        if (global.keyboard->is_pressed('h') || global.keyboard->is_pressed('H')) {
            global.body_manager->add_request(std::make_shared<rg::BodyManager::RemoveBodyRequest>(rg::BodyType::HEXAHEDRON, global.mouse->position()));
        } else {
            global.body_manager->add_request(std::make_shared<rg::BodyManager::RemoveBodyRequest>(rg::BodyType::NON_PLATONIC, global.mouse->position()));
        }
    }
}

void on_mouse_move(int x, int y) {
    global.mouse->set_position({x,y});
}

void on_keyboard(unsigned char key, int x1, int y1) {
	
	switch (key){
        //Ako je pritisnut 'esc' izlazimo iz programa
        case esc:
            exit(EXIT_SUCCESS);
            break;

        default: global.keyboard->press_key(key);

        //case 't':
        //case 'T':
        //	//TODO
        //break;
        //
        //
        //case 'h':
        //case 'H':
        //	//TODO
        //break;
        //
        //case 'o':
        //case 'O':
        ////Uz klik misa uklanja oktaedar
        //	//TODO
        //break;	

        //case 'd':
        //case 'D':
        ////Uz klik misa uklanja dodekaedar
        //	//TODO
        //break;    

        //case 'i':
        //case 'I':
        ////Uz klik misa uklanja ikosaedar
        //	//TODO
        //break;
  }
}

static void on_keyboard_release(unsigned char key, int x1, int y1) { 
    global.keyboard->release_key(key);
}

void on_display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

    // Ucitamo pogled (kameru)
    glLoadMatrixf(glm::value_ptr(*global.view_matrix)); 	

    // Iscrtamo tela koje je generisao body generator
    for (const auto &body : global.body_manager->bodies()) {
        glColor3f(1,0,0);
        glMultMatrixf(body->raw_transformation_matrix());

        body->draw();
        //std::cout << body->project_to_plane(glm::vec3(0,0,0), *global.viewport, *global.projection_matrix).x << std::endl;
        //std::cout << body->world_coordinates(glm::vec3(0,0,0)).z << std::endl;
    }

	glutSwapBuffers();
}


void on_reshape(int width, int height)
{
    *global.viewport = glm::vec4(0.0f, 0.0f, width, height);
	glViewport(0.0f,0.0f,width,height);
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    *global.projection_matrix = glm::perspective(45.0f, static_cast<float>(width)/height, 0.01f, 100.0f);
    glMultMatrixf(glm::value_ptr(*global.projection_matrix));
}

static void on_timer(int value)
{
	if (value != global.timer_id || !global.game_running)
		return;

    if (auto opt_error = global.body_manager->update(); opt_error.has_value()) {
        std::cerr << opt_error.value() << std::endl;
        exit(EXIT_FAILURE);
    }

	glutPostRedisplay();
	glutTimerFunc(global.timer_interval, on_timer, global.timer_id);
}
