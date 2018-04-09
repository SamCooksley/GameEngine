#ifndef _ENGINE_TRANSFORM_H_
#define _ENGINE_TRANSFORM_H_

#include "glm\glm.hpp"
#include "glm\ext.hpp"

#include "Component.h"

namespace engine
{
  class Transform : public Component
  {
  public:
    static glm::mat4 getTransform(
      const glm::vec3 & _position, 
      const glm::quat & _rotation, 
      const glm::vec3 & _scale
    );

    static glm::vec3 getRight(const glm::mat4 & _transform);
    static glm::vec3 getUp(const glm::mat4 & _transform);
    static glm::vec3 getForward(const glm::mat4 & _transform);

    Transform();
    ~Transform();

    void get(
      glm::vec3 * _position, 
      glm::quat * _rotation  = nullptr, 
      glm::vec3 * _scale     = nullptr, 
      glm::mat4 * _transform = nullptr
    ) const;

    glm::vec3 getPosition() const;
    glm::quat getRotation() const;
    glm::vec3 getScale() const;

    glm::vec3 getLocalPosition() const;
    glm::quat getLocalRotation() const;
    glm::vec3 getLocalScale() const;

    glm::vec3 getRight() const;
    glm::vec3 getUp() const;
    glm::vec3 getForward() const;

    glm::vec3 TransformPoint(const glm::vec3 & _point) const;
    glm::vec3 InverseTransformPoint(const glm::vec3 & _point) const;
    
    Transform & setPosition(const glm::vec3 & _position);
    Transform & setRotation(const glm::quat & _rotation);

    Transform & setLocalPosition(const glm::vec3 & _position);
    Transform & setLocalRotation(const glm::quat & _rotation);
    Transform & setLocalScale(const glm::vec3 & _scale);

    Transform & setParent(std::shared_ptr<Transform> _parent);

  protected:
    void OnAwake() override;
    void OnDestroy() override;

    void DetachChildren(std::shared_ptr<Transform> _newParent = std::shared_ptr<Transform>());
    void DestroyChildren();

  private:
    void AddChild(std::shared_ptr<Transform> _child);
    void RemoveChild(std::shared_ptr<Transform> _child);

    glm::mat4 getLocalTransform() const;

    glm::vec3 m_localPosition;
    glm::quat m_localRotation;
    glm::vec3 m_localScale;

    std::weak_ptr<Transform> m_parent;
    std::vector<std::weak_ptr<Transform>> m_children;

    ENGINE_SETUPSHARED(Transform);
  };
}

#endif //_ENGINE_TRANSFORM_H_