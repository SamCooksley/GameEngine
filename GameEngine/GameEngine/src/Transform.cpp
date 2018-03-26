#include "stdafx.h"

#include "Transform.h"

namespace engine {

  glm::mat4 Transform::getTransform(
      const glm::vec3 & _position,
      const glm::quat & _rotation,
      const glm::vec3 & _scale
    )
  {
    return glm::translate(_position) * glm::mat4_cast(_rotation) * glm::scale(_scale);
  }

  glm::vec3 Transform::getRight(const glm::mat4 & _transform)
  {
    return glm::normalize(glm::vec3(_transform[0]));
  }

  glm::vec3 Transform::getUp(const glm::mat4 & _transform)
  {
    return glm::normalize(glm::vec3(_transform[1]));
  }

  glm::vec3 Transform::getForward(const glm::mat4 & _transform)
  {
    return glm::normalize(glm::vec3(_transform[2]));
  }

  Transform::Transform()
  { }

  Transform::~Transform()
  { }

  void Transform::get(glm::vec3 * _position, glm::quat * _rotation, glm::vec3 * _scale, glm::mat4 * _transform) const
  {
    glm::mat4 transform(1.f);
    glm::quat rotation;
    glm::vec3 scale(1.f);

    std::shared_ptr<const Transform> trs = Transform::getShared();

    while (trs)
    {
      transform = trs->getLocalTransform() * transform;

      rotation = trs->m_localRotation * rotation;
      scale = trs->m_localScale * scale;

      trs = trs->m_parent.lock();
    }

    if (_position != nullptr)
    {
      *_position = glm::vec3(transform * glm::vec4(0.f, 0.f, 0.f, 1.f));
    }

    if (_rotation != nullptr)
    {
      *_rotation = rotation;
    }

    if (_scale != nullptr)
    {
      *_scale = scale;
    }

    if (_transform != nullptr)
    {
      *_transform = transform;
    }
  }

  glm::vec3 Transform::getPosition() const
  {
    glm::vec3 pos;
    get(&pos);
    return pos;
  }

  glm::quat Transform::getRotation() const
  {
    glm::quat rot;
    get(nullptr, &rot);
    return rot;
  }

  glm::vec3 Transform::getScale() const
  {
    glm::vec3 scale;
    get(nullptr, nullptr, &scale);
    return scale;
  }

  glm::vec3 Transform::getLocalPosition() const
  {
    return m_localPosition;
  }

  glm::quat Transform::getLocalRotation() const
  {
    return m_localRotation;
  }

  glm::vec3 Transform::getLocalScale() const
  {
    return m_localScale;
  }

  glm::vec3 Transform::getRight() const
  {
    glm::vec3 pos; glm::quat rot;
    get(&pos, &rot);
    return getRight(getTransform(pos, rot, glm::vec3(1.f)));
  }

  glm::vec3 Transform::getUp() const
  {
    glm::vec3 pos; glm::quat rot;
    get(&pos, &rot);
    return getUp(getTransform(pos, rot, glm::vec3(1.f)));
  }

  glm::vec3 Transform::getForward() const
  {
    glm::vec3 pos; glm::quat rot;
    get(&pos, &rot);
    return getForward(getTransform(pos, rot, glm::vec3(1.f)));
  }

  glm::vec3 Transform::TransformPoint(const glm::vec3 & _point) const
  {
    glm::mat4 trs;
    get(nullptr, nullptr, nullptr, &trs);
    return glm::vec3(trs * glm::vec4(_point, 1.f));
  }

  glm::vec3 Transform::InverseTransformPoint(const glm::vec3 & _point) const
  {
    glm::mat4 trs;
    get(nullptr, nullptr, nullptr, &trs);
    return glm::vec3(glm::inverse(trs) * glm::vec4(_point, 1.f));
  }

  Transform & Transform::setPosition(const glm::vec3 & _position)
  {
    std::shared_ptr<Transform> parent = m_parent.lock();

    if (parent)
    {
      glm::mat4 transform(1.f);

      while (parent)
      {
        transform = parent->getLocalTransform() * transform;

        parent = parent->m_parent.lock();
      }

      m_localPosition = glm::vec3(glm::inverse(transform) * glm::vec4(_position, 1.f));
    }
    else
    {
      m_localPosition = _position;
    }
    
    return *this;
  }

  Transform & Transform::setRotation(const glm::quat & _rotation)
  {
    std::shared_ptr<Transform> parent = m_parent.lock();

    if (parent)
    {
      glm::quat rotation;

      while (parent)
      {
        rotation = glm::inverse(parent->m_localRotation) * rotation;

        parent = parent->m_parent.lock();
      }

      m_localRotation = rotation * _rotation;
    }
    else
    {
      m_localRotation = _rotation;
    }

    return *this;
  }

  Transform & Transform::setLocalPosition(const glm::vec3 & _position)
  {
    m_localPosition = _position;
    return *this;
  }

  Transform & Transform::setLocalRotation(const glm::quat & _rotation)
  {
    m_localRotation = _rotation;
    return *this;
  }

  Transform & Transform::setLocalScale(const glm::vec3 & _scale)
  {
    m_localScale = _scale;
    return *this;
  }

  void Transform::OnAwake()
  {
    Component::OnAwake();

    m_localPosition = glm::vec3(0.f);
    m_localRotation = glm::quat();
    m_localScale = glm::vec3(1.f);

    m_parent.reset();
    m_children.clear();
  }

  void Transform::OnDestroy()
  {
    Component::OnDestroy();
    setParent(nullptr);
    DestroyChildren();
  }

  void Transform::DetachChildren(std::shared_ptr<Transform> _newParent)
  {
    //if it is being added to the same parent, nothing changes.
    if (_newParent.get() == this) { return; }

    while (!m_children.empty())
    {
      std::shared_ptr<Transform> child = m_children.back().lock();
      child->setParent(_newParent);
    }
  }

  void Transform::DestroyChildren()
  {
    while (!m_children.empty())
    {
      std::shared_ptr<Transform> child = m_children.back().lock();
      child->setParent(nullptr);
      child->Destroy();
    }
  }

  Transform & Transform::setParent(std::shared_ptr<Transform> _parent)
  {
    std::shared_ptr<Transform> parent = m_parent.lock();
    if (parent)
    {
      parent->RemoveChild(Transform::getShared());
    }

    m_parent = _parent;

    if (_parent)
    {
      _parent->AddChild(Transform::getShared());
    }

    return *this;
  }

  void Transform::AddChild(std::shared_ptr<Transform> _child)
  {
    //exit if the child already exits.
    for (size_t i = 0; i < m_children.size(); ++i)
    {
      if (m_children[i].lock() == _child)
      {
        return;
      }
    }

    m_children.push_back(_child);
  }

  void Transform::RemoveChild(std::shared_ptr<Transform> _child)
  {
    for (size_t i = 0; i < m_children.size();)
    {
      if (m_children[i].lock() == _child)
      {
        m_children.erase(std::begin(m_children) + i);
      }
      else { ++i; }
    }
  }

  glm::mat4 Transform::getLocalTransform() const
  {
    return getTransform(m_localPosition, m_localRotation, m_localScale);
  }

}