#include "Rcore.h"
#include "ViewLayer.h"
#include "SettingsLayer.h"
#include "Preset3D.h"

// TODO
// Fix per frame material data binding
// Variable editor
// Support multiple light sources
// PBR shader

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
  rcore::DeviceDesc devDesc;
  rcore::initDevice(devDesc);

  // --- Model Window ---
  rcore::WindowDesc modelWindDesc = rcore::Preset3D::makeStandardWindowDescription(L"Material Editor - View", 1600, 1200);
  auto modelWindow = rcore::makeWindow(modelWindDesc);

  rcore::D3DContextDesc modelCtxDesc = rcore::Preset3D::makeStandardContextDescription(modelWindDesc.width(), modelWindDesc.height());
  rcore::makeD3D11Context(modelWindow, modelCtxDesc);

  modelWindow.lock()->addLayer<ViewLayer>(modelCtxDesc);

  // --- Settings Window ---
  rcore::WindowDesc settingsWindDesc = rcore::Preset3D::makeStandardWindowDescription(L"Material Editor - Settings", 400, 800);
  settingsWindDesc.windowPosX(100);
  auto settingsWindow = rcore::makeWindow(settingsWindDesc);

  rcore::D3DContextDesc settingsCtxDesc = rcore::Preset3D::makeStandardContextDescription(settingsWindDesc.width(), settingsWindDesc.height());
  rcore::makeD3D11Context(settingsWindow, settingsCtxDesc);

  settingsWindow.lock()->addLayer<SettingsLayer>(settingsCtxDesc);

  rcore::init();
}