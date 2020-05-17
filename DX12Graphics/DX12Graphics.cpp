// DX12Graphics.cpp : Defines the functions for the static library.
//

#include "pch.h"

#include <cstdio>
#include <d3d12.h>
#include <dxgi.h>
#include <Windows.h>

#include "dx12graphics.h"

DX12GraphicsDevice::DX12GraphicsDevice(const HWND h_wnd, const int width, const int height, const bool fullscreen)
 : GraphicsDevice(h_wnd, width, height, fullscreen) {

#if defined (DEBUG) || defined(_DEBUG)
 auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller_));
 if (FAILED(hr))
 {
	 printf("debug layer failed. exiting.");
	 exit(1);
 }
#endif

 hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgi_factory_));
 if (FAILED(hr))
 {
	 exit(1);
 }

 hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3d_device_));
 if (FAILED(hr))
 {
	 printf("error while initializing d3d12. exiting.");
	 exit(1);
 }

 if (FAILED(d3d_device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_))))
 {
	 exit(1);
 }

 rtv_desc_size_ = d3d_device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
 dsv_desc_size_ = d3d_device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
 cbv_srv_desc_size_ = d3d_device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

 // TODO check for msaa 4x

 D3D12_COMMAND_QUEUE_DESC qd = {};
 qd.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
 qd.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
 if (FAILED(d3d_device_->CreateCommandQueue(&qd, IID_PPV_ARGS(&command_queue_)))) { exit(1); }
 if (FAILED(
	 d3d_device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(command_allocator_.GetAddressOf
		 ())))) { exit(1); }
 if (FAILED(
	 d3d_device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_allocator_.Get(), nullptr,
		 IID_PPV_ARGS(command_list_.GetAddressOf()))));

 command_list_->Close();

 DXGI_SWAP_CHAIN_DESC scd;
 scd.BufferDesc.Width = width;
 scd.BufferDesc.Height = height;
 scd.BufferDesc.RefreshRate.Numerator = 60;
 scd.BufferDesc.RefreshRate.Denominator = 1;
 scd.BufferDesc.Format = back_buffer_format_;
 scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
 scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
 scd.SampleDesc.Count = 1;
 scd.SampleDesc.Quality = 0;
 scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
 scd.BufferCount = 2;
 scd.OutputWindow = h_wnd;
 scd.Windowed = !fullscreen;
 scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
 scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
 dxgi_factory_->CreateSwapChain(command_queue_.Get(), &scd, swap_chain_.GetAddressOf());

 D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc;
 rtv_heap_desc.NumDescriptors = 2;
 rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
 rtv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
 rtv_heap_desc.NodeMask = 0;
 d3d_device_->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(rtv_heap_.GetAddressOf()));

 D3D12_DESCRIPTOR_HEAP_DESC dsv_heap_desc;
 dsv_heap_desc.NumDescriptors = 1;
 dsv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
 dsv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
 dsv_heap_desc.NodeMask = 0;
 d3d_device_->CreateDescriptorHeap(&dsv_heap_desc, IID_PPV_ARGS(dsv_heap_.GetAddressOf()));
	
	
}

DX12GraphicsDevice::~DX12GraphicsDevice()
= default;

void DX12GraphicsDevice::present()
{
}

void DX12GraphicsDevice::clear_screen(float r, float g, float b, float a)
{
}
