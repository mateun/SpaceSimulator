#pragma once

#include <d3d12.h>
#include <dxgi.h>
#include "graphics.h"
#include <wrl/client.h>

using namespace Microsoft::WRL;

class DX12GraphicsDevice : public GraphicsDevice {

	

public:
	DX12GraphicsDevice(HWND h_wnd, int width, int height, bool fullscreen);
	~DX12GraphicsDevice();

	void present() override;
	void clear_screen(float r, float g, float b, float a) override;
	
private:

	// D3D variables
	
	ID3D12Device* d3d_device_{};
	ComPtr<ID3D12Debug> debug_controller_;
	ComPtr<IDXGIFactory> dxgi_factory_;
	ComPtr<ID3D12Fence> fence_;
	ComPtr<ID3D12CommandQueue> command_queue_;
	ComPtr<ID3D12GraphicsCommandList> command_list_;
	ComPtr<ID3D12CommandAllocator> command_allocator_;
	ComPtr<IDXGISwapChain> swap_chain_;
	ComPtr<ID3D12DescriptorHeap> rtv_heap_;
	ComPtr<ID3D12DescriptorHeap> dsv_heap_;
	UINT rtv_desc_size_ = 0;
	UINT dsv_desc_size_ = 0;
	UINT cbv_srv_desc_size_ = 0;
	DXGI_FORMAT back_buffer_format_ = DXGI_FORMAT_R8G8B8A8_UNORM;
	int current_back_buffer_ = 0;
	
};
