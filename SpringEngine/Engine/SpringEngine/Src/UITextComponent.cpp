#include "Precompiled.h"
#include "UITextComponent.h"

#include "UIRenderService.h"
#include "GameWorld.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

void UITextComponent::Initialize()
{
    UIRenderService* uiRenderService = GetOwner().GetWorld().GetService<UIRenderService>();
    uiRenderService->Register(this);
}

void UITextComponent::Terminate()
{
    UIRenderService* uiRenderService = GetOwner().GetWorld().GetService<UIRenderService>();
    uiRenderService->Unregister(this);
}

void UITextComponent::Render()
{
    UIFont::Get()->DrawString(mText.wstring().c_str(), mPosition, mSize, mColor);
}

void UITextComponent::Deserialize(const rapidjson::Value& value)
{
    if (value.HasMember("Text"))
    {
        mText = value["Text"].GetString();
    }
    if (value.HasMember("Size"))
    {
        mSize = value["Size"].GetFloat();
    }
    if (value.HasMember("Position"))
    {
        auto pos = value["Position"].GetArray();
        mPosition.x = pos[0].GetFloat();
        mPosition.y = pos[1].GetFloat();
    }
    if (value.HasMember("Color"))
    {
        auto color = value["Color"].GetArray();
        mColor.r = color[0].GetFloat();
        mColor.g = color[1].GetFloat();
        mColor.b = color[2].GetFloat();
        mColor.a = color[3].GetFloat();
    }
}