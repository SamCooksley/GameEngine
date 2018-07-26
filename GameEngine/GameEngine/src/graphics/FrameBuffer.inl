namespace engine {
namespace graphics {

  template <class T>
  std::shared_ptr<T> FrameBuffer::getColourAttachment(int _i)
  {
    static_assert(std::is_base_of<Texture, T>::value, "Object must be type of texture");

    auto tex = getColourAttachment<Texture>(_i);

    if (!tex)
    {
      return nullptr;
    }

    if (T::type != tex->getType())
    {
      throw std::logic_error("Texture not of type");
    }

    return std::static_pointer_cast<T>(tex);
  }

  template <>
  std::shared_ptr<Texture> FrameBuffer::getColourAttachment(int _i)
  {
    auto tex = m_colourAttachments.find(_i);
    if (tex == m_colourAttachments.end())
    {
      return nullptr;
    }
    
    return tex->second;
  }

  template <class T>
  std::shared_ptr<T> FrameBuffer::getDepthAttachment()
  {
    static_assert(std::is_base_of<Texture, T>::value, "Object must be type of texture");

    if (!m_depthAttachment)
    {
      return nullptr;
    }

    if (T::type != m_depthAttachment->getType())
    {
      throw std::logic_error("Texture not of type");
    }

    return std::static_pointer_cast<T>(m_depthAttachment);
  }

  template <>
  std::shared_ptr<Texture> FrameBuffer::getDepthAttachment()
  {
    return m_depthAttachment;
  }

  template <class T>
  std::shared_ptr<T> FrameBuffer::getStencilAttachment()
  {
    static_assert(std::is_base_of<Texture, T>::value, "Object must be type of texture");

    if (!m_stencilAttachment)
    {
      return nullptr;
    }
    
    if (T::type != m_stencilAttachment->getType())
    {
      throw std::logic_error("Texture not of type");
    }

    return std::static_pointer_cast<T>(m_stencilAttachment);
  }

  template <>
  std::shared_ptr<Texture> FrameBuffer::getStencilAttachment()
  {
    return m_stencilAttachment;
  }

} } // engine::graphics