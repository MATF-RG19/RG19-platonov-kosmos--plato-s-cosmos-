#ifndef CLASSES_H
#define CLASSES_H

#include <vector>
#include <functional>
#include <memory>
#include <random>
#include <iostream>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "glm/glm.hpp"
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

namespace rg {

enum class BodyType { NON_PLATONIC = 0, TETRAHEDRON = 1, HEXAHEDRON = 2, OCTAHEDRON = 3, DODECAHEDRON = 4, ICOSAHEDRON = 5 };

using error_t = std::string;

class Keyboard {
private:
    std::unordered_map<char, bool> m_presed_keys_map;
public:
    void press_key(char key);
    void release_key(char key);
    bool is_pressed(char key) const;
};

void Keyboard::press_key(char key) {
    m_presed_keys_map[key] = true;
}

void Keyboard::release_key(char key) {
    m_presed_keys_map[key] = false;
}

bool Keyboard::is_pressed(char key) const {
    auto it = m_presed_keys_map.find(key);
    if (it != m_presed_keys_map.end())
        return it->second;
    else 
        return false;
}

class Mouse {
private:
    bool m_left_click_active = false;
    glm::vec2 m_mouse_position;
public:
    void press_left_click();
    void release_left_click();
    bool is_left_click_pressed();
    const glm::vec2& position() const;
    void set_position(glm::vec2 new_position);
};

const glm::vec2& Mouse::position() const {
    return m_mouse_position;
}

void Mouse::set_position(glm::vec2 new_position) {
    m_mouse_position = new_position;
}


void Mouse::press_left_click() {
    m_left_click_active = true;
}

void Mouse::release_left_click() {
    m_left_click_active = false;
}

bool Mouse::is_left_click_pressed() {
    return m_left_click_active;
}


class Drawable {
public:
    // c++17 podrzava inline inicijalizaciju statickih promenljivi
    // u suprotnom sve staticke promenljive se moraju inicijalizovati
    // u nekom od .cpp fajlova
    
    // Ukoliko imamo virtuelni metod,
    // destruktor mora biti virtuelan
    virtual ~Drawable() = default;
	void draw() const;
private:
    virtual void do_draw() const = 0;
protected:
    // Napravimo operator dodele protected jer se nece
    // koristiti nikada u kontekstu bazne klase
    Drawable &operator=(Drawable const&) = default;
};

void Drawable::draw() const {
	/* opt pre-work put here */ 
    glPushMatrix();
        do_draw(); 
    glPopMatrix();
	/* opt post-work put here */ 
}

class Transformation {
public:
    virtual ~Transformation() = default;
    glm::mat4 transform(glm::mat4 const& matrix) const;
private:
    virtual glm::mat4 do_transform(glm::mat4 const& matrix) const = 0;
};

glm::mat4 Transformation::transform(glm::mat4 const& matrix) const {
    /* Pre work */
    auto result = do_transform(matrix);
    /* post work */
    return result;
}

namespace transformation {
    class SetPosition final : public Transformation {
    public:
        SetPosition(glm::vec3 new_center_wcs = glm::vec3(0,0,0));

    private:
        glm::vec3 m_new_center;
        virtual glm::mat4 do_transform(glm::mat4 const &m) const override;
    };

    SetPosition::SetPosition(glm::vec3 new_center_wcs) : m_new_center(new_center_wcs) {}

    glm::mat4 SetPosition::do_transform(glm::mat4 const& m) const {
        return glm::translate(glm::mat4(1.0f), m_new_center);
    }
}

class Transformator : public Transformation {
public:
    Transformator(std::function<glm::mat4(glm::mat4 const &)> const& transf_func);
    Transformator(std::function<glm::mat4(glm::mat4 const &)> &&transf_func);
             
private:
    std::function<glm::mat4(glm::mat4 const &m)> m_transf_func;
    virtual glm::mat4 do_transform(glm::mat4 const &m) const override;
};

Transformator::Transformator(std::function<glm::mat4(glm::mat4 const &)> const& transf_func) : m_transf_func(transf_func) {
}

Transformator::Transformator(std::function<glm::mat4(glm::mat4 const &)> &&transf_func) : m_transf_func(std::move(transf_func)) {
}

glm::mat4 Transformator::do_transform(glm::mat4 const &m) const {
    return m_transf_func(m);
}

class TransformatorMatrix : public Transformation {
public:
    TransformatorMatrix(glm::mat4 const &m);
    TransformatorMatrix(glm::mat4 &&m);

private:
    glm::mat4 m_matrix;
    virtual glm::mat4 do_transform(glm::mat4 const& transformation_matrix) const override;
};

TransformatorMatrix::TransformatorMatrix(glm::mat4 const &m) : m_matrix(m) { }
TransformatorMatrix::TransformatorMatrix(glm::mat4 &&m) : m_matrix(std::move(m)) { }

glm::mat4 TransformatorMatrix::do_transform(glm::mat4 const& transformation_matrix) const {
    return transformation_matrix * m_matrix;
}

class Transformable {
protected:
	glm::mat4 m_transformation_matrix;
public:
    Transformable(glm::mat4 initial_value = glm::mat4(1.0f));

    // TODO - Da li ovo 2 funkcije ispod pripadaju ovde
    glm::vec3 world_coordinates(const glm::vec3 &point_in_ucs) const;
    glm::vec2 project_to_plane(const glm::vec3 &point_in_ucs, const glm::vec4 &viewport, const glm::mat4& projection_matrix) const;

    void transform(Transformation const& transformation);
    void transform(std::function<glm::mat4(glm::mat4 const&)>);
    void transform(glm::mat4 const& transformation_matrix);

    const glm::mat4& transformation_matrix() const;
    const float * raw_transformation_matrix() const;

    void set_transformation_matrix(glm::mat4 const &to);
    void set_transformation_matrix(glm::mat4 && to);
};

Transformable::Transformable(glm::mat4 initial_value) : m_transformation_matrix(std::move(initial_value)) { }

void Transformable::transform(Transformation const& transformation) {
    m_transformation_matrix = transformation.transform(m_transformation_matrix);
}

void Transformable::transform(std::function<glm::mat4(glm::mat4 const&)> f) {
    m_transformation_matrix = f(m_transformation_matrix);
}

void Transformable::transform(glm::mat4 const& transformation_matrix) {
    m_transformation_matrix *= transformation_matrix;
}

const glm::mat4& Transformable::transformation_matrix() const {
    return m_transformation_matrix;
}

void Transformable::set_transformation_matrix(glm::mat4 const &to) {
    m_transformation_matrix = to;
}

void Transformable::set_transformation_matrix(glm::mat4 && to) {
    m_transformation_matrix = std::move(to);
}

const float * Transformable::raw_transformation_matrix() const {
    return static_cast<const float*>(glm::value_ptr(m_transformation_matrix));
}

glm::vec3 Transformable::world_coordinates(const glm::vec3 &point_in_ucs) const {
    glm::vec4 point_in_ucs_vec4(point_in_ucs.x, point_in_ucs.y, point_in_ucs.z, 1.0f);
	glm::vec4 result_vec4 = m_transformation_matrix*point_in_ucs_vec4;
	float w = result_vec4.w;

	return glm::vec3(result_vec4.x/w, result_vec4.y/w, result_vec4.z/w);
}


glm::vec2 Transformable::project_to_plane(const glm::vec3 &ucs_point, const glm::vec4 &viewport, const glm::mat4& projection_matrix) const {
    return glm::project(ucs_point, m_transformation_matrix, projection_matrix, viewport);
}

class Body : public Drawable, 
             public Transformable

{
public:
   Body(BodyType body_type, glm::vec2 body_location_on_screen);
   BodyType type() const;
private:
   BodyType m_body_type;
   glm::vec2 m_body_location_on_screen;
   virtual void do_draw() const = 0;
};

Body::Body(BodyType body_type, glm::vec2 body_location_on_screen) : m_body_type(body_type), m_body_location_on_screen(body_location_on_screen) {}

BodyType Body::type() const {
    return m_body_type;
}

class Hexahedron : public Body {
public:
    Hexahedron(glm::vec2 body_location_on_screen);
    void do_draw() const override;
};

Hexahedron::Hexahedron(glm::vec2 body_location_on_screen) : Body(BodyType::HEXAHEDRON, body_location_on_screen) {}
void Hexahedron::do_draw() const {
    glutSolidCube(1.0f);
}


class BodyManager {

public:
    BodyManager(float translate_parameter, glm::vec3 generation_point, std::shared_ptr<glm::vec4> viewport_ptr, std::shared_ptr<glm::mat4> projection_matrix_ptr);
    class Request {
    public:
         std::optional<error_t> process_request(BodyManager &bm) const;
    private:
         virtual std::optional<error_t> do_process_request(BodyManager &bm) const = 0;
    };

    class GenerateBodyRequest : public Request {
    public:
        GenerateBodyRequest(std::shared_ptr<Body> body);
    private:
        std::shared_ptr<Body> m_body;
        virtual std::optional<error_t> do_process_request(BodyManager &bm) const override;
    };

    class RemoveBodyRequest : public Request {
    public:
        RemoveBodyRequest(BodyType type, glm::vec2 pointer_position);
    private:
        BodyType m_body_type;
        glm::vec2 m_pointer_position;

       virtual std::optional<error_t> do_process_request(BodyManager &bm) const override;
    };

std::optional<error_t> update();
std::vector<std::shared_ptr<Body>> const& bodies() const;
void add_request(std::shared_ptr<Request> request);
     
private:
    float m_translate_parameter = 0;
    glm::vec3 m_generation_point = glm::vec3(0,0,0);
    std::shared_ptr<glm::vec4> m_viewport_ptr = nullptr;
    std::shared_ptr<glm::mat4> m_projection_matrix_ptr = nullptr;
 
	std::optional<error_t> process_requests();
	std::optional<error_t> destroy_bodies_behind_screen();
	void translate_bodies_closer();

    std::vector <std::shared_ptr<Body>> m_bodies;
    std::vector<std::shared_ptr<Request>> m_requests;
};

BodyManager::BodyManager(float translate_parameter, glm::vec3 generation_point, std::shared_ptr<glm::vec4> viewport_ptr, std::shared_ptr<glm::mat4> projection_matrix_ptr)
    : m_translate_parameter(translate_parameter), m_generation_point(generation_point), m_viewport_ptr(viewport_ptr), m_projection_matrix_ptr(projection_matrix_ptr) { }

std::optional<error_t> BodyManager::update() {
    this->translate_bodies_closer(); 
    if (auto err = this->process_requests(); err.has_value()) return err;
    if (auto err = this->destroy_bodies_behind_screen(); err.has_value()) return err;
    return std::nullopt;
}

std::vector<std::shared_ptr<Body>> const& BodyManager::bodies() const {
    return m_bodies;
}

void BodyManager::add_request(std::shared_ptr<Request> request) {
    m_requests.push_back(request);
}

std::optional<error_t> BodyManager::process_requests() {
    /* obradimo sve request-ove */
    for (const auto &r : m_requests) {
        // ako je doslo do greske pri request-u vrati gresku
        if (auto opt_err = r->process_request(*this); opt_err.has_value()) {
            m_requests.clear(); // oslobadjamo vector
            return opt_err;
        }
    }

    m_requests.clear(); // oslobadjamo vector
    return std::nullopt;
}

std::optional<error_t> BodyManager::destroy_bodies_behind_screen() { 

    // nadji platonovo telo iza ekrana ako postoji
    auto iter = std::find_if(m_bodies.begin(), m_bodies.end(),
            [](const std::shared_ptr<Body>& bptr) {
                auto body_center = bptr->world_coordinates(glm::vec3(0,0,0));
                if (body_center.z >= 0.0f)
                    return bptr->type() != BodyType::NON_PLATONIC;
                else
                    return false;
            });

    // ako pronadjeno platonovo telo koje je proslo iza ekrana vrati gresku
   if (iter != m_bodies.end())
       return "Platonovo telo je proslo izvan ekrana";

   // ako nema platonovih tela iza ekrana, obrisi sva obicna tela izvan ekrana
    m_bodies.erase(std::remove_if(m_bodies.begin(), m_bodies.end(), 
            [](const std::shared_ptr<Body>& bptr) {
                auto body_center = bptr->world_coordinates(glm::vec3(0,0,0));
                if (body_center.z >= 0.0f)
                    return true;
                return false;
            }), m_bodies.end());

    return std::nullopt;
}

void BodyManager::translate_bodies_closer() {
    for (const auto &b_ptr : m_bodies) {
        b_ptr->transform(glm::translate(glm::vec3(0,0,m_translate_parameter)));
    }
}

std::optional<error_t> BodyManager::Request::process_request(BodyManager &bm) const {
    return do_process_request(bm);
}

BodyManager::RemoveBodyRequest::RemoveBodyRequest(BodyType type, glm::vec2 pointer_position) 
    : m_body_type(type), m_pointer_position(pointer_position) { }


std::optional<error_t> BodyManager::RemoveBodyRequest::do_process_request(BodyManager &bm) const {
    // TODO REIMPLEMENTIRATI
    // ZA SADA RADI SA RASTOJANJEM OD CENTRA MANJIM OD 100px
    auto iter = std::find_if(bm.m_bodies.begin(), bm.m_bodies.end(),
        [&](const std::shared_ptr<Body>& bptr) {
            auto body_center = bptr->project_to_plane(glm::vec3(0,0,0), *bm.m_viewport_ptr, *bm.m_projection_matrix_ptr);
            if (glm::distance(body_center, m_pointer_position) <= 100)
                return m_body_type == BodyType::NON_PLATONIC;
            else
                return false;
        });

    if (iter != bm.m_bodies.end())
       return "Pogresno ste kliknuli na objekat";

    // obrisi tela na koja se klik odnosi
    bm.m_bodies.erase(std::remove_if(bm.m_bodies.begin(), bm.m_bodies.end(),
            [&](const std::shared_ptr<Body>& bptr) {
                auto body_center = bptr->project_to_plane(glm::vec3(0,0,0), *bm.m_viewport_ptr, *bm.m_projection_matrix_ptr);
                if (glm::distance(body_center, m_pointer_position) <= 100)
                    return true;
                else 
                    return false;
            }), bm.m_bodies.end());

     return std::nullopt;
}

BodyManager::GenerateBodyRequest::GenerateBodyRequest(std::shared_ptr<Body> body) 
    : m_body(body) { }


std::optional<error_t> BodyManager::GenerateBodyRequest::do_process_request(BodyManager &bm) const {
    m_body->transform(glm::translate(bm.m_generation_point));
    bm.m_bodies.push_back(m_body);
    return std::nullopt;
}

} // namespace rg

#endif // CLASSES_H
