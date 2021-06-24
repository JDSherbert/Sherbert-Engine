#pragma once
#include "Graphics.h"
#include <string>
#include <memory>
#include <filesystem>

class Node;
class Mesh;
class ModelWindow;
struct aiMesh;
struct aiMaterial;
struct aiNode;

namespace n_SherbRendergraphpass
{
	class RenderGraph;
}

class Model
{
public:
	Model( Graphics& gfx,const std::string& pathString,float scale = 1.0f );
	void Submit( size_t channels ) const noxnd;
	void SetRootTransform( DirectX::FXMMATRIX tf ) noexcept;
	void Accept( class ModelProbe& probe );
	void LinkTechniques( n_SherbRendergraphpass::RenderGraph& );

	void Rotate(float dx, float dy) noexcept;
	void Translate(DirectX::XMFLOAT3 translation) noexcept;
	~Model() noexcept;
private:
	static std::unique_ptr<Mesh> ParseMesh( Graphics& gfx,const aiMesh& mesh,const aiMaterial* const* pMaterials,const std::filesystem::path& path,float scale );
	std::unique_ptr<Node> ParseNode( int& nextId,const aiNode& node,float scale ) noexcept;
private:
	std::unique_ptr<Node> pRoot;
	// sharing meshes here perhaps dangerous?
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
};