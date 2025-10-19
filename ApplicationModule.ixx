module;
#include <webgpu/webgpu.hpp>
#include <webgpu/webgpu.h>

#include "sdl3webgpu.h"
#include "SDL3/SDL_video.h"

export module ApplicationModule;


const char* shaderSource = R"(
@vertex
fn vs_main(@builtin(vertex_index) in_vertex_index: u32) -> @builtin(position) vec4f {
	var p = vec2f(0.0, 0.0);
	if (in_vertex_index == 0u) {
		p = vec2f(-0.5, -0.5);
	} else if (in_vertex_index == 1u) {
		p = vec2f(0.5, -0.5);
	} else {
		p = vec2f(0.0, 0.5);
	}
	return vec4f(p, 0.0, 1.0);
}

@fragment
fn fs_main() -> @location(0) vec4f {
	return vec4f(0.0, 0.4, 1.0, 1.0);
}
)";


export class Application
{
public:
	// Initialize everything and return true if it went all right
	void Initialize()
	{
		window = SDL_CreateWindow("Great UI", 640, 480, 0);
		wgpu::Instance instance = wgpu::createInstance();

		wgpu::Surface surface = SDL_GetWGPUSurface(instance, window);

		wgpu::RequestAdapterOptions adapterOpts{};
		adapterOpts.compatibleSurface = surface;

		wgpu::Adapter adapter = instance.requestAdapter(adapterOpts);

		instance.release();

		wgpu::DeviceDescriptor deviceDesc{};
		deviceDesc.label = wgpu::StringView{"Device"};
		deviceDesc.requiredFeatureCount = 0;
		deviceDesc.requiredLimits = nullptr;
		deviceDesc.defaultQueue.nextInChain = nullptr;
		deviceDesc.defaultQueue.label = wgpu::StringView{"Device"};
		deviceDesc.uncapturedErrorCallbackInfo.callback = [](struct WGPUDeviceImpl* const* device, WGPUErrorType error,
		                                                     WGPUStringView message, void*, void*)
		{
			std::cout << "Uncaptured device error: type " << error << " (" << wgpu::StringView(message) << ")" <<
				std::endl;
		};
		deviceDesc.deviceLostCallbackInfo.callback = [](WGPUDevice const* device, WGPUDeviceLostReason reason,
		                                                WGPUStringView message, void*, void* /* pUserData */)
		{
			std::cout << "Device lost: " << wgpu::StringView(message);
		};
		device = adapter.requestDevice(deviceDesc);
		std::cout << "Got device: " << device << std::endl;

		queue = device.getQueue();

		wgpu::SurfaceCapabilities capabilities;

		surface.getCapabilities(adapter, &capabilities);

		wgpu::SurfaceConfiguration config{};
		surface.configure(WGPUSurfaceConfiguration{
			.device = device,
			.format = capabilities.formats[0],
			.usage = wgpu::TextureUsage::RenderAttachment,
			.width = 640,
			.height = 480,
			.viewFormatCount = 0,
			.viewFormats = nullptr,
			.alphaMode = wgpu::CompositeAlphaMode::Auto,
			.presentMode = wgpu::PresentMode::Fifo,
		});

		adapter.release();
	}

	// Uninitialize everything that was initialized
	void Terminate()
	{
	}

	// Draw a frame and handle events
	void MainLoop()
	{
		SDL_Event event;
		SDL_PollEvent(&event);
		if (event.type == SDL_EVENT_QUIT)
		{
			m_isRunning = false;
		}

		wgpu::TextureView targetView = GetNextSurfaceTextureView();
		if (!targetView) return;

		// Create a command encoder for the draw call
		wgpu::CommandEncoderDescriptor encoderDesc = {};
		encoderDesc.label = {"My command encoder"};
		wgpu::CommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, &encoderDesc);

		// Create the render pass that clears the screen with our color
		wgpu::RenderPassDescriptor renderPassDesc = {};

		// The attachment part of the render pass descriptor describes the target texture of the pass
		wgpu::RenderPassColorAttachment renderPassColorAttachment = {};
		renderPassColorAttachment.view = targetView;
		renderPassColorAttachment.resolveTarget = nullptr;
		renderPassColorAttachment.loadOp = wgpu::LoadOp::Clear;
		renderPassColorAttachment.storeOp = wgpu::StoreOp::Store;
		renderPassColorAttachment.clearValue = WGPUColor{0.9, 0.1, 0.2, 1.0};
#ifndef WEBGPU_BACKEND_WGPU
		renderPassColorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
#endif // NOT WEBGPU_BACKEND_WGPU

		renderPassDesc.colorAttachmentCount = 1;
		renderPassDesc.colorAttachments = &renderPassColorAttachment;
		renderPassDesc.depthStencilAttachment = nullptr;
		renderPassDesc.timestampWrites = nullptr;

		wgpu::RenderPassEncoder renderPass = encoder.beginRenderPass(renderPassDesc);

		// Select which render pipeline to use
		renderPass.setPipeline(pipeline);
		// Draw 1 instance of a 3-vertices shape
		renderPass.draw(3, 1, 0, 0);

		renderPass.end();
		renderPass.release();

		// Finally encode and submit the render pass
		wgpu::CommandBufferDescriptor cmdBufferDescriptor = {};
		cmdBufferDescriptor.label = {"Command buffer"};
		wgpu::CommandBuffer command = encoder.finish(cmdBufferDescriptor);
		encoder.release();

		std::cout << "Submitting command..." << std::endl;
		queue.submit(1, &command);
		command.release();
		std::cout << "Command submitted." << std::endl;

		// At the enc of the frame
		targetView.release();
#ifndef __EMSCRIPTEN__
		surface.present();
#endif

#if defined(WEBGPU_BACKEND_DAWN)
		device.tick();
#elif defined(WEBGPU_BACKEND_WGPU)
		device.poll(false, nullptr);
#endif
	}

	// Return true as long as the main loop should keep on running
	bool IsRunning()
	{
		return m_isRunning;
	}

private:
	wgpu::TextureView GetNextSurfaceTextureView()
	{
		// Get the surface texture
		wgpu::SurfaceTexture surfaceTexture;
		surface.getCurrentTexture(&surfaceTexture);
		if (surfaceTexture.status != wgpu::SurfaceGetCurrentTextureStatus::SuccessOptimal)
		{
			return nullptr;
		}
		wgpu::Texture texture = surfaceTexture.texture;

		// Create a view for this surface texture
		wgpu::TextureViewDescriptor viewDescriptor;
		viewDescriptor.label = {"Surface texture view"};
		viewDescriptor.format = texture.getFormat();
		viewDescriptor.dimension = wgpu::TextureViewDimension::_2D;
		viewDescriptor.baseMipLevel = 0;
		viewDescriptor.mipLevelCount = 1;
		viewDescriptor.baseArrayLayer = 0;
		viewDescriptor.arrayLayerCount = 1;
		viewDescriptor.aspect = wgpu::TextureAspect::All;
		wgpu::TextureView targetView = texture.createView(viewDescriptor);

#ifndef WEBGPU_BACKEND_WGPU
		// We no longer need the texture, only its view
		// (NB: with wgpu-native, surface textures must not be manually released)
		wgpu::Texture(surfaceTexture.texture).release();
#endif // WEBGPU_BACKEND_WGPU

		return targetView;
	}

	// Substep of Initialize() that creates the render pipeline
	void InitializePipeline()
	{
		wgpu::ShaderModuleDescriptor shaderDesc;

		// We use the extension mechanism to specify the WGSL part of the shader module descriptor
		wgpu::ShaderSourceWGSL shaderCodeDesc;
		// Set the chained struct's header
		shaderCodeDesc.chain.next = nullptr;
		shaderCodeDesc.chain.sType = wgpu::SType::ShaderSourceWGSL;
		// Connect the chain
		shaderDesc.nextInChain = &shaderCodeDesc.chain;
		shaderCodeDesc.code = {shaderSource, strlen(shaderSource)};
		wgpu::ShaderModule shaderModule = device.createShaderModule(shaderDesc);

		// Create the render pipeline
		wgpu::RenderPipelineDescriptor pipelineDesc;

		// We do not use any vertex buffer for this first simplistic example
		pipelineDesc.vertex.bufferCount = 0;
		pipelineDesc.vertex.buffers = nullptr;

		// NB: We define the 'shaderModule' in the second part of this chapter.
		// Here we tell that the programmable vertex shader stage is described
		// by the function called 'vs_main' in that module.
		pipelineDesc.vertex.module = shaderModule;
		pipelineDesc.vertex.entryPoint = {"vs_main"};
		pipelineDesc.vertex.constantCount = 0;
		pipelineDesc.vertex.constants = nullptr;

		// Each sequence of 3 vertices is considered as a triangle
		pipelineDesc.primitive.topology = wgpu::PrimitiveTopology::TriangleList;

		// We'll see later how to specify the order in which vertices should be
		// connected. When not specified, vertices are considered sequentially.
		pipelineDesc.primitive.stripIndexFormat = wgpu::IndexFormat::Undefined;

		// The face orientation is defined by assuming that when looking
		// from the front of the face, its corner vertices are enumerated
		// in the counter-clockwise (CCW) order.
		pipelineDesc.primitive.frontFace = wgpu::FrontFace::CCW;

		// But the face orientation does not matter much because we do not
		// cull (i.e. "hide") the faces pointing away from us (which is often
		// used for optimization).
		pipelineDesc.primitive.cullMode = wgpu::CullMode::None;

		// We tell that the programmable fragment shader stage is described
		// by the function called 'fs_main' in the shader module.
		wgpu::FragmentState fragmentState;
		fragmentState.module = shaderModule;
		fragmentState.entryPoint = {"fs_main"};
		fragmentState.constantCount = 0;
		fragmentState.constants = nullptr;

		wgpu::BlendState blendState;
		blendState.color.srcFactor = wgpu::BlendFactor::SrcAlpha;
		blendState.color.dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha;
		blendState.color.operation = wgpu::BlendOperation::Add;
		blendState.alpha.srcFactor = wgpu::BlendFactor::Zero;
		blendState.alpha.dstFactor = wgpu::BlendFactor::One;
		blendState.alpha.operation = wgpu::BlendOperation::Add;

		wgpu::ColorTargetState colorTarget;
		colorTarget.format = surfaceFormat;
		colorTarget.blend = &blendState;
		colorTarget.writeMask = wgpu::ColorWriteMask::All; // We could write to only some of the color channels.

		// We have only one target because our render pass has only one output color
		// attachment.
		fragmentState.targetCount = 1;
		fragmentState.targets = &colorTarget;
		pipelineDesc.fragment = &fragmentState;

		// We do not use stencil/depth testing for now
		pipelineDesc.depthStencil = nullptr;

		// Samples per pixel
		pipelineDesc.multisample.count = 1;

		// Default value for the mask, meaning "all bits on"
		pipelineDesc.multisample.mask = ~0u;

		// Default value as well (irrelevant for count = 1 anyways)
		pipelineDesc.multisample.alphaToCoverageEnabled = false;
		pipelineDesc.layout = nullptr;

		pipeline = device.createRenderPipeline(pipelineDesc);

		// We no longer need to access the shader module
		shaderModule.release();
	}

	bool m_isRunning = true;
	SDL_Window* window{};
	wgpu::Device device;
	wgpu::Queue queue;
	wgpu::Surface surface;
	wgpu::TextureFormat surfaceFormat = wgpu::TextureFormat::Undefined;
	wgpu::RenderPipeline pipeline;
};
