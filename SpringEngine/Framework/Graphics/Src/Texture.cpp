#include "Precompiled.h"
#include "Texture.h"

#include "GraphicsSystem.h"
#include <DirectXTK/Inc/WICTextureLoader.h>

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

void Texture::UnbindPS(uint32_t slot)
{
	static ID3D11ShaderResourceView* dummy = nullptr;
	GraphicsSystem::Get()->GetContext()->PSSetShaderResources(slot, 1, &dummy);
}

Texture::~Texture()
{
	ASSERT(mShaderResourceView == nullptr, "Texture: terminate must be called before destruction");
}

Texture::Texture(Texture&& rhs) noexcept
	: mShaderResourceView(rhs.mShaderResourceView)
{
	rhs.mShaderResourceView = nullptr;
}

Texture& Texture::operator=(Texture&& rhs) noexcept
{
	mShaderResourceView = rhs.mShaderResourceView;
	rhs.mShaderResourceView = nullptr;
	return *this;
}

void Texture::Initialize(const std::filesystem::path& fileName)
{
	auto device = GraphicsSystem::Get()->GetDevice();
	auto context = GraphicsSystem::Get()->GetContext();
	HRESULT hr = DirectX::CreateWICTextureFromFile(device, context, fileName.c_str(), nullptr, &mShaderResourceView);
	ASSERT(SUCCEEDED(hr), "Texture: failed to create texture %ls.", fileName.c_str());

	ID3D11Resource* resource = nullptr;
	mShaderResourceView->GetResource(&resource);
	ID3D11Texture2D* texture2d = nullptr;
	hr = resource->QueryInterface(&texture2d);
	ASSERT(SUCCEEDED(hr), "Texture -- failed to find texture data");
	D3D11_TEXTURE2D_DESC desc;
	texture2d->GetDesc(&desc);
	mWidth = static_cast<uint32_t>(desc.Width);
	mHeight = static_cast<uint32_t>(desc.Height);
	SafeRelease(texture2d);
	SafeRelease(resource);
}

void Texture::Initialize(uint32_t width, uint32_t height, Format format)
{
	ASSERT(false, "Texture: not yet implemented");
}

void Texture::Terminate()
{
	SafeRelease(mShaderResourceView);
}

void Texture::BindVS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->VSSetShaderResources(slot, 1, &mShaderResourceView);
}

void Texture::BindPS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetShaderResources(slot, 1, &mShaderResourceView);
}

DXGI_FORMAT Texture::GetDXGIFormat(Format format)
{
	switch (format)
	{
	case Format::RGBA_U8: return DXGI_FORMAT_R8G8B8A8_UNORM;
	case Format::RGBA_U32: return DXGI_FORMAT_R32G32B32A32_UINT;
	default:
		break;
	}
	return DXGI_FORMAT_R8G8B8A8_UNORM;
}
