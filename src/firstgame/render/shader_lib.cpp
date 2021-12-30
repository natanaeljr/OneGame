#include <firstgame/opengl/shader.h>
#include "shader_lib.h"

#include "firstgame/system/log.h"
#include "firstgame/system/asset_mgr.h"
#include "firstgame/util/filesystem_literals.h"
#include "firstgame/util/scoped.h"

namespace firstgame::render {

using util::filesystem_literals::operator""_path;

opengl::GLShader& ShaderLibrary::get(MyShader my_shader)
{
    auto idx = static_cast<size_t>(my_shader);
    if (shaders_[idx].exists())
        return shaders_[idx].get();

    ASSERT_MSG("MyShader index {} not loaded!", idx);
    throw std::runtime_error("MyShader not loaded");
}

template<>
void ShaderLibrary::load<MyShader::SIMPLE>()
{
    auto& asset_mgr = system::AssetManager::current();
    auto vert = asset_mgr.Open("shaders"_path / "main.vert").Assert()->ReadToString();
    auto frag = asset_mgr.Open("shaders"_path / "main.frag").Assert()->ReadToString();
    auto shader = opengl::GLShader::build("simple", { vert, frag }).Assert();
    shader->load_attr_loc({
        { opengl::GLAttr::POSITION, "aPosition" },
        { opengl::GLAttr::COLOR, "aColor" },
    });
    shader->load_unif_loc({
        { opengl::GLUnif::MODEL, "uModel" },
        { opengl::GLUnif::VIEW, "uView" },
        { opengl::GLUnif::PROJECTION, "uProjection" },
    });
    shaders_[static_cast<size_t>(MyShader::SIMPLE)] = std::move(shader);
}

template<>
void ShaderLibrary::load<MyShader::SIMPLE_INSTANCE>()
{
    auto& asset_mgr = system::AssetManager::current();
    auto vert = asset_mgr.Open("shaders"_path / "instance.vert").Assert()->ReadToString();
    auto frag = asset_mgr.Open("shaders"_path / "main.frag").Assert()->ReadToString();
    auto shader = opengl::GLShader::build("instance", { vert, frag }).Assert();
    shader->load_attr_loc({
        { opengl::GLAttr::POSITION, "aPosition" },
        { opengl::GLAttr::COLOR, "aColor" },
        { opengl::GLAttr::MODEL, "aModel" },
    });
    shader->load_unif_loc({
        { opengl::GLUnif::VIEW, "uView" },
        { opengl::GLUnif::PROJECTION, "uProjection" },
    });
    shaders_[static_cast<size_t>(MyShader::SIMPLE_INSTANCE)] = std::move(shader);
}

void ShaderLibrary::unload(MyShader my_shader)
{
    shaders_[static_cast<size_t>(my_shader)] = {};
}

}  // namespace firstgame::render