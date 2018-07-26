#include "stdafx.h"

#include "LoadObjects.h"

#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

#include "utilities\File.h"
#include "Resources.h"

#include "MeshRenderer.h"
#include "utilities\MeshProcessor.h"

namespace engine {

  static void LoadMaterials(const aiScene * _aiScene, const String & _directory, const std::shared_ptr<graphics::Material> _defaultMaterial, std::vector<std::shared_ptr<graphics::Material>> & _outMaterials)
  {
    for (uint i = 0; i < _aiScene->mNumMaterials; ++i)
    {
      const aiMaterial * aimat = _aiScene->mMaterials[i];

      std::shared_ptr<graphics::Material> mat = std::make_shared<graphics::Material>(*_defaultMaterial);
    
      aiString name;
      
      if (aimat->Get(AI_MATKEY_NAME, name) == AI_SUCCESS)
      {
        mat->setName(name.C_Str());
      }

      aiColor3D colour;
      float scalar;
      aiString path;

      std::shared_ptr<graphics::Texture2D> texture;

      if (aimat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
      {
        texture = Resources::Load<graphics::Texture2D>(_directory + path.C_Str());
      }
      else if (aimat->Get(AI_MATKEY_COLOR_DIFFUSE, colour) == AI_SUCCESS)
      {
        texture = std::make_shared<graphics::Texture2D>(64, 64, glm::vec4(colour.r, colour.g, colour.b, 1.f));
      }

      if (texture)
      {
        mat->setTexture("diffuse", texture);
      }

      texture.reset();

      if (aimat->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS)
      {
        texture = Resources::Load<graphics::Texture2D>(_directory + path.C_Str());
      }
      else if (aimat->Get(AI_MATKEY_COLOR_SPECULAR, colour) == AI_SUCCESS)
      {
        texture = std::make_shared<graphics::Texture2D>(64, 64, glm::vec4(colour.r, colour.r, colour.r, 1.f));
      }

      if (texture)
      {
        mat->setTexture("specular", texture);
      }

      texture.reset();
      //use shininess as assimp does not seem to support norm in mtl files?
      //AI_MATKEY_TEXTURE_NORMALS
      if (aimat->GetTexture(aiTextureType_SHININESS, 0, &path) == AI_SUCCESS)
      {
        texture = Resources::Load<graphics::Texture2D>(_directory + path.C_Str());
      }
      else
      {
        texture = std::make_shared<graphics::Texture2D>(64, 64, glm::vec4(.5f, .5f, 1.f, 1.f));
      }

      if (texture)
      {
        mat->setTexture("normal", texture);
      }

      texture.reset();

      if (aimat->GetTexture(aiTextureType_OPACITY, 0, &path) == AI_SUCCESS)
      {
        texture = Resources::Load<graphics::Texture2D>(_directory + path.C_Str());
      }
      else
      {
        texture = std::make_shared<graphics::Texture2D>(64, 64, glm::vec4(1.f));
      }

      if (texture)
      {
        mat->setTexture("opacity", texture);
      }

      texture.reset();

      if (aimat->GetTexture(aiTextureType_HEIGHT, 0, &path) == AI_SUCCESS)
      {
        texture = Resources::Load<graphics::Texture2D>(_directory + path.C_Str());
      }
      else
      {
        texture = std::make_shared<graphics::Texture2D>(64, 64, glm::vec4(0.f));// 0.f, 0.f, 1.f));
      }

      if (texture)
      {
        mat->setTexture("displacement", texture);
      }

      if (aimat->Get(AI_MATKEY_SHININESS, scalar) == AI_SUCCESS)
      {
        mat->setUniform<float>("shininess", scalar);
      }

      _outMaterials.push_back(mat);
    }
  }

  static std::shared_ptr<graphics::Mesh> LoadMesh()
  {
    return nullptr;
  }

  std::shared_ptr<GameObject> Load(const String & _path, std::shared_ptr<graphics::Material> _baseMaterial)
  {
    Assimp::Importer importer;

    const aiScene * scene = importer.ReadFile(_path,
      //aiProcess_CalcTangentSpace      |
      aiProcess_Triangulate           |
      aiProcess_JoinIdenticalVertices |
      aiProcess_SortByPType
    );

    if (scene == nullptr)
    {
      debug::LogError("Failed to import file " + _path + ": " + importer.GetErrorString());
      return nullptr;
    }

    String directory = file::getDirectory(_path);

    std::vector<std::shared_ptr<graphics::Material>> materials;
    LoadMaterials(scene, directory, _baseMaterial, materials);

    auto parent = GameObject::Instantiate();
    auto parentTransform = parent->getComponent<Transform>();
    
    utilities::MeshProcessor processor;

    for (uint i = 0; i < scene->mNumMeshes; ++i)
    {
      const aiMesh * aimesh = scene->mMeshes[i];

      graphics::MeshData meshData;
      meshData.positions.resize(aimesh->mNumVertices);
      meshData.uvs.resize(aimesh->mNumVertices);
      meshData.normals.resize(aimesh->mNumVertices);

      for (uint j = 0; j < aimesh->mNumVertices; ++j)
      {
        const aiVector3D pos = aimesh->mVertices[j];
        meshData.positions[j] = glm::vec3(pos.x, pos.y, pos.z);

        if (aimesh->HasTextureCoords(0))
        {
          const aiVector3D uv = aimesh->mTextureCoords[0][j];
          meshData.uvs[j] = glm::vec2(uv.x, 1.0f - uv.y);
        }

        if (aimesh->HasNormals())
        {
          const aiVector3D n = aimesh->mNormals[j];
          meshData.normals[j] = glm::vec3(n.x, n.y, n.z);
        }
      }

      for (uint j = 0; j < aimesh->mNumFaces; ++j)
      {
        const aiFace aiface = aimesh->mFaces[j];
        for (uint k = 0; k < aiface.mNumIndices; ++k)
        {
          meshData.indices.add(aiface.mIndices[k]);
        }
      }

      if (aimesh->HasTextureCoords(0) && aimesh->HasNormals())
      {
        processor.CalculateTangents(
          meshData.indices, 
          meshData.positions, meshData.uvs, meshData.normals, 
          &meshData.tangents, &meshData.bitangents
        );
      }

      auto mesh = std::make_shared<graphics::Mesh>(meshData);
      mesh->setName(aimesh->mName.C_Str());

      auto go = GameObject::Instantiate();
      go->setName(aimesh->mName.C_Str());

      auto renderer = go->AddComponent<MeshRenderer>();
      renderer->setMesh(mesh);
      if (aimesh->mMaterialIndex < materials.size())
      {
        renderer->setMaterial(materials.at(aimesh->mMaterialIndex));
      }

      go->getComponent<Transform>()->setParent(parentTransform);

      debug::Log("Complete Mesh");
    }

    return parent;
  }

} // engine