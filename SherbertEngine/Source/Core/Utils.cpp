//©2021 JDSherbert. All Rights Reserved.

#include "Utils.h"

#include <string>
#include <stdio.h>
#include <filesystem>
#include "../DirectX/DX.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_stdlib.h"
#include <shobjidl.h>
#include <comdef.h>

static DX* dx = &DXClass();

UINT Utils::GetDisplayWidth()
{
	return GetSystemMetrics(SM_CXSCREEN);
}

UINT Utils::GetDisplayHeight()
{
	return GetSystemMetrics(SM_CYSCREEN);
}

UINT Utils::GetContextWidth()
{
	RECT rc;
	GetClientRect(dx->hwnd, &rc);
	UINT width = rc.right - rc.left;
	return width;
}

UINT Utils::GetContextHeight()
{
	RECT rc;
	GetClientRect(dx->hwnd, &rc);
	UINT height = rc.bottom - rc.top;
	return height;
}

void Utils::AddLog(const char* text, ...)
{
	char buff[MAX_PATH];
	va_list args;
	va_start(args, text);
	vsnprintf(buff, MAX_PATH, text, args);
	va_end(args);
	OutputDebugStringA(buff);
	OutputDebugStringA("\n");
}

HRESULT Utils::CompileShaderFromFile(std::wstring srcFile, std::string entryPoint, std::string profile, ID3DBlob** blob)
{
	assert(!srcFile.empty());
	assert(!entryPoint.empty());
	assert(!profile.empty());
	assert(blob);

	if (srcFile.empty() || entryPoint.empty() || profile.empty() || !blob)
		return E_INVALIDARG;

	///////////////////////////////////////////////////////////

	*blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile(srcFile.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint.c_str(), profile.c_str(),
		flags, 0, &shaderBlob, &errorBlob);

	if (FAILED(hr))
	{
		if (errorBlob)
		{
			Utils::AddLog((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (shaderBlob)
			shaderBlob->Release();

		return hr;
	}

	*blob = shaderBlob;

	///////////////////////////////////////////////////////////

	return hr;
}

RECT Utils::GetClientRect()
{
	RECT mainWindow;
	GetClientRect(dx->hwnd, &mainWindow);

	POINT left_top = { mainWindow.left, mainWindow.top };
	POINT right_bottom = { mainWindow.right, mainWindow.bottom };
	ClientToScreen(dx->hwnd, &left_top);
	ClientToScreen(dx->hwnd, &right_bottom);

	RECT clip;
	SetRect(&clip, left_top.x, left_top.y, right_bottom.x, right_bottom.y);
	return clip;
}

HRESULT Utils::CompileShaderFromSource(const char* data, std::string entryPoint, std::string profile, ID3DBlob** blob)
{
	assert(data);
	assert(!entryPoint.empty());
	assert(!profile.empty());
	assert(blob);

	if (!data || entryPoint.empty() || profile.empty() || !blob)
		return E_INVALIDARG;

	///////////////////////////////////////////////////////////

	*blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompile(data, strlen(data), NULL, NULL, NULL, entryPoint.c_str(), profile.c_str(), flags, 0, &shaderBlob, &errorBlob);

	if (FAILED(hr))
	{
		if (errorBlob)
		{
			Utils::AddLog((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (shaderBlob)
			shaderBlob->Release();

		return hr;
	}

	*blob = shaderBlob;

	///////////////////////////////////////////////////////////

	return hr;
}

std::wstring Utils::ConvertString(std::string buffer)
{
	return std::wstring(buffer.begin(), buffer.end());
}

static LPDIRECTINPUT8 gDirectInput;
static IDirectInputDevice8* DIKeyboard;
static IDirectInputDevice8* DIMouse;

bool Utils::InitInput()
{
	if (FAILED(DirectInput8Create(*dx->hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&gDirectInput,
		NULL)))
		return false;

	if (FAILED(gDirectInput->CreateDevice(GUID_SysKeyboard,
		&DIKeyboard,
		NULL)))
		return false;

	if (FAILED(gDirectInput->CreateDevice(GUID_SysMouse,
		&DIMouse,
		NULL)))
		return false;

	if (FAILED(DIKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	if (FAILED(DIKeyboard->SetCooperativeLevel(dx->hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		return false;

	if (FAILED(DIMouse->SetDataFormat(&c_dfDIMouse)))
		return false;

	if (FAILED(DIMouse->SetCooperativeLevel(dx->hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		return false;

	return true;
}

bool Utils::InputGetKey(unsigned char key)
{
	if (DIKeyboard)
	{
		DIKeyboard->Acquire();
		uint8_t keyboardState[256];
		ZeroMemory(&keyboardState, sizeof(uint8_t));
		DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
		if (keyboardState[key] & 0x80)
		{
			return true;
		}
	}

	return false;
}

Vector2 Utils::InputGetMouse()
{
	if (DIMouse)
	{
		DIMouse->Acquire();
		DIMOUSESTATE mouseState;
		ZeroMemory(&mouseState, sizeof(DIMOUSESTATE));
		DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
		return Vector2((float)mouseState.lX, (float)mouseState.lY);
	}

	return Vector2::Zero;
}

bool Utils::InputAcquire()
{
	if (DIKeyboard) if (FAILED(DIKeyboard->Acquire())) return false;
	if (DIMouse)    if (FAILED(DIMouse->Acquire()))    return false;

	return true;
}

Vector3 Utils::ToRadians(Vector3 rotation)
{
	using namespace DirectX;
	return Vector3(
		XMConvertToRadians(rotation.x),
		XMConvertToRadians(rotation.y),
		XMConvertToRadians(rotation.z));
}

Vector3 Utils::ToDegrees(Vector3 rotation)
{
	using namespace DirectX;
	return Vector3(
		XMConvertToDegrees(rotation.x),
		XMConvertToDegrees(rotation.y),
		XMConvertToDegrees(rotation.z));
}

physx::PxVec3 Utils::vector3_to_physics(Vector3 value)
{
	return physx::PxVec3(value.x, value.y, value.z);
}

physx::PxQuat Utils::quat_to_physics(Quaternion value)
{
	return physx::PxQuat(value.x, value.y, value.z, value.w);
}

Vector3 Utils::physics_to_vector3(physx::PxVec3 value)
{
	return Vector3(value.x, value.y, value.z);
}

Quaternion Utils::physics_to_quat(physx::PxQuat value)
{
	return Quaternion(value.x, value.y, value.z, value.w);
}

physx::PxMat44 Utils::matrix_to_physics(Matrix value)
{
	physx::PxMat44 matrix;

	matrix[0][0] = value.m[0][0];
	matrix[0][1] = value.m[0][1];
	matrix[0][2] = value.m[0][2];
	matrix[0][3] = value.m[0][3];

	matrix[1][0] = value.m[1][0];
	matrix[1][1] = value.m[1][1];
	matrix[1][2] = value.m[1][2];
	matrix[1][3] = value.m[1][3];

	matrix[2][0] = value.m[2][0];
	matrix[2][1] = value.m[2][1];
	matrix[2][2] = value.m[2][2];
	matrix[2][3] = value.m[2][3];

	matrix[3][0] = value.m[3][0];
	matrix[3][1] = value.m[3][1];
	matrix[3][2] = value.m[3][2];
	matrix[3][3] = value.m[3][3];

	return matrix;
}

Matrix Utils::physics_to_matrix(physx::PxMat44 value)
{
	Matrix matrix;

	matrix.m[0][0] = value[0][0];
	matrix.m[0][1] = value[0][1];
	matrix.m[0][2] = value[0][2];
	matrix.m[0][3] = value[0][3];

	matrix.m[1][0] = value[1][0];
	matrix.m[1][1] = value[1][1];
	matrix.m[1][2] = value[1][2];
	matrix.m[1][3] = value[1][3];

	matrix.m[2][0] = value[2][0];
	matrix.m[2][1] = value[2][1];
	matrix.m[2][2] = value[2][2];
	matrix.m[2][3] = value[2][3];

	matrix.m[3][0] = value[3][0];
	matrix.m[3][1] = value[3][1];
	matrix.m[3][2] = value[3][2];
	matrix.m[3][3] = value[3][3];

	return matrix;
}

/* 12/11/2022 */
const char* Utils::OpenFileDialog(LPCWSTR dir, LPCWSTR filter, LPCWSTR title)
{
	wchar_t wtext[MAX_PATH];
	ZeroMemory(&wtext, sizeof(wtext));

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = dx->hwnd;
	ofn.lpstrFile = wtext;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = dir;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrTitle = title;
	GetOpenFileName(&ofn);

	_bstr_t b(wtext);
	return b;
}

bool Utils::CreateDirectory(const char* path)
{
	return CreateDirectoryA(path, NULL) != 0;
}

physx::PxTransform Utils::position_rotation_to_physics(Vector3 position, Quaternion rotation)
{
	return physx::PxTransform(
		position.x, position.y, position.z,
		physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w));
}

void Utils::SerializeVector2(YAML::Emitter& out, Vector2 value)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << value.x << value.y << YAML::EndSeq;
}

void Utils::SerializeVector3(YAML::Emitter& out, Vector3 value)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
}

void Utils::SerializeVector4(YAML::Emitter& out, Vector4 value)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << value.x << value.y << value.z << value.w << YAML::EndSeq;
}

void Utils::SerializeQuaternion(YAML::Emitter& out, Quaternion value)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << value.x << value.y << value.z << value.w << YAML::EndSeq;
}

Vector2 Utils::DeserializeVector2(YAML::Node& in)
{
	Vector2 value;
	if (in.IsSequence())
	{
		value.x = in[0].as<float>();
		value.y = in[1].as<float>();
	}
	return value;
}

Vector3 Utils::DeserializeVector3(YAML::Node& in)
{
	Vector3 value;
	if (in.IsSequence())
	{
		value.x = in[0].as<float>();
		value.y = in[1].as<float>();
		value.z = in[2].as<float>();
	}
	return value;
}

Vector4 Utils::DeserializeVector4(YAML::Node& in)
{
	Vector4 value;
	if (in.IsSequence())
	{
		value.x = in[0].as<float>();
		value.y = in[1].as<float>();
		value.z = in[2].as<float>();
		value.w = in[3].as<float>();
	}
	return value;
}

Quaternion Utils::DeserializeQuaternion(YAML::Node& in)
{
	Quaternion value;
	if (in.IsSequence())
	{
		value.x = in[0].as<float>();
		value.y = in[1].as<float>();
		value.z = in[2].as<float>();
		value.w = in[3].as<float>();
	}
	return value;
}

float Utils::RadToDeg(float value)
{
	return DirectX::XMConvertToDegrees(value);
}

float Utils::DegToRad(float value)
{
	return DirectX::XMConvertToRadians(value);
}