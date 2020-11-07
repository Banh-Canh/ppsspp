#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>  // for size_t

// GLSL_1xx and GLSL_3xx each cover a lot of sub variants. All the little quirks
// that differ are covered in ShaderLanguageDesc.
// Defined as a bitmask so stuff like GetSupportedShaderLanguages can return combinations.
enum ShaderLanguage {
	GLSL_1xx = 1,
	GLSL_3xx = 2,
	GLSL_VULKAN = 4,
	HLSL_D3D9 = 8,
	HLSL_D3D11 = 16,
};

inline bool ShaderLanguageIsOpenGL(ShaderLanguage lang) {
	return lang == GLSL_1xx || lang == GLSL_3xx;
}

enum class ShaderStage {
	Vertex,
	Fragment,
	Geometry,
	Compute,
};

struct ShaderLanguageDesc {
	ShaderLanguageDesc() {}
	explicit ShaderLanguageDesc(ShaderLanguage lang);

	void Init(ShaderLanguage lang);

	int glslVersionNumber = 0;
	ShaderLanguage shaderLanguage;
	bool gles = false;
	const char *varying_fs = nullptr;
	const char *varying_vs = nullptr;
	const char *attribute = nullptr;
	const char *fragColor0 = nullptr;
	const char *fragColor1 = nullptr;
	const char *texture = nullptr;
	const char *texelFetch = nullptr;
	const char *lastFragData = nullptr;
	const char *framebufferFetchExtension = nullptr;
	const char *vsOutPrefix = "";
	const char *viewportYSign = "";
	bool glslES30 = false;  // really glslES30Features. TODO: Clean this up.
	bool bitwiseOps = false;
	bool forceMatrix4x4 = false;
	bool coefsFromBuffers = false;
};

enum class UniformType : int8_t {
	FLOAT1,
	FLOAT2,
	FLOAT3,
	FLOAT4,
	MATRIX4X4,
};

// Describe uniforms intricately enough that we can support them on all backends.
// This will generate a uniform struct on the newer backends and individual uniforms on the older ones.
struct UniformDesc {
	const char *name;  // For GL
	int16_t vertexReg;        // For D3D
	int16_t fragmentReg;      // For D3D
	UniformType type;
	int16_t offset;
	// TODO: Support array elements etc.
};

struct UniformBufferDesc {
	size_t uniformBufferSize;
	std::vector<UniformDesc> uniforms;
};

// For passing error messages from shader compilation (and other critical issues) back to the host.
// This can run on any thread - be aware!
// TODO: See if we can find a less generic name for this.
typedef void (*ErrorCallbackFn)(const char *shortDesc, const char *details, void *userdata);
