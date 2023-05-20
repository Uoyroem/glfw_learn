//
// Created by uoyro on 5/16/23.
//

#ifndef GLFW_LEARN_INCLUDE_GLFW_LEARN_TRANSFORM_H_
#define GLFW_LEARN_INCLUDE_GLFW_LEARN_TRANSFORM_H_

#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace glfw_learn {
struct Transform {
  glm::vec3 position{};
  glm::vec3 scale{};
  glm::mat4 transform{1.0f};
};

struct Direction {
  glm::vec3 euler_angles{-90.0f, 0, 0};

  glm::vec3 forward{};
  glm::vec3 right{};
  glm::vec3 up{};
};

struct LookAtDirectionTag {};

/*! @brief Изменяет Transform.transform с помощью glm::lookAt. Обычно
 *  применяется для сущностей которые были исключены из TransformSystem. Требует
 *  сущностей с: Transform, Direction и LookAtDirectionTag.
 *
 *  Прежде чем использовать эту систему используйте систему которая вычесляет
 *  направлений(см. DirectionSystem).
 */
void LookAtDirectionSystem(entt::registry &registry);

/*
Вычесляет направление Direction.forward, Direction.right и Direction.up
относительно Direction.euler_angles. Требует сущностей с: Direction.
*/
void DirectionSystem(entt::registry &registry);

/*
Применяет масштабирование/позиционирование с помощью glm::scale/glm::translate к
матрице Transform.transform исключая всех сущностой которые имеют компонент
Direction(см. LookAtDirectionSystem). Требует сущностей с: Transform.
*/
void TransformSystem(entt::registry &registry);
}  // namespace glfw_learn

#endif  // GLFW_LEARN_INCLUDE_GLFW_LEARN_TRANSFORM_H_
