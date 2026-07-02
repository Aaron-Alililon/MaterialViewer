#include "Rcore.h"
#include "EditorLayer.h"
#include "Preset3D.h"

// TODO
// Fix per frame material data binding
// Support multiple light sources
// PBR shader

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
  rcore::DeviceDesc devDesc;
  rcore::initDevice(devDesc);

  rcore::WindowDesc windDesc = rcore::Preset3D::makeStandardWindowDescription(L"Material Editor", 1600, 1200);
  auto mainWindow = rcore::makeWindow(windDesc);
  // auto subWindow = rcore::makeWindow(windDesc);

  rcore::D3DContextDesc ctxDesc = rcore::Preset3D::makeStandardContextDescription(windDesc.width(), windDesc.height());
  rcore::makeD3D11Context(mainWindow, ctxDesc);

  mainWindow.lock()->addLayer<EditorLayer>(ctxDesc);

  rcore::init();
}