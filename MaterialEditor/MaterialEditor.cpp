#include "Rcore.h"
#include "ViewLayer.h"
#include "SettingsLayer.h"
#include "Preset3D.h"

// TODO
// Use sphere with same amount of rows and columns
// Changeable scene (Model sphere, cube, plane, ...; Skybox sky, studio, ...)
// Resizable windows
// Drag-and-drop textures
// Fix paths for textures, shaders and models

// BEFORE PUBLISHING
// Build as cmake
// rcore as git submodule

// INFO
// WS_EX_ACCEPTFILES for accepting files

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
  rcore::DeviceDesc devDesc;
  rcore::initDevice(devDesc);

  // --- Settings Window ---
  rcore::WindowDesc settingsWindDesc = rcore::Preset3D::makeStandardWindowDescription(L"Material Editor - Settings", 500, 800);
  settingsWindDesc.windowPosX(100);
  auto settingsWindow = rcore::makeWindow(settingsWindDesc);

  rcore::D3DContextDesc settingsCtxDesc = rcore::Preset3D::makeStandardContextDescription(settingsWindDesc.width(), settingsWindDesc.height());
  rcore::makeD3D11Context(settingsWindow, settingsCtxDesc);

  settingsWindow.lock()->addLayer<SettingsLayer>(settingsCtxDesc);

  // --- Model Window ---
  rcore::WindowDesc modelWindDesc = rcore::Preset3D::makeStandardWindowDescription(L"Material Editor - View", 1600, 1200);
  auto modelWindow = rcore::makeWindow(modelWindDesc);

  rcore::D3DContextDesc modelCtxDesc = rcore::Preset3D::makeStandardContextDescription(modelWindDesc.width(), modelWindDesc.height());
  rcore::makeD3D11Context(modelWindow, modelCtxDesc);

  modelWindow.lock()->addLayer<ViewLayer>(settingsWindow, modelCtxDesc);

  rcore::init();
}