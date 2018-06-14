namespace engine {
namespace graphics {

  template <class T>
  std::shared_ptr<T> FrameBuffer::getColour(size_t _i)
  {
    static_assert(std::is_base_of<Texture, T>::value, "Object must be type of Texture");

    assert(_i < m_colourAttachments.size() && "Colour attachment out of range");

    assert(T::type == m_colourAttachments[_i]->type && "Texture not of same type");

    return std::static_pointer_cast<T>(m_colourAttachments[_i]);
  }

  template <class T>
  std::shared_ptr<T> FrameBuffer::getDepth()
  {
    static_assert(std::is_base_of<Texture, T>::value, "Object must be type of Texture");

    if (!m_depthAttachment)
    {
      return nullptr;
    }

    assert(T::type == m_depthAttachment->type && "Texture not of same type");

    return std::static_pointer_cast<T>(m_depthAttachment);
  }

  template <class T>
  std::shared_ptr<T> FrameBuffer::getStencil()
  {
    static_assert(std::is_base_of<Texture, T>::value, "Object must be type of Texture");

    if (!m_stencilAttachment)
    {
      return nullptr;
    }

    assert(T::type == m_stencilAttachment->type && "Texture not of same type");

    return std::static_pointer_cast<T>(m_stencilAttachment);
  }

} }  // engine::graphics