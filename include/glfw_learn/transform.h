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

/*! @brief �������� Transform.transform � ������� glm::lookAt. ������
 *  ����������� ��� ��������� ������� ���� ��������� �� TransformSystem. �������
 *  ��������� �: Transform, Direction � LookAtDirectionTag.
 *
 *  ������ ��� ������������ ��� ������� ����������� ������� ������� ���������
 *  �����������(��. DirectionSystem).
 */
void LookAtDirectionSystem(entt::registry &registry);

/*
��������� ����������� Direction.forward, Direction.right � Direction.up
������������ Direction.euler_angles. ������� ��������� �: Direction.
*/
void DirectionSystem(entt::registry &registry);

/*
��������� ���������������/���������������� � ������� glm::scale/glm::translate �
������� Transform.transform �������� ���� ��������� ������� ����� ���������
Direction(��. LookAtDirectionSystem). ������� ��������� �: Transform.
*/
void TransformSystem(entt::registry &registry);
}  // namespace glfw_learn

#endif  // GLFW_LEARN_INCLUDE_GLFW_LEARN_TRANSFORM_H_
